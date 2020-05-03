#ifndef LOBBY_H
#define LOBBY_H

#include <stdlib.h>

int routeFieldList(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeListGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeCreateGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeJoinGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);
int routeStartGame(int connfd, char * recvBuff, size_t sendBuffSize, char * sendBuff);

#endif