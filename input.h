#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"

extern void doInput(void);
extern void doKeyUp(SDL_KeyboardEvent *event);
extern void doKeyDown(SDL_KeyboardEvent *event);

#endif