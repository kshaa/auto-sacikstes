#include "../../common/protocol_messages.h"
#include "../../common/fetch.h"
#include "../state.h"

int getGameCount() {
    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolListGamesRequest request;
    strncpy(request.type, PROTOCOL_LIST_GAMES_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));

    // Fetch games
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolListGamesRequest));
    if (!recvBuff) {
        printf("[listgames] List games request failed\n");
        return -1;
    }

    // Process response
    if (isResponseType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        // Request success
        ProtocolListGamesResponse * response = (ProtocolListGamesResponse *) recvBuff;
        printf("[listgames] There are %d games currently running\n", response->gameIDsCount);
    } else {
        // Request error
        if (isResponseType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            printf("[listgames] List games failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            printf("[listgames] List games failed w/ unknown error\n");
        }

        return -1;
    }
}