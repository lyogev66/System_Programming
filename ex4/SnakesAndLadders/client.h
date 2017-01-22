/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		gamePlay.h
			common.h
			WinBase.h
			winsock2.h
			stdarg.h
	
Description -	The client header file contains a handle of the client case routine
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef CLIENT_H
#define CLIENT_H

#include "gamePlay.h"
#include "common.h"
#include <WinBase.h>	//for GetProcessId 
#include <winsock2.h>
#include <stdarg.h>        //implement printf + fprintf
#pragma comment(lib, "ws2_32.lib")


#define NO_WAIT				0
#define MAX_BUFFER_SIZE		250
#define MESSAGE_SIZE		81
#define USERNAME_SIZE		31
#define SCANF_INDEX			0
#define RECV_INDEX			1
#define NUMBER_OF_THREADS	2
#define SERVER_ADDRESS		"127.0.0.1"




//a struct for client data handling
typedef struct clientStruct{                        
                char userName[USERNAME_SIZE];
                char pawnType;
                SOCKET clientSocket;
				volatile BOOL winner;
				volatile BOOL myTurn;
}clientStruct;

//a struct contaning the data that is passed around to functions and sub routines
typedef struct ARGUMENTS_S{
                clientStruct client;
                DWORD WaitCode;
                FILE *logFile;
                char recvBuffer[MAX_BUFFER_SIZE];
                char sendBuffer[MAX_BUFFER_SIZE];
                int recvBufferSize,sendBufferSize;
				HANDLE engineWorkingEvent;
				HANDLE msgInProgress;
				HANDLE writingLock;
				HANDLE *threadEvents;
				boardStruct board;
				volatile BOOL closeAllThreads;
}ARGUMENTS_S;
 
int clientRoutine(char userName[USERNAME_SIZE], int portNumber,char* logFile);
 
#endif
