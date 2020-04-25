#include <unistd.h>
#include "const.h"
#include "init.h"
#include "input.h"
#include "state.h"
#include "scene.h"
#include "../common/networking/translate.h"
#include "queries/lobby.h"
#include "queries/ping.h"

void runGame() {
    // Prepare game state
    initState();

    // Prepare scene
    initScene();

    // SDL window lifecycle
    startSDL();
    atexit(stopSDL);

    // Game loop
    while (1) {
        // Input engine
        registerInput();

        // Networking engine
        // Send input

        // Physics engine
        // if (controls.up) {
        //     printf("Up");
        // }

        // if (controls.down) {
        //     printf("Down");
        // }

        // if (controls.left) {
        //     printf("Left");
        // }

        // if (controls.right) {
        //     printf("Right");
        // }

        // Scene engine
        // SDL_Rect car = malloc(sizeof(car));
        // cars[0].x = player.x;
        // cars[0].y = player.y;
        // car.w = 100;
        // car.h = 100;

        // Render engine

        // Background
        SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
        SDL_RenderClear(app.renderer);

        // Walls
        for (int i = 0; i < wallsCount; i++) {
            SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(app.renderer, &walls[i]);
        }

        // Car
        for (int i = 0; i < carsCount; i++) {
            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(app.renderer, &cars[i]);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(4);
    }
}

// External getopt parameters
extern char *optarg;
extern int optind;

// Usage instruction
const char usageFormat[] =
    "Autosacikstes client\n\n"

    "Usage: %s [FLAGS]\n"
    "Connection flags:\n"
    "   -a [ADDRESS] Server IPv4 connection address\n"
    "   -p [PORT] Server connection port\n"
    "Action flags:\n"
    "   -h Print this help\n"
    "   -l List running games on server\n"
    "   -c [NAME] Create game with a given name \n"
    "   -j [ID] Join game with ID\n"
    "   -k Send keep-alive i.e. ping to server\n"
    "Parameter flags:\n"
    "   -u [NAME] Player name for created game\n\n"

    "Parameter flags are additions to action flags\n"
    "At least one action flag is required\n";

int main(int argc, char *argv[]) {
    // Init state
    int stateSuccess = initState();
    if (!stateSuccess) {
        fprintf(stderr, "[client] Failed to initialize state\n");
        return 1;
    }

    // Prepare action
    int keepAliveFlag = 0;
    int helpFlag = 0;
    int listGamesFlag = 0;
    int createGameFlag = 0;
    int joinGameFlag = 0;
    int joinGameID = 0;

    // Input parameters
    char inputGameName[PROTOCOL_MAX_GAME_NAME];
    char inputPlayerName[PROTOCOL_MAX_PLAYER_NAME];
    int inputFieldID = 0;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "kha:p:lj:c:u:")) != -1) {
        switch (option) {
            case 'k':
                keepAliveFlag = 1;
                break;
            case 'h':
                helpFlag = 1;
                break;
            case 'p':
                server.port = atoi(optarg);
                if (server.port == 0) {
                    fprintf(stderr, "[client] Port must not be zero\n");
                    fprintf(stderr, usageFormat, argv[0]);
                    return 1;
                }
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
            case 'l':
                listGamesFlag = 1;
                break;
            case 'j':
                joinGameFlag = 1;
                joinGameID = atoi(optarg);
                break;
            case 'c':
                createGameFlag = 1;
                if (strlen(optarg) > PROTOCOL_MAX_GAME_NAME - 1) {
                    fprintf(stderr, "[client] Game name too long\n");
                    fprintf(stderr, usageFormat, argv[0]);
                    return 1;
                }
                strncpy(inputGameName, optarg, PROTOCOL_MAX_GAME_NAME);
                break;
            case 'u':
                if (strlen(optarg) > PROTOCOL_MAX_PLAYER_NAME - 1) {
                    fprintf(stderr, "[client] Player name too long\n");
                    fprintf(stderr, usageFormat, argv[0]);
                    return 1;
                }
                strncpy(inputPlayerName, optarg, PROTOCOL_MAX_PLAYER_NAME);
                break;
            default:
                fprintf(stderr, "[client] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }

    // At least one action is required
    if (!(keepAliveFlag || helpFlag || listGamesFlag || createGameFlag || joinGameFlag)) {
        fprintf(stderr, "[client] At least one action is required\n");
        fprintf(stderr, usageFormat, argv[0]);
        return 1;
    }

    // Print help
    if (helpFlag) {
        fprintf(stderr, usageFormat, argv[0]);
        return 0;
    }
    
    // Connect to server
    int connectionSuccess = initConnection();
    if (!connectionSuccess) {
        fprintf(stderr, "[client] Failed to initialize connection to server\n");
        return 1;
    }

    // Process the provided action
    if (keepAliveFlag) {
        int pingSuccess = ping();
        if (pingSuccess) {
            printf("[client] Ping successful\n");
        } else {
            printf("[client] Ping failed\n");
        }
    } else if (listGamesFlag) {
        ProtocolListGamesResponse gameIDs;
        int gameCountSuccess = getGameIDs(&gameIDs, 1);
        if (!gameCountSuccess) {
            fprintf(stderr, "[client] Failed to list game ids\n");
            return 1;
        }
        gameIDs.gameIDs = malloc(sizeof(int) * gameIDs.gameIDsCount);
        int gameIDsSuccess = getGameIDs(&gameIDs, 0);
        if (!gameIDsSuccess) {
            fprintf(stderr, "[client] Failed to list game ids\n");
            return 1;
        }
        printf("[client] Currently there are %d games running\n", gameIDs.gameIDsCount);
        for (int i = 0; i < gameIDs.gameIDsCount; i++) {
            printf("[client] Game w/ ID %d\n", gameIDs.gameIDs[i]);
        }
        free(gameIDs.gameIDs);
    } else if (createGameFlag) {
        // Create game
        printf("[client] Creating game!\n");
        ProtocolCreateGameResponse createGameResponse;
        int createGameSuccess = createGame(&createGameResponse, inputGameName, inputPlayerName, inputFieldID);
        if (!createGameSuccess) {
            fprintf(stderr, "[client] Failed to create game\n");
            return 1;
        }
        printf(
            "[client] Created game! Game ID: %d, player ID: %d, player password: %s\n",
            createGameResponse.gameID,
            createGameResponse.playerID,
            createGameResponse.playerPassword
        );
        // Wait until need to start
        printf("[client] Press enter to start the game!\n");
        getchar();
        // Send start request
        ProtocolStartGameResponse startGameResponse;
        startGameResponse.playerInfos = malloc(sizeof(ProtocolPlayerInfo) * MAX_GAME_PLAYERS);
        int startSuccess = startGame(&startGameResponse, createGameResponse.gameID, createGameResponse.playerPassword);
        if (!startSuccess) {
            fprintf(stderr, "[client] Failed to start game\n");
            return 1;
        }
        printf(
            "[client] Starting game! Player count: %d, Field ID: %d, field width: %d, field height: %d\n",
            startGameResponse.playerInfoCount,
            startGameResponse.field.id,
            startGameResponse.field.width,
            startGameResponse.field.height
        );
        // Start the game
        runGame();
    } else if (joinGameFlag) {
        printf("[client] Joining game w/ ID: %d\n", joinGameID);
        runGame();
    }

    return 0;
}
