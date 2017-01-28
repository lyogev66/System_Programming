/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		common.h
			tchar.h
	
Description -	The server header file 
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef SERVER_H
#define SERVER_H

#include "common.h"
#include <tchar.h>

#define WIN32_LEAN_AND_MEAN

#define MAXPLAYERSNUMBER	4
#define MINUTE				60000
#define MAX_MESSAGE_SIZE	250
#define	ROUTINE_NUMBER		2
#define MAX_ACCEPT_INDEX	9


#define	PLAYER_UPDATE_LOCK			_T("playersUpdateLock")
#define MESSAGE_IN_PROGRESS			_T("messageInProgress")
#define	WRITE_PROTECT				_T("writingToLogFile")
#define	SEND_MESSAGE_EVENT			_T("sendingMessage")
#define	MESSAGE_DONE_EVENT			_T("doneSendingMessage")


typedef enum
{
	ALL_OK_E,
	ERROR_ECCURED_E,
	WINNER_FOUND_E
}E_EXIT_REASON;



typedef struct
{
	E_MESSAGE_TYPE e_msgType;
	char playerSenderName	[PLAYER_NAME_SIZE];
	char playerRecieverName [PLAYER_NAME_SIZE];
	char playerSenderPawn;
	char message			[MAX_MESSAGE_SIZE];
	unsigned int msgSize;
	int	diceResult;
	int playerNumber;

}MsgBox_S;

typedef struct
{
	char 				playerName[PLAYER_NAME_SIZE];
	char				playerPawn;
	SOCKET				playerSocket;
	int					playerNumber;
	volatile BOOL		playerTurn;
	volatile BOOL		gameStarted;
	volatile BOOL		playerPlayed;
	MsgBox_S			*msgBox;
}playersInformation;


typedef struct
{
	SOCKET	tmpSocket;
	char	playerName[30];
	BOOL	isRegistered;
	BOOL	activePlayer;
}temporaryData;

typedef struct
{
	playersInformation			playerInfo[MAXPLAYERSNUMBER+1];
	SOCKET						serverSocket;
	int							activePlayersNum;
	volatile int				playerTurn;
	temporaryData				tmpData[10];
	volatile BOOL				gameStarted;
	FILE						*logFile;
	MsgBox_S					msgBox;
	volatile E_EXIT_REASON		exitRes;
	volatile BOOL		playerPlayed;

}DataCollection;

typedef enum
{
	GET_PLAYERS_THREAD_E,
	SEND_MESSAGE_ROUTINE_E,
	LAST_THREAD_E
}THREADS_ID_E;

int serverRoutine(int portNumber,char* logFile);

#endif