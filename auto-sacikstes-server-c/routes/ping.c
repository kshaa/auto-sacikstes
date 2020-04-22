#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../sockets.h"
#include "../../common/protocol_messages.h"

int routePing(int connfd) {
    // Clean send buffer
    int success = 1;
    memset(sendBuff, 0, sizeof(sendBuff));

    // Prepare pong message
    ProtocolPongResponse pong;
    strcpy(pong.type, PROTOCOL_PONG_TYPE);
    memcpy(sendBuff, &pong, sizeof(pong));

    // Send message
    int sendResult = send(connfd, sendBuff, sizeof(sendBuff), MSG_DONTWAIT);
    if (sendResult == -1) {
        success = 0;
        fprintf(stderr, "[socket] Pong response failed for connection %d: %s\n", connfd, strerror(errno));
    }

    return success;
}