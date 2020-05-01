#include "../const.h"
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Network buffers
unsigned char sendBuff[COMMON_SEND_BUFF_SIZE];
unsigned char recvBuff[COMMON_SEND_BUFF_SIZE];
int loadSendBuff(void * buff, size_t buffSize) {
    if (buffSize >= COMMON_SEND_BUFF_SIZE) {
        fprintf(stderr, "[network] Can't send request with buffer exceeding default size\n");
        return 0;
    }

    memcpy(sendBuff, buff, buffSize);
    return 1;
}

// Network requests
int sendMessage(int connectionfd, void * buff, size_t buffSize, int flags) {
    // Load message
    int loadSuccess = loadSendBuff(buff, buffSize);
    if (!loadSuccess) {
        if (COMMON_VERBOSE) fprintf(stderr, "[network] Network request preparation (loadSendBuff) failed.\n");
        return 0;
    }

    // Send message
    int sendResult = send(connectionfd, &sendBuff, sizeof(sendBuff), flags);
    if (sendResult == -1) {
        if (COMMON_VERBOSE) fprintf(stderr, "[network] Network message sending (send) failed: [%d] %s.\n", errno, strerror(errno));
        return 0;
    }

    return 1;
}

unsigned char * receiveMessage(int connectionfd, int flags) {
    memset(recvBuff, 0, sizeof(recvBuff));
    int recvResult = recv(connectionfd, recvBuff, sizeof(recvBuff), flags);
    if (recvResult == -1) {
        if (COMMON_VERBOSE) fprintf(stderr, "[network] Network message receiving (recv) failed: [%d] %s.\n", errno, strerror(errno));
        return NULL;
    }

    return recvBuff;
}

unsigned char * fetch(int connectionfd, void * buff, size_t buffSize) {
    // Send message
    int sendSuccess = sendMessage(connectionfd, buff, buffSize, 0);
    if (!sendSuccess) {
        return NULL;
    }

    // Receive response
    memset(recvBuff, 0, sizeof(recvBuff));
    unsigned char * recvBuff = receiveMessage(connectionfd, 0);
    if (!recvBuff) {
        return NULL;
    }

    return recvBuff;
}

int isConnectionBroken(int err) {
    return err == EPIPE;
}