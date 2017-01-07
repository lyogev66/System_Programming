#ifndef SERVER_H
#define SERVER_H

#include "common.h"

#define WIN32_LEAN_AND_MEAN

#define MAXPLAYERSNUMBER	4
#define MINUTE				60000

int serverRoutine(int portNumber,char* logFile);

typedef struct
{
	char 				playerName[31];
	char				playerPawn;
	SOCKET				playerSocket;
//	WSAPROTOCOL_INFO	playerSocketInfo;
}playersInformation;

typedef struct
{
	playersInformation	playersInfo[MAXPLAYERSNUMBER];
	SOCKET				serverSocket;
	int					activePlayersNum;
}serverDataCollection;




#endif