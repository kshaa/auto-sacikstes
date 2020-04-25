#include "../protocol/protocol_messages.h"
#include <string.h>
#include "../const.h"
#include <stdio.h>

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

// Error
char PROTOCOL_ERROR_TYPE[] = { 'E', 'R' };

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

// List games
char PROTOCOL_LIST_GAMES_TYPE[] = { 'L', 'I' };
int getProtocolListGamesResponseCount(void * buff, size_t buffSize) {
    // Check if the buff is way too small
    if (buffSize < sizeof(ProtocolListGamesResponse)) {
        return -1;
    }

    return ((ProtocolListGamesResponse *) buff)->gameIDsCount;
}

void unserializeProtocolListGamesResponse(unsigned char * buff, size_t buffSize, ProtocolListGamesResponse * response) {
    // Check if the buff is way too small
    if (buffSize < sizeof(ProtocolListGamesResponse)) {
        return;
    }

    // Unserialize type
    strncpy(response->type, buff, 2);
    // Unserialize gameIDsCount
    response->gameIDsCount = ((ProtocolListGamesResponse *)buff)->gameIDsCount;
    // Unserialize gameIDs
    response->gameIDs = malloc(response->gameIDsCount * sizeof(int));
    for (int i = 0; i < response->gameIDsCount; i++) {
        response->gameIDs[i] = (((ProtocolListGamesResponse *)buff)->gameIDs)[i];
    }
}

void serializeProtocolListGamesResponse(ProtocolListGamesResponse * response, unsigned char * buff, size_t buffSize) {
    // Serialize type & gameIDsCount
    memcpy(buff, response, sizeof(ProtocolListGamesResponse) - sizeof(int));
    strncpy(((ProtocolListGamesResponse *)buff)->type, response->type, 2);
    ((ProtocolListGamesResponse *)buff)->gameIDsCount = response->gameIDsCount;
    // Serialize gameIDs
    for (int i = 0; i < response->gameIDsCount; i++) {
        ((ProtocolListGamesResponse *)buff)->gameIDs[i] = response->gameIDs[i];
    }
}