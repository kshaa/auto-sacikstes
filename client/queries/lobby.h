#ifndef LOBBY_H
#define LOBBY_H

#include "../../common/protocol/protocol_messages.h"
#include "../../common/protocol/protocol_types.h"

int getFieldCount(ProtocolListFieldsResponse * buff);
int getGameIDs(ProtocolListGamesResponse * buff, int copyOnlyCount);
int createGame(ProtocolCreateGameResponse * buff, char * gameName, char * playerName, int fieldID);
int joinGame(ProtocolJoinGameResponse * buff, int gameID, char * playerName);
int startGame(ProtocolStartGameResponse * buff, int gameID, char * playerPassword);
int waitForStart(ProtocolStartGameResponse * buff);

#endif