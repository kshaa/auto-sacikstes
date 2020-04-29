#include "../protocol/protocol_types.h"
#include <stdio.h>

void storeLine(ProtocolLine * line, float ax, float ay, float bx, float by) {
    line->beggining.x = ax;
    line->beggining.y = ay;
    line->end.x = bx;
    line->end.y = by;
}

void printLine(ProtocolLine * line) {
    printf(
        "(%f;%f);(%f;%f)",
        line->beggining.x,
        line->beggining.y,
        line->end.x,
        line->end.y
    );
}