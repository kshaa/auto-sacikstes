#ifndef CONST_H
#define CONST_H

#include "../common/const.h"

// Server
#define SERVER_DEFAULT_LISTEN_ADDRESS "0.0.0.0"
#define SERVER_DEFAULT_LISTEN_PORT COMMON_DEFAULT_SERVER_PORT

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
#define MAX_GAME_PLAYERS COMMON_MAX_GAME_PLAYERS

#endif