/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 3
Using -			ListManagment.h	 
	
Description -	The List implementation
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "ListManagment.h"

extern DatabaseCell* g_DatabaseHead; //imports g_DatabaseHead, the head of the list
extern FILE* g_RuntimeLogFile;//imports an handle to the runtime log file
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		PushList
Input arguments:	DatabaseCell* cell - a pointer to the cell to be inserted

return:				None

Description-		inserts a cell struct into the global list in a sorted method
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void PushList(DatabaseCell* cell)
{
	DatabaseCell *current = g_DatabaseHead, *previous=NULL;
	cell ->next = NULL;
	if(g_DatabaseHead == NULL)// handels empty list
	{
		g_DatabaseHead = cell;
		return;
	}
	if(current->AccountNumber >= cell->AccountNumber)//handle situation that the first node at the list allready has lower process_id then the newely entered cell
	{
		cell->next = current;
		g_DatabaseHead = cell;
		return;
	}
	previous = current;
	current = current->next;

	while(current != NULL && current->AccountNumber <= cell->AccountNumber)
	{
		previous = current;
		current = current->next;
	}

	cell ->next = current;
	previous->next = cell;
	return;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CreateDatabaseCell
Input arguments:	unsigned long	AccountNumber- the account number of the new cell
					double	InitialBalance- the initial balance of the new cell

return:				DatabaseCell* cell	-a pointer to the new cell

Description-		creates a new node for the list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
DatabaseCell *CreateDatabaseCell(unsigned long		AccountNumber,
	double				InitialBalance)
{
	DatabaseCell * node = NULL;
	node = (DatabaseCell*)malloc(sizeof(DatabaseCell));
	CHECK_MEMORY_ALLOCATION(node);

	node->AccountNumber		= AccountNumber;
	node->InitialBalance	= InitialBalance;
	node->CurrentBalance	= InitialBalance;
	node->NumOfDeposits		= 0;
	node->NumOfWithdrawals	= 0;
	node->TotalDeposits		= 0;
	node->TotalWithdrawals	= 0;
	node->AccountSem =     CreateMutex(NULL,              // default security attributes
										FALSE,             // initially not owned
										NULL);	
	CHECK_ALLOCATION(node->AccountSem);
	node ->next = NULL;
	return node;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		DeleteCell
Input arguments:	unsigned long	AccountNumber- the account number to be deleted

return:				None

Description-		deletes an account from the global list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void DeleteCell(unsigned long AccountNumber)
{
	DatabaseCell *previus = NULL;
	DatabaseCell *current = g_DatabaseHead;
	DatabaseCell *tmp = NULL;

	if(current->AccountNumber == AccountNumber)
	{
		tmp = current;
		g_DatabaseHead = current->next;
		tmp->next = NULL;
		free(tmp);
		return;
	}
	while(current)
	{
		if(current->AccountNumber == AccountNumber)
		{
			previus->next = current->next;
			current->next =NULL;
			free(current);
			return;
		}
		previus = current;
		current = current->next;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		IsInList
Input arguments:	unsigned long	AccountNumber- the account number to be searched

return:				int				TRUE or FALSE according the the account number

Description-		search for an account number in the global list and returns TRUE if found else returns FALSE 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int IsInList(unsigned long AccountNumber)
{
	DatabaseCell *tmp = NULL;
	for(tmp = g_DatabaseHead; tmp != NULL ; tmp = tmp->next)
	{
		if(tmp->AccountNumber == AccountNumber)
			return TRUE;
	}
	return FALSE;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		PrintList
Input arguments:	None

return:				None 

Description-		prints the global list into the output runtime logfile
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void PrintList(void)
{
	DatabaseCell *tmp = g_DatabaseHead;
	if(g_DatabaseHead == NULL)
		return;
	for(tmp = g_DatabaseHead; tmp != NULL ; tmp = tmp->next)
	{
		fprintf(g_RuntimeLogFile,"%lu,%.2lf\n",tmp->AccountNumber,tmp->CurrentBalance);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		FreeDatabase
Input arguments:	None

return:				None 

Description-		frees all the elements in the global list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void FreeDatabase(void)
{
	DatabaseCell *tmp = NULL;

	while (g_DatabaseHead)
	{
		tmp = g_DatabaseHead;
		g_DatabaseHead = g_DatabaseHead->next;
		CloseHandle(tmp->AccountSem);
		free(tmp);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		GetCellFromList
Input arguments:	unsigned long AccountNumber -an account number to be returned

return:				DatabaseCell* a pointer to the specific cell

Description-		returns a pointer to a cell from the global list accrding to it's accoutn number
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
DatabaseCell* GetCellFromList(unsigned long AccountNumber)
{
	DatabaseCell *tmp = NULL;
	for(tmp = g_DatabaseHead; tmp != NULL ; tmp = tmp->next)
	{
		if(tmp->AccountNumber == AccountNumber)
			return tmp;
	}
	return NULL;
}