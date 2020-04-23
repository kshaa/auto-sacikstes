#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../../common/protocol_messages.h"

int routeError(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff, char errorCode[2]) {
    ProtocolErrorResponse error;
    strncpy(error.type, PROTOCOL_ERROR_TYPE, sizeof(PROTOCOL_ERROR_TYPE));
    strncpy(error.errorCode, errorCode, 2);

    int canSend = sizeof(error) <= sendBuffSize;
    if (canSend) {
        memcpy(sendBuff, &error, sizeof(ProtocolErrorResponse));
        return 1;
    } else {
        fprintf(stderr, "[error] Can't send message, buffer too small, fd: %d", connfd);
        return 0;
    }
}