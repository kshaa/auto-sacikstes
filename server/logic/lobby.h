#ifndef LOGIC_LOBBY_H
#define LOGIC_LOBBY_H

int createGame(char * gameName, int fieldID);
int addGamePlayer(int gameID, char * playerName, int connectionfd);
int joinGame(int gameID, char * playerName, int connectionfd);

#endif