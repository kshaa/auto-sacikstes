#ifndef LOBBY_H
#define LOBBY_H

#include <stdlib.h>

int routeFieldList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeGameList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeGameCreate(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeStartGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);

#endif