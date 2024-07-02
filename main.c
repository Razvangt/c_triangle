#include "src/toolkit.h"

int main() {
  State state = {
      .config =
          {
              .applicationName = "Triangle",
              .engineName = "Triangle ENGINE",
              .windowTitle = "Triangle",
              .windowWidth = 720,
              .windowHeight = 480,
              .windowFullscreen = false,
              .apiVersion = VK_API_VERSION_1_3,
              .swapchainComponentsMapping = (VkComponentMapping){},
              .swapchainBuffering = SWAPCHAIN_TRIPLE_BUFFERING,
          },
  };

  init(&state);
  loop(&state);
  cleanup(&state);

  return EXIT_SUCCESS;
}
