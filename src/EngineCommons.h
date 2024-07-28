#ifndef ENGINE_COMMONS
#define ENGINE_COMMONS
#include "ExternalLibs.h"
#include "errors.h"
#include "tools.h"

struct Window {

    GLFWwindow* handle;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
    VkPresentModeKHR* presentModes;
};
struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue;
    bool presentFamilyHasValue;
};
 

struct Config {
    const char* applicationName;
    const char* engineName;
    const char* windowTitle;
    int w_height;
    int w_width;


    uint32_t apiVersion;
};


// for a string vector i need to hold
// array  data
// each string size
// n of elements
struct Context {
    bool shouldClose;
    
    // they might grow so not constant if at some point i refactor and find a better way i might change it
    struct StringArray* validationLayers;
    struct StringArray* deviceExtensions;
    bool enableValidationLayers;

    struct Window window;
    // vulkan fields
    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkCommandPool commandPool;
};

struct EngineState {
    struct Context context;
    struct Config config;
};
#endif // !ENGINE_COMMONS
