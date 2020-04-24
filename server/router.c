#include "socket.h"
#include "../common/protocol_messages.h"
#include "../common/fetch.h"
#include "routes/ping.h"
#include "routes/error.h"
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

int routeTraffic(int connfd) {
    int success = 1;
    // Copy route from receive buffer
    char route[2];
    route[0] = toupper(*(recvBuff));
    route[1] = toupper(*(recvBuff + 1));

    // Notify of incoming traffic
    if (*recvBuff != '\0') {
        char a = route[0];
        char b = route[1];
        if (a < 'A' || a > 'Z') a = '?';
        if (b < 'A' || b > 'Z') b = '?';
        printf("[router] Received message from %d w/ route: %c%c\n", connfd, a, b);
    }

    // Skip empty messages
    if (strcmp(route, "\0\0") == 0) {
        return success;
    }

    // Clean the buffer
    memset(sendBuff, 0, sizeof(sendBuff));

    // Route message to handler
    if (strcmp(route, PROTOCOL_PING_TYPE) == 0) {
        printf("[router] Routing to ping\n");
        success = routePing(connfd, recvBuff, sizeof(sendBuff), sendBuff);
    } else {
        printf("[router] Routing to error\n");
        success = routeError(connfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_INCORRECT_TYPE);
    }

    // Generate server internal error message if needed
    if (!success) {
        fprintf(stderr, "[router] Message route failed for connection %d: %s\n", connfd, strerror(errno));
        success = routeError(connfd, recvBuff, sizeof(sendBuff), sendBuff, PROTOCOL_ERROR_CODE_SERVER_IS_SAD);
        
        // Even generating internal server error failed
        if (!success) {
            fprintf(stderr, "[router] Internal server error response failed for connection %d: %s\n", connfd, strerror(errno));
            return success;
        }
    }

    // Send message
    int sendResult = sendMessage(connfd, sendBuff, sizeof(sendBuff), MSG_DONTWAIT);
    // int sendResult = send(connfd, sendBuff, sizeof(sendBuff), MSG_DONTWAIT);
    if (sendResult == -1) {
        success = 0;
        fprintf(stderr, "[router] Sending response failed for connection %d: %s\n", connfd, strerror(errno));
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
        
        // Listen to incoming traffic
        memset(recvBuff, 0, sizeof(recvBuff));
        int recvResult = recv(connfd, recvBuff, sizeof(recvBuff), MSG_DONTWAIT);
        if (recvResult == -1 && errno != EWOULDBLOCK) {
            // Couldn't receive result, because error occured
            success = 0;
            fprintf(stderr, "[traffic] Connection segment %d read error: %s\n", connfd, strerror(errno));
        }

        success = success && routeTraffic(connfd);
    }; 

    return success;
}