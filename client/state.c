#include <string.h>
#include "../common/networking/fetch.h"
#include "../common/networking/translate.h"
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

int getGamePlayerCount() {
    int count = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game.player[i].created) {
            count++;
        }
    }
    return count;
}