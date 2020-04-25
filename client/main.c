#include <unistd.h>
#include "const.h"
#include "init.h"
#include "input.h"
#include "state.h"
#include "scene.h"
#include "../common/networking/translate.h"
#include "queries/lobby.h"
#include "queries/ping.h"

// External getopt parameters
extern char *optarg;
extern int optind;

// Usage instruction
const char usageFormat[] =
    "Autosacikstes client\n\n"

    "Usage: %s [FLAGS]\n"
    "Connection flags:\n"
    "   -a Server IPv4 connection address\n"
    "   -p Server connection port\n"
    "Action flags:\n"
    "   -h Print this help\n"
    "   -l List running games on server\n"
    "   -j Join game with ID\n"
    "   -k Send keep-alive i.e. ping to server\n\n"

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
    int joinGameFlag = 0;
    int joinGameID = 0;

    // Parse options
    int option;
    while ((option = getopt(argc, argv, "kha:p:lj:")) != -1) {
        switch (option) {
            case 'k':
                keepAliveFlag = 1;
                break;
            case 'h':
                helpFlag = 1;
                break;
            case 'p':
                server.port = atoi(optarg);
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
            default:
                fprintf(stderr, "[client] Incorrect option provided\n");
                fprintf(stderr, usageFormat, argv[0]);
                return 1;
        }
    }
    
    // Validate port
    if (server.port == 0) {
        fprintf(stderr, "[server] Port must not be zero\n");
        return 1;
    }

    // At least one action is required
    if (!(keepAliveFlag || helpFlag || listGamesFlag || joinGameFlag)) {
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
        int gameCount = getGameCount();
        printf("[client] Currently there are %d games running\n", gameCount);
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

    return 0;
}
