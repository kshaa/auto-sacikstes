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

// Socket traffic
char sendBuff[SEND_BUFF_SIZE];
char recvBuff[RECV_BUFF_SIZE];

int initClientSocket(uint32_t address, int port, int type) {
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

    // Create, and connect to a socket
    int serverfd = socket(AF_INET, type, 0);
    if (serverfd == -1) {
        fprintf(stderr, "[socket] Couldn't create socket for server connection: %s\n", strerror(errno));
        return failurefd;
    }

    // Only TCP requires an actual connection establishment
    // UDP is too cool for reliability
    if (type == SOCK_STREAM) {
        int connectResult = connect(serverfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
        if (connectResult == -1) {
            fprintf(stderr, "[socket] Couldn't connect to server: %s\n", strerror(errno));
            return failurefd;
        }
    }

    // Set socket as non-blocking
    // fcntl(serverfd, F_SETFL, O_NONBLOCK);

    return serverfd;
}
