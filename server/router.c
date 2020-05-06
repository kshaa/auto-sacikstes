#include "networking/socket.h"
#include "../common/protocol/protocol_messages.h"
#include "../common/networking/fetch.h"
#include "state.h"
#include "routes/ping.h"
#include "routes/error.h"
#include "routes/lobby.h"
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

int routeTraffic(int connectionfd) {
    int success = 1;
    int globalFlag = 0;

    // Receive incoming message
    errno = 0;
    unsigned char * recvBuff = receiveMessage(connectionfd, MSG_DONTWAIT);
    if (!recvBuff) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // No message to receive in queue
            // Or socket temporarily unavailable
            // Regardless - just try later
            return success;
        } else {
            // Failed to receive message
            printf("[router] Receiving message failed: [%d] %s\n", errno, strerror(errno));
            success = 0;
            return success;
        }
    }

    if (isMessageType(recvBuff, "\0\0")) {
        // Skip empty messages
        return success;
    } else {
        // Notify of real messages
        printf("[router] Received message from %d w/ route: %s\n", connectionfd, getVolatilePrintableResponseType(recvBuff));
    }

    // Route message to handler
    unsigned char sendBuff[SEND_BUFF_SIZE];
    if (isMessageType(recvBuff, PROTOCOL_PING_TYPE)) {
        printf("[router] Routing to ping\n");
        success = routePing(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else if (isMessageType(recvBuff, PROTOCOL_LIST_FIELDS_TYPE)) {
        printf("[router] Routing to lobby field list\n");
        success = routeFieldList(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else if (isMessageType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        printf("[router] Routing to lobby game list\n");
        success = routeListGame(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else if (isMessageType(recvBuff, PROTOCOL_CREATE_GAME_TYPE)) {
        printf("[router] Routing to lobby create game\n");
        success = routeCreateGame(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else if (isMessageType(recvBuff, PROTOCOL_JOIN_GAME_TYPE)) {
        printf("[router] Routing to lobby join game\n");
        success = routeJoinGame(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
        // globalFlag = 1;
    } else if (isMessageType(recvBuff, PROTOCOL_START_GAME_TYPE)) {
        printf("[router] Routing to lobby start game\n");
        success = routeStartGame(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
        globalFlag = 1;
    } else {
        printf("[router] Routing to error w/ unknown type\n");
        success = routeError(connectionfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_INCORRECT_TYPE);
    }

    // Generate server internal error message if needed
    if (!success) {
        fprintf(stderr, "[router] Message route failed for connection %d\n", connectionfd);
        fprintf(stderr, "[router] Routing to error w/ internal server error\n");
        success = routeError(connectionfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_SERVER_IS_SAD);
        
        // Even generating internal server error failed
        if (!success) {
            fprintf(stderr, "[router] Internal server error response failed for connection %d: %s\n", connectionfd, strerror(errno));
            return success;
        }
    }

    if (globalFlag) {
        // Send response to all joined players
        int gameID;

        if (isMessageType(recvBuff, PROTOCOL_START_GAME_TYPE)) {
            gameID = ((ProtocolStartGameRequest *) recvBuff)->gameID;
        } else if (isMessageType(recvBuff, PROTOCOL_JOIN_GAME_TYPE)) {
            gameID = ((ProtocolJoinGameRequest *) recvBuff)->gameID;
        }

        for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
            if (games[gameID].player[i].created) {
                connectionfd = games[gameID].player[i].connectionfd;

                int sendSuccess = sendMessage(connectionfd, sendBuff, SEND_BUFF_SIZE, MSG_DONTWAIT);

                if (!sendSuccess) {
                    success = 0;
                    fprintf(stderr, "[router] Sending response failed for connection %d\n", connectionfd);
                }
            }
        }
    } else {
        // Send response to requester
        printf("[router] Responding to connection %d with %s regarding route %s\n", connectionfd, getVolatilePrintableResponseType(sendBuff), getVolatilePrintableResponseType(recvBuff));
        int sendSuccess = sendMessage(connectionfd, sendBuff, SEND_BUFF_SIZE, MSG_DONTWAIT);
        if (!sendSuccess) {
            success = 0;
            fprintf(stderr, "[router] Sending response failed for connection %d\n", connectionfd);
        }
    }
    
    return success;
}

int handleConnectionTraffic() {
    int success = 1;

    // Handle traffic for every connection
    for (int i = 0; i < connectionCount; i++) {
        int connfd = connectionfds[i];

        // Skip closed connections
        if (connfd == CLOSED_CONNECTION) continue;

        // Conditionally receive and process messages
        success = success && routeTraffic(connfd);
        
        // Kill dead connection
        if (!success && isConnectionBroken(errno)) {
            connectionfds[i] = CLOSED_CONNECTION;
            fprintf(stderr, "[traffic] Connection with %d seems broken, ignoring it\n", connfd);
            success = 1; // A broken connection isn't a server fault really.
        }
    }; 

    return success;
}

int handleConnectionlessTraffic() {
    int success = routeTraffic(serverfdUDP);

    return success;
}