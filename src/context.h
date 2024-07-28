#ifndef CONTEXT_HEADERS
#define CONTEXT_HEADERS
#include "EngineCommons.h"
#include "Window.h"
ErrorCode initContext(struct EngineState* state);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void endContext(struct EngineState* state);
ErrorCode createSurface( struct EngineState* state);
ErrorCode pickPhysicalDevice( struct EngineState* state);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
VkDebugUtilsMessageTypeFlagsEXT messageType,
const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
void* pUserData);
bool isDeviceSuitable(struct EngineState* state, VkPhysicalDevice device);
ErrorCode setupDebugMessenger(struct EngineState* state);
ErrorCode createLogicalDevice(struct EngineState* state);
ErrorCode createCommandPool(struct EngineState* state);

bool queueFamilyIndicesIsComplete(struct QueueFamilyIndices* qfi){ return qfi -> graphicsFamilyHasValue  && qfi -> presentFamilyHasValue ;}

//TODO: REFACTOR THIS MESS  state might be not nedeed
struct QueueFamilyIndices findQueueFamilies(struct EngineState* state, VkPhysicalDevice device);
struct QueueFamilyIndices findPhysicalQueueFamilies(struct EngineState* state) { return findQueueFamilies(state,state->context.physicalDevice); }
#endif // !CONTEXT_HEADERS
