# Auto sacīkstes
A client-server racing game. Developed for a linux systems programming course in university.  

## Compilation
For compilation you need `make` and `libsdl2-dev`.  

Server compilation
```
cd ./server
make
```

Client compilation
```
cd ./client
make
```

## Project structure 
```
$ tree
.
├── client - Contains client-side code
│   ├── const.h
│   ├── init.c - Code for initialising SDL window
│   ├── init.h
│   ├── input.c - Code for receiving input from player
│   ├── input.h
│   ├── main.c
│   ├── makefile
│   ├── networking - Socket creation & connection establishment related code 
│   │   ├── socket.c
│   │   └── socket.h
│   ├── queries - Queries that can be resolved by the server, used mostly by main.c
│   │   ├── lobby.c
│   │   ├── lobby.h
│   │   ├── ping.c
│   │   └── ping.h
│   ├── scene.c - Scene management code. Mostly coordinate transformations & SDL_Rect management
│   ├── scene.h 
│   ├── state.c - Client state storage & client connection stte storage & state manipulation
│   └── state.h
├── common - Contains code shared between client-side and server-side
│   ├── const.h - Constants shared between client-side and server-side
│   ├── logic - Code related for coordinate struct manipulations
│   │   ├── math.c
│   │   ├── math.h
│   │   └── math.o
│   ├── networking - Code related to socket communications
│   │   ├── fetch.c
│   │   ├── fetch.h
│   │   ├── fetch.o
│   │   ├── translate.c
│   │   ├── translate.h
│   │   ├── translate.js
│   │   └── translate.o
│   ├── protocol - Autosacikstes-specific protocol structs & constants & message serialization
│   │   ├── protocol_messages.c
│   │   ├── protocol_messages.h
│   │   ├── protocol_messages.o
│   │   └── protocol_types.h
│   └── state.h - Game state & connection state related structs shared between client-side and server-side
├── play.png
├── README.md
├── server
│   ├── const.h - Server-specific constants
│   ├── logic - Functions used by routes to mutate game states
│   │   ├── lobby.c
│   │   └── lobby.h
│   ├── main.c
│   ├── makefile
│   ├── networking - Socket creation & connection establishment related code 
│   │   ├── socket.c
│   │   └── socket.h
│   ├── router.c - Code that parses incoming requests, triggers routes & sends responses
│   ├── router.h
│   ├── routes - Code for handling every specific request "type"
│   │   ├── error.c - Error handling
│   │   ├── error.h
│   │   ├── lobby.c - Game creation
│   │   ├── lobby.h
│   │   ├── ping.c - Ping
│   │   └── ping.h
│   ├── state.c - Game server state
│   └── state.h
└── start.png
```

## Usage

![Start client and server](start.png)  
  
![Play game](play.png)  

## Notes
Actual gameplay wasn't implemented because of limited time for development.  
To finish the project, the following functionality would be required:  
- UDP connectivity between client & server
- Sending controls from client to server
- Primitive physics engine in server
- Scheduled, broadcasted game state updates
- Winning player detection & broadcasting

