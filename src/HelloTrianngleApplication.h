#ifndef APP_HEADERS
#define APP_HEADERS
#include "Headers.h"
#include <vector>
class HelloTriangleApplication {
public:
  const uint32_t WIDTH = 800;
  const uint32_t HEIGHT = 600;

private:
  GLFWwindow *window;
  VkInstance instance;

public:
  void run();

private:
  void initWindow();
  void initVulkan();
  void createInstance();
  void mainLoop();
  void cleanup();
};
#endif // !APP_HEADERS
