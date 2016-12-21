#ifndef BANK_MANAGMENT
#define BANK_MANAGMENT


#include "Common.h"
#include "ListManagment.h"

#define MAX_COMMAND_LENGTH			14
#define SLEEP_TIME					10
#define NO_WAIT						0

typedef enum
{
	READ_INPUT,
	SPARE1,
	SPARE2,
	SPARE3
}THREAD_NAME;

typedef struct arg
{
	double Amount;
	double Balance;
	unsigned long AccountNumber;
	DWORD WaitCode;
}ARGUMENTS_S;




void PrintBalances(ARGUMENTS_S* Args);
void Deposit(ARGUMENTS_S* Args);
void Withdrawal(ARGUMENTS_S* Args);
void CreatAccount(ARGUMENTS_S* Args);
void CloseAccount(ARGUMENTS_S* Args);
void AddHandleToArray(HANDLE handle);
//void ThreadCreation(void (*functionPtr)(ARGUMENTS_S),ARGUMENTS_S* Args);
void ThreadCreation(void (*functionPtr)(ARGUMENTS_S*),ARGUMENTS_S* Args);

#endif