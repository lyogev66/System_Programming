#include "BankManagment.h"


DatabaseCell* g_DatabaseHead = NULL;
HANDLE *g_HandlesArray = NULL;
int g_WaitCode = 0;
DWORD g_HandelArraySize = 0;
HANDLE g_Locker;

int CheckAccountNumber(char *CommandLine)
{
	char *token = NULL, *ch = NULL,*CommandLineCopy = NULL ;
	char Bool = TRUE;
	Sleep(SLEEP_TIME);
	if(!strcmp(CommandLine,"PrintBalances"))
		return Bool;

	CommandLineCopy = (char*)calloc(strlen(CommandLine)+1,sizeof(char));
	CHECK_STRING_ALLOCATION(CommandLineCopy);
	strcpy(CommandLineCopy,CommandLine);

	token = strtok(CommandLineCopy, " ");
    token = strtok(NULL, " ");

	for(ch = token ; *ch!='\0'; ch++)
	{
		if((*ch < 48) || (*ch>57))
		{
			Bool = FALSE;
			break;
		}
	}
	free(CommandLineCopy);
	return Bool;
}

char* ReadLineFromFile(FILE* File)
{
	char ch;
	char *str = NULL;
	int length = 1,index = 0;

	ch = fgetc(File);
	while((ch!='\n') && (ch!= EOF) && (ch!= '\r'))
	{
		str = (char*) realloc((char*)str,++length);
		CHECK_STRING_ALLOCATION(str);
		str[index++] = ch;
		str[index] = '\0';
		ch = fgetc(File);
	}

	return str;
}

void CreatAccount(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double InitialBalance = Args->Balance;
	printf("CreatAccount going to sleep \n");
	Sleep(10000);
	printf("CreatAccount waked up \n");
	if(SearchList(AccountNumber))
		printf("!!! Account number %lu already exists. Can't create account. Skipping command. !!!\n",AccountNumber);
	else
	{
		PushList(CreateDatabaseCell(AccountNumber,InitialBalance));
	}
	free(Args);
	printf("CreatAccount finnsihed \n"); 
}

void CloseAccount(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	printf("CloseAccount going to sleep \n");
	Sleep(10000);
	printf("CloseAccount waked up \n");
	if(!SearchList(AccountNumber))
		printf("!!! Account number %lu doesn't exist. Can't close account. Skipping command. !!!\n",AccountNumber);
	else
	{
		DeleteCell(AccountNumber);
		printf("Successfully closed bank account number %lu.\n",AccountNumber);
	}
	free(Args);
	printf("CloseAccount finnished \n");
}

void PrintBalances()
{

	printf("PrintBalances going to sleep \n");
	Sleep(10000);
	printf("PrintBalances waked up \n");
	printf("Current balances in bank accounts are:\n");
	printf("Bank Account #,Current Balance\n");
	PrintList();
	printf("PrintBalances finnished\n");
}

void Deposit(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double Amount = Args->Amount;
	DatabaseCell *tmp = NULL;

	printf("Deposit going to sleep \n");
	Sleep(10000);
	printf("Deposit waked up \n");

	if(!SearchList(AccountNumber))
		printf("!!! Unable to deposited %.2lf to account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{
		tmp = GetCellFromList(AccountNumber);
		WaitForSingleObject(tmp->AccountSem,INFINITE);
		tmp->CurrentBalance += Amount;
		tmp->NumOfDeposits ++;
		printf("Successfully deposited %.2lf to account number %lu.\n",Amount,AccountNumber);
		ReleaseMutex(tmp->AccountSem);
	}
	free(Args);
	printf("Deposit finnished\n");
}

void Withdrawal(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double Amount = Args->Amount;
	DatabaseCell *tmp = NULL;

	printf("Withdrawal going to sleep \n");
	Sleep(10000);
	printf("Withdrawal waked up \n");

	if(!SearchList(AccountNumber))
		printf("!!! Unable to withdrew %.2lf from account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{
		tmp = GetCellFromList(AccountNumber);
		WaitForSingleObject(tmp->AccountSem,INFINITE);
		tmp->CurrentBalance -= Amount;
		tmp->NumOfWithdrawals ++;
		printf("Successfully withdrew %.2lf from account number %lu.\n",Amount,AccountNumber);
		ReleaseMutex(tmp->AccountSem);
	}

	free(Args);
	ReleaseMutex(g_DatabaseHead->AccountSem);
	printf("Withdrawal finnished\n");
}

void RunCommand(char *CommandLine)
{
	char command[MAX_COMMAND_LENGTH]  = {0};
	void (*functionPtr)(ARGUMENTS_S*);

	ARGUMENTS_S* Args = (ARGUMENTS_S*) calloc(1,sizeof(ARGUMENTS_S));
	CHECK_ALLOCATION(Args);
	if(!CheckAccountNumber (CommandLine))
	{
		printf("Illegal Account Number\n");
		return;
	}
	sscanf(CommandLine,"%s",command);

	if (!strcmp(command,"CreatAccount"))
	{
		sscanf(CommandLine,"%s %lu %lf ",command,&Args->AccountNumber,&Args->Balance);
		g_WaitCode = INFINITE;
		Args->WaitCode = INFINITE;
		functionPtr = &CreatAccount;
		ThreadCreation(functionPtr,Args);
		return;
	}
	else if (!strcmp(command,"CloseAccount"))
	{
		sscanf(CommandLine,"%s %lu ",command,&Args->AccountNumber);
		g_WaitCode = INFINITE;
		Args->WaitCode = INFINITE;
		functionPtr = &CloseAccount;
		ThreadCreation(functionPtr,Args);
		return;
	}
	else if (!strcmp(command,"PrintBalances"))
	{
		g_WaitCode		=	INFINITE;
		Args->WaitCode	=	INFINITE;
		functionPtr = &PrintBalances;
		ThreadCreation(functionPtr,Args);
		free(Args);
		return;
	}
	else if (!strcmp(command,"Deposit"))
	{
		sscanf(CommandLine,"%s %lu %lf ",command,&Args->AccountNumber,&Args->Amount);
		g_WaitCode		= NO_WAIT;
		Args->WaitCode	= NO_WAIT;
		functionPtr = &Deposit;
		ThreadCreation(functionPtr,Args);
		return;
	}	
	else if (!strcmp(command,"Withdrawal"))
	{
		sscanf(CommandLine,"%s %lu %lf ",command,&Args->AccountNumber,&Args->Amount);
		g_WaitCode		= NO_WAIT;
		Args->WaitCode	= NO_WAIT;
		functionPtr = &Withdrawal;
		ThreadCreation(functionPtr,Args);
		return;
	}
	else
		printf("Illegal Command %s\n",command);

	return;

}

void ThreadCreation(void (*functionPtr)(ARGUMENTS_S*),ARGUMENTS_S* Args)
{
	DWORD ThreadID;
	HANDLE tmpHandle;

	if(g_HandlesArray!= NULL)
		WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,Args->WaitCode);
	tmpHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)functionPtr,Args,0,&ThreadID);
	CHECK_THREAD_CREATION(tmpHandle);

	AddHandleToArray(tmpHandle);
}

void ReadInputFileRoutine()
{
	errno_t err;
	FILE *Instream = NULL;
	char *CommandLine = NULL;

	err = fopen_s(&Instream,"Test_file.txt","r+");
	CHECK_FILE_CREATION(err);
	while(!feof(Instream))
	{
		CommandLine = ReadLineFromFile(Instream);
		RunCommand(CommandLine);

		printf("waiting for %d items \n",g_HandelArraySize);
		WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,g_WaitCode);
	}
	printf("EOF reached \n");
	WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,INFINITE);
}

void AddHandleToArray(HANDLE handle)
{
	DWORD ReleaseRes;
	DWORD WaitRes = WaitForSingleObject( g_Locker, INFINITE );
	
	if(g_HandlesArray == NULL)
	{
		g_HandelArraySize ++;
		g_HandlesArray = (HANDLE*) calloc (g_HandelArraySize,sizeof(HANDLE));
	}
	else
	{
		g_HandelArraySize ++;
		g_HandlesArray = (HANDLE*)realloc(g_HandlesArray,g_HandelArraySize*sizeof(HANDLE));
	}
	CHECK_ALLOCATION(g_HandlesArray);
	g_HandlesArray[g_HandelArraySize -1] = handle;

	ReleaseRes = ReleaseMutex( g_Locker );
}

void CloseHandleArray()
{
	DWORD index;
	for(index = 0; index < g_HandelArraySize; index++)
	{
		CloseHandle(g_HandlesArray[index]);
	}
}

void main()
{
	g_Locker = CreateMutex(NULL,FALSE, "LockerM");  
	ReadInputFileRoutine();
	CloseHandleArray();
}

