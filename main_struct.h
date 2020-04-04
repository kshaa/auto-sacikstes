#ifndef MAIN_STRUCT_H
#define MAIN_STRUCT_H

#include "SDL2/SDL.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
} App;

typedef struct {
    int x;
    int y;
    SDL_Texture *texture;
} Entity;

#endif
