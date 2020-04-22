#ifndef PROTOCOL_MESSAGES_H
#define PROTOCOL_MESSAGES_H

// Empty message
typedef struct {
    char type[2];
} ProtocolEmpty;

// Ping
char PROTOCOL_PING_TYPE[] = { 'P', 'I' };
typedef ProtocolEmpty ProtocolPingRequest;

// Pong
char PROTOCOL_PONG_TYPE[] = { 'P', 'O' };
typedef ProtocolEmpty ProtocolPongResponse;

#endif