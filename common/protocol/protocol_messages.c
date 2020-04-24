#include "../protocol/protocol_messages.h"
#include <string.h>

// Message types
unsigned char volatileResponseType[2];
unsigned char * getVolatileResponseType(void * message) {
    memcpy(volatileResponseType, message, 2);
    return volatileResponseType;
}

unsigned char volatilePrintableResponseType[3];
unsigned char * getVolatilePrintableResponseType(void * message) {
    unsigned char a = ((unsigned char *) message)[0];
    unsigned char b = ((unsigned char *) message)[1];
    if (a < 'A' || a > 'Z') a = '?';
    if (b < 'A' || b > 'Z') b = '?';
    volatilePrintableResponseType[0] = a;
    volatilePrintableResponseType[1] = b;
    volatilePrintableResponseType[2] = '\0';
    return volatilePrintableResponseType;
}

int isMessageType(unsigned char * a, unsigned char * b) {
    return strncmp(a, b, 2) == 0;
}

// Ping
char PROTOCOL_PING_TYPE[] = { 'P', 'I' };

// Pong
char PROTOCOL_PONG_TYPE[] = { 'P', 'O' };

// Error codes
char PROTOCOL_ERROR_CODE_INCORRECT_TYPE[] = { 'I', 'T' };
char PROTOCOL_ERROR_CODE_SERVER_IS_SAD[] = { 'S', 'S' };
int isErrorCode(char * a, char * b) {
    return strncmp(a, b, 2) == 0;
}

// Error code messages
#define MAX_MESSAGE_SIZE 256
char PROTOCOL_ERROR_MESSAGE_INCORRECT_TYPE[MAX_MESSAGE_SIZE] = "Unknown request type";
char PROTOCOL_ERROR_MESSAGE_SERVER_IS_SAD[MAX_MESSAGE_SIZE] = "Server is sad";
char PROTOCOL_ERROR_MESSAGE_UNKNOWN[MAX_MESSAGE_SIZE] = "Unknown error code";
char * getVolatileErrorMessage(char errorCode[2]) {
    if (isErrorCode(errorCode, PROTOCOL_ERROR_CODE_INCORRECT_TYPE)) {
        return PROTOCOL_ERROR_MESSAGE_INCORRECT_TYPE;
    } else if (isErrorCode(errorCode, PROTOCOL_ERROR_CODE_SERVER_IS_SAD)) {
        return PROTOCOL_ERROR_MESSAGE_SERVER_IS_SAD;
    }

    return PROTOCOL_ERROR_MESSAGE_UNKNOWN;
}

// Error
char PROTOCOL_ERROR_TYPE[] = { 'E', 'R' };

// List games
char PROTOCOL_LIST_GAMES_TYPE[] = { 'L', 'I' };