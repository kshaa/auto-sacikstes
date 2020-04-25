#ifndef LOBBY_H
#define LOBBY_H

#include "../../common/protocol/protocol_types.h"

typedef struct {
    int success;
    int gameID;
    int playerID;
    char playerPassword[PROTOCOL_MAX_PASSWORD_LENGTH];
} GameCreationResult;

int getGameCount();
GameCreationResult createGame(char * gameName, char * playerName, int fieldID);

#endif