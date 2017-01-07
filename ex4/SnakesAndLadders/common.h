#ifndef COMMON_H
#define	COMMON_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ws2tcpip.h>
#include <Ws2spi.h>
#include <WinSock.h>


#define EXIT_ERROR			-1
#define OK					1
#define BUFFERSIZE			250



#define CHECK_THREAD_CREATION(arg) { if(NULL == arg) \
										{ printf("Thread creation failed with %d\n ",GetLastError()); exit(GetLastError());}}

#define CHECK_MUTEX_CREATION(arg) {	if (arg == NULL)\
										{ printf("CreateMutex error: %d\n", GetLastError());	 exit(GetLastError());	}\
											else {	if (GetLastError() == ERROR_ALREADY_EXISTS)\
														{printf("CreateMutex opened existing mutex\n");exit(GetLastError());} \
														else printf("CreateMutex created new mutex\n");	}}



#endif