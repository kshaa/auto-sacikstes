#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../../common/protocol/protocol_messages.h"
#include "../state.h"
#include <stdio.h>
#include "../logic/lobby.h"

int routeGameList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
    int gamesCount = getGameCount();
    int canSend = sizeof(ProtocolListGamesResponse) + sizeof(int) * gamesCount <= sendBuffSize;
    if (!canSend) {
        return 0;
    }

    // Generate response
    ProtocolListGamesResponse response;
    strncpy(response.type, PROTOCOL_LIST_GAMES_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));
    response.gameIDsCount = gamesCount;
    response.gameIDs = malloc(sizeof(int) * gamesCount);
    // Serialize it
    serializeProtocolListGamesResponse(&response, sendBuff, sendBuffSize);
    // Collect garbage
    free(response.gameIDs);

    return 1;
}

int routeGameCreate(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
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