#ifndef FETCH_H
#define FETCH_H

#include <stdio.h>

// Network requests
int sendMessage(int connectionfd, void * buff, size_t buffSize, int flags);
unsigned char * receiveMessage(int connectionfd, int flags);
unsigned char * fetch(int connectionfd, void * buff, size_t buffSize);
int isConnectionBroken(int errno);

#endif