#include "tools.h"
void infoLog() {
    uint32_t instanceApiVersion;

    VkResult result = vkEnumerateInstanceVersion(&instanceApiVersion);
    if (result != VK_SUCCESS) {
        printf(" Couldn't enumerate instance version \n");
        return;
    }

    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor   = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor   = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch   = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}
