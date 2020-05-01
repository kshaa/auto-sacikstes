#ifndef MATH_H
#define MATH_H

#include "../protocol/protocol_types.h"

void storeLine(ProtocolLine * line, float ax, float ay, float bx, float by);
void storeCoordinate(ProtocolCoordinates * coords, float x, float y);
void printLine(ProtocolLine * line);

#endif