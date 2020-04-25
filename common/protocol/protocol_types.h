#ifndef PROTOCOL_TYPES_H
#define PROTOCOL_TYPES_H

// Constants used in protocol
#define PROTOCOL_NO_PLAYER_ID -1
#define PROTOCOL_MAX_PASSWORD_LENGTH 10
#define PROTOCOL_MAX_FIELD_NAME 20
#define PROTOCOL_MAX_GAME_NAME 20
#define PROTOCOL_MAX_PLAYER_NAME 30
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

// Types used in protocol
typedef struct {
    float x;
    float y;
} ProtocolCoordinates;

typedef struct {
    int status;
    char name[PROTOCOL_MAX_GAME_NAME];
    int winnerId;
} ProtocolGameInfo;

typedef struct {
    int id;
    char name[PROTOCOL_MAX_PLAYER_NAME];
    ProtocolCoordinates position;
    float angle;
    float speed;
    float acceleration;
    int laps;
} ProtocolPlayerInfo;

typedef struct {
    int id;
    char name[PROTOCOL_MAX_FIELD_NAME];
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