#include "Window.h"

void initWindow(struct EngineState* state) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    state->context.window.handle =
    glfwCreateWindow(state->config.w_width, state->config.w_height, state->config.windowTitle, NULL, NULL);
}
void cleanWindow(struct EngineState* state) {
    glfwDestroyWindow(state->context.window.handle);
    glfwTerminate();
}

bool shouldWindowClose(struct EngineState* state) {
    return glfwWindowShouldClose(state->context.window.handle);
}
