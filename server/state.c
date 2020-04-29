#include <string.h>
#include "../common/networking/fetch.h"
#include "../common/networking/translate.h"
#include "networking/socket.h"
#include "const.h"
#include "state.h"

Server server;
Game games[MAX_GAMES];

int initState() {
    // Prepare server
    memset(&server, 0, sizeof(server));
    strcpy(server.addressSerialized, SERVER_DEFAULT_LISTEN_ADDRESS);
    server.address = translateIPAddress(server.addressSerialized);
    server.port = COMMON_DEFAULT_SERVER_PORT;
    server.fd = -1;

    // Prepare games
    memset(&games, 0, sizeof(games));

    // Notify of state
    printf("[state] Initialized server state\n");

    return 1;
}

int initConnection() {
    printf("[state] Server socket defined as %s:%d\n", server.addressSerialized, server.port);
    serverfd = listenTCP(server.address, server.port);
    if (serverfd == -1) {
        fprintf(stderr, "[state] Failed to create server socket\n");
        return 0;
    } else {
        if (DEBUG) printf("[state] Server socket is listening, fd: %d\n", serverfd);
        server.fd = serverfd;
        return 1;
    }
}

int getGameCount() {
    int count = 0;
    for (int i = 0; i < MAX_GAMES; i++) {
        if (games[i].created) {
            count ++;
        }
    }
    return count;
}

int getGamePlayerCount(int gameID) {
    int count = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (games[gameID].player[i].created) {
            count++;
        }
    }
    return count;
}