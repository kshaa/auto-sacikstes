#ifndef STATE_H
#define STATE_H

#include "const.h"
#include "../common/protocol_types.h"
#include "../common/state.h"

typedef struct {
    ProtocolPlayerInfo info;
    int connectionfd;
} Player;

// Server game state
typedef struct {
    Field field;
    Player player[MAX_GAME_PLAYERS];
} Game;

// Global state
extern Game games[MAX_GAMES];

// Initialize global state
int initState();

#endif