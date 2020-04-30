#ifndef SCENE_H
#define SCENE_H

#include "SDL2/SDL.h"
#include "../common/protocol/protocol_types.h"

enum WALL_TYPE {
    STARTWALL = 0,
    EXTRAWALL = 1,
    DECORATION_BG = 2,
    DECORATION_A = 3,
    DECORATION_B = 4
};

typedef struct {
    int type;
    ProtocolLine line;
    SDL_Rect rect;
} SceneWall;

void initScene();
void resizeScene();
void renderScene();

#endif