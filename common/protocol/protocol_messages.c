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

// List fields
char PROTOCOL_LIST_FIELDS_TYPE[] = { 'N', 'F' };

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
        int * gameIDsOffset = (int *) buff + sizeof(char) * 2 + sizeof(int);
        int * nthGameIDOffset = gameIDsOffset + sizeof(int) * i;
        response->gameIDs[i] = *nthGameIDOffset;
    }
}

void serializeProtocolListGamesResponse(ProtocolListGamesResponse * response, unsigned char * buff, size_t buffSize) {
    // Serialize type & gameIDsCount
    strncpy(((ProtocolListGamesResponse *)buff)->type, response->type, 2);
    ((ProtocolListGamesResponse *)buff)->gameIDsCount = response->gameIDsCount;
    // Serialize gameIDs
    for (int i = 0; i < response->gameIDsCount; i++) {
        int * gameIDsOffset = (int *) buff + sizeof(char) * 2 + sizeof(int);
        int * nthGameIDOffset = gameIDsOffset + sizeof(int) * i;
        memcpy(nthGameIDOffset, &response->gameIDs[i], sizeof(int));
    }
}

// Create game
char PROTOCOL_CREATE_GAME_TYPE[] = { 'C', 'G' };

// Start game
char PROTOCOL_START_GAME_TYPE[] = { 'S', 'G' };
void unserializeProtocolStartGameResponse(unsigned char * buff, size_t buffSize, ProtocolStartGameResponse * response) {
    // Check if the buff is way too small
    if (buffSize < sizeof(ProtocolStartGameResponse)) {
        return;
    }

    // Unserialize type
    strncpy(response->type, buff, 2);
    // Unserialize playerInfoCount
    response->playerInfoCount = ((ProtocolStartGameResponse *)buff)->playerInfoCount;
    // Unserialize field
    memcpy(&response->field, &((ProtocolStartGameResponse *)buff)->field, sizeof(ProtocolFieldInfo));
    // Unserialize playerInfos
    response->playerInfos = malloc(response->playerInfoCount * sizeof(ProtocolPlayerInfo));
    for (int i = 0; i < response->playerInfoCount; i++) {
        ProtocolPlayerInfo * playerInfosOffset = (ProtocolPlayerInfo *) buff + sizeof(char) * 2 + sizeof(int);
        ProtocolPlayerInfo * nthPlayerInfoOffset = playerInfosOffset + sizeof(ProtocolPlayerInfo) * i;
        memcpy(&response->playerInfos[i], nthPlayerInfoOffset, sizeof(ProtocolPlayerInfo));
    }
}

void serializeProtocolStartGameResponse(ProtocolStartGameResponse * response, unsigned char * buff, size_t buffSize) {
    // Serialize type & playerInfoCount
    strncpy(((ProtocolStartGameResponse *)buff)->type, response->type, 2);
    ((ProtocolStartGameResponse *)buff)->playerInfoCount = response->playerInfoCount;
    // Serialize playerInfos & field
    for (int i = 0; i < response->playerInfoCount; i++) {
        ProtocolPlayerInfo * playerInfosOffset = (ProtocolPlayerInfo *) buff + sizeof(char) * 2 + sizeof(int);
        ProtocolPlayerInfo * nthPlayerInfoOffset = playerInfosOffset + sizeof(ProtocolPlayerInfo) * i;
        memcpy(nthPlayerInfoOffset, &response->playerInfos[i], sizeof(int));
    }
    // Serialize field
    ProtocolFieldInfo * fieldOffset = (ProtocolFieldInfo *) buff + sizeof(char) * 2 + sizeof(int) + sizeof(ProtocolPlayerInfo) * response->playerInfoCount;
    memcpy(fieldOffset, &response->field, sizeof(ProtocolFieldInfo));
}
