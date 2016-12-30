/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 3
Using -			Common.h		 
	
Description -	The List implementation header file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef LIST_MANAGMENT_H
#define LIST_MANAGMENT_H

#include "Common.h"

//the data base cell object
typedef struct DatabaseCell
{
	unsigned long		AccountNumber;
	double				InitialBalance;
	double				CurrentBalance;
	unsigned int		NumOfDeposits;
	unsigned int		NumOfWithdrawals;
	double				TotalDeposits;
	double				TotalWithdrawals;
	HANDLE				AccountSem;

	struct DatabaseCell		*next;
}DatabaseCell;

void PrintList(void);
void FreeDatabase(void);
void PushList(DatabaseCell* cell);
int IsInList(unsigned long AccountNumber);
void DeleteCell(unsigned long AccountNumber);
DatabaseCell* GetCellFromList(unsigned long AccountNumber);
DatabaseCell *CreateDatabaseCell(unsigned long AccountNumber, double InitialBalance);


#endif