#ifndef PROTOCOL_MESSAGES_H
#define PROTOCOL_MESSAGES_H

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

// Error codes
extern char PROTOCOL_ERROR_CODE_INCORRECT_TYPE[2];
extern char PROTOCOL_ERROR_CODE_SERVER_IS_SAD[2];
int isErrorCode(char * a, char * b);
char * getVolatileErrorMessage(char errorCode[2]);

// Error
extern char PROTOCOL_ERROR_TYPE[2];
typedef struct {
    char type[2];
    char errorCode[2];
} ProtocolErrorResponse;

// List games
extern char PROTOCOL_LIST_GAMES_TYPE[2];
typedef ProtocolEmpty ProtocolListGamesRequest;
typedef struct {
    char type[2];
    int gameIDsCount;
    int * gameIDs;
} ProtocolListGamesResponse;

#endif