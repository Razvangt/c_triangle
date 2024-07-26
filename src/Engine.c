#include "Engine.h"
#include "errors.h"


ErrorCode init(struct EngineState* state) {
    struct Context context;
    context.shouldClose = false;
#ifdef NDEBUG
    context.enableValidationLayers = false;
#else
    context.enableValidationLayers = true;
#endif
    state->context.physicalDevice = VK_NULL_HANDLE;

    state->context.validationLayers = g_ptr_array_new();
    g_ptr_array_add(&state->context.validationLayers, "VK_LAYER_KHRONOS_validation");
    state->context.deviceExtensions[0] = "VK_KHR_SWAPCHAIN_EXTENSION_NAME";
    state->context                     = context;

    SOR(initContext(state));
    initWindow(state);
    init_pipeline();
    return SUCCESS;
}
// TODO: REFACTOR WITH ErrorCode
ErrorCode mainLoop(struct EngineState* state) {
    while (!shouldWindowClose(state)) {
        glfwPollEvents();
    }
    return SUCCESS;
}

// TODO: REFACTOR WITH ErrorCode
ErrorCode cleanup(struct EngineState* state) {
    cleanWindow(state);
    endContext(state);
    return SUCCESS;
}


ErrorCode run(struct EngineState* state) {
    printf("Start Engine\n");
    SOR(init(state));
    SOR(mainLoop(state));
    SOR(cleanup(state));
    printf("Finish Engine\n");
    return SUCCESS;
}
