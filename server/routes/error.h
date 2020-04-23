#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

int routeError(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff, char errorCode[2]);

#endif