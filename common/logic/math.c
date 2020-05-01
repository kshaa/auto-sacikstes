#include "../protocol/protocol_types.h"
#include <stdio.h>

void storeCoordinate(ProtocolCoordinates * coords, float x, float y) {
    coords->x = x;
    coords->y = y;
}

void storeLine(ProtocolLine * line, float ax, float ay, float bx, float by) {
    storeCoordinate(&line->beggining, ax, ay);
    storeCoordinate(&line->end, bx, by);
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