#ifndef LOBBY_H
#define LOBBY_H

#include "../../common/protocol/protocol_messages.h"
#include "../../common/protocol/protocol_types.h"

int getGameIDs(ProtocolListGamesResponse * buff, int copyOnlyCount);
int createGame(ProtocolCreateGameResponse * buff, char * gameName, char * playerName, int fieldID);

#endif