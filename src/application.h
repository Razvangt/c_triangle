#ifndef APPLICATION_HEADERS_INCLUDED
#define APPLICATION_HEADERS_INCLUDED
#include "context/context.h"
#include "headers.h"
#include "render/renderer.h"
#include "window/window.h"
void init(State *state);
void loop(State *state);
void cleanup(State *state);
#endif // !APPLICATION_HEADERS_INCLUDED
