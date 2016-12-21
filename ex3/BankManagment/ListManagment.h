/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 2
Using -				 
	
Description -	The List implementation header file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef LIST_MANAGMENT_H
#define LIST_MANAGMENT_H

#include "Common.h"

typedef struct DatabaseCell
{
	unsigned long		AccountNumber;
	double				InitialBalance;
	double				CurrentBalance;
	int					NumOfDeposits;
	int					NumOfWithdrawals;
	HANDLE				AccountSem;
	struct DatabaseCell		*next;
}DatabaseCell;

void PrintList(void);
void PushList(DatabaseCell* cell);
int SearchList(unsigned long AccountNumber);
void DeleteCell(unsigned long AccountNumber);
DatabaseCell* GetCellFromList(unsigned long AccountNumber);
DatabaseCell *CreateDatabaseCell(unsigned long AccountNumber, double InitialBalance);

#endif