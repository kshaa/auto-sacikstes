#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdint.h>
#include "const.h"

// Socket connections
#define CLOSED_CONNECTION -1
extern int serverfd;
extern int connectionCount;
extern int connectionfds[MAX_CONNECTIONS];

// Socket traffic
extern char sendBuff[SEND_BUFF_SIZE];
extern char recvBuff[RECV_BUFF_SIZE];

// Socket management
int initServer(uint32_t address, int port);
int acceptConnections();

#endif