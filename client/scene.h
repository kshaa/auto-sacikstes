#ifndef SCENE_H
#define SCENE_H

#include "SDL2/SDL.h"
#include "../common/protocol/protocol_types.h"

enum WALL_TYPE {
    STARTWALL = 0,
    EXTRAWALL = 1,
};

typedef struct {
    int type;
    ProtocolLine line;
    SDL_Rect rect;
} SceneWall;

void initScene();

#endif