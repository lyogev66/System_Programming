#include "ListManagment.h"

extern DatabaseCell* g_DatabaseHead;

char* u_long2String(unsigned long* num)
{
	int len = 2;
	unsigned long tmp = *num;
	char* str = NULL;

	while(tmp > 0)
	{
		tmp/=10;
		len++;
	}

	str = (char*)calloc(len +1,sizeof(char));
	CHECK_STRING_ALLOCATION(str);
	sprintf(str,"%lu",*num);
	return str;
}

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
	node->NumOfDeposits		= 0;
	node->AccountSem =     CreateMutex(NULL,              // default security attributes
										FALSE,             // initially not owned
										u_long2String(&node->AccountNumber));
	CHECK_ALLOCATION(node->AccountSem);
	node ->next = NULL;
	return node;
}

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
		return;
	}
	while(current)
	{
		if(current->AccountNumber == AccountNumber)
		{
			previus->next = current->next;
			current->next =NULL;
			return;
		}
		previus = current;
		current = current->next;
	}
}

int SearchList(unsigned long AccountNumber)
{
	DatabaseCell *tmp = NULL;
	for(tmp = g_DatabaseHead; tmp != NULL ; tmp = tmp->next)
	{
		if(tmp->AccountNumber == AccountNumber)
			return 1;
	}
	return 0;
}

void PrintList(void)
{
	DatabaseCell *tmp = g_DatabaseHead;
	if(g_DatabaseHead == NULL)
		return;
	for(tmp = g_DatabaseHead; tmp != NULL ; tmp = tmp->next)
	{
		printf("%lu with %.2lf\n",tmp->AccountNumber,tmp->CurrentBalance);
	}
}

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