#include <string.h>
#include "../common/networking/fetch.h"
#include "../common/networking/translate.h"
#include <sys/socket.h>
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
    server.fdUDP = -1;

    // Prepare games
    memset(&games, 0, sizeof(games));

    // Notify of state
    printf("[state] Initialized server state\n");

    return 1;
}

int initConnection() {
    printf("[state] Server socket defined as %s:%d\n", server.addressSerialized, server.port);
    printf("[state] Initialising TCP socket\n");
    serverfd = initServerSocket(server.address, server.port, SOCK_STREAM);
    if (serverfd == -1) {
        fprintf(stderr, "[state] Failed to create server TCP socket\n");
        return 0;
    } else {
        if (DEBUG) printf("[state] Server TCP socket is listening, fd: %d\n", serverfd);
        server.fd = serverfd;
    }
    printf("[state] Initialising UDP socket\n");
    serverfdUDP = initServerSocket(server.address, server.port, SOCK_DGRAM);
    if (serverfdUDP == -1) {
        fprintf(stderr, "[state] Failed to create server UDP socket\n");
        return 0;
    } else {
        if (DEBUG) printf("[state] Server UDP socket is listening, fd: %d\n", serverfdUDP);
        server.fdUDP = serverfdUDP;
    }
    return 1;
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