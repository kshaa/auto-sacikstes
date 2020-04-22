#include "sockets.h"
#include "routes/ping.h"
#include "routes/error.h"
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

int routeTraffic(int connfd) {
    int success = 1;
    // Copy route from receive buffer
    char route[2];
    memcpy(route, recvBuff, sizeof(route));

    // Notify of incoming traffic
    if (*recvBuff != '\0') {
        printf("[router] Received message from %d w/ route %s\n", connfd, recvBuff);
    }

    // Skip empty messages
    if (strcmp(route, "\0\0") == 0) {
        return success;
    }

    // Route message to handler
    if (strcmp(route, "PI") == 0) {
        printf("[router] Routing to ping\n");
        routePing(connfd);
    } else {
        printf("[router] Routing to error\n");
        routeError(connfd);
    }
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

        routeTraffic(connfd);
    }; 

    return success;
}