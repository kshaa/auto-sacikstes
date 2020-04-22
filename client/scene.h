#ifndef SCENE_H
#define SCENE_H

#include "SDL2/SDL.h"

int carsCount;
SDL_Rect * cars;

int wallsCount;
SDL_Rect * walls;

void initScene();

#endif