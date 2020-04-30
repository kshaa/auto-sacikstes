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

            // For some reason doesn't work
            // case SDL_WINDOWEVENT:
            //     switch (event.window.type) {
            //         case SDL_WINDOWEVENT_SIZE_CHANGED:
            //         case SDL_WINDOWEVENT_RESIZED:
            //             app.resized = 1;
            //             break;
            //     }
            //     break;

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

    // Hack to track window resize changes
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);
    if (sceneWidth != app.oldWidth || sceneHeight != app.oldHeight) {
        app.resized = 1;
        app.oldWidth = sceneWidth;
        app.oldHeight = sceneHeight;
    }
}
