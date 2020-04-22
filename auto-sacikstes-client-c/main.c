#include "const.h"
#include "init.h"
#include "input.h"
#include "state.h"
#include "scene.h"

int main(int argc, char *argv[]) {
    // Prepare game state
    initState();

    // Prepare scene
    initScene();

    // SDL window lifecycle
    startSDL();
    atexit(stopSDL);

    // Game loop
    while (1) {
        // Input engine
        registerInput();

        // Networking engine
        // Send input

        // Physics engine
        if (controls.up) {
            printf("Up");
        }

        if (controls.down) {
            printf("Down");
        }

        if (controls.left) {
            printf("Left");
        }

        if (controls.right) {
            printf("Right");
        }

        // Scene engine
        // SDL_Rect car = malloc(sizeof(car));
        // cars[0].x = player.x;
        // cars[0].y = player.y;
        // car.w = 100;
        // car.h = 100;

        // Render engine
        // Background
        SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
        SDL_RenderClear(app.renderer);

        // Walls
        for (int i = 0; i < wallsCount; i++) {
            SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(app.renderer, &walls[i]);
        }

        // Car
        for (int i = 0; i < carsCount; i++) {
            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(app.renderer, &cars[i]);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(4);
    }

    return 0;
}
