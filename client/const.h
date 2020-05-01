#ifndef CONST_H
#define CONST_H

#include "../common/const.h"

// Debug mode
#define DEBUG COMMON_DEBUG

// Network buffer sizes
#define SEND_BUFF_SIZE COMMON_SEND_BUFF_SIZE
#define RECV_BUFF_SIZE COMMON_RECV_BUFF_SIZE

// Server connection defaults
#define SERVER_DEFAULT_CONNECT_ADDRESS "127.0.0.1"
#define SERVER_DEFAULT_CONNECT_PORT COMMON_DEFAULT_SERVER_PORT

// Window configurations
#define WINDOW_NAME "Auto sacīkstes"
#define INITIAL_SCREEN_WIDTH 1280
#define INITIAL_SCREEN_HEIGHT 720

// Game configs
#define MAX_GAME_PLAYERS COMMON_MAX_GAME_PLAYERS
#define MAX_GAME_WALLS 1000

// Color configs
#define BACKGROUND_R 50
#define BACKGROUND_G 50
#define BACKGROUND_B 50

#define STARTWALL_R 0
#define STARTWALL_G 255
#define STARTWALL_B 0
#define STARTWALL_A 70

#define EXTRAWALL_R 0
#define EXTRAWALL_G 0
#define EXTRAWALL_B 0

#define DECORATION_BG_R 255
#define DECORATION_BG_G 255
#define DECORATION_BG_B 255

#define DECORATION_A_R 200
#define DECORATION_A_G 200
#define DECORATION_A_B 200

#define DECORATION_B_R 240
#define DECORATION_B_G 240
#define DECORATION_B_B 240

#define SCENECAR_R 128
#define SCENECAR_G 0
#define SCENECAR_B 128

#endif