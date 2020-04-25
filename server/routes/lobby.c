#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../../common/protocol/protocol_messages.h"
#include "../state.h"
#include <stdio.h>

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