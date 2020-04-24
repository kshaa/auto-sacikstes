#ifndef STATE_H
#define STATE_H

#include "const.h"
#include "../common/protocol/protocol_types.h"
#include "../common/state.h"

// Player state
typedef struct {
    ProtocolPlayerInfo info;
    int connectionfd;
} Player;

// Game state
typedef struct {
    Field field;
    Player player[MAX_GAME_PLAYERS];
} Game;

// Global state
extern Server server;
extern Game games[MAX_GAMES];

// Initialize global state
int initState();

// Initialize server listener
int initConnection();

#endif