#ifndef STATE_H
#define STATE_H

#include "SDL2/SDL.h"
#include "const.h"
#include "../common/state.h"
#include "../common/protocol/protocol_types.h"
#include "scene.h"

// SDL window
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int oldWidth;
    int oldHeight;
    int resized;
} App;

// Player
typedef struct {
    int created;
    ProtocolPlayerInfo info;
} Player;

// Game
typedef struct {
    Field field;
    ProtocolAction controls;
    ProtocolGameInfo info;
    int selfPlayerID;
    char selfPlayerPassword[PROTOCOL_MAX_PASSWORD_LENGTH];
    Player player[MAX_GAME_PLAYERS];
} Game;

// SDL Game scene
typedef struct {
    int decorationCount;
    SceneWall * decorations;
    int wallCount;
    SceneWall * walls;
} Scene;

// Global state
extern Server server;
extern App app;
extern Game game;
extern Scene scene;

// Initialize global state
int initState();

// Initialize server connection
int initConnection();

#endif