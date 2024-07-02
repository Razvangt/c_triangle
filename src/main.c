#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#define PANIC(ERROR, FORMAT, ...)                                              \
  {                                                                            \
    if (ERROR) {                                                               \
      fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n",          \
              __FILE_NAME__, __FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__);    \
      raise(SIGABRT);                                                          \
    }                                                                          \
  }

void glfwErrorCallback(int error_code, const char *description) {
  PANIC(error_code, "GLFW: %s", description)
}
void exitCallback() { glfwTerminate(); }

typedef struct {
  // Configurable
  const char *window_title;
  const char *applicationName;
  const char *engineName;

  int window_width, window_height;
  bool window_resizable;
  bool window_fullscreen;

  // GLFW
  int frameBufferWidth;
  int frameBufferHeight;

  GLFWmonitor *window_monitor;
  GLFWwindow *window;

  // Vulkan
  uint32_t api_version;
  uint32_t queueFamily;
  uint32_t swapchainImagesCount;
  bool recreateSwapchain;

  VkAllocationCallbacks *allocator;
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR surface;
  VkDevice device;
  VkQueue queue;
  VkSwapchainKHR swapchain;
  VkImage *swapchainImages;
  VkImageView *swapchainImagesViews;
} State;

void setUpErrorHandling() {
  glfwSetErrorCallback(glfwErrorCallback);
  atexit(exitCallback);
}

void glfwframebufferSizeCallback(GLFWwindow *window, int width, int height) {
  State *state = glfwGetWindowUserPointer(window);
  state->recreateSwapchain = true;
  state->frameBufferHeight = height;
  state->frameBufferWidth = width;
  printf("RESIZED %i %i \n", height, width);
}
void createWindow(State *state) {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, state->window_resizable);
  if (state->window_fullscreen) {
    state->window_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(state->window_monitor);
    state->window_width = mode->width;
    state->window_height = mode->height;
  }
  state->window =
      glfwCreateWindow(state->window_width, state->window_height,
                       state->window_title, state->window_monitor, NULL);

  glfwSetWindowUserPointer(state->window, state);
  glfwSetFramebufferSizeCallback(state->window, glfwframebufferSizeCallback);
  int width, height;
  glfwGetFramebufferSize(state->window, &width, &height);
  glfwframebufferSizeCallback(state->window, width, height);
}
void createInstance(State *state) {
  uint32_t requiredExtensionsCount;
  const char **requiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

  VkApplicationInfo applicationInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .apiVersion = state->api_version,
      .pApplicationName = state->applicationName,
      .pEngineName = state->engineName,

  };

  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &applicationInfo,
      .enabledExtensionCount = requiredExtensionsCount,
      .ppEnabledExtensionNames = requiredExtensions,
  };

  PANIC(vkCreateInstance(&createInfo, state->allocator, &state->instance),
        "Couldn't create instance")
}

void logInfo() {
  uint32_t instanceApiVersion;

  PANIC(vkEnumerateInstanceVersion(&instanceApiVersion),
        "Couldnt enumerate instance version");

  uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
  uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
  uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
  uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);

  printf("Vulkan Api %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor,
         apiVersionMinor, apiVersionPatch);
  printf("GLFW %s \n", glfwGetVersionString());
}

void selectPhisicalDevice(State *state) {
  uint32_t count;
  PANIC(vkEnumeratePhysicalDevices(state->instance, &count, NULL),
        "Couldnt enumerate phisical devices  count");
  PANIC(count == 0, "Couldnt find  a vulkan supported physical device");
  PANIC(vkEnumeratePhysicalDevices(state->instance, &(uint32_t){2},
                                   &state->physicalDevice),
        "Couldnt enumerate phisical devices  count");
}

void createSurface(State *state) {
  PANIC(glfwCreateWindowSurface(state->instance, state->window,
                                state->allocator, &state->surface),
        "Couldnt create window surface");
}

void selectQueuefamily(State *state) {
  state->queueFamily = UINT32_MAX;

  uint32_t count;
  vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count, NULL);

  VkQueueFamilyProperties *queue_families =
      malloc(count * sizeof(VkQueueFamilyProperties));
  PANIC(queue_families == NULL, "Couldn't allocate memory")
  vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count,
                                           queue_families);

  for (uint32_t queue_family_index = 0; queue_family_index < count;
       ++queue_family_index) {
    VkQueueFamilyProperties properties = queue_families[queue_family_index];

    if ((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
        glfwGetPhysicalDevicePresentationSupport(
            state->instance, state->physicalDevice, queue_family_index)) {
      state->queueFamily = queue_family_index;
      break;
    }
  }

  PANIC(state->queueFamily == UINT32_MAX,
        "Couldn't find a suitable queue family")
  free(queue_families);
}
void createDevice(State *state) {
  PANIC(vkCreateDevice(
            state->physicalDevice,
            &(VkDeviceCreateInfo){
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pQueueCreateInfos =
                    &(VkDeviceQueueCreateInfo){
                        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                        .queueFamilyIndex = state->queueFamily,
                        .queueCount = 1,
                        .pQueuePriorities = &(float){1.0},
                    },
                .queueCreateInfoCount = 1,
                .enabledExtensionCount = 1,
                .ppEnabledExtensionNames =
                    &(const char *){VK_KHR_SWAPCHAIN_EXTENSION_NAME},
            },
            state->allocator, &state->device),
        "Couldnt create device")
}
void getQueue(State *state) {
  vkGetDeviceQueue(state->device, state->queueFamily, 0, &state->queue);
}
uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}
void createSwapChain(State *state) {
  // FILL capabilities
  VkSurfaceCapabilitiesKHR capabilities;
  PANIC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            state->physicalDevice, state->surface, &capabilities),
        "Failed to get surface capabilities");

  // FILL formatCount
  uint32_t formatCount;
  PANIC(vkGetPhysicalDeviceSurfaceFormatsKHR(
            state->physicalDevice, state->surface, &formatCount, NULL),
        "Failed to get  surface format count");
  // create fomats buffer
  VkSurfaceFormatKHR *formats =
      malloc(formatCount * sizeof(VkSurfaceFormatKHR));
  PANIC(!formats, "Couldnt allocate memory  on formats");
  // FILL formats buffer
  PANIC(vkGetPhysicalDeviceSurfaceFormatsKHR(
            state->physicalDevice, state->surface, &formatCount, formats),
        "Failed to get  surface format");

  // Iterate and select format from formats buffer with 0 as default
  // this selects the last available format that complies with the conditions
  uint32_t formatIndex = 0;
  for (int i = 0; i < formatCount; ++i) {
    VkSurfaceFormatKHR format = formats[i];
    if (format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR &&
        format.format == VK_FORMAT_B8G8R8A8_SRGB) {
      formatIndex = i;
    }
  }

  VkSurfaceFormatKHR format = formats[formatIndex];

  // Maybe refactor latter not really  sure on the way the loop is made
  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  // GET COUNT
  uint32_t presentModeCount;
  PANIC(vkGetPhysicalDeviceSurfacePresentModesKHR(
            state->physicalDevice, state->surface, &presentModeCount, NULL),
        "Couldnt get presentModeCount");
  // buffer on count size
  VkPresentModeKHR *presentModes =
      malloc(formatCount * sizeof(VkSurfaceFormatKHR));
  // fill buffer
  PANIC(!presentModes, "Couldnt allocate memory on  presentModes");
  PANIC(vkGetPhysicalDeviceSurfacePresentModesKHR(
            state->physicalDevice, state->surface, &presentModeCount,
            presentModes),
        "Couldnt get presentModeCount");
  uint32_t presentModeIndex = UINT32_MAX;
  for (int i = 0; i < presentModeCount; ++i) {
    VkPresentModeKHR mode = presentModes[i];
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      presentModeIndex = i;
      break;
    }
  }

  if (presentModeIndex != UINT32_MAX) {
    presentMode = presentModes[presentModeIndex];
  }

  // free  buffers
  free(presentModes);
  free(formats);
  VkSwapchainKHR swapchain;
  // Research all options other day
  PANIC(vkCreateSwapchainKHR(
            state->device,
            &(VkSwapchainCreateInfoKHR){
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .surface = state->surface,
                .queueFamilyIndexCount = 1,
                .pQueueFamilyIndices = &state->queueFamily,
                .clipped = true,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .imageArrayLayers = capabilities.maxImageArrayLayers,
                .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .oldSwapchain = state->swapchain,
                .preTransform = capabilities.currentTransform,
                .imageExtent = capabilities.currentExtent,
                .imageFormat = format.format,
                .imageColorSpace = format.colorSpace,
                .presentMode = presentMode,
                .minImageCount = clamp(
                    capabilities.minImageCount + 1, capabilities.minImageCount,
                    capabilities.maxImageCount ? capabilities.maxImageCount
                                               : UINT32_MAX),
            },
            state->allocator, &swapchain),
        "Couldnt get swapchain");

  if (state->swapchainImagesViews) {
    for (int i = 0; i < state->swapchainImagesCount; ++i) {
      vkDestroyImageView(state->device, state->swapchainImagesViews[i],
                         state->allocator);
    }
    free(state->swapchainImagesViews);
    free(state->swapchainImages);
  }
  vkDestroySwapchainKHR(state->device, state->swapchain, state->allocator);
  state->swapchain = swapchain;

  PANIC(vkGetSwapchainImagesKHR(state->device, swapchain,
                                &state->swapchainImagesCount, NULL),
        "Couldnt get Images Count");
  state->swapchainImages =
      malloc(state->swapchainImagesCount * sizeof(VkImage));
  PANIC(!state->swapchainImages, "Couldnt allocate memory on  swapchainImages");
  PANIC(vkGetSwapchainImagesKHR(state->device, swapchain,
                                &state->swapchainImagesCount,
                                state->swapchainImages),
        "Couldnt get Swapchain Images");
  state->swapchainImagesViews =
      malloc(state->swapchainImagesCount * sizeof(VkImageView));
  PANIC(!state->swapchainImagesViews,
        "Couldnt allocate memory on  swapchainImagesViews");
  for (int i = 0; i < state->swapchainImagesCount; ++i) {
    PANIC(
        vkCreateImageView(state->device,
                          &(VkImageViewCreateInfo){
                              .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                              .format = format.format,
                              .image = state->swapchainImages[i],
                              .components = (VkComponentMapping){},
                              .subresourceRange =
                                  (VkImageSubresourceRange){
                                      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  },
                              .viewType = VK_IMAGE_VIEW_TYPE_2D,
                          },
                          state->allocator, &state->swapchainImagesViews[i]),
        "couldnt create image view %i", i);
  }
}
void init(State *state) {
  setUpErrorHandling();
  logInfo();

  createWindow(state);
  createInstance(state);

  selectPhisicalDevice(state);
  createSurface(state);
  selectQueuefamily(state);
  createDevice(state);

  getQueue(state);
}
void loop(State *state) {
  while (!glfwWindowShouldClose(state->window)) {
    glfwPollEvents(); // for games  every frame is redrawn
    // glwgWaitEvents for gui no need to redraw unless there is an event
    if (state->recreateSwapchain) {
      state->recreateSwapchain = false;
      printf("Recreated Swapchain \n");
      createSwapChain(state);
    }

    uint32_t imageIndex;
    vkAcquireNextImageKHR(state->device, state->swapchain, UINT64_MAX, NULL,
                          NULL, &imageIndex);
    VkResult result = vkQueuePresentKHR(
        state->queue, &(VkPresentInfoKHR){
                          .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                          .pImageIndices = &(uint32_t){imageIndex},
                          .swapchainCount = 1,
                          .pSwapchains = &state->swapchain,
                      });
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
      state->recreateSwapchain = true;
    }
  }
}
void cleanup(State *state) {
  for (int i = 0; i < state->swapchainImagesCount; ++i) {
    vkDestroyImageView(state->device, state->swapchainImagesViews[i],
                       state->allocator);
  }
  vkDestroySwapchainKHR(state->device, state->swapchain, state->allocator);
  vkDestroyDevice(state->device, state->allocator);
  vkDestroySurfaceKHR(state->instance, state->surface, state->allocator);
  vkDestroyInstance(state->instance, state->allocator);
}

int main(int argc, char *argv[]) {
#ifdef __clang_major__
  printf("clang detected version %d.%d\n", __clang_major__, __clang_minor__);
#endif

#ifdef __GNUC__
  printf("gcc detected version %d.%d\n", __GNUC__, __GNUC_MINOR__);
#endif

  State state = {
      .window_title = "Triangle",
      .applicationName = "TriangleApp",
      .engineName = "TriangleEngine",
      .window_width = 720,
      .window_height = 480,
      .window_resizable = true,
      .window_fullscreen = false,
      .api_version = VK_API_VERSION_1_3,
  };
  init(&state);
  loop(&state);
  cleanup(&state);
  return EXIT_SUCCESS;
}
