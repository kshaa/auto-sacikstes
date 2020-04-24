#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdint.h>
#include "../const.h"

// Socket connections
#define CLOSED_CONNECTION 0
extern int serverfd;
extern int connectionCount;
extern int connectionfds[MAX_CONNECTIONS];

// Socket management
int listenTCP(uint32_t address, int port);
int acceptConnections();

#endif