#include "SDL2/SDL.h"
#include "../common/state.h"
#include "state.h"

void registerKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            game.controls.y = Y_NONE;
        }

        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            game.controls.y = Y_NONE;
        }

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            game.controls.x = X_NONE;
        }

        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            game.controls.x = X_NONE;
        }
    }
}

void registerKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            game.controls.y = Y_UP;
        }

        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            game.controls.y = Y_DOWN; 
        }

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            game.controls.x = X_LEFT;
        }

        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            game.controls.x = X_RIGHT;
        }
    }
}

void registerInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                registerKeyDown(&event.key);
                break;

            case SDL_KEYUP:
                registerKeyUp(&event.key);
                break;

            default:
                break;
        }
    }
}
