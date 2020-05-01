#include "../../common/protocol/protocol_types.h"
#include "../state.h"
#include <string.h>
#include <stdlib.h>
#include "../../common/logic/math.h"

// Sourced from https://stackoverflow.com/a/440240
void randomAlphanumeric(char * buff, const int length) {
    static const char alphanumerics[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; ++i) {
        buff[i] = alphanumerics[rand() % (sizeof(alphanumerics) - 1)];
    }

    buff[length] = 0;
}

int createGame(char * gameName, int fieldID) {
    int freeSlotAvailable = 0;
    int gameID = 1;

    // Find free slot
    for (int i = 0; i < MAX_GAMES; i++) {
        if (!games[i].created) {
            gameID = i;
            freeSlotAvailable = 1;
            break;
        }
    }

    // Fail if all slots taken
    if (!freeSlotAvailable) {
        gameID = -1;
        return gameID;
    }

    // Initialize game
    Game * game = &games[gameID];
    game->created = 1;
    strncpy(game->info.name, gameName, PROTOCOL_MAX_GAME_NAME);
    game->info.winnerId = -1;
    game->info.status = WAITING_PLAYERS;

    // Initialize field info
    game->field.info.id = fieldID;

    // // Hardcoded field temporarily
    int height = 20;
    int width = 30;
    game->field.info.width = width;
    game->field.info.height = height;
    storeLine(&game->field.startLine, 14, 1, 14, 9);
    game->field.extraLinesCount = 5;
    game->field.extraLines = malloc(sizeof(ProtocolLine) * game->field.extraLinesCount);
    // top
    storeLine(&game->field.extraLines[0], 0, 0, width - 1, 0);
    // bottom
    storeLine(&game->field.extraLines[1], 0, height - 1, width - 1, height - 1);
    // left
    storeLine(&game->field.extraLines[2], 0, 0, 0, height - 1);
    // right
    storeLine(&game->field.extraLines[3], width - 1, 0, width - 1, height - 1);
    // middle
    storeLine(&game->field.extraLines[4], 7, 10, 22, 10);
    return gameID;
}

int addGamePlayer(int gameID, char * playerName, int connectionfd) {
    Game * game = &games[gameID];
    int playerID = -1;

    // Find free slot
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (!game->player[i].created) {
            playerID = i;
            break;
        }
    }

    // Fail if all slots taken
    if (playerID == -1) {
        return playerID;
    }

    // Initialize player
    Player * player = &game->player[playerID];
    player->created = 1;
    player->connectionfd = connectionfd;
    randomAlphanumeric(player->password, PROTOCOL_MAX_PLAYER_NAME - 1);

    // Initialize player info
    player->info.id = playerID;
    strncpy(player->info.name, playerName, PROTOCOL_MAX_PLAYER_NAME);

    // // Hardcoded player positions
    

    return playerID;
}
