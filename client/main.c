#include <unistd.h>
#include "const.h"
#include "init.h"
#include "input.h"
#include "state.h"
#include "scene.h"
#include "../common/networking/translate.h"
#include "queries/lobby.h"
#include "queries/ping.h"
#include "../common/logic/math.h"

void runGame() {
    // Prepare scene from global storage
    initScene();

    // SDL window lifecycle
    startSDL();
    atexit(stopSDL);

    // Game loop
    int resizedRecently = app.resized;
    while (1) {
        // === Input engine
        registerInput();
        if (app.resized) {
            resizedRecently = app.resized;
            app.resized = 0;
        }

        // === Networking engine
        // Send input

        // === Physics engine
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

        // === Scene engine
        if (resizedRecently) {
            resizeScene();
        }

        // === Render engine
        renderScene();

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
    "   -i List fields available on server\n"
    "   -c [NAME] Create game with a given name \n"
    "   -j [ID] Join game with ID\n"
    "   -k Send keep-alive i.e. ping to server\n"
    "Parameter flags:\n"
    "   -u [NAME] Player name for created game\n"
    "   -f [ID] Field id for created game\n\n"

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
    int listFieldsFlag = 0;
    int createGameFlag = 0;
    int joinGameFlag = 0;

    // Input parameters
    char inputGameName[PROTOCOL_MAX_GAME_NAME];
    char inputPlayerName[PROTOCOL_MAX_PLAYER_NAME];
    int inputGameID = 0;
    int inputFieldID = 0;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "ikha:p:lj:c:u:f:")) != -1) {
        switch (option) {
            case 'i':
                listFieldsFlag = 1;
                break;
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
                inputGameID = atoi(optarg);
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
            case 'f':
                inputFieldID = atoi(optarg);
                if (inputFieldID == 0) {
                    fprintf(stderr, "[client] Field ID must be a positive integer as per protocol\n");
                    fprintf(stderr, usageFormat, argv[0]);
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "[client] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }

    // At least one action is required
    if (!(keepAliveFlag || helpFlag || listFieldsFlag || listGamesFlag || createGameFlag || joinGameFlag)) {
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
    } else if (listFieldsFlag) {
        ProtocolListFieldsResponse fieldIDs;
        int fieldCountSuccess = getFieldCount(&fieldIDs);
        if (!fieldCountSuccess) {
            fprintf(stderr, "[client] Failed to list fields\n");
            return 1;
        }
        if (fieldIDs.fieldCount > 0) {
            printf("[client] Available fields: ");
            for (int i = 0; i < fieldIDs.fieldCount; i++) {
                printf("%d ", i + 1);
            }
            printf("\n");
        } else {
            printf("[client] No fields available\n");
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
        // Load game info globally
        strncpy(game.info.name, inputGameName, PROTOCOL_MAX_GAME_NAME);
        game.info.status = WAITING_PLAYERS;
        // Load game player info globally
        game.selfPlayerID = createGameResponse.playerID;
        strncpy(game.selfPlayerPassword, createGameResponse.playerPassword, PROTOCOL_MAX_PASSWORD_LENGTH);
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
        printf("[client] Starting game!\n");
        // Debug sent lines
        if (DEBUG) {
            printf(
                "[client] Player count: %d, Field ID: %d, field width: %d, field height: %d\n",
                startGameResponse.playerInfoCount,
                startGameResponse.field.id,
                startGameResponse.field.width,
                startGameResponse.field.height
            );
            printf("[client] Field start line: ");
            printLine(&startGameResponse.startLine);
            printf("\n");
            for (int i = 0; i < startGameResponse.extraLineCount; i++) {
                printf("[client] Field extra line: ");
                printLine(&startGameResponse.extraLines[i]);
                printf("\n");
            }
        }

        // Store game info globally
        game.info.status = STARTED;

        // Store field in global storage
        memcpy(&game.field.info, &startGameResponse.field, sizeof(ProtocolFieldInfo));
        memcpy(&game.field.startLine, &startGameResponse.startLine, sizeof(ProtocolLine));
        game.field.extraLinesCount = startGameResponse.extraLineCount;
        game.field.extraLines = malloc(sizeof(ProtocolLine) * game.field.extraLinesCount);
        for (int i = 0; i < game.field.extraLinesCount; i++) {
            memcpy(&game.field.extraLines[i], &startGameResponse.extraLines[i], sizeof(ProtocolLine));
        }

        // Store players in global storage
        for (int i = 0; i < startGameResponse.playerInfoCount; i++) {
            game.player[i].created = 1;
            memcpy(&game.player[i].info, &startGameResponse.playerInfos[i], sizeof(ProtocolPlayerInfo));
        }

        // Start the game
        runGame();
    } else if (joinGameFlag) {
        // Join game
        printf("[client] Joining game w/ ID: %d\n", inputGameID);

        ProtocolJoinGameResponse joinGameResponse;
        int joinGameSuccess = joinGame(&joinGameResponse, inputGameID, inputPlayerName);
        if (!joinGameSuccess) {
            fprintf(stderr, "[client] Failed to join game\n");
            return 1;
        }

        // Debug sent lines
        if (DEBUG) {
            printf(
                "[client] Joined game! Game ID: %d, player ID: %d, player password: %s\n",
                inputGameID,
                joinGameResponse.playerID,
                joinGameResponse.playerPassword
            );
        }
        
        printf("[TEMPORARY] Press enter to start the game!\n");
        getchar();

        runGame();
    }

    return 0;
}
