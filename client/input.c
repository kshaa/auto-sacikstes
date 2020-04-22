#include "SDL2/SDL.h"
#include "state.h"

void registerKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            controls.up = 0;
        }

        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            controls.down = 0;
        }

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            controls.left = 0;
        }

        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            controls.right = 0;
        }
    }
}

void registerKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            controls.up = 1;
        }

        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            controls.down = 1;
        }

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            controls.left = 1;
        }

        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            controls.right = 1;
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
