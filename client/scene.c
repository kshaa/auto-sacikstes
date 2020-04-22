#include "scene.h"

void initScene() {
    // Init cars
    carsCount = 2;
    cars = malloc(carsCount * sizeof(SDL_Rect));

    for (int i = 0; i < carsCount; i++) {
        cars[i].x = 100*2*i;
        cars[i].y = 100;
        cars[i].w = 100;
        cars[i].h = 100;
    }

    // Init walls
    wallsCount = 8;
    walls = malloc(wallsCount * sizeof(SDL_Rect));

    for (int i = 0; i < wallsCount; i++) {
        walls[i].x = 200 + 10*2*i;
        walls[i].y = 300;
        walls[i].w = 10;
        walls[i].h = 10;
    }
}