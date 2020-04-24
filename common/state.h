#ifndef PROTOCOL_STATE
#define PROTOCOL_STATE

#include <stdio.h>
#include "protocol_types.h"

typedef struct {
    ProtocolFieldInfo info;
    ProtocolLine startLine;
    ProtocolLine mainLine;
    size_t extraLinesCount;
    ProtocolLine * extraLines;
} Field;

#endif