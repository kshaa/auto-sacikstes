#ifndef STATE_H
#define STATE_H

#include "SDL2/SDL.h"
#include "../common/state.h"
#include "../common/protocol/protocol_types.h"

// SDL window
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

// Global state
extern Server server;
extern App app;
extern Field field;
extern ProtocolAction controls;

// Initialize global state
int initState();

// Initialize server connection
int initConnection();

#endif