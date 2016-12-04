/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 2
Using -				 
	
Description -	The thead function header file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MY_ERROR				0
#define STATUS_OK				1
#define THREAD_NUMBER			4
#define	SIZE_QUANT				1024
#define NumberOfBytesToRead		5
#define ALL_THREAD_FINNISHED	15
#define NUMBER_OF_ARGUMENTS		3
#define NO_TOKEN_FOUND			0x15

typedef struct
{
	FILETIME	CreationTime;
	FILETIME	lastmodified;
	SYSTEMTIME	UTC_lastmodified;
	SYSTEMTIME	UTC_CreationTime;
	SYSTEMTIME	UTC_Isreal_lastmodified;
	SYSTEMTIME	UTC_Israel_CreationTime;
}FILE_TIMES;

typedef struct
{
	char *file_extention;
	char *file_name;
	char *First_five_chars;
}FILE_STRINGS;

typedef enum FILE_UNITS
{
	B,
	KB,
	MB,
	GB,
	TB,
}FILE_UNITS;

typedef struct
{
	double file_size;
	FILE_UNITS units;
}SIZE_INFORMATION;

typedef enum
{
	TIME_THREAD,
	SIZE_FILE_THREAD,
	FIRST_5_CHARS_THREAD,
	EXTENTION_FILE_THREAD
}THREAD_ID;

//typedef struct
//{
//	DWORD exit_code_time_thread;
//	DWORD exit_code_size_file_thread;
//	DWORD exit_code_first_5_chars_thread;
//	DWORD exit_code_extention_file_thread;
//}EXIT_CODES;


#pragma pack(1)
typedef struct {
	char TIME_THREAD_STATUS				:1;
	char SIZE_FILE_THREAD_STATUS		:1;
	char FIRST_5_CHARS_THREAD_STATUS	:1;
	char EXTENTION_FILE_THREAD_STATUS	:1;
} THREAD_STATUS;

typedef union STATUS_DATA
{
	THREAD_STATUS ts;
	char c_ts;
}STATUS_DATA;


#endif