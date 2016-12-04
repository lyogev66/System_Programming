/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 2
Using -				 
	
Description -	The TestManagar header file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <stdio.h>
#include <windows.h>
#include <tchar.h> 
#include <string.h>
#include <stdlib.h>
#include "list_implemetation.h"



#define CORRECT_ARGUMEN_NUMVER				4
#define OK									0
#define ERROR_CODE							-1
#define CHARACTER_SIZE						sizeof(char)
#define TARGET_NAME							"TestFile.exe"
#define PADDING_SPACE						3
#define END_OF_STRING_OFFSET				1
#define CREATE_DIRECTORY_FLAG				0
#define	CREATE_FILE_INSIDE_DIRECTORY_PATH	1
#define	SINGLE_OBJECT						1

typedef enum DATA_TYPE
{
	CHAR_PTR,
	PROCESS_LIST
}DATA_TYPE;



#endif