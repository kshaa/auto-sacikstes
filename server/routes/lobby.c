#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../../common/protocol/protocol_messages.h"
#include "../state.h"
#include <stdio.h>
#include "../logic/lobby.h"

int routeFieldList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
    // Generate response
    ProtocolListFieldsResponse response;
    strncpy(response.type, PROTOCOL_LIST_FIELDS_TYPE, sizeof(PROTOCOL_LIST_FIELDS_TYPE));

    // Temporarily hardcoded
    response.fieldCount = 1;

    // Send response
    memcpy(sendBuff, &response, sendBuffSize);

    return 1;
}

int routeGameList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
    // Generate response
    ProtocolListGamesResponse response;
    strncpy(response.type, PROTOCOL_LIST_GAMES_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));
    response.gameIDs = malloc(sizeof(int) * getGameCount());
    int gamesCount = 0;
    for (int i = 0; i < MAX_GAMES; i++) {
        if (games[i].created) {
            response.gameIDs[gamesCount] = i;
            gamesCount++;
        }
    }
    response.gameIDsCount = gamesCount;

    // Serialize it
    serializeProtocolListGamesResponse(&response, sendBuff, sendBuffSize);

    // Collect garbage
    free(response.gameIDs);

    return 1;
}

int routeGameCreate(int connfd, char * recvBuff, size_t sendBuffSize, unsigned char * sendBuff) {
    int gamesCount = getGameCount();
    int canSend = sizeof(ProtocolCreateGameResponse) <= sendBuffSize;
    if (!canSend) {
        return 0;
    }

    // Parse request
    ProtocolCreateGameRequest * request = (ProtocolCreateGameRequest *) recvBuff;

    // Create game
    int gameID = createGame(request->gameName, request->fieldID);
    if (gameID == -1) {
        // Todo: Send error response
    }

    // Add game creator player
    int playerID = addGamePlayer(gameID, request->playerName, connfd);
    if (playerID == -1) {
        // Todo: Send error response
    }

    // Generate response
    ProtocolCreateGameResponse response;
    strncpy(response.type, PROTOCOL_CREATE_GAME_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));
    response.gameID = gameID;
    response.playerID = playerID;
    strncpy(response.playerPassword, games[gameID].player[playerID].password, PROTOCOL_MAX_PASSWORD_LENGTH);
    memcpy(sendBuff, &response, sendBuffSize);

    return 1;
}

int routeStartGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
    int canSend = sizeof(ProtocolStartGameResponse) <= sendBuffSize;
    if (!canSend) {
        return 0;
    }

    // Parse request
    ProtocolStartGameRequest * request = (ProtocolStartGameRequest *) recvBuff;

    // Start game
    games[request->gameID].info.status = STARTED;

    // Generate response
    ProtocolStartGameResponse response;
    strncpy(response.type, PROTOCOL_START_GAME_TYPE, sizeof(PROTOCOL_START_GAME_TYPE));
    response.playerInfoCount = getGamePlayerCount(request->gameID);
    response.playerInfos = malloc(sizeof(ProtocolPlayerInfo) * response.playerInfoCount);
    for (int i = 0; i < response.playerInfoCount; i++) {
        memcpy(&response.playerInfos[i], &games[request->gameID].player[i].info, sizeof(ProtocolPlayerInfo));
    }
    memcpy(&response.field, &games[request->gameID].field.info, sizeof(ProtocolFieldInfo));
    response.extraLines = malloc(sizeof(ProtocolLine) * games[request->gameID].field.extraLinesCount);
    memcpy(response.extraLines, games[request->gameID].field.extraLines, sizeof(ProtocolLine) * games[request->gameID].field.extraLinesCount);
    memcpy(&response.startLine, &games[request->gameID].field.startLine, sizeof(ProtocolLine));
    memcpy(&response.mainLine, &games[request->gameID].field.mainLine, sizeof(ProtocolLine));
    response.extraLineCount = games[request->gameID].field.extraLinesCount;

    // Serialize it
    serializeProtocolStartGameResponse(&response, sendBuff, sendBuffSize);
    free(response.playerInfos);
    free(response.extraLines);

    return 1;
}
