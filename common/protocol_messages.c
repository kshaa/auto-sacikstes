#ifndef PROTOCOL_MESSAGES_H
#define PROTOCOL_MESSAGES_H

// Ping
char PROTOCOL_PING_TYPE[] = { 'P', 'I' };

// Pong
char PROTOCOL_PONG_TYPE[] = { 'P', 'O' };

// Error codes
char PROTOCOL_ERROR_CODE_INCORRECT_TYPE[] = { 'I', 'T' };
char PROTOCOL_ERROR_CODE_SERVER_IS_SAD[] = { 'S', 'S' };

// Error
char PROTOCOL_ERROR_TYPE[] = { 'E', 'R' };

#endif