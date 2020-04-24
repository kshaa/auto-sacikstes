#include "fetch.h"
#include <string.h>

// Ping
char PROTOCOL_PING_TYPE[] = { 'P', 'I' };

// Pong
char PROTOCOL_PONG_TYPE[] = { 'P', 'O' };

// Error codes
char PROTOCOL_ERROR_CODE_INCORRECT_TYPE[] = { 'I', 'T' };
char PROTOCOL_ERROR_CODE_SERVER_IS_SAD[] = { 'S', 'S' };

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