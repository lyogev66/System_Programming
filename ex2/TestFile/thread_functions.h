#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MY_ERROR				0
#define THREAD_NUMBER			4
#define	SIZE_QUANT				1024
#define NumberOfBytesToRead		5
#define ALL_THREAD_FINNISHED	15
#define NUMBER_OF_ARGUMENTS		3

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

typedef enum
{
	EXIT_CODE_TIME_THREAD			=	0x10,
	EXIT_CODE_SIZE_FILE_THREAD		=	0x20,
	EXIT_CODE_FIRST_5_CHARS_THREAD	=	0x30,
	EXIT_CODE_EXTENTION_FILE_THREAD	=	0x40,
	EXIT_CODE_OK					=	0x00
}EXIT_CODES;


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