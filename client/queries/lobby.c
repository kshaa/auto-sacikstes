#include "lobby.h"
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

    // Send request
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

GameCreationResult createGame(char * gameName, char * playerName, int fieldID) {
    GameCreationResult result;
    result.success = 1;

    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolCreateGameRequest request;
    strncpy(request.type, PROTOCOL_CREATE_GAME_TYPE, sizeof(PROTOCOL_CREATE_GAME_TYPE));
    strncpy(request.gameName, gameName, sizeof(PROTOCOL_MAX_GAME_NAME));
    strncpy(request.playerName, playerName, sizeof(PROTOCOL_MAX_PLAYER_NAME));

    request.fieldID = fieldID;

    // Send request
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolCreateGameRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[creategame] Create game request failed\n");
        result.success = 0;
        return result;
    }

    // Process response
    printf("[listgames] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    fflush(stdout);
    if (isMessageType(recvBuff, PROTOCOL_CREATE_GAME_TYPE)) {
        // Request success
        ProtocolCreateGameResponse * response = (ProtocolCreateGameResponse *) recvBuff;
        result.gameID = response->gameID;
        result.playerID = response->playerID;
        strncpy(result.playerPassword, response->playerPassword, PROTOCOL_MAX_PASSWORD_LENGTH);
        return result;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[listgames] List games failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[listgames] List games failed w/ unknown error\n");
        }

        result.success = 0;
        return result;
    }
}