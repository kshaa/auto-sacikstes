#ifndef PROTOCOL_TYPES_H
#define PROTOCOL_TYPES_H

// Constants used in protocol
#define PROTOCOL_NO_PLAYER_ID -1
enum PROTOCOL_GAME_STATUS {
    WAITING_PLAYERS = 0,
    STARTED = 1,
    ENDED = -1
};

enum PROTOCOL_ACTION_X {
    X_LEFT = -1,
    X_NONE = 0,
    X_RIGHT = 1
};

enum PROTOCOL_ACTION_Y {
    Y_UP = 1,
    Y_NONE = 0,
    Y_DOWN = -1
};

// Complex types sent in protocol
typedef struct {
    float x;
    float y;
} ProtocolCoordinates;

typedef struct {
    int status;
    char name[20];
    int winnerId;
} ProtocolGame;

typedef struct {
    int id;
    char name[30];
    ProtocolCoordinates position;
    float angle;
    float speed;
    float acceleration;
    int laps;
} ProtocolPlayerInfo;

typedef struct {
    int id;
    char name[20];
    int width;
    int height;
} ProtocolFieldInfo;

typedef struct {
    ProtocolCoordinates beggining;
    ProtocolCoordinates end;
} ProtocolLine;

typedef struct {
    int x;
    int y;
} ProtocolAction;

#endif