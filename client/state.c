#include <string.h>
#include "../common/networking/fetch.h"
#include "../common/networking/translate.h"
#include <sys/socket.h>
#include "networking/socket.h"
#include "const.h"
#include "state.h"

Server server;
App app;
Game game;
Scene scene;

int initState() {
    // Prepare server
    memset(&server, 0, sizeof(Server));
    strcpy(server.addressSerialized, SERVER_DEFAULT_CONNECT_ADDRESS);
    server.address = translateIPAddress(server.addressSerialized);
    server.port = SERVER_DEFAULT_CONNECT_PORT;
    server.fd = -1;

    // Prepare app
    memset(&app, 0, sizeof(App));
    app.oldWidth = INITIAL_SCREEN_WIDTH;
    app.oldHeight = INITIAL_SCREEN_HEIGHT;

    // Prepare game
    memset(&game, 0, sizeof(Game));

    return 1;
}

int initConnection() {
    printf("[state] Server connection defined as %s:%d\n", server.addressSerialized, server.port);
    int connectionfd = initClientSocket(server.address, server.port, SOCK_STREAM);
    if (connectionfd == -1) {
        printf("[state] Failed to create TCP socket and/or a connection to server\n");
        return 0;
    } else {
        printf("[state] Created TCP socket and connected to server, fd: %d\n", connectionfd);
        server.fd = connectionfd;
    }
    int connectionfdUDP = initClientSocket(server.address, server.port, SOCK_DGRAM);
    if (connectionfdUDP == -1) {
        printf("[state] Failed to create UDP socket for server communications\n");
        return 0;
    } else {
        printf("[state] Created UDP socket for server communications, fd: %d\n", connectionfdUDP);
        server.fdUDP = connectionfdUDP;
    }
    return 1;
}

int getGamePlayerCount() {
    int count = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game.player[i].created) {
            count++;
        }
    }
    return count;
}