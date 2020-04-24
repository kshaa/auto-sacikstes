#include <unistd.h>
#include "const.h"
#include "init.h"
#include "input.h"
#include "state.h"
#include "scene.h"
#include "networking/translate.h"
#include "queries/game.h"

// External getopt parameters
extern char *optarg;
extern int optind;

// Usage instruction
const char usageFormat[] =
    "Usage: %s [FLAGS]\n"
    "Connection flags:\n"
    "   -p Server port\n"
    "   -h Server IPv4 address\n"
    "Action flags:\n"
    "   -l List running games on server\n"
    "   -j Join game with ID\n"
    "At least one action flag is required\n";

int main(int argc, char *argv[]) {
    // Prepare action
    int listGamesFlag = 0;
    int joinGameFlag = 0;
    int joinGameID = 0;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "h:p:lj:")) != -1) {
        switch (option) {
            case 'p':
                server.port = atoi(optarg);
                break;
            case 'h':
                strncpy(server.addressSerialized, optarg, 16);
                server.address = translateIPAddress(server.addressSerialized);
                if (server.address == INVALID_IP_ADDRESS) {
                    fprintf(stderr, "[client] Invalid server address provided\n");
                    fprintf(stderr, usageFormat, argv[0]);
                }
                break;
            case 'l':
                listGamesFlag = 1;
                break;
            case 'j':
                joinGameFlag = 1;
                joinGameID = atoi(optarg);
                break;
            default:
                fprintf(stderr, "[client] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }

    // At least one action is required
    if (!(listGamesFlag || joinGameFlag)) {
        fprintf(stderr, "[client] At least one action is required\n");
        fprintf(stderr, usageFormat, argv[0]);
        return 1;
    }

    // Init state
    int stateSuccess = initState();
    if (!stateSuccess) {
        fprintf(stderr, "[client] Failed to initialize state\n");
        return 1;
    }
    
    // Connect to server
    int connectionSuccess = initConnection();
    if (!connectionSuccess) {
        fprintf(stderr, "[client] Failed to initialize connection to server\n");
        return 1;
    }

    // Process the provided action
    if (listGamesFlag) {
        printf("[client] Listing games\n");
        int gameCount = getGameCount();
    } else if (joinGameFlag) {
        printf("[client] Joining game w/ ID: %d\n", joinGameID);

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
            if (controls.up) {
                printf("Up");
            }

            if (controls.down) {
                printf("Down");
            }

            if (controls.left) {
                printf("Left");
            }

            if (controls.right) {
                printf("Right");
            }

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

    return 0;
}
