#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include "sockets.h"
#include "traffic.h"

int main(int argc, char *argv[])
{
    int address = INADDR_ANY;
    int port = 5000;
    int initSuccess = initServer(address, port);
    if (!initSuccess) {
        fprintf(stderr, "[server] Failed to initialize server\n");
        return 1;
    }

    int serverRunning = 1;
    while (serverRunning) {
        // Accept new connections
        acceptConnections();

        // Handle traffic
        handleTraffic();

        // Sleep a little, to avoid making CPU sad
        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 1; // 0.01 ms
        nanosleep(&timeout, NULL);
    }

    return 0;
} 
