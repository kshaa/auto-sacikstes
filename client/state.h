#ifndef STATE_H
#define STATE_H

#include "SDL2/SDL.h"
#include "../common/protocol_types.h"

// Global state types
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct {
    ProtocolFieldInfo info;
    ProtocolLine startLine;
    ProtocolLine mainLine;
    size_t extraLinesCount;
    ProtocolLine * extraLines;
} Field;

typedef struct {
    int up;
    int down;
    int left;
    int right;
} Controls;

// Global state
App app;
Field field;
Controls controls;

void initState();

#endif