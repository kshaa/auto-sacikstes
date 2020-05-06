#ifndef PROTOCOL_STATE
#define PROTOCOL_STATE

#include <stdio.h>
#include "protocol/protocol_types.h"

// Server
typedef struct {
    char addressSerialized[16];
    int address;
    int port;
    int fd;
    int fdUDP;
} Server;

// Field
typedef struct {
    ProtocolFieldInfo info;
    ProtocolLine startLine;
    ProtocolLine mainLine;
    int extraLinesCount;
    ProtocolLine * extraLines;
} Field;

#endif