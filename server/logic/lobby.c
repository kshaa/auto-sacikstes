#include "../../common/protocol/protocol_types.h"
#include "../state.h"
#include <string.h>

int createGame(char * gameName, int fieldID) {
    int freeSlotAvailable = 0;
    int success = 1;
    Game game;

    // Find free slot
    for (int i = 0; i < MAX_GAMES; i++) {
        if (!games[i].created) {
            game = games[i]; 
            freeSlotAvailable = 1;
        }
    }

    // Fail if all slots taken
    if (!freeSlotAvailable) {
        success = 0;
        return success;
    }

    // Initialize game
    game.created = 1;
    strncpy(game.info.name, gameName, PROTOCOL_MAX_GAME_NAME);
    game.info.winnerId = -1;
    game.info.status = WAITING_PLAYERS;

    // Initialize field
    // @Todo

    return success;
}

int addGamePlayer(int gameID, char * playerName, int connectionfd) {
    Game game = games[gameID];
    int playerID = -1;
    Player player;

    // Find free slot
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (!game.player[i].created) {
            player = game.player[i]; 
            playerID = i;
        }
    }

    // Fail if all slots taken
    if (playerID == -1) {
        return playerID;
    }

    // Initialize player
    player.created = 1;
    player.connectionfd = connectionfd;
    strncpy(player.password, "secret", PROTOCOL_MAX_PASSWORD_LENGTH);

    // Initialize player info
    player.info.id = playerID;
    strncpy(player.info.name, playerName, PROTOCOL_MAX_PLAYER_NAME);

    return playerID;
}
