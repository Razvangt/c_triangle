#include "Engine.h"
#include "errors.h"
#include "tools.h"


ErrorCode init(struct EngineState* state) {
    struct Context context;
    context.shouldClose = false;
#ifdef NDEBUG
    context.enableValidationLayers = false;
#else
    context.enableValidationLayers = true;
#endif
    state->context.physicalDevice = VK_NULL_HANDLE;

    // Not Bad there might be some suggar Syntax for this  A macro for sure 
    struct StringArray valLayers =   create_string_array( (const char* []) {"VK_LAYER_KHRONOS_validation",NULL},1);
    struct StringArray deviceExtensions =   create_string_array( (const char* []) {"VK_KHR_SWAPCHAIN_EXTENSION_NAME",NULL},1);

    state->context.validationLayers =  &valLayers;
    state->context.deviceExtensions = &deviceExtensions;
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
