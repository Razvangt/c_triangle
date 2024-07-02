#ifndef UTILS_HEADERS_INCLUDED
#define UTILS_HEADERS_INCLUDED
#include "../headers.h"

#define EXPECT(ERROR, FORMAT, ...)                                             \
  {                                                                            \
    int macroErrorCode = ERROR;                                                \
    if (macroErrorCode) {                                                      \
      fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n",          \
              __FILE_NAME__, __func__, __LINE__, macroErrorCode,               \
              ##__VA_ARGS__);                                                  \
      raise(SIGABRT);                                                          \
    }                                                                          \
  }

void infoLog();
void exitCallback();
void glfwErrorCallback(int errorCode, const char *description);
void errorHandlingSetup();
uint32_t clamp(uint32_t value, uint32_t min, uint32_t max);
#endif // !UTILS_HEADERS_INCLUDED
