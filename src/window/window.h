
#ifndef WINDOW_HEADERS_INCLUDED
#define WINDOW_HEADERS_INCLUDED
#include "../headers.h"
#include "../utils/utils.h"
void windowPollEvents(const State *state);
bool windowShouldClose(const State *state);
void surfaceCreate(State *state);
void surfaceDestroy(State *state);
VkSurfaceCapabilitiesKHR surfaceCapabilitiesGet(const State *state);
VkSurfaceFormatKHR surfaceFormatsSelect(const State *state);
VkPresentModeKHR surfacePresentModesSelect(const State *state);
void swapchainImagesGet(State *state);
void swapchainCreate(State *state);
void swapchainDestroy(State *state);
void windowCreate(State *state);
void windowDestroy(State *state);
#endif // !WINDOW_HEADERS_INCLUDED
