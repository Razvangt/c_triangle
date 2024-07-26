#include "Engine.h"


int main() {
    printf("APP Start\n");
    struct Config config;
    config.engineName      = "RazEngine";
    config.applicationName = "RazApp";
    config.windowTitle     = "WindowM";
    config.w_height        = 600;
    config.w_width         = 800;
    struct EngineState state;
    state.config  = config;
    ErrorCode err = run(&state);
    if (err != SUCCESS) {
        return EXIT_FAILURE;
        printf("APP Finished With Error %d\n", err);
    }
    printf("APP Finished\n");
    return EXIT_SUCCESS;
}
