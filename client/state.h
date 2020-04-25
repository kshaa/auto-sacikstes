#ifndef STATE_H
#define STATE_H

#include "SDL2/SDL.h"
#include "const.h"
#include "../common/state.h"
#include "../common/protocol/protocol_types.h"

// SDL window
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

// Player
typedef struct {
    int created;
    char password[PROTOCOL_MAX_PASSWORD_LENGTH];
    ProtocolPlayerInfo info;
} Player;

// Game
typedef struct {
    Field field;
    ProtocolAction controls;
    ProtocolGameInfo info;
    Player player[MAX_GAME_PLAYERS];
} Game;

// Global state
extern Server server;
extern App app;
extern Game game;

// Initialize global state
int initState();

// Initialize server connection
int initConnection();

#endif