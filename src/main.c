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
  const char *window_title;
  int window_width, window_height;
  bool window_resizable;
  bool window_fullscreen;
  uint32_t api_version;

  GLFWmonitor *window_monitor;
  GLFWwindow *window;

  VkAllocationCallbacks *allocator;
  VkInstance instance;
} State;

void setUpErrorHandling() {
  glfwSetErrorCallback(glfwErrorCallback);
  atexit(exitCallback);
}
void createWindow(State *state) {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, state->window_resizable);
  if (state->window_fullscreen) {
    state->window_monitor = glfwGetPrimaryMonitor();
  }
  state->window =
      glfwCreateWindow(state->window_width, state->window_height,
                       state->window_title, state->window_monitor, NULL);
}
void createInstance(State *state) {
  uint32_t requiredExtensionsCount;
  const char **requiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

  VkApplicationInfo applicationInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .apiVersion = state->api_version,
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
  vkEnumerateInstanceVersion(&instanceApiVersion);
  uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
  uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
  uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
  uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);
  printf("Vulkan Api %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor,
         apiVersionMinor, apiVersionPatch);
  printf("GLFW %s \n", glfwGetVersionString());
}

void init(State *state) {
  setUpErrorHandling();
  logInfo();
  createWindow(state);
  createInstance(state);
}
void loop(State *state) {
  while (!glfwWindowShouldClose(state->window)) {
    glfwPollEvents(); // for games  every frame is redrawn
    // glwgWaitEvents for gui no need to redraw unless there is an event
  }
}
void cleanup(State *state) {
  glfwDestroyWindow(state->window);
  vkDestroyInstance(state->instance, state->allocator);
  state->window = NULL;
}

int main(int argc, char *argv[]) {
  State state = {
      .window_title = "Triangle",
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
