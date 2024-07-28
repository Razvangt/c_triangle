#include "context.h"


bool checkValidationLayerSupport(struct EngineState* state) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
    if (availableLayers == NULL) {
        return false;
    }
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    const char* layerName;
    for (int j = 0; j < state->context.validationLayers->count; ++j) {
        layerName = state->context.validationLayers->data[j];
        for (int i = 0; i < layerCount; ++i) {
            VkLayerProperties layerProperties = availableLayers[i];
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                return true;
            }
        }
    }
    return false;
}

const struct StringArray getRequiredExtensions(struct EngineState* state) {
    uint32_t extensionsCount;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

    struct StringArray extensions;
    extensions.count = extensionsCount;
    extensions.data  = glfwExtensions;
    // Do i need to save this ???
    if (state->context.enableValidationLayers) {
        add_s_to_string_array(&extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

// local callback functions
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
VkDebugUtilsMessageTypeFlagsEXT messageType,
const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
void* pUserData) {
    printf("validation layer: %s \n", pCallbackData->pMessage);
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* crI) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData       = NULL;
    crI                        = &createInfo;
}


ErrorCode hasGflwRequiredInstanceExtensions(struct EngineState* state) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));

    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
    printf("available extensions: \n");

    VkExtensionProperties ext;
    char** available = (char**)malloc(extensionCount * sizeof(char*));
    for (int i = 0; i < extensionCount; ++i) {
        ext = extensions[i];
        printf("Extension Name : %s \n", ext.extensionName);
        available[i] = extensions->extensionName;
    }
    struct StringArray availableArr = { .count = extensionCount, .data = available };

    printf("required extensions : \n");
    struct StringArray requiredExtensions = getRequiredExtensions(state);

    // so i should find all extensions from required in available

    char* required;
    for (int i = 0; i < requiredExtensions.count; ++i) {
        required = requiredExtensions.data[i];
        if (!find_in_string_array(required, availableArr)) {
            printf("Required Extension Not Found : %s \n", required);
            return ERROR_VULKAN;
        }
    }
    return SUCCESS;
}

// TODO: REVIEW WHAT TO FREE LATTER WHEN THE CONTEXT ENDS
ErrorCode createInstance(struct EngineState* state) {
    if (state->context.enableValidationLayers && !checkValidationLayerSupport(state)) {
        printf("Validation layers requested, but not available!");
        return ERROR_VULKAN;
    }

    VkApplicationInfo appInfo  = {};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "LittleVulkanEngine App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo     = &appInfo;

    // TODO: To much warnings with constants i shold review them for sure
    struct StringArray extensions      = getRequiredExtensions(state);
    createInfo.enabledExtensionCount   = extensions.count;
    createInfo.ppEnabledExtensionNames = extensions.data;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    if (state->context.enableValidationLayers) {
        createInfo.enabledLayerCount   = state->context.validationLayers->count;
        createInfo.ppEnabledLayerNames = state->context.validationLayers->data;
        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext             = NULL;
    }

    if (vkCreateInstance(&createInfo, NULL, &state->context.instance) != VK_SUCCESS) {
        printf("failed to create instance!");
        return ERROR_VULKAN;
    }
    SOR(hasGflwRequiredInstanceExtensions(state));
    return SUCCESS;
}
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
const VkAllocationCallbacks* pAllocator,
VkDebugUtilsMessengerEXT* pDebugMessenger) {


    PFN_vkCreateDebugUtilsMessengerEXT func =
    (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
ErrorCode setupDebugMessenger(struct EngineState* state) {
    if (!state->context.enableValidationLayers)
        return SUCCESS;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    if (CreateDebugUtilsMessengerEXT(state->context.instance, &createInfo, NULL, state->context.debugMessenger) != VK_SUCCESS) {
        printf("failed to set up debug messenger! \n");
        return ERROR_VULKAN;
    }
}

ErrorCode createSurface(struct EngineState* state) {
    SOR(createWindowSurface(state->context.window, state->context.instance, &state->context.surface_));
    return SUCCESS;
}
struct QueueFamilyIndices findQueueFamilies(struct EngineState* state, VkPhysicalDevice device) {
    struct QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);


    VkQueueFamilyProperties queueFamily;
    for (int i = 0; i < queueFamilyCount; ++i) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily         = i;
            indices.graphicsFamilyHasValue = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, state->context.surface_, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily         = i;
            indices.presentFamilyHasValue = true;
        }
        if (queueFamilyIndicesIsComplete(&indices)) {
            break;
        }
    }
    return indices;
}


bool checkDeviceExtensionSupport(VkPhysicalDevice device, struct StringArray* deviceExtensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

    VkExtensionProperties* availableExtensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

    // so what i want to know is if deviceExtensions are inside the availableExtensions;
    // so if i iterate the device extensions  and for each device extension if is not inside the availableExtension; return false;

    char* requiredExtension;
    char* extName;
    for (int i = 0; i < deviceExtensions->count; i++) {
        requiredExtension = deviceExtensions->data[i];
        bool exists       = false;
        for (int j = 0; j < extensionCount; ++j) {
            extName = availableExtensions[j].extensionName;
            if (extName == requiredExtension) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            printf("No extension Required found in extension Support  %s \n", requiredExtension);
            return false;
        }
    }

    return true;
}

struct SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, struct EngineState* state) {
    struct SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, state->context.surface_, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, state->context.surface_, &formatCount, NULL);

    if (formatCount != 0) {
        // i need to resize this to  formatCount
        details.formats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, state->context.surface_, &formatCount, details.formats);
    }


    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, state->context.surface_, &presentModeCount, NULL);


    if (presentModeCount != 0) {
        details.presentModes = (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, state->context.surface_, &presentModeCount, details.presentModes);
    }

    return details;
}

bool isDeviceSuitable(struct EngineState* state, VkPhysicalDevice device) {
    struct QueueFamilyIndices indices = findQueueFamilies(state, device);
    bool extensionsSupported          = checkDeviceExtensionSupport(device, state->context.deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        struct SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, state);
        swapChainAdequate = count_pointer(swapChainSupport.formats, sizeof(VkSurfaceFormatKHR)) != 0 &&
        count_pointer(swapChainSupport.presentModes, sizeof(VkPresentModeKHR)) != 0;
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return queueFamilyIndicesIsComplete(&indices) && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

ErrorCode pickPhysicalDevice(struct EngineState* state) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(state->context.instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        printf("failed to find GPUs with Vulkan support! \n");
    }

    printf("Device Count :   %d\n", deviceCount);
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(state->context.instance, &deviceCount, devices);

    VkPhysicalDevice device;
    for (int i = 0; i < deviceCount; i++) {
        device = devices[i];
        if (isDeviceSuitable(state, device)) {
            state->context.physicalDevice = device;
            break;
        }
    }

    if (state->context.physicalDevice == VK_NULL_HANDLE) {
        printf("failed to find a suitable GPU! \n");
        return ERROR_VULKAN;
    }


    vkGetPhysicalDeviceProperties(state->context.physicalDevice, &state->context.properties);
    printf("physical device: %s", state->context.properties.deviceName);
    return SUCCESS;
};

ErrorCode createLogicalDevice(struct EngineState* state) {
    // TODO:REFACTOR THIS
    struct QueueFamilyIndices indices = findQueueFamilies(state, state->context.physicalDevice);


    uint32_t uniqueQueueFamilies[2] = { indices.graphicsFamily, indices.presentFamily };
    uint32_t queueCreateInfosCount  = 2;
    // sholb be uniiqueQueueFamilies sinze is hardcoded i leave the same size if the unique is created dinamic this shold be aswell
    VkDeviceQueueCreateInfo* queueCreateInfos =
    (VkDeviceQueueCreateInfo*)malloc(queueCreateInfosCount * (sizeof(VkDeviceQueueCreateInfo)));
    float queuePriority = 1.0f;

    for (int i = 0; i < queueCreateInfosCount; ++i) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex        = uniqueQueueFamilies[i];
        queueCreateInfo.queueCount              = 1;
        queueCreateInfo.pQueuePriorities        = &queuePriority;
        queueCreateInfos[i]                     = queueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy        = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType              = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = queueCreateInfosCount;
    createInfo.pQueueCreateInfos    = queueCreateInfos;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount   = state->context.deviceExtensions->count;
    createInfo.ppEnabledExtensionNames = state->context.deviceExtensions->data;

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (state->context.enableValidationLayers) {
        createInfo.enabledLayerCount   = state->context.validationLayers->count;
        createInfo.ppEnabledLayerNames = state->context.validationLayers->data;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(state->context.physicalDevice, &createInfo, NULL, &state->context.device_) != VK_SUCCESS) {
        printf("failed to create logical device! \n");
        return ERROR_VULKAN;
    }
    vkGetDeviceQueue(state->context.device_, indices.graphicsFamily, 0, &state->context.graphicsQueue_);
    vkGetDeviceQueue(state->context.device_, indices.presentFamily, 0, &state->context.presentQueue_);
    return SUCCESS;
};


ErrorCode createCommandPool(struct EngineState* state) {
    struct QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies(state);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex        = queueFamilyIndices.graphicsFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(state->context.device_, &poolInfo, NULL, &state->context.commandPool) != VK_SUCCESS) {
        printf("failed to create command pool! \n");
        return ERROR_VULKAN;
    }
    return SUCCESS;
};

// TODO: RUN AND DEBUG
ErrorCode initContext(struct EngineState* state) {
    SOR(createInstance(state));
    SOR(setupDebugMessenger(state));
    SOR(createSurface(state));
    SOR(pickPhysicalDevice(state));
    SOR(createLogicalDevice(state));
    SOR(createCommandPool(state));
    return SUCCESS;
}

// Not sure how this works if not null it just makes a new reference which points nowhere but  he dosnt free it
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func =
    (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

// TODO: FREE ALL AFTER FINISH
void endContext(struct EngineState* state) {

    // i think i shold also free the  structs with extensions and maube some fields more
    vkDestroyCommandPool(state->context.device_, state->context.commandPool, NULL);
    vkDestroyDevice(state->context.device_, NULL);

    if (state->context.enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(state->context.instance, state->context.debugMessenger, NULL);
    }
     vkDestroySurfaceKHR(state->context.instance, state->context.surface_, NULL);
     vkDestroyInstance(state->context.instance, NULL);
}
