#ifndef STATE_H
#define STATE_H

#include "SDL2/SDL.h"
#include "../common/protocol_types.h"

// Server
typedef struct {
    char addressSerialized[16];
    int address;
    int port;
    int fd;
} Server;

// SDL window
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

// Game field
typedef struct {
    ProtocolFieldInfo info;
    ProtocolLine startLine;
    ProtocolLine mainLine;
    size_t extraLinesCount;
    ProtocolLine * extraLines;
} Field;

// Global state
Server server;
App app;
Field field;
ProtocolAction controls;

// Initialize global state
int initState();

// Initialize server connection
int initConnection();

#endif