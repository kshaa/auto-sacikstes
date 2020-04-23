#ifndef PING_H
#define PING_H

#include <stdlib.h>

int routePing(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);

#endif