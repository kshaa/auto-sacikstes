#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "socket.h"
#include "traffic.h"
#include <unistd.h>

// External getopt parameters
extern char *optarg;
extern int optind;

// Usage instruction
const char usageFormat[] = "Usage: %s [-p port]\n";

int main(int argc, char *argv[])
{
    // Configure server address
    int address = INADDR_ANY;
    int port = 5000;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "p:")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "[server] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }

    // Validate port
    if (port == 0) {
        fprintf(stderr, "[server] Port must not be zero\n");
        return 1;
    }

    // Initialize server
    printf("[server] Initialising on port %d\n", port);
    int initSuccess = initServer(address, port);
    if (!initSuccess) {
        fprintf(stderr, "[server] Failed to initialize server\n");
        return 1;
    }

    // Process server'y tasks
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
