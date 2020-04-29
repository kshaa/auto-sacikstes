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
#define BACKGROUND_R 255
#define BACKGROUND_G 255
#define BACKGROUND_B 255

#define STARTWALL_R 128
#define STARTWALL_G 128
#define STARTWALL_B 128

#define EXTRAWALL_R 0
#define EXTRAWALL_G 0
#define EXTRAWALL_B 0

#endif