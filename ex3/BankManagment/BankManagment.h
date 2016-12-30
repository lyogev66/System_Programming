/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 3
Using -			Common.h	
				ListManagment.h
	
Description -	The bank managment header file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef BANK_MANAGMENT
#define BANK_MANAGMENT


#include "Common.h"
#include "ListManagment.h"

#define MAX_COMMAND_LENGTH			14
#define SLEEP_TIME					10
#define NO_WAIT						0
#define MAX_HANDLES_NUMBER			64
#define NUM_OF_ARGUMENTS			3
#define	FORCED_EXIT					35
#define	ERROR_EXIT					66
#define	EXIT_OK						10
#define SET_OFFSET					1

//a struct containing the data for thread functions from the input file
typedef struct arg
{
	double Amount;
	double Balance;
	unsigned long AccountNumber;
	DWORD WaitCode;
}ARGUMENTS_S;


DWORD CheckForErrors(void);
void PrintBalances(ARGUMENTS_S* Args);
void Deposit(ARGUMENTS_S* Args);
void Withdrawal(ARGUMENTS_S* Args);
void CreatAccount(ARGUMENTS_S* Args);
void CloseAccount(ARGUMENTS_S* Args);
void AddHandleToArray(HANDLE handle);
void WaitForThreads(DWORD waitcode);
void ThreadCreation(void (*functionPtr)(ARGUMENTS_S*),ARGUMENTS_S* Args);


#endif