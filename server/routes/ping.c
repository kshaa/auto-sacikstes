#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../../common/protocol_messages.h"

int routePing(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff) {
    ProtocolPongResponse pong;
    strncpy(pong.type, PROTOCOL_PONG_TYPE, sizeof(PROTOCOL_PONG_TYPE));
    memcpy(sendBuff, &pong, sizeof(pong));

    int canSend = sizeof(pong) <= sendBuffSize;
    if (canSend) {
        memcpy(sendBuff, &pong, sizeof(pong));
        return 1;
    } else {
        fprintf(stderr, "[pong] Can't send message, buffer too small, fd: %d", connfd);
        return 0;
    }
}