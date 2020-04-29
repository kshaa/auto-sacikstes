#include "scene.h"
#include "state.h"
#include "SDL2/SDL.h"

int getOnscreenBlockSize() {
    int fieldWidth = game.field.info.width;
    int fieldHeight = game.field.info.height;
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);
    if (sceneWidth == 0) sceneWidth = INITIAL_SCREEN_WIDTH;
    if (sceneHeight == 0) sceneHeight = INITIAL_SCREEN_HEIGHT;
    float xblock = (double)sceneWidth / (double)fieldWidth;
    float yblock = (double)sceneHeight / (double)fieldHeight;
    float minBlock = (xblock < yblock) ? xblock : yblock;

    return (int)minBlock;
}

int getOffsetX() {
    int block = getOnscreenBlockSize();
    int fieldWidth = game.field.info.width;
    int fieldHeight = game.field.info.height;
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);
    if (sceneWidth == 0) sceneWidth = INITIAL_SCREEN_WIDTH;
    if (sceneHeight == 0) sceneHeight = INITIAL_SCREEN_HEIGHT;

    return (sceneWidth - block * fieldWidth) / 2;
}

int getOffsetY() {
    int block = getOnscreenBlockSize();
    int fieldWidth = game.field.info.width;
    int fieldHeight = game.field.info.height;
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);
    if (sceneWidth == 0) sceneWidth = INITIAL_SCREEN_WIDTH;
    if (sceneHeight == 0) sceneHeight = INITIAL_SCREEN_HEIGHT;

    return (sceneHeight - block * fieldHeight) / 2;
}

int transformX(int x) {
    return getOffsetX() + getOnscreenBlockSize() * x;
}

int transformY(int y) {
    return getOffsetY() + getOnscreenBlockSize() * y;
}

int scenifyLine(SceneWall * wall, ProtocolLine * line) {
    int block = getOnscreenBlockSize();

    memcpy(&wall->line, line, sizeof(ProtocolLine));
    wall->rect.x = transformX(line->beggining.x);
    wall->rect.y = transformY(line->beggining.y);
    wall->rect.w = block * (line->end.x - line->beggining.x + 1);
    wall->rect.h = block * (line->end.y - line->beggining.y + 1);
}

void initScene() {
    // Get scene info
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);

    // Init walls
    scene.wallCount = game.field.extraLinesCount + 1;
    scene.walls = malloc(scene.wallCount * sizeof(SceneWall));

    scenifyLine(&scene.walls[0], &game.field.startLine);
    for (int i = 0; i < game.field.extraLinesCount; i++) {
        scenifyLine(&scene.walls[i + 1], &game.field.extraLines[i]);
    }

    // // Init cars
    // carsCount = 2;
    // cars = malloc(carsCount * sizeof(SDL_Rect));

    // for (int i = 0; i < carsCount; i++) {
    //     cars[i].x = 100*2*i;
    //     cars[i].y = 100;
    //     cars[i].w = 100;
    //     cars[i].h = 100;
    // }
}
