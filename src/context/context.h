
#ifndef CONTEXT_HEADERS_INCLUDED
#define CONTEXT_HEADERS_INCLUDED()
#include "../headers.h"
#include "../utils/utils.h"
void instanceCreate(State *state);
void physicalDeviceSelect(State *state);
void queueFamilySelect(State *state);
void deviceCreate(State *state);
void queueGet(State *state);
void contextCreate(State *state);
void contextDestroy(State *state);
#endif // !CONTEXT_HEADERS_INCLUDED
