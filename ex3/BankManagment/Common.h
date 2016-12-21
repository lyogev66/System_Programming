#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <Windows.h>


#define ERROR_CODE		0
#define THREAD_NUMBER	1

#define CHECK_FILE_CREATION(arg) {if(0 != arg) \
										{ printf("File creation failed with %d\n",GetLastError()); exit(0);}}

#define CHECK_STRING_ALLOCATION(arg) { if(NULL==arg) \
										{ printf("string allocation failed with %d\n ",GetLastError()); exit(0);}}

#define PRINT_STRING(arg) printf("%s ",arg)


#define CHECK_THREAD_CREATION(arg) { if(NULL==arg) \
										{ printf("Thread creation failed with %d\n ",GetLastError()); exit(0);}}

#define CHECK_MEMORY_ALLOCATION(arg) { if(NULL==arg) \
										{ printf("Memmory allocation failed with %d\n ",GetLastError()); exit(0);}}

#define CHECK_ALLOCATION(arg) { if(NULL==arg) \
										{ printf("Object allocation failed with %d\n ",GetLastError()); exit(0);}}

//#define CHECK_WAIT_RETURN_VALUE(WaitReturnValue) {if (WaitReturnValue==WAIT_FAILED) \
//										{printf("Wait function failed with %d \n" ,GetLastError()); exit(0);}}

#endif