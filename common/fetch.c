#include "const.h"
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Message types
unsigned char volatileResponseType[2];
unsigned char * getVolatileResponseType(void * message) {
    memcpy(volatileResponseType, message, 2);
    return volatileResponseType;
}

unsigned char volatilePrintableResponseType[3];
unsigned char * getVolatilePrintableResponseType(void * message) {
    memcpy(volatileResponseType, message, 2);
    volatilePrintableResponseType[2] = '\0';
    return volatilePrintableResponseType;
}

int isResponseType(unsigned char * a, unsigned char * b) {
    return strncmp(a, b, 2) == 0;
}

// Message error codes
int isErrorCode(char * a, char * b) {
    return strncmp(a, b, 2) == 0;
}

// Network buffers
unsigned char sendBuff[COMMON_SEND_BUFF_SIZE];
unsigned char recvBuff[COMMON_SEND_BUFF_SIZE];
int loadSendBuff(void * buff, size_t buffSize) {
    if (buffSize > COMMON_SEND_BUFF_SIZE) {
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
        if (COMMON_DEBUG) fprintf(stderr, "[network] Network request preparation (loadSendBuff) failed: %s.\n", strerror(errno));
        return 0;
    }

    // Send message
    int sendResult = send(connectionfd, &sendBuff, sizeof(sendBuff), flags);
    if (sendResult == -1) {
        if (COMMON_DEBUG) fprintf(stderr, "[network] Network request (send) failed: %s.\n", strerror(errno));
        return 0;
    }

    return 1;
}

unsigned char * receiveMessage(int connectionfd, int flags) {
    memset(recvBuff, 0, sizeof(recvBuff));
    int recvResult = recv(connectionfd, recvBuff, sizeof(recvBuff), flags);
    if (recvResult == -1) {
        if (COMMON_DEBUG) fprintf(stderr, "[network] Network response (recv) failed: %s.\n", strerror(errno));
        return NULL;
    }

    return recvBuff;
}

unsigned char * fetch(int connectionfd, void * buff, size_t buffSize) {
    // Send message
    int sendSuccess = sendMessage(connectionfd, buff, buffSize, 0);
    if (!sendSuccess) {
        if (COMMON_DEBUG) fprintf(stderr, "[network] Network request (send) failed: %s.\n", strerror(errno));
        return NULL;
    }

    // Receive response
    memset(recvBuff, 0, sizeof(recvBuff));
    unsigned char * recvBuff = receiveMessage(connectionfd, 0);
    if (!recvBuff) {
        if (COMMON_DEBUG) fprintf(stderr, "[network] Network response (recv) failed: %s.\n", strerror(errno));
        return NULL;
    }

    return recvBuff;
}

