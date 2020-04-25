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
    } else if (isMessageType(recvBuff, PROTOCOL_LIST_GAMES_TYPE)) {
        printf("[router] Routing to lobby game list\n");
        success = routeGameList(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else if (isMessageType(recvBuff, PROTOCOL_CREATE_GAME_TYPE)) {
        printf("[router] Routing to lobby create game\n");
        success = routeGameCreate(connectionfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else {
        printf("[router] Routing to error w/ unknown type\n");
        success = routeError(connectionfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_INCORRECT_TYPE);
    }

    // Generate server internal error message if needed
    if (!success) {
        fprintf(stderr, "[router] Message route failed for connection %d: %s\n", connectionfd, strerror(errno));
        fprintf(stderr, "[router] Routing to error w/ internal server error\n");
        success = routeError(connectionfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_SERVER_IS_SAD);
        
        // Even generating internal server error failed
        if (!success) {
            fprintf(stderr, "[router] Internal server error response failed for connection %d: %s\n", connectionfd, strerror(errno));
            return success;
        }
    }

    // Send message
    printf("[router] Responding to connection %d regarding route %s\n", connectionfd, getVolatilePrintableResponseType(recvBuff));
    int sendSuccess = sendMessage(connectionfd, sendBuff, sizeof(sendBuff), MSG_DONTWAIT);
    if (!sendSuccess) {
        success = 0;
        fprintf(stderr, "[router] Sending response failed for connection %d: %s\n", connectionfd, strerror(errno));
    }
    
    return success;
}

int handleTraffic() {
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
            fprintf(stderr, "[traffic] Connection with %d seems broken, killing it\n", connfd);
        }
    }; 

    return success;
}