#ifndef CONST_H
#define CONST_H

#include "../common/const.h"

// Debug mode
#define DEBUG COMMON_DEBUG

// Network buffer sizes
#define SEND_BUFF_SIZE COMMON_SEND_BUFF_SIZE
#define RECV_BUFF_SIZE COMMON_RECV_BUFF_SIZE

// Socket configurations
#define MAX_CONNECTIONS 100
#define MAX_PENDING_CONNECTIONS 10

// Games configurations
#define MAX_GAMES 1000
#define MAX_GAME_PLAYERS 100

#endif