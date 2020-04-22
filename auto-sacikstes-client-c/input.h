#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"

extern void registerInput();
extern void registerKeyUp(SDL_KeyboardEvent *event);
extern void registerKeyDown(SDL_KeyboardEvent *event);

#endif