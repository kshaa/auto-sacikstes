#ifndef STATE_H
#define STATE_H

#include "const.h"
#include "../common/protocol/protocol_types.h"
#include "../common/state.h"

// Player state
typedef struct {
    int created;
    int connectionfd;
    char password[PROTOCOL_MAX_PASSWORD_LENGTH];
    ProtocolPlayerInfo info;
} Player;

// Game state
typedef struct {
    int created;
    ProtocolGameInfo info;
    Field field;
    Player player[MAX_GAME_PLAYERS];
} Game;

// Global state
extern Server server;
extern Game games[MAX_GAMES];
int getGameCount();

// Initialize global state
int initState();

// Initialize server listener
int initConnection();

#endif