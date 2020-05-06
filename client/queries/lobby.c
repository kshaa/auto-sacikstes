#include "lobby.h"
#include "../state.h"
#include "../../common/protocol/protocol_messages.h"
#include "../../common/networking/fetch.h"
#include "../const.h"
#include <errno.h>
#include <sys/socket.h>

int getFieldCount(ProtocolListFieldsResponse * buff) {
    int success = 1;
    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolListFieldsRequest request;
    strncpy(request.type, PROTOCOL_LIST_FIELDS_TYPE, sizeof(PROTOCOL_LIST_FIELDS_TYPE));

    // Send request
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolListFieldsRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[listfields] List fields request failed\n");
        success = 0;
        return success;
    }

    // Process response
    printf("[listgames] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    if (isMessageType(recvBuff, PROTOCOL_LIST_FIELDS_TYPE)) {
        memcpy(buff, recvBuff, sizeof(ProtocolListFieldsResponse));
        return success;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[listfields] List fields failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[listfields] List fields failed w/ unknown error\n");
        }
        success = 0;
        return success;
    }
}

int getGameIDs(ProtocolListGamesResponse * buff, int copyOnlyCount) {
    int success = 1;
    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolListGamesRequest request;
    strncpy(request.type, PROTOCOL_LIST_GAMES_TYPE, sizeof(PROTOCOL_LIST_GAMES_TYPE));

    // Send request
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolListGamesRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[listgames] List games request failed\n");
        success = 0;
        return success;
    }

    // Process response
    printf("[listgames] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    if (isMessageType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        // Request success
        int gameCount = getProtocolListGamesResponseCount(recvBuff, RECV_BUFF_SIZE);
        if (copyOnlyCount) {
            buff->gameIDsCount = gameCount;
        } else {
            unserializeProtocolListGamesResponse(recvBuff, RECV_BUFF_SIZE, buff);
        }
        return success;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[listgames] List games failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[listgames] List games failed w/ unknown error\n");
        }
        success = 0;
        return success;
    }
}

int createGame(ProtocolCreateGameResponse * buff, char * gameName, char * playerName, int fieldID) {
    int success = 1;

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
        success = 0;
        return success;
    }

    // Process response
    printf("[listgames] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    fflush(stdout);
    if (isMessageType(recvBuff, PROTOCOL_CREATE_GAME_TYPE)) {
        // Request success
        memcpy(buff, recvBuff, sizeof(ProtocolCreateGameResponse));
        return success;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[listgames] List games failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[listgames] List games failed w/ unknown error\n");
        }

        success = 0;
        return success;
    }
}

int joinGame(ProtocolJoinGameResponse * buff, int gameID, char * playerName)
{
    int success = 1;

    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolJoinGameRequest request;
    strncpy(request.type, PROTOCOL_JOIN_GAME_TYPE, sizeof(PROTOCOL_JOIN_GAME_TYPE));
    strncpy(request.playerName, playerName, sizeof(PROTOCOL_MAX_PLAYER_NAME));

    request.gameID = gameID;

    // Send request
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolJoinGameRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[joingame] Join game request failed\n");
        success = 0;
        return success;
    }

    // Process response
    printf("[joingame] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    fflush(stdout);
    if (isMessageType(recvBuff, PROTOCOL_JOIN_GAME_TYPE)) {
        // Request success
        memcpy(buff, recvBuff, sizeof(ProtocolJoinGameResponse));
        return success;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[joingame] Join game failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[joingame] Join game failed w/ unknown error\n");
        }

        success = 0;
        return success;
    }
}

int startGame(ProtocolStartGameResponse * buff, int gameID, char * playerPassword) {
    int success = 1;

    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolStartGameRequest request;
    strncpy(request.type, PROTOCOL_START_GAME_TYPE, sizeof(PROTOCOL_START_GAME_TYPE));
    request.gameID = gameID;
    strncpy(request.playerPassword, playerPassword, sizeof(PROTOCOL_MAX_PASSWORD_LENGTH));

    // Send request
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolStartGameRequest));
    if (!recvBuff) {
        if (DEBUG) printf("[startgame] Start game request failed\n");
        success = 0;
        return success;
    }

    // Process response
    printf("[startgame] Received: %s\n", getVolatilePrintableResponseType(recvBuff));
    fflush(stdout);
    if (isMessageType(recvBuff, PROTOCOL_START_GAME_TYPE)) {
        // Request success
        unserializeProtocolStartGameResponse(recvBuff, RECV_BUFF_SIZE, buff);
        return success;
    } else {
        // Request error
        if (isMessageType(recvBuff, PROTOCOL_ERROR_TYPE)) {
            ProtocolErrorResponse * error = (ProtocolErrorResponse *) recvBuff;
            if (DEBUG) printf("[startgame] Start game failed w/ error: %s\n", getVolatileErrorMessage(error->errorCode));
        } else {
            if (DEBUG) printf("[startgame] Start game failed w/ unknown error\n");
        }

        success = 0;
        return success;
    }
}

int waitForStart(ProtocolStartGameResponse * buff) {
    errno = 0;

    // Use connection from global client state
    int connectionfd = server.fd;
   
        // Receive incoming message
        unsigned char * recvBuff = receiveMessage(connectionfd, MSG_DONTWAIT); // TODO: NOT SURE IF BAD
        if (!recvBuff) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // No message to receive in queue
                // Or socket temporarily unavailable
                // Regardless - just try later
                return 0;
            } else {
                // Failed to receive message
                printf("[waiting] Receiving game start request failed: [%d] %s\n", errno, strerror(errno));
            }
        }

        if (isMessageType(recvBuff, "\0\0")) {
            // Skip empty messages
            return 0;
        }

        // Parse request
        unsigned char sendBuff[SEND_BUFF_SIZE];
        if (isMessageType(recvBuff, PROTOCOL_START_GAME_TYPE)) {
            unserializeProtocolStartGameResponse(recvBuff, RECV_BUFF_SIZE, buff);
            return 1;
        }

    return 0;
}