
#include "src/Headers.h"
#include "src/HelloTrianngleApplication.h"
int main() {
  printf("Engine Started\n");
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  printf("Engine Finished\n");
  return EXIT_SUCCESS;
}
