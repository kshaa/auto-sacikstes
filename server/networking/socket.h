#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdint.h>
#include "../const.h"

// Socket connections
#define CLOSED_CONNECTION 0
extern int serverfd;
extern int serverfdUDP;
extern int connectionCount;
extern int connectionfds[MAX_CONNECTIONS];

// Socket management
int initServerSocket(uint32_t address, int port, int type);
int acceptConnections();

#endif