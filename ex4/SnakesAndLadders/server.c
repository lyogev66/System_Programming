#include "server.h"
#pragma comment(lib,"Ws2_32.lib")



int getPlayers(serverDataCollection *serverData)
{
	struct sockaddr_in client;
	int client_len = sizeof(client);
	HANDLE	playersUpdateLocker;
	int index = 1,len =sizeof(client);
	DWORD WaitRes;
	SOCKET	tmp;

	playersUpdateLocker = CreateMutex(NULL,FALSE,NULL);
	CHECK_MUTEX_CREATION(playersUpdateLocker);

	while(TRUE)
	{
		tmp = accept(serverData->serverSocket,(struct sockaddr*)&client,&len);
		if (tmp == INVALID_SOCKET)
		{
			printf("Server: accept() error %d\n", WSAGetLastError());
			WSACleanup();
			return EXIT_ERROR;
		}

		WaitRes = WaitForSingleObject( playersUpdateLocker, INFINITE );
		switch (WaitRes)
		{
		case WAIT_OBJECT_0:
			if(serverData->activePlayersNum <= MAXPLAYERSNUMBER)
			{
				serverData->playersInfo[index].playerSocket = tmp;
				serverData->activePlayersNum ++;
			}
			else
				printf("the game is full\n");

			ReleaseMutex(playersUpdateLocker);	
			break;
		case WAIT_ABANDONED:
			printf("wait playersUpdateLocker abandoned \n");
			ReleaseMutex(playersUpdateLocker);
			break;
		case WAIT_FAILED:
		case WAIT_TIMEOUT:
			printf("wait playersUpdateLocker failed  \n");
			break;
		}

	}
}



int serverRoutine(int portNumber,char* logFile)
{	
	char *serverAddress = "127.0.0.1";
	serverDataCollection serverData;
	char buffer[BUFFERSIZE] = {0};
	struct sockaddr_in server;
	HANDLE	getPlayersThread;
	WSADATA wsaData;
	DWORD threadID;
	int retval;

	serverData.activePlayersNum = 0;

	if ((retval = WSAStartup(0x202, &wsaData)) != 0)
	{
		printf("Server: WSAStartup() failed with error %d\n", retval);
		WSACleanup();
		return EXIT_ERROR;
	}
	printf("Server: WSAStartup() is OK.\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverAddress);
	server.sin_port = htons(portNumber);

	serverData.serverSocket = socket(AF_INET,SOCK_STREAM,0);
	if (serverData.serverSocket == INVALID_SOCKET)
	{
		printf("Server: socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return EXIT_ERROR;
	}
	printf("Server: socket() is OK.\n");


	if (bind(serverData.serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Server: bind() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return EXIT_ERROR;
	}
	printf("Server: bind() is OK.\n");


	if (listen(serverData.serverSocket,MAXPLAYERSNUMBER) == SOCKET_ERROR)
	{
		fprintf(stderr,"Server: listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return EXIT_ERROR;
	}
	printf("Server: listen() is OK.\n");
	printf("Server: I'm listening and waiting connection on port %d\n",portNumber);

	getPlayersThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)getPlayers,&serverData,0,&threadID);
	CHECK_THREAD_CREATION(getPlayersThread);

	WaitForSingleObject(getPlayersThread,MINUTE);
	if(!TerminateThread(getPlayersThread,threadID))
		printf("can't terminate getPlayersThread \n");

	printf("the number of active players = %d \n",serverData.activePlayersNum);
	if(serverData.activePlayersNum == 0)
		printf("no players no game \n");

	return OK;
}
