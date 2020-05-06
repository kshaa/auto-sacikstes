#ifndef FETCH_H
#define FETCH_H

#include <netinet/in.h>
#include <stdio.h>

// Network requests
struct sockaddr_in * getVolatileAddress(int address, int port);
int sendMessage(int connectionfd, void * buff, size_t buffSize, int flags);
int sendMessageTo(int connectionfd, void * buff, size_t buffSize, int flags, struct sockaddr * addr, socklen_t addrSize);
unsigned char * receiveMessage(int connectionfd, int flags);
unsigned char * receiveMessageFrom(int connectionfd, int flags, struct sockaddr * addr, size_t addrSize);
unsigned char * fetch(int connectionfd, void * buff, size_t buffSize);
int isConnectionBroken(int errno);

#endif