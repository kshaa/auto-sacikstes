#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "../const.h"
#include "socket.h"

// Socket connections
int serverfd = CLOSED_CONNECTION;
int serverfdUDP = CLOSED_CONNECTION;
int connectionCount = 0;
int connectionfds[MAX_CONNECTIONS];

int initServerSocket(uint32_t address, int port, int type) {
    int failurefd = -1;

    if (!(type == SOCK_STREAM || type == SOCK_DGRAM)) {
        fprintf(stderr, "[socket] Trying to create a socket of type I wasn't built for [%d]\n", type);
        return failurefd;
    }

    // Build server listening address
    struct sockaddr_in serv_addr; 
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(address);
    serv_addr.sin_port = htons(port); 

    // Create, bind and listen to a socket
    int listenfd = socket(AF_INET, type, 0);
    if (listenfd == -1) {
        fprintf(stderr, "[socket] Couldn't create socket: %s\n", strerror(errno));
        return failurefd;
    }
    int bindResult = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    if (bindResult == -1) {
        fprintf(stderr, "[socket] Couldn't bind socket: %s\n", strerror(errno));
        return failurefd;
    }
    // TCP streams require listen for accepting connections later
    if (type == SOCK_STREAM) {
        int listenResult = listen(listenfd, MAX_PENDING_CONNECTIONS);
        if (listenResult == -1) {
            fprintf(stderr, "[socket] Couldn't listen to socket: %s\n", strerror(errno));
            return failurefd;
        }
    }

    // Set socket as non-blocking
    fcntl(listenfd, F_SETFL, O_NONBLOCK);

    return listenfd;
}

int acceptConnections() {
    // Accept a new connections
    int connectionsAvailable = 1;
    int success = 1;
    while (connectionsAvailable) {
        int connfd = accept(serverfd, NULL, NULL);
        if (connfd == -1) {
            // Couldn't accept any connections
            connectionsAvailable = 0;
            if (errno != EWOULDBLOCK) {
                success = 0;
                // Legitimate error happened while accepting a connection
                fprintf(stderr, "[socket] Socket connection error: %s\n", strerror(errno));
            }
        } else {
            // Connection established
            connectionfds[connectionCount++] = connfd;
            if (DEBUG) printf("[socket] Accepted connection, fd: %d\n", connfd);
        }
    }

    return success;
}
