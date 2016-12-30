/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 3
Using -		stdio.h
			Windows.h
	
Description -	The common header file contains a check for various kinds of situations
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <Windows.h>

extern volatile HANDLE g_ErrorEvent;

#define ERROR_CODE		0
#define THREAD_NUMBER	1

#define CHECK_FILE_CREATION(arg) {if(0 != arg) \
										{ printf("File creation failed with %d\n",GetLastError()); exit(GetLastError());}}

#define CHECK_STRING_ALLOCATION(arg) { if(NULL==arg) \
										{ printf("string allocation failed with %d\n ",GetLastError()); exit(GetLastError());}}

#define PRINT_STRING(arg) printf("%s ",arg)


#define CHECK_THREAD_CREATION(arg) { if(NULL == arg) \
										{ printf("Thread creation failed with %d\n ",GetLastError()); exit(GetLastError());}}

#define CHECK_MEMORY_ALLOCATION(arg) { if(NULL == arg) \
										{ printf("Memmory allocation failed with %d\n ",GetLastError()); exit(GetLastError());}}

#define CHECK_ALLOCATION(arg) { if(NULL == arg) \
										{ printf("Object allocation failed with %d\n ",GetLastError()); exit(GetLastError());}}

#define CHECK_MUTEX_CREATION(arg) {	if (arg == NULL)\
										{ printf("CreateMutex error: %d\n", GetLastError());	 exit(GetLastError());	}\
											else {	if (GetLastError() == ERROR_ALREADY_EXISTS)\
														{printf("CreateMutex opened existing mutex\n");exit(GetLastError());} \
														else printf("CreateMutex created new mutex\n");	}}

#define CHECK_WAIT_MULTIPLE(arg) {if((WAIT_OBJECT_0 != arg) && (WAIT_TIMEOUT != arg)) \
										{ printf("WaitForMutlipleObjets failed Error: %d\n", GetLastError());	 exit(GetLastError());	}}

#define CHECK_EVENT_CREATION(arg) {if(NULL == arg) \
										{ printf("Error creating Event Error: %d\n", GetLastError());	 exit(GetLastError());	}}
 
#define CHECK_WAIT_FOR_SINGLE(arg) {if(WAIT_OBJECT_0 != arg)\
										{printf("Error in getting semaphore Error: %d\n", GetLastError());\
										if(	ERROR == SetEvent(g_ErrorEvent))\
											printf("Failed to signal event Error %d\n",GetLastError()); \
										 ExitThread(ERROR_EXIT);}}


#define CHECK_RELEASE(arg) {if(ERROR == arg)\
										{printf("Error in releasing mutex Error: %d\n", GetLastError());\
										if(ERROR == SetEvent(g_ErrorEvent))\
											printf("Failed to signal event Error %d\n",GetLastError()); \
										 ExitThread(ERROR_EXIT);}}
#endif