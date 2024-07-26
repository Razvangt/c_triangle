
#ifndef WINDOW_HEADER
#define WINDOW_HEADER
#include "EngineCommons.h"

void initWindow(struct EngineState* state);
void cleanWindow(struct EngineState* state);
bool shouldWindowClose(struct EngineState* state);
#endif // !WINDOW_HEADER
