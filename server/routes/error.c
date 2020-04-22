#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../sockets.h"

int routeError(int connfd) {
    int success = 1;
    memset(sendBuff, 0, sizeof(sendBuff));

    int sendResult = send(connfd, sendBuff, sizeof(sendBuff), MSG_DONTWAIT);
    if (sendResult == -1) {
        success = 0;
        fprintf(stderr, "[socket] Error response failed for connection %d: %s\n", connfd, strerror(errno));
    }

    return success;
}