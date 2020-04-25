#ifndef PROTOCOL_MESSAGES_H
#define PROTOCOL_MESSAGES_H

#include <stdlib.h>
#include "protocol_types.h"

// Message types
unsigned char * getVolatileResponseType(void * message);
unsigned char * getVolatilePrintableResponseType(void * message);
int isMessageType(unsigned char * a, unsigned char * b);

// Empty message
typedef struct {
    char type[2];
} ProtocolEmpty;

// Ping
extern char PROTOCOL_PING_TYPE[2];
typedef ProtocolEmpty ProtocolPingRequest;

// Pong
extern char PROTOCOL_PONG_TYPE[2];
typedef ProtocolEmpty ProtocolPongResponse;

// Error
extern char PROTOCOL_ERROR_TYPE[2];
typedef struct {
    char type[2];
    char errorCode[2];
} ProtocolErrorResponse;

// Error codes
extern char PROTOCOL_ERROR_CODE_INCORRECT_TYPE[2];
extern char PROTOCOL_ERROR_CODE_SERVER_IS_SAD[2];
int isErrorCode(char * a, char * b);
char * getVolatileErrorMessage(char errorCode[2]);

// List games
extern char PROTOCOL_LIST_GAMES_TYPE[2];
typedef ProtocolEmpty ProtocolListGamesRequest;
typedef struct {
    char type[2];
    int gameIDsCount;
    int * gameIDs;
} ProtocolListGamesResponse;
int getProtocolListGamesResponseCount(void * buff, size_t buffSize);
void unserializeProtocolListGamesResponse(unsigned char * buff, size_t buffSize, ProtocolListGamesResponse * response);
void serializeProtocolListGamesResponse(ProtocolListGamesResponse * response, unsigned char * buff, size_t buffSize);

// Create game
extern char PROTOCOL_CREATE_GAME_TYPE[2];
typedef struct {
    char type[2];
    char gameName[PROTOCOL_MAX_GAME_NAME];
    char playerName[PROTOCOL_MAX_PLAYER_NAME];
    int fieldID;
} ProtocolCreateGameRequest;

typedef struct {
    char type[2];
    int gameID;
    int playerID;
    char playerPassword[PROTOCOL_MAX_PASSWORD_LENGTH];
} ProtocolCreateGameResponse;

// Start game
extern char PROTOCOL_START_GAME_TYPE[2];
typedef struct {
    char type[2];
    int gameID;
    char playerPassword[PROTOCOL_MAX_PASSWORD_LENGTH];
} ProtocolStartGameRequest;
typedef struct {
    char type[2];
    int playerInfoCount;
    ProtocolPlayerInfo * playerInfos;
    ProtocolFieldInfo field;
} ProtocolStartGameResponse;
void unserializeProtocolStartGameResponse(unsigned char * buff, size_t buffSize, ProtocolStartGameResponse * response);
void serializeProtocolStartGameResponse(ProtocolStartGameResponse * response, unsigned char * buff, size_t buffSize);

#endif