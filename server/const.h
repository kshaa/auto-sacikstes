#ifndef CONST_H
#define CONST_H

#include "../common/const.h"

// Debug mode 0 or 1
#define DEBUG 1

// Network buffer sizes
#define SEND_BUFF_SIZE COMMON_SEND_BUFF_SIZE
#define RECV_BUFF_SIZE COMMON_RECV_BUFF_SIZE

// Socket configurations
#define MAX_CONNECTIONS 100
#define MAX_PENDING_CONNECTIONS 10

#endif