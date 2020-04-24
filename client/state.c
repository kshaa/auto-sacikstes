#include <string.h>
#include "../common/networking/fetch.h"
#include "../common/networking/translate.h"
#include "networking/socket.h"
#include "const.h"
#include "state.h"

Server server;
App app;
Field field;
ProtocolAction controls;

int initState() {
    // Prepare server
    memset(&server, 0, sizeof(Server));
    strcpy(server.addressSerialized, SERVER_DEFAULT_CONNECT_ADDRESS);
    server.address = translateIPAddress(server.addressSerialized);
    server.port = SERVER_DEFAULT_CONNECT_PORT;
    server.fd = -1;

    // Prepare app
    memset(&app, 0, sizeof(App));

    // Prepare field
    memset(&field, 0, sizeof(Field));

    // Prepare controls
    memset(&controls, 0, sizeof(ProtocolAction));

    // Notify of state
    printf("[state] Initialized game state\n");

    return 1;
}

int initConnection() {
    printf("[state] Server connection defined as %s:%d\n", server.addressSerialized, server.port);
    int connectionfd = connectTCP(server.address, server.port);
    if (connectionfd == -1) {
        printf("[state] Failed to connect to server\n");
        return 0;
    } else {
        printf("[state] Connected to server, fd: %d\n", connectionfd);
        server.fd = connectionfd;
        return 1;
    }
}