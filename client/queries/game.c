#include "../../common/protocol/protocol_messages.h"
#include "../../common/networking/fetch.h"
#include "../state.h"
#include "../const.h"

int getGameCount() {
    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolListGamesRequest request;
    strncpy(request.type, PROTOCOL_LIST_GAMES_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));

    // Fetch games
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolListGamesRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[listgames] List games request failed\n");
        return -1;
    }

    // Process response
    if (isMessageType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        // Request success
        ProtocolListGamesResponse * response = (ProtocolListGamesResponse *) recvBuff;
        if (DEBUG) printf("[listgames] There are %d games currently running\n", response->gameIDsCount);
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[listgames] List games failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[listgames] List games failed w/ unknown error\n");
        }

        return -1;
    }
}