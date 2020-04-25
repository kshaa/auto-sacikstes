#include "../state.h"
#include "../../common/protocol/protocol_messages.h"
#include "../../common/networking/fetch.h"
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
    printf("[listgames] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    if (isMessageType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        // Request success
        return getProtocolListGamesResponseCount(recvBuff, RECV_BUFF_SIZE);
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