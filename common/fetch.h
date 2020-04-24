#ifndef FETCH_H
#define FETCH_H

#include <stdio.h>

// Message types
int getVolatileResponseType(void * message);
int getVolatilePrintableResponseType(void * message);
int isResponseType(unsigned char * a, unsigned char * b);

// Message error codes
int isErrorCode(char * a, char * b);

// Network requests
int sendMessage(int connectionfd, void * buff, size_t buffSize, int flags);
unsigned char * receiveMessage(int connectionfd, int flags);
unsigned char * fetch(int connectionfd, void * buff, size_t buffSize);

#endif