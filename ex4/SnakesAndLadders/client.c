/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		client.h
			gamePlay.h
			common.h
	
Description -	The client implementation
				this file doesn't handle the board
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "client.h"
#include "gamePlay.h"
#include "common.h"


#define DEBUG 0			//used for debuging using printf 
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		printDebug
Input arguments:	const char *fmt,
					...

return:				None

Description-		this function implements printf only for debug
					it takes in argument the same way printf and fprintf
					and prints it to the stdout - meaining the screen
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void printDebug(const char *fmt, ...)
{
	va_list ap;

	if (DEBUG)
	{
		va_start(ap, fmt);
		vfprintf(stdout, fmt, ap);
		va_end(ap);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		printToLogAndScreenLocked
Input arguments:	ARGUMENTS_S *Args - the arguments struct
					const char *fmt
					...

return:				None

Description-		this function implements printf and fprintf with a use of a lock
					it takes in argument the same way printf and fprintf
					and prints it to the file and screen
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void printToLogAndScreenLocked(ARGUMENTS_S *Args,const char *fmt, ...)
{
	va_list ap;

	printDebug("secure printing\n");
	WaitForSingleObject( Args->writingLock,INFINITE);	//start of lock protected screen and log write

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);							//write to screen
	vfprintf(Args->logFile, fmt, ap);					//write to log file
	va_end(ap);

	ReleaseMutex( Args->writingLock );					//release lock
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fprintfWithLock
Input arguments:	ARGUMENTS_S *Args - the arguments struct
					const char *fmt
					...

return:				None

Description-		this function implements fprintf with a use of a lock
					it takes in argument the same way printf and fprintf
					and prints it to the file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void fprintfWithLock(ARGUMENTS_S *Args,const char *fmt, ...)
{
	va_list ap;

	WaitForSingleObject(Args->writingLock,INFINITE);	//start of lock protected screen and log write

	va_start(ap, fmt);
	vfprintf(Args->logFile, fmt, ap);
	va_end(ap);

	ReleaseMutex(Args->writingLock);					//release lock
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		sendAndLog
Input arguments:	ARGUMENTS_S *Args - the arguments struct
					stringToSend - the string to be sended to the server


return:				None

Description-		this function sends strings to the server and writes them to a file as well
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void sendAndLog(ARGUMENTS_S *Args,char* stringToSend)
{
	char sendbuffer[MAX_BUFFER_SIZE] = {0};

	sprintf(sendbuffer,"%s\n",stringToSend);

	if(Args->client.winner)
	{
		printDebug("sending winner msg\n");
		send(Args->client.clientSocket,sendbuffer,strlen(sendbuffer)+5,0);
	}
	else
		send(Args->client.clientSocket,sendbuffer,strlen(sendbuffer)+1,0);

	fprintfWithLock(Args,"Sent to server:%s\n",stringToSend);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		closeMySocket
Input arguments:	SOCKET socket - the socket object


return:				None

Description-		this function closes the inputed socket
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void closeMySocket(SOCKET socket)
{

	if ( closesocket(socket) == SOCKET_ERROR ){
		printf("Error closing client socket %d\n",WSAGetLastError());
		exit(EXIT_ERROR);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		parseFromUser
Input arguments:	ARGUMENTS_S* Args - the arguments struct


return:				None

Description-		this function parse input from the user
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void parseFromUser(ARGUMENTS_S* Args)
{
	char ch;
	int bufferIndex = 0;

	ch = fgetc(stdin);

	while((ch != '\n') && (ch != EOF) && (ch != '\r'))		//recive data from user up untill a ENTER key
	{
		Args->sendBuffer[bufferIndex++] = ch;
		ch = fgetc(stdin);
	}
	Args->sendBuffer[bufferIndex] = '\0';  
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		getConnectionResponce
Input arguments:	ARGUMENTS_S* Args - the arguments struct


return:				None

Description-		this function waits for the initial responce from the server and
					parse the username and pawntype if succesful else it sends
					the required message to log file and screen
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void getConnectionResponce(ARGUMENTS_S* Args)
{
	char buffer[MAX_BUFFER_SIZE] = {0};
	char tempString[PLAYER_NAME_SIZE] = {0};
	int iResult;
	clientStruct *myClient = &(Args->client);

	iResult = recv(myClient->clientSocket,buffer,MAX_BUFFER_SIZE,0);
	if ( iResult > 0 )
	{
		sscanf(buffer,"%s",tempString);

		if(!strcmp(tempString, Args->client.userName))   //the message is correct pase the username and pawn type
		{
			sscanf(buffer,"%s %s %s %s %s %s",tempString,tempString,tempString,tempString,tempString,tempString);
			myClient->pawnType=tempString[0];
		}
		else{											//else connection is refused
			printToLogAndScreenLocked(Args,"Connection to server refused. Exiting.\n");
			exit(EXIT_ERROR);
		}
	}
	else{
		printf("recv failed with error: %d\n", WSAGetLastError());
		exit(EXIT_ERROR);
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		connectToServer
Input arguments:	ARGUMENTS_S* Args - the arguments struct
					int portNumber -the port number 

return:				None

Description-		this function creates a new socket connects to server and parse 
					the connection authentication from the server 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void connectToServer(ARGUMENTS_S *Args,int portNumber)
{
	struct sockaddr_in clientAdd;
	WSADATA wsaData;
	int retval;
	char buffer[MAX_BUFFER_SIZE] = {0};
	FILE *logFile = Args->logFile;

	if ((retval = WSAStartup(0x202, &wsaData)) != 0)					
	{
		printf("Client: WSAStartup() failed with error %d\n", retval);
		WSACleanup();
		exit(EXIT_ERROR);
	}

	clientAdd.sin_family = AF_INET;									
	clientAdd.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);					//define the ip to be connected to
	clientAdd.sin_port = htons(portNumber);									//define the port number

	Args->client.clientSocket = socket(AF_INET,SOCK_STREAM,0);				//try to create a socket object
	if (Args->client.clientSocket == INVALID_SOCKET)
	{
		WSACleanup();
		exit(EXIT_ERROR);
	}

	if(connect(Args->client.clientSocket,(struct sockaddr*)&clientAdd,sizeof(clientAdd)) != SOCKET_ERROR)	//try to connect to the server
	{	//connect succes
		printToLogAndScreenLocked(Args,"Connected to server on port %d\n",portNumber);
		sprintf(buffer,"username=%s",Args->client.userName);
		sendAndLog(Args,buffer);

	}
	else
	{	//connect fail
		printToLogAndScreenLocked(Args,"Failed connecting to server on port %d\n",portNumber);
		closesocket(Args->client.clientSocket);
		WSACleanup();
		exit (EXIT_ERROR);
	}
	getConnectionResponce(Args);										//get responce
	printDebug("DEBUG: completed connecting to server\n");

}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initiateClientThreads
Input arguments:	HANDLE *threadsRoutinesArray			-the threads hadnels array
					LPTHREAD_START_ROUTINE *threadRoutines	-the names of the threads
					ARGUMENTS_S* Args						- the arguments struct


return:				None

Description-		this function initiates the User interface and communication threads
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initiateClientThreads(HANDLE *threadsRoutinesArray,LPTHREAD_START_ROUTINE *threadRoutines,ARGUMENTS_S* Args)
{
	int i;
	DWORD threadID = 0;

	for(i = 0; i < NUMBER_OF_THREADS; i++)
	{
		threadsRoutinesArray[i] = CreateThread(NULL,0,threadRoutines[i],Args,0,&threadID);
		CHECK_THREAD_CREATION(threadsRoutinesArray[i]);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initiateClientEvents
Input arguments:	HANDLE *eventsArray			-the events hadnels array
					ARGUMENTS_S* Args			- the arguments struct


return:				None

Description-		this function initiates the client's events and stores a their handle in the Args struct
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initiateClientEvents(HANDLE *eventsArray,ARGUMENTS_S* Args)
{
	int i;

	for(i = 0; i < NUMBER_OF_THREADS; i++)
	{
		eventsArray[i] = CreateEvent(NULL,TRUE,FALSE,NULL);			//the events started by the threads
		CHECK_EVENT_CREATION(eventsArray[i]);
	}
	Args->threadEvents = eventsArray;

	//create Engine Event
	Args->engineWorkingEvent = CreateEvent(NULL,TRUE,TRUE,NULL);	//the event representing that the engine is processing a thread	input
	CHECK_EVENT_CREATION(Args->engineWorkingEvent);
	printDebug("DEBUG: Created engineWorkingEvent\n");
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initiateLocks
Input arguments:	ARGUMENTS_S* Args			- the arguments struct



return:				None

Description-		this function initiates the mutex that are used by the client routine and stores a their handle in the Args struct
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initiateLocks(ARGUMENTS_S* Args)
{
	Args->msgInProgress = CreateMutex(NULL,FALSE,NULL);
	CHECK_MUTEX_CREATION(Args->msgInProgress);

	Args->writingLock = CreateMutex(NULL,FALSE,NULL);
	CHECK_MUTEX_CREATION(Args->writingLock);

	printDebug("DEBUG: created all mutex\n");

}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		parseMessage
Input arguments:	char sendBuffer[MAX_BUFFER_SIZE]	-the complete buffer of the message
					char message[MESSAGE_SIZE]			-the message to be parsed by the function
					char userName[USERNAME_SIZE]		-the username to be parsed by the function
					E_MESSAGE_TYPE type					-type BROADCAST or PLAYER TO PLAYER


return:				None

Description-		this function parse the message from the broadcast and player message input 
					in order to help understad if the input is valid meaning if the number of argumnets are correct
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void parseMessage(char sendBuffer[MAX_BUFFER_SIZE],char message[MESSAGE_SIZE],char userName[USERNAME_SIZE],E_MESSAGE_TYPE type)
{
	char command[MESSAGE_SIZE] = {0};
	int i = 0,j = 0;
	switch(type)
	{
	case PLAYER_TO_PLAYER_MESSAGE_E:			// inputed "message"
		{
			sscanf(sendBuffer,"%s %s",command,userName);	//parse username
			j = strlen(command)+strlen(userName)+2;
			while( sendBuffer[j] != '\0')
			{
				message[i++] = sendBuffer[j++];				//parse the rest of the message
			}
			message[i] = '\0';
			break;
		}
	case BROADCAT_MESSAGE_E:				// inputed "broadcast"
		{
			sscanf(sendBuffer,"%s",command);
			j = strlen(command)+1;
			while(sendBuffer[j] != '\0')
			{
				message[i++] = sendBuffer[j++];			//parse the message
			}
			message[i] = '\0';
			break;
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CheckInputMessageValid
Input arguments:	ARGUMENTS_S* Args		- the arguments struct


return:				BOOL					-true if the input is valid false if not 

Description-		this function checks all the input
					checks if the command is on of the possible commands
					and in case it is a valid command checks that it's arguments are valid
					returns true if the message is valid else false
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
BOOL CheckInputMessageValid(ARGUMENTS_S* Args)
{
	char command[MAX_BUFFER_SIZE] = {0};
	char message[MESSAGE_SIZE] = {0};
	char username[USERNAME_SIZE] = {0};
	FILE* logFile = Args->logFile;

	sscanf(Args->sendBuffer,"%s",command);		//parse the command type
	if( !strcmp(command,"players") )			//players command
	{
		//checing if the arguments are valid arguments
		if(strlen(Args->sendBuffer) == strlen("players"))
			return TRUE;
		else
			printToLogAndScreenLocked(Args,"Illegal argument for command players. Command format is players.\n");
	}
	else if( !strcmp(command,"play"))		//play command
	{
		if(strlen(Args->sendBuffer) == strlen("play"))
		{
			return TRUE;
		}
		else
			printToLogAndScreenLocked(Args,"Illegal argument for command play. Command format is play.\n");
	}
	else if( !strcmp(command,"message"))	//message command
	{
		parseMessage(Args->sendBuffer,message,username,PLAYER_TO_PLAYER_MESSAGE_E);
		if(strlen(Args->sendBuffer) == (strlen(command)+strlen(username)+strlen(message)+2))
			return TRUE;
		else
			printToLogAndScreenLocked(Args,"Illegal argument for command message. Command format is message <user> <message>.\n");
	}
	else if( !strcmp(command,"broadcast"))	//broadcast command
	{             
		parseMessage(Args->sendBuffer,message,username,BROADCAT_MESSAGE_E);
		if( strlen(Args->sendBuffer) == (strlen(command)+strlen(message)+1))
			return TRUE;
		else
			printToLogAndScreenLocked(Args,"Illegal argument for command message. Command format is broadcast <message>.\n");//not ok
	}
	else
		printToLogAndScreenLocked(Args,"Command %s is not recognized. Possible commands are: players, message, broadcast and play.\n",command);
	return FALSE;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CheckInputMessageValid
Input arguments:	ARGUMENTS_S* Args		- the arguments struct


return:				None

Description-		this is the User interface thread function
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void UIThread(ARGUMENTS_S* Args)
{
	HANDLE threadEvent = Args->threadEvents[SCANF_INDEX];
	HANDLE msgInProgress = Args->msgInProgress;
	HANDLE engineWorkingEvent = Args->engineWorkingEvent;
	char retMsg[MAX_BUFFER_SIZE]={0};
	int inputMessageValid = -1;

	printDebug("DEBUG: starting UIThread\n");
	do
	{
		Sleep(10);
		parseFromUser(Args);	//parse the input

		WaitForSingleObject(msgInProgress,INFINITE);	// engine working
		ResetEvent(engineWorkingEvent);

		inputMessageValid = CheckInputMessageValid(Args);

		if ( inputMessageValid == TRUE ) //if command is valid - engine starts working on it
			SetEvent(threadEvent);

		else  //else no need for engine to work
			SetEvent(engineWorkingEvent);

		WaitForSingleObject(engineWorkingEvent,INFINITE);
		ReleaseMutex(msgInProgress);

	}while(TRUE);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CheckInputMessageValid
Input arguments:	ARGUMENTS_S* Args		- the arguments struct


return:				None

Description-		this is the Client Communication thread function
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void ClientCommunicationThread(ARGUMENTS_S* Args)
{
	HANDLE threadEvent = Args->threadEvents[RECV_INDEX];
	HANDLE engineWorkingEvent = Args->engineWorkingEvent;
	HANDLE msgInProgress = Args->msgInProgress;

	printDebug("DEBUG: starting ClientCommunicationThread\n");
	do{
		Sleep(10);
		memset(Args->recvBuffer,0,sizeof(Args->recvBuffer));
		Args->recvBufferSize = recv(Args->client.clientSocket,Args->recvBuffer,MAX_BUFFER_SIZE,0);		//recive the message from the server

		WaitForSingleObject(msgInProgress,INFINITE);	// engine working
		ResetEvent(engineWorkingEvent);

		if ( Args->recvBufferSize > 0 )			//engine starts working
		{
			SetEvent(threadEvent);
		}
		else if( Args->recvBufferSize == 0 )	//connection was dropped by the serverS
		{
			printf("Connection closed\n");
			Args->closeAllThreads=TRUE;
			SetEvent(threadEvent);
			ReleaseMutex(msgInProgress);
			ExitThread(OK);
		}
		else								//error with the recv function
		{
			printf("recv failed with error: %d\n",WSAGetLastError());
			Args->closeAllThreads=TRUE;
			SetEvent(threadEvent);
			ReleaseMutex(msgInProgress);
			ExitThread(ERROR);
		}
		WaitForSingleObject(engineWorkingEvent,INFINITE);
		ReleaseMutex(msgInProgress);

	}while(Args->recvBufferSize > 0);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		parseRecv
Input arguments:	ARGUMENTS_S* Args		- the arguments struct


return:				None

Description-		this function parse the recv message from the server
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void parseRecv(ARGUMENTS_S* Args)
{
	char command[MAX_BUFFER_SIZE] = {0},notRelevent[MAX_BUFFER_SIZE] = {0};
	char numberStr[3] = {0},pawnType[2] = {0};
	int stepSize;

	sscanf(Args->recvBuffer,"%s",command);
	
	if(!strcmp(command,"Your"))				//your turn command
		Args->client.myTurn = TRUE;
	else if( !strcmp(command,"Player")){	//player turn command
		sscanf(Args->recvBuffer,"%s %s %s %s %s %s",command,pawnType,notRelevent,notRelevent,notRelevent,numberStr);
		numberStr[2]='\0';
		stepSize = atoi(numberStr);
		playGame(pawnType[0],stepSize,&Args->board,Args->writingLock);		//start the game

		printDebug("DEBUG: client:type %s draw a %d \n",pawnType,stepSize);
		printDebug("DEBUG: current pawn is %c my pawn is %c\n",pawnType[0],Args->client.pawnType);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		cleanUp
Input arguments:	ARGUMENTS_S* Args								- the arguments struct
					HANDLE threadsRoutinesArray[NUMBER_OF_THREADS]	- the theards handle array

return:				None

Description-		this function closes all the open handles for the threads the mutex,
					the events and the file and close the socket connetion
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void cleanUp(HANDLE threadsRoutinesArray[NUMBER_OF_THREADS], ARGUMENTS_S* Args)
{
    printDebug("DEBUG: terminating both of the threads\n");

    if(!TerminateThread(threadsRoutinesArray[SCANF_INDEX],EXIT_ERROR))
		printf("can't terminate UIThread \n");
    if(!TerminateThread(threadsRoutinesArray[RECV_INDEX],EXIT_ERROR))
		printf("can't terminate ClientCommunicationThread \n");

	printDebug("DEBUG: closing the handles\n");
	CloseHandle(threadsRoutinesArray[SCANF_INDEX]);
	CloseHandle(threadsRoutinesArray[RECV_INDEX]);
	CloseHandle(Args->threadEvents[SCANF_INDEX]);
	CloseHandle(Args->threadEvents[RECV_INDEX]);
	CloseHandle(Args->engineWorkingEvent);
	CloseHandle(Args->msgInProgress);
	CloseHandle(Args->writingLock);
	fclose(Args->logFile);
	closeMySocket(Args->client.clientSocket);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		drewAndPlay
Input arguments:	ARGUMENTS_S* Args				- the arguments struct


return:				None

Description-		this function drew a random number and starts a game play routine
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void drewAndPlay(ARGUMENTS_S *Args)
{
		int tossResult,gameResult;


		tossResult = rand()%6 + 1;
		printDebug("DEBUG: %c tossResult=%d\n",Args->client.pawnType,tossResult);
		
		sprintf(Args->sendBuffer,"Player %c (%s) drew a %d.",Args->client.pawnType,Args->client.userName,tossResult);
		gameResult = playGame(Args->client.pawnType,tossResult,&Args->board,Args->writingLock);		//starting the game
		if (gameResult == RETURN_WINNER)
		{
			printDebug("DEBUG: winner is found\n");
			Args->client.winner = TRUE;
		}
		else
			printDebug("DEBUG: not a winner\n");

		Args->client.myTurn = FALSE;//turn finnished
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		clientRoutine
Input arguments:	char userName[PLAYER_NAME_SIZE]		- the commandline inputed username
					int portNumber						- the commandline inputed port number
					char* logFileName					- the commandline inputed log file name

return:				None

Description-		this function is the main client routine that calls all the sub routines
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int clientRoutine(char userName[PLAYER_NAME_SIZE], int portNumber,char* logFileName)
{
	int counter=0;		//for debug
	ARGUMENTS_S Args;
	FILE *logFile = NULL;
	int waitRes,threadIndex;
	HANDLE threadEvents[NUMBER_OF_THREADS] = {0};
	HANDLE threadsRoutinesArray[NUMBER_OF_THREADS] = {0};
	LPTHREAD_START_ROUTINE threadRoutines[] = { (LPTHREAD_START_ROUTINE)UIThread,
		(LPTHREAD_START_ROUTINE)ClientCommunicationThread};

	printDebug("DEBUG: starting main\n");
	ZeroMemory(&Args, sizeof(Args));

	srand((unsigned int)GetCurrentProcessId());

	//opening log file and starting the connection with the server
	if(fopen_s(&logFile,logFileName,"w") != 0)
	{
		printf("Can't open log file\n");
		exit(EXIT_ERROR);
	}

	//initiate the game boards
	initBoard(&Args.board);
	//creating locks
	initiateLocks(&Args);
	Args.logFile = logFile;
	memcpy(Args.client.userName,userName,strlen(userName)+1);

	connectToServer(&Args,portNumber);         //connecting to server and sending the required message

	//creating events and initiating them
	initiateClientEvents(threadEvents,&Args);
	//creating threads and initiating them
	initiateClientThreads(threadsRoutinesArray,threadRoutines,&Args);

	printDebug("DEBUG: starting while\n");
	//the main loop - waiting for the threads to return with inputs
	while(!Args.closeAllThreads)
	{
		waitRes = WaitForMultipleObjects(NUMBER_OF_THREADS,threadEvents,FALSE,INFINITE);
		threadIndex = waitRes-WAIT_OBJECT_0;		//which thread returned
		ResetEvent(threadEvents[threadIndex]);		//reset the event that started the loop

		if (Args.closeAllThreads)
		{
			break;
		}

		switch (threadIndex)
		{
		case(SCANF_INDEX)://take care of scanf

			if(!strcmp(Args.sendBuffer,"play"))
			{
				if (Args.client.myTurn)
				{
					drewAndPlay(&Args);
				}else
					break;
			}
			sendAndLog(&Args,Args.sendBuffer);
			ZeroMemory(Args.sendBuffer,sizeof(Args.sendBuffer));
			break;

		case(RECV_INDEX)://take care of recv

			printToLogAndScreenLocked(&Args,"Received from server:%s",Args.recvBuffer);
			parseRecv(&Args);
			ZeroMemory(Args.recvBuffer,sizeof(Args.recvBuffer));
			break;

		default:
			printf("why am I here?\n");
		}
        if ( Args.client.winner )
        {
			break;
        }
		Sleep(10);

		printDebug("DEBUG: threadIndex =%d SCANF_INDEX=%d RECV_INDEX=%d\n while counter=%d\n",threadIndex,SCANF_INDEX,RECV_INDEX,counter++);
		printDebug("DEBUG: pawns location- @=%d #=%d %=%d *%d\n",Args.board.pawnLocation[0],Args.board.pawnLocation[1],Args.board.pawnLocation[2],Args.board.pawnLocation[3]);

		threadIndex = -1;

		SetEvent(Args.engineWorkingEvent);      //relevant thread starts working again
	}

	cleanUp(threadsRoutinesArray,&Args);
	return 0;
}