/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 3
Using -			BankManagment.h 
	
Description -	The bank managment implementation
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include "BankManagment.h"

volatile DatabaseCell* g_DatabaseHead = NULL;	//a pointer to the head of the sorted list contaning all the bank accounts
volatile HANDLE g_ErrorEvent;					//an event indicates that an error occured at one of the threads
HANDLE *g_HandlesArray = NULL;					//a global handle array containing all the thread handles
FILE* g_RuntimeLogFile = NULL;					//an handle to the runtime log file
DWORD g_HandelArraySize = 0;					//the size of the global handle array 
HANDLE g_ThreadLocker;							//a mutex to protect handle array
HANDLE g_FileLocker;							//a mutex to protect when writing to the global runtime log file
int g_WaitCode = 0;								// aglobal defining the time needed to wait when a thread starts

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CheckAccountNumber
Input arguments:	char *CommandLine - the current action 

return:				int		- TRUE or FALSE

Description-		returns TRUE if the account number given is a valid number else false
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		ReadLineFromFile
Input arguments:	FILE* File - the commands file to be readed from

return:				char*		the current command

Description-		returns a line from the input file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CreatAccount
Input arguments:	ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		creates a new account
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void CreatAccount(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double InitialBalance = Args->Balance;
	DWORD ErrorInfo = WaitForSingleObject(g_ErrorEvent,NO_WAIT);

	free(Args); //freeing arguments struct

	if(WAIT_OBJECT_0 == ErrorInfo)
	{
		printf("Error occured at other thread - Exiting\n");
		ExitThread(FORCED_EXIT);
	}

	Sleep(SLEEP_TIME);
	if(IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Account number %lu already exists. Can't create account. Skipping command. !!!\n",AccountNumber);
	else
	{
		PushList(CreateDatabaseCell(AccountNumber,InitialBalance));
		fprintf(g_RuntimeLogFile,"Successfully created bank account number %lu with current balance of %.2lf.\n",AccountNumber,InitialBalance);
	}
	//printf("CreatAccount finnished \n");
	ExitThread(EXIT_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CloseAccount
Input arguments:	ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		close an account
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void CloseAccount(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	DWORD ErrorInfo = WaitForSingleObject(g_ErrorEvent,NO_WAIT);

	free(Args); //freeing arguments struct

	if(WAIT_OBJECT_0 == ErrorInfo)
	{
		printf("Error occured at other thread - Exiting\n");
		ExitThread(FORCED_EXIT);
	}

	Sleep(SLEEP_TIME);
	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Account number %lu doesn't exist. Can't close account. Skipping command. !!!\n",AccountNumber);
	else
	{
		DeleteCell(AccountNumber);
		fprintf(g_RuntimeLogFile,"Successfully closed bank account number %lu.\n",AccountNumber);
	}
	//printf("CloseAccount finnished \n");
	ExitThread(EXIT_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		PrintBalances
Input arguments:	ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		prints the current balnce in all the open accounts
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void PrintBalances(ARGUMENTS_S* Args)
{
	DWORD ErrorInfo = WaitForSingleObject(g_ErrorEvent,NO_WAIT);

	free(Args); //freeing arguments struct

	if(WAIT_OBJECT_0 == ErrorInfo)
	{
		printf("Error occured at other thread - Exiting\n");
		ExitThread(FORCED_EXIT);
	}

	Sleep(SLEEP_TIME);
	fprintf(g_RuntimeLogFile,"Current balances in bank accounts are:\n");
	fprintf(g_RuntimeLogFile,"Bank Account #,Current Balance\n");
	PrintList();
	//printf("PrintBalances finnished\n");
	ExitThread(EXIT_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		Deposit
Input arguments:	ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		making a deposit to a specified account number
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void Deposit(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double Amount = Args->Amount;
	DatabaseCell *tmp = NULL;
	DWORD ErrorInfo = WaitForSingleObject(g_ErrorEvent,NO_WAIT);
	DWORD WaitRes,ReleaseRes;

	free(Args); //freeing arguments struct

	if(WAIT_OBJECT_0 == ErrorInfo)
	{
		printf("Error occured at other thread - Exiting\n");
		ExitThread(FORCED_EXIT);
	}

	Sleep(SLEEP_TIME);

	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Unable to deposited %.2lf to account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{

		tmp = GetCellFromList(AccountNumber);
		WaitRes = WaitForSingleObject(tmp->AccountSem,INFINITE);
		CHECK_WAIT_FOR_SINGLE(WaitRes);

													/*~~~~CRITICAL REGION~~~*/
		tmp->CurrentBalance += Amount;
		tmp->TotalDeposits	+= Amount;
		tmp->NumOfDeposits ++;

		WaitRes = WaitForSingleObject(g_FileLocker,INFINITE);
		if(WAIT_OBJECT_0 != WaitRes)
		{
			printf("failed recieving g_FileLocker\n");
			ReleaseMutex(tmp->AccountSem);
			CHECK_WAIT_FOR_SINGLE(WaitRes);
		}
		fprintf(g_RuntimeLogFile,"Successfully deposited %.2lf to account number %lu.\n",Amount,AccountNumber);
													/*~~~~CRITICAL REGION~~~*/
		ReleaseRes = ReleaseMutex(g_FileLocker);
		CHECK_RELEASE(ReleaseRes);
		ReleaseRes = ReleaseMutex(tmp->AccountSem);
		CHECK_RELEASE(ReleaseRes);
	}
	
	//printf("Deposit finnished\n");
	ExitThread(EXIT_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		Withdrawal
Input arguments:	ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		making a withdrawal to a specified account number
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void Withdrawal(ARGUMENTS_S* Args)
{
	unsigned long AccountNumber = Args->AccountNumber;
	double Amount = Args->Amount;
	DatabaseCell *tmp = NULL;
	DWORD ErrorInfo = WaitForSingleObject(g_ErrorEvent,NO_WAIT);
	DWORD WaitRes,ReleaseRes;

	free(Args); //freeing arguments struct

	if(WAIT_OBJECT_0 == ErrorInfo)
	{
		printf("Error occured at other thread - Exiting\n");
		ExitThread(FORCED_EXIT);
	}

	Sleep(SLEEP_TIME);

	if(!IsInList(AccountNumber))
		fprintf(g_RuntimeLogFile,"!!! Unable to withdrew %.2lf from account number %lu. Account doesn't exist. Skipping command. !!!\n",Amount,AccountNumber);
	else
	{
		tmp = GetCellFromList(AccountNumber);
		WaitRes = WaitForSingleObject(tmp->AccountSem,INFINITE);
		CHECK_WAIT_FOR_SINGLE(WaitRes);
											/*~~~~CRITICAL REGION~~~*/
		tmp->CurrentBalance		-= Amount;
		tmp->TotalWithdrawals	+= Amount;
		tmp->NumOfWithdrawals ++;
		WaitRes = WaitForSingleObject(g_FileLocker,INFINITE);
		if(WAIT_OBJECT_0 != WaitRes)
		{
			printf("failed recieving g_FileLocker\n");
			ReleaseMutex(tmp->AccountSem);
			CHECK_WAIT_FOR_SINGLE(WaitRes);
			
		}
		fprintf(g_RuntimeLogFile,"Successfully withdrew %.2lf from account number %lu.\n",Amount,AccountNumber);
										/*~~~~CRITICAL REGION~~~*/
		ReleaseRes = ReleaseMutex(g_FileLocker);
		CHECK_RELEASE(ReleaseRes);
		ReleaseRes = ReleaseMutex(tmp->AccountSem);
		CHECK_RELEASE(ReleaseRes);
	}
	//printf("Withdrawal finnished\n");
	ExitThread(EXIT_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		RunCommand
Input arguments:	char *CommandLine - a command line 

return:				None

Description-		runs the required thread according to the command line
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
	{
		printf("Illegal Command %s\n",command);
		free(Args);
		return;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		ThreadCreation
Input arguments:	void (*functionPtr)(ARGUMENTS_S*) -  a pointer to a function to be initiated by the thread 
					ARGUMENTS_S* Args - a pointer to a struct containg all the arguments for the command

return:				None

Description-		initiates a thread 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void ThreadCreation(void (*functionPtr)(ARGUMENTS_S*),ARGUMENTS_S* Args)
{
	DWORD ThreadID;
	HANDLE tmpHandle;

	if(g_HandlesArray!= NULL)
		WaitForThreads(Args->WaitCode);

	tmpHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)functionPtr,Args,0,&ThreadID);
	CHECK_THREAD_CREATION(tmpHandle);

	AddHandleToArray(tmpHandle);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		ReadInputFileRoutine
Input arguments:	char* filename-	a file name

return:				None

Description-		reads the file line by line and initiats every line as thread
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void ReadInputFileRoutine(char* filename)
{
	errno_t err;
	FILE *Instream = NULL;
	char *CommandLine = NULL;
	DWORD ErrorFlag;

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
		//printf("waiting for %d items \n",g_HandelArraySize);
		if(!g_HandelArraySize)	//handles a case when the first commands in the command file are invalid
			continue;
		WaitForThreads(g_WaitCode);
		ErrorFlag = CheckForErrors();
		if(ErrorFlag == ERROR_EXIT)
			break;
	}
	
	WaitForThreads(INFINITE);
	if(ErrorFlag == ERROR_EXIT)
		fprintf(g_RuntimeLogFile,"Error Eccured. Exiting.");
	else
		fprintf(g_RuntimeLogFile,"Program successfully finished running. Exiting.");
	printf("EOF reached \n");
	fclose(g_RuntimeLogFile);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		AddHandleToArray
Input arguments:	HANDLE handle-	a handle to be added to the array

return:				None

Description-		updates the global handle array and it's size
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void AddHandleToArray(HANDLE handle)
{
	DWORD WaitRes = WaitForSingleObject( g_ThreadLocker, INFINITE );

	switch (WaitRes)
	{
	case WAIT_OBJECT_0:
		g_HandelArraySize ++;
		if(g_HandlesArray == NULL)
			g_HandlesArray = (HANDLE*) calloc (g_HandelArraySize,sizeof(HANDLE));
		else
			g_HandlesArray = (HANDLE*)realloc(g_HandlesArray,g_HandelArraySize*sizeof(HANDLE));
		if(g_HandlesArray == NULL)
		{
			ReleaseMutex(g_ThreadLocker);
			printf("AddHandleToArray Allocation failed\n");
			exit(GetLastError());
		}
		g_HandlesArray[g_HandelArraySize -1] = handle;

		if(!ReleaseMutex( g_ThreadLocker ))
		{
			printf("Fail to Release Mutex g_ThreadLocker Error= %d\n", GetLastError());
			exit(GetLastError());
		}
		break;
	case WAIT_TIMEOUT:
		printf("Wait Timeout Mutex g_ThreadLocker\n");
		exit(0);
		break;

	case WAIT_ABANDONED:
		printf("WAIT_ABANDONED Mutex g_ThreadLocker\n");
		if(!ReleaseMutex( g_ThreadLocker ))
		{
			printf("Fail to Release Mutex g_ThreadLocker Error= %d\n", GetLastError());
			exit(GetLastError());
		}
		exit(0);
		break;

	case WAIT_FAILED:
		printf("WAIT_FAILED Mutex g_ThreadLocker\n");
		exit(0);
		break;

	default:
		printf("Unknown Error\n");
		exit(0);
		break;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CloseHandleArray
Input arguments:	None

return:				None

Description-		close all the handles in the global array
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void CloseHandleArray()
{
	DWORD index;
	for(index = 0; index < g_HandelArraySize; index++)
	{
		CloseHandle(g_HandlesArray[index]);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CreateBlanceReport
Input arguments:	char* BlanceReportFileName - the output blance report file name

return:				None

Description-		creates a report file containing the balance of each account in the Database
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void CreateBlanceReport(char* BlanceReportFileName)
{
	errno_t err;
	FILE *Outstream = NULL;
	int TotalDeposits = 0, TotalWithdrawals = 0;
	volatile DatabaseCell *tmp = g_DatabaseHead;
	err = fopen_s(&Outstream,BlanceReportFileName,"w+");
	CHECK_FILE_CREATION(err);
	fprintf(Outstream,"Summary of balances in bank accounts:\n");
	fprintf(Outstream,"Bank Account #,Current Balance,Initial Balance,Total Deposited,Total Withdrawal,# of Deposits,# of Withdrawals\n");
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		WaitForThreads
Input arguments:	DWORD waitcode - the time needed to be waited for multiple objects INFINITE or NO_WAIT 

return:				None

Description-		replaces the WaitForMultipleObjects function and handles a situation when there are more than 64 objects
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void WaitForThreads(DWORD waitcode)
{
	DWORD NumberOfSets = (g_HandelArraySize -1) / MAX_HANDLES_NUMBER + SET_OFFSET ; //the number of sub sets of size 64 handles 
	DWORD index ,NumberOFThreads = g_HandelArraySize,WaitRes = 0;
	HANDLE* Headptr = NULL;
	//printf("NumberOFThreads= %d\n",NumberOFThreads);
	//printf("%p\n",g_HandlesArray);
	for(index = NumberOfSets; index > 0 ; index --)			//a loop that goes over all the sets and check each sub set
	{
		Headptr = &g_HandlesArray[(index-1) * MAX_HANDLES_NUMBER];		//a pointer to the sub set head
		//printf("%p\n",Headptr);
		if(index == NumberOfSets)										//handles a case where the last sub set is not full 
		{
			if(NumberOFThreads%MAX_HANDLES_NUMBER==0)
				NumberOFThreads= MAX_HANDLES_NUMBER; 
			else
				NumberOFThreads = NumberOFThreads % (MAX_HANDLES_NUMBER);		//the residue number of threads in an unfull sub set 
			WaitRes = WaitForMultipleObjects(NumberOFThreads,Headptr,TRUE,waitcode);		
		}
		else
			WaitRes = WaitForMultipleObjects(MAX_HANDLES_NUMBER ,Headptr,TRUE,waitcode);
		//printf("wait result at index %d is %x\n",index,WaitRes);
		CHECK_WAIT_MULTIPLE(WaitRes);							//checks the return value of WaitForMultipleObjects 
	}


}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CheckForErrors
Input arguments:	None

return:				DWORD waitcode - EXIT_OK if there are no errors else ERROR_EXIT

Description-		returns an indication that an error occured 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
DWORD CheckForErrors(void)
{
	DWORD index,Exitcode;
	for(index = 0; index < g_HandelArraySize; index++ )
	{
		GetExitCodeThread(g_HandlesArray[index],&Exitcode);
		if( Exitcode == ERROR_EXIT)
			return ERROR_EXIT;
	}
	return EXIT_OK;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main
Input arguments:	int argc		-the number of arguments 
					char *argv[]	- the arguments strings

return:				None

Description-		the programs main routine
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void main(int argc, char *argv[])
{
	FILE* Outstream = NULL;
	if(argc < NUM_OF_ARGUMENTS)
	{
		printf("Incorrect number of arguments \n");
		exit(0);
	}

	g_ThreadLocker = CreateMutex(NULL,FALSE,NULL); 
	CHECK_MUTEX_CREATION(g_ThreadLocker);
	g_FileLocker = CreateMutex(NULL,FALSE,NULL); 
	CHECK_MUTEX_CREATION(g_FileLocker);
	g_ErrorEvent = CreateEvent(NULL ,TRUE,FALSE,NULL);
	CHECK_EVENT_CREATION(g_ErrorEvent);


	g_RuntimeLogFile = fopen(argv[3],"w+");
	if(NULL == g_RuntimeLogFile)
	{
		printf("failed open RuntimeLogFile for writing\n");
		CloseHandle(g_ThreadLocker);
		CloseHandle(g_FileLocker);
		CloseHandle(g_ErrorEvent);
		exit(GetLastError());
	}

	ReadInputFileRoutine(argv[1]);
	CreateBlanceReport(argv[2]);
	CloseHandleArray();
	CloseHandle(g_ErrorEvent);
	FreeDatabase();
}

