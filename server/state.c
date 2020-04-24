#include "const.h"
#include "state.h"

int initState() {
    // Prepare games
    memset(&games, 0, sizeof(games));

    // Notify of state
    printf("[state] Initialized server state\n");

    return 1;
}
