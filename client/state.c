#include "state.h"

void initState() {
    // App state
    memset(&app, 0, sizeof(App));

    // Field state
    memset(&field, 0, sizeof(Field));

    // Controls state
    memset(&controls, 0, sizeof(Controls));
}