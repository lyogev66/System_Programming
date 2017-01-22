/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		stdio.h
			stdlib.h
			ws2tcpip.h
			Ws2spi.h
			WinSock.h
			time.h
			tchar.h
	
Description -	The common header file contains values that are used by both the server and the client
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef COMMON_H
#define	COMMON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include <Ws2spi.h>
#include <WinSock.h>
#include <time.h>
#include <tchar.h>

#pragma warning(disable : 4996)


#define EXIT_ERROR			-1
#define OK					1
#define BUFFERSIZE			250
#define	PLAYER_NAME_SIZE	31


#define CHECK_THREAD_CREATION(arg) { if(arg == NULL)\
										{ printf("Thread creation failed with %d\n",GetLastError()); exit(GetLastError());}}


#define CHECK_EVENT_CREATION(arg)  {if(arg == NULL)\
										{ printf("Error creating Event Error: %d\n",GetLastError()); exit(GetLastError());}}


#define CHECK_MUTEX_CREATION(arg) {	if(arg == NULL)\
										{ printf("CreateMutex error: %d\n", GetLastError());	 exit(GetLastError());	}\
											else {	if (GetLastError() == ERROR_ALREADY_EXISTS)\
														{printf("CreateMutex opened existing mutex\n");exit(GetLastError());} \
														else /*printf("CreateMutex created new mutex\n")*/;	}}

#define CHECK_WAIT_FOR_OBJECT(arg) {if(arg != WAIT_OBJECT_0)\
										{ printf("wait fr single objec error: %d\n", GetLastError());exit(GetLastError());}}
   

typedef enum
{
	GAME_MESSAGE_E,
	BROADCAT_MESSAGE_E,
	PLAYER_TO_PLAYER_MESSAGE_E,
	SINGLE_PLAYER_MESSAGE_E,
	PLAYER_TURN_MSG_E,
	NEW_PLAYER_MESSAGE_E,
	PLAYER_LIST_MSG_E,
	SINGLE_PLAYER_LIST_MSG_E,
	BROADCAST_PLAYER_LIST_MSG_E,
	EXIT_MESSAGE_E,
	WINNER_MESSAGE_E,
	NONE
}E_MESSAGE_TYPE;

#endif