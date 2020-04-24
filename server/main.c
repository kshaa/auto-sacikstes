#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "const.h"
#include "state.h"
#include "router.h"
#include "networking/socket.h"
#include "../common/networking/translate.h"

// External getopt parameters
extern char *optarg;
extern int optind;

// Usage instruction
const char usageFormat[] =
    "Autosacikstes server\n\n"

    "Usage: %s [FLAGS]\n"
    "Listen flags:\n"
    "   -a Server IPv4 listening address\n"
    "   -p Server listening port\n"
    "Action flags:\n"
    "   -h Print this help\n";

int main(int argc, char *argv[])
{
    // Initialize state
    printf("[server] Initialising server state\n");
    int initStateSuccess = initState();
    if (!initStateSuccess) {
        fprintf(stderr, "[server] Failed to initialize server state\n");
        return 1;
    }

    // Action flags
    int helpFlag = 0;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "hp:a:")) != -1) {
        switch (option) {
            case 'h':
                helpFlag = 1;
                break;
            case 'p':
                server.port = atoi(optarg);
                break;
            case 'a':
                strncpy(server.addressSerialized, optarg, 16);
                server.address = translateIPAddress(server.addressSerialized);
                if (server.address == INVALID_IP_ADDRESS) {
                    fprintf(stderr, "[client] Invalid server address provided\n");
                    fprintf(stderr, usageFormat, argv[0]);
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "[server] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }

    // Print help
    if (helpFlag) {
        fprintf(stderr, usageFormat, argv[0]);
        return 0;
    }

    // Validate port
    if (server.port == 0) {
        fprintf(stderr, "[server] Port must not be zero\n");
        return 1;
    }

    // Initialize socket
    int initConnectionSuccess = initConnection();
    if (!initConnectionSuccess) {
        fprintf(stderr, "[server] Failed to initialize server socket\n");
        return 1;
    }

    // Process server'y tasks
    int serverRunning = 1;
    while (serverRunning) {
        // Accept new connections
        acceptConnections();

        // Handle traffic
        int handleTrafficSuccess = handleTraffic();
        if (!handleTrafficSuccess) {
            fprintf(stderr, "[server] Failed to handle traffic\n");
        }

        // Sleep a little, to avoid making CPU sad
        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 1; // 0.01 ms
        nanosleep(&timeout, NULL);
    }

    return 0;
} 
