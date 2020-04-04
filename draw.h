#ifndef DRAW_H
#define DRAW_H

#include "SDL2/SDL.h"

extern void prepareScene(void);
extern void presentScene(void);
extern SDL_Texture *loadTexture(char *filename);
extern void blit(SDL_Texture *texture, int x, int y);

#endif