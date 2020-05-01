#include "scene.h"
#include "state.h"
#include "../common/logic/math.h"
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

void retransformWall(SceneWall * wall) {
    ProtocolLine * line = &wall->line;
    int block = getOnscreenBlockSize();

    wall->rect.x = transformX(line->beggining.x);
    wall->rect.y = transformY(line->beggining.y);
    wall->rect.w = block * (line->end.x - line->beggining.x + 1);
    wall->rect.h = block * (line->end.y - line->beggining.y + 1);
}

int scenifyLine(SceneWall * wall, ProtocolLine * line, int type) {
    memcpy(&wall->line, line, sizeof(ProtocolLine));
    wall->type = type;
    retransformWall(wall);
}

void resizeScene() {
    for (int i = 0; i < scene.decorationCount; i++) {
        retransformWall(&scene.decorations[i]);
    }
    for (int i = 0; i < scene.wallCount; i++) {
        retransformWall(&scene.walls[i]);
    }
}

void initScene() {
    ProtocolLine tmpline;

    // Get scene info
    int sceneWidth = 0;
    int sceneHeight = 0;
    SDL_GetWindowSize(app.window, &sceneWidth, &sceneHeight);

    // Init decorations
    scene.decorationCount = 1 + game.field.info.width * game.field.info.height;
    scene.decorations = malloc(scene.decorationCount * sizeof(SceneWall));
    storeLine(&tmpline, 0, 0, game.field.info.width - 1, game.field.info.height - 1);
    scenifyLine(&scene.decorations[0], &tmpline, DECORATION_BG);
    for (int i = 0; i < game.field.info.width * game.field.info.height; i++) {
        int x = i % game.field.info.width;
        int y = i / game.field.info.width;
        int odd = (x + y) % 2 == 0;
        int type = odd ? DECORATION_A : DECORATION_B;
        storeLine(&tmpline, (float)x, (float)y, (float)x, (float)y);
        scenifyLine(&scene.decorations[i + 1], &tmpline, type);
    }

    // Init walls
    scene.wallCount = game.field.extraLinesCount + 1;
    scene.walls = malloc(scene.wallCount * sizeof(SceneWall));

    scenifyLine(&scene.walls[0], &game.field.startLine, STARTWALL);
    for (int i = 0; i < game.field.extraLinesCount; i++) {
        scenifyLine(&scene.walls[i + 1], &game.field.extraLines[i], EXTRAWALL);
    }

    // Init cars
    int playerCount = getGamePlayerCount();
    scene.carsCount = playerCount;
    scene.cars = malloc(scene.carsCount * sizeof(SceneWall));

    int lastPlayer = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (!game.player[i].created) {
            continue;
        }

        float x = game.player[i].info.position.x - 1;
        float y = game.player[i].info.position.y - 0.5;
        storeLine(&tmpline, x, y, x + 1, y);
        scenifyLine(&scene.cars[i], &tmpline, SCENECAR);

        lastPlayer++;
    }
}

void renderRect(SDL_Rect * rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(app.renderer, r, g, b, a);
    SDL_RenderFillRect(app.renderer, rect);
}

void getTypeColor(int type, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a) {
    switch (type) {
        case (STARTWALL):
            *r = STARTWALL_R;
            *g = STARTWALL_G;
            *b = STARTWALL_B;
            *a = STARTWALL_A;
            break;

        case (EXTRAWALL):
            *r = EXTRAWALL_R;
            *g = EXTRAWALL_G;
            *b = EXTRAWALL_B;
            break;

        case (DECORATION_BG):
            *r = DECORATION_BG_R;
            *g = DECORATION_BG_G;
            *b = DECORATION_BG_B;
            break;

        case (DECORATION_A):
            *r = DECORATION_A_R;
            *g = DECORATION_A_G;
            *b = DECORATION_A_B;
            break;
        
        case (DECORATION_B):
            *r = DECORATION_B_R;
            *g = DECORATION_B_G;
            *b = DECORATION_B_B;
            break;

        case (SCENECAR):
            *r = SCENECAR_R;
            *g = SCENECAR_G;
            *b = SCENECAR_B;
            break;
    }
}

void renderWall(SceneWall * wall) {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
    getTypeColor(wall->type, &r, &g, &b, &a);
    renderRect(&wall->rect, r, g, b, a);
}

void renderScene() {
    // Background
    SDL_SetRenderDrawColor(app.renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 255);
    SDL_RenderClear(app.renderer);

    // Decorations
    for (int i = 0; i < scene.decorationCount; i++) {
        if (i == 0) continue;
        renderWall(&scene.decorations[i]);
    }

    // Walls
    for (int i = 0; i < scene.wallCount; i++) {
        renderWall(&scene.walls[i]);
    }

    // Cars
    for (int i = 0; i < scene.carsCount; i++) {
        renderWall(&scene.cars[i]);
    }
}