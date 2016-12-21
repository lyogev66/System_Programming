#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include "BankManagment.h"

volatile DatabaseCell* g_DatabaseHead = NULL;
HANDLE *g_HandlesArray = NULL;
int g_WaitCode = 0;
DWORD g_HandelArraySize = 0;
HANDLE g_ThreadLocker;
HANDLE g_FileLocker;
FILE* g_RuntimeLogFile = NULL;

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
	Sleep(SLEEP_TIME);
	printf("CreatAccount waked up \n");
	if(IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Account number %lu already exists. Can't create account. Skipping command. !!!\n",AccountNumber);
	else
	{
		PushList(CreateDatabaseCell(AccountNumber,InitialBalance));
		fprintf(g_RuntimeLogFile,"Successfully created bank account number %lu with current balance of %lf.\n",Args->AccountNumber,Args->Balance);
	}
	free(Args);
	printf("CreatAccount finnished \n"); 
}

void CloseAccount(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	printf("CloseAccount going to sleep \n");
	Sleep(SLEEP_TIME);
	printf("CloseAccount waked up \n");
	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Account number %lu doesn't exist. Can't close account. Skipping command. !!!\n",AccountNumber);
	else
	{
		DeleteCell(AccountNumber);
		fprintf(g_RuntimeLogFile,"Successfully closed bank account number %lu.\n",AccountNumber);
	}
	free(Args);
	printf("CloseAccount finnished \n");
}

void PrintBalances(ARGUMENTS_S* Args)
{
	printf("PrintBalances going to sleep \n");
	Sleep(SLEEP_TIME);
	printf("PrintBalances waked up \n");
	fprintf(g_RuntimeLogFile,"Current balances in bank accounts are:\n");
	fprintf(g_RuntimeLogFile,"Bank Account #,Current Balance\n");
	PrintList();
	free(Args);
	printf("PrintBalances finnished\n");
}

void Deposit(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double Amount = Args->Amount;
	DatabaseCell *tmp = NULL;

	printf("Deposit going to sleep \n");
	Sleep(SLEEP_TIME);
	printf("Deposit waked up \n");

	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Unable to deposited %.2lf to account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{
		tmp = GetCellFromList(AccountNumber);
		WaitForSingleObject(tmp->AccountSem,INFINITE);
													/*~~~~CRITICAL REGION~~~*/
		tmp->CurrentBalance += Amount;
		tmp->TotalDeposits	+= Amount;
		tmp->NumOfDeposits ++;

		WaitForSingleObject(g_FileLocker,INFINITE);
		fprintf(g_RuntimeLogFile,"Successfully deposited %.2lf to account number %lu.\n",Amount,AccountNumber);
													/*~~~~CRITICAL REGION~~~*/
		ReleaseMutex(g_FileLocker);	
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
	Sleep(SLEEP_TIME);
	printf("Withdrawal waked up \n");

	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Unable to withdrew %.2lf from account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{
		tmp = GetCellFromList(AccountNumber);
		WaitForSingleObject(tmp->AccountSem,INFINITE);

											/*~~~~CRITICAL REGION~~~*/
		tmp->CurrentBalance		-= Amount;
		tmp->TotalWithdrawals	+= Amount;
		tmp->NumOfWithdrawals ++;
		WaitForSingleObject(g_FileLocker,INFINITE);
		fprintf(g_RuntimeLogFile,"Successfully withdrew %.2lf from account number %lu.\n",Amount,AccountNumber);
		ReleaseMutex(g_FileLocker);
											/*~~~~CRITICAL REGION~~~*/
		ReleaseMutex(tmp->AccountSem);
	}

	free(Args);
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
		free(Args);
		return;
	}
	sscanf(CommandLine,"%s",command);

	if (!strcmp(command,"CreateAccount"))
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
		sscanf(CommandLine,"%s %lu",command,&Args->AccountNumber);
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
		return;
	}
	else if (!strcmp(command,"Deposit"))
	{
		sscanf(CommandLine,"%s %lu %lf",command,&Args->AccountNumber,&Args->Amount);
		g_WaitCode		= NO_WAIT;
		Args->WaitCode	= NO_WAIT;
		functionPtr = &Deposit;
		ThreadCreation(functionPtr,Args);
		return;
	}	
	else if (!strcmp(command,"Withdrawal"))
	{
		sscanf(CommandLine,"%s %lu %lf",command,&Args->AccountNumber,&Args->Amount);
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
		WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,Args->WaitCode);//not including the yet to be initiated thread.

	tmpHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)functionPtr,Args,0,&ThreadID);
	CHECK_THREAD_CREATION(tmpHandle);

	AddHandleToArray(tmpHandle);
}

void ReadInputFileRoutine(char* filename)
{
	errno_t err;
	FILE *Instream = NULL;
	char *CommandLine = NULL;

	err = fopen_s(&Instream,filename,"r+");
	CHECK_FILE_CREATION(err);
	while(!feof(Instream))
	{
		CommandLine = ReadLineFromFile(Instream);
		if(CommandLine == NULL)
		{
			printf("Empty line..continuing\n");
			continue;
		}
		RunCommand(CommandLine);
		free(CommandLine);
		printf("waiting for %d items \n",g_HandelArraySize);
		WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,g_WaitCode);
	}
	
	WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,INFINITE);
	fprintf(g_RuntimeLogFile,"Program successfully finished running. Exiting.");
	printf("EOF reached \n");
}

void AddHandleToArray(HANDLE handle)
{
	DWORD ReleaseRes;
	DWORD WaitRes = WaitForSingleObject( g_ThreadLocker, INFINITE );
	DWORD index;
	DWORD WaitReturnValue;
	
	if(g_HandelArraySize < MAX_HANDLES_NUMBER)
	{
		g_HandelArraySize ++;
		if(g_HandlesArray == NULL)
			g_HandlesArray = (HANDLE*) calloc (g_HandelArraySize,sizeof(HANDLE));
		else
			g_HandlesArray = (HANDLE*)realloc(g_HandlesArray,g_HandelArraySize*sizeof(HANDLE));
		if(g_HandlesArray == NULL)
		{
			ReleaseRes = ReleaseMutex(g_ThreadLocker);
			printf("Inside AddHandleToArray Allocation failed\n");
			exit(GetLastError());
		}
		g_HandlesArray[g_HandelArraySize -1] = handle;
	}
	else
	{
		WaitReturnValue = WaitForMultipleObjects(g_HandelArraySize,g_HandlesArray,TRUE,INFINITE);	//waiting till all the thread finish and clearing the array
		if(WaitReturnValue == WAIT_FAILED)
		{
			ReleaseRes = ReleaseMutex(g_ThreadLocker);
			printf("Inside AddHandleToArray WaitForMultipleObjects failed\n");
			exit(GetLastError());
		}
		for(index = g_HandelArraySize; index > 0 ; index --)
		{
			//printf("removing index location=%d\n\n",index);
			CloseHandle(g_HandlesArray[index-1]);
			
		}
		g_HandelArraySize=1;
		g_HandlesArray = (HANDLE*)realloc(g_HandlesArray,g_HandelArraySize*sizeof(HANDLE));
		if(g_HandlesArray == NULL)
		{
			ReleaseRes = ReleaseMutex(g_ThreadLocker);
			printf("Inside AddHandleToArray Allocation failed\n");
			exit(GetLastError());
		}
		//printf("index location=%d\n\n",index);
		g_HandlesArray[index] = handle;
	}

	ReleaseRes = ReleaseMutex( g_ThreadLocker );
}

void CloseHandleArray()
{
	DWORD index;
	for(index = 0; index < g_HandelArraySize; index++)
	{
		CloseHandle(g_HandlesArray[index]);
	}
}

void CreateBlanceReport(char* BlanceReportFileName)
{
	errno_t err;
	FILE *Outstream = NULL;
	int TotalDeposits = 0, TotalWithdrawals = 0;
	volatile DatabaseCell *tmp = g_DatabaseHead;
	err = fopen_s(&Outstream,BlanceReportFileName,"w+");
	CHECK_FILE_CREATION(err);
	fprintf(Outstream,"Summary of balances in bank accounts:\n");
	fprintf(Outstream,"Bank Account #,Current Balance,Initial Balance,Total Deposited,Total	Withdrawal,# of Deposits,# of Withdrawals\n");
	while(tmp)
	{
		if (tmp ->next == NULL)
			fprintf(Outstream,"%lu,%.2lf,%.2lf,%.2lf,%.2lf,%lu,%lu",tmp->AccountNumber,
														tmp->CurrentBalance,
														tmp->InitialBalance,
														tmp->TotalDeposits,
														tmp->TotalWithdrawals,
														tmp->NumOfDeposits,
														tmp->NumOfWithdrawals);
		else
			fprintf(Outstream,"%lu,%.2lf,%.2lf,%.2lf,%.2lf,%lu,%lu\n",tmp->AccountNumber,
														tmp->CurrentBalance,
														tmp->InitialBalance,
														tmp->TotalDeposits,
														tmp->TotalWithdrawals,
														tmp->NumOfDeposits,
														tmp->NumOfWithdrawals);
		tmp = tmp->next;
		
	}
	fclose(Outstream);
}

void main(int argc, char *argv[])
{
	FILE* Outstream = NULL;

	if(argc < NUM_OF_ARGUMENTS)
	{
		printf("Incorrect number of arguments \n");
		exit(0);
	}

	g_ThreadLocker = CreateMutex(NULL,FALSE,NULL); 
	g_FileLocker = CreateMutex(NULL,FALSE,NULL); 

	g_RuntimeLogFile = fopen(argv[3],"w+");
	if(g_RuntimeLogFile == NULL)
	{
		printf("failed open RuntimeLogFile for writing\n");
		CloseHandle(g_ThreadLocker);
		CloseHandle(g_FileLocker);
		exit(GetLastError());
	}

	ReadInputFileRoutine(argv[1]);

	CreateBlanceReport(argv[2]);
	CloseHandleArray();
	FreeDatabase();
}

