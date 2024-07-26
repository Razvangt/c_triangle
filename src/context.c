#include "context.h"
#include "EngineCommons.h"
#include "errors.h"


bool checkValidationLayerSupport(struct EngineState* state) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
    if (availableLayers == NULL) {
        return false;
    }
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);


    // Validation layers seems to be a list of strings

    // I could refactor however i will leave it until the end.
    for (const char* layerName : state->context.validationLayers) {
        bool layerFound = false;
        for (int i = 0; i < layerCount; ++i) {
            VkLayerProperties layerProperties = availableLayers[i];
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}
const char** getRequiredExtensions(EngineState state) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    const char* extensions[glfwExtensionCount] = glfwExtensions;
    if (state.context.enableValidationLayers) {
        // So how the fuck  do i manage do push_back this ???
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

ErrorCode createInstance(EngineState* state) {
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

    const char** extensions            = getRequiredExtensions();
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext             = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    hasGflwRequiredInstanceExtensions();

    return SUCCESS;
}


ErrorCode initContext(EngineState* state) {
    // I meay need a better error managment
    // also some kind of type err wold be usefull that holds
    SOR(createInstance(state));
    //   setupDebugMessenger(state);
    //   createSurface(state);
    //   pickPhysicalDevice(state);
    //   createLogicalDevice(state);
    //   createCommandPool(state);

    return SUCCESS;
};


void endContext(EngineState* state) {
    /**
vkDestroyCommandPool(device_, commandPool, nullptr);
vkDestroyDevice(device_, nullptr);

if (enableValidationLayers) {
   DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}
    vkDestroySurfaceKHR(instance, surface_, nullptr);
    vkDestroyInstance(instance, nullptr);
*/
}
