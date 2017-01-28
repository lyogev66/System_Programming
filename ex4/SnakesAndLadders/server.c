/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		server.h
	
Description -	The server implementation
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "server.h"
#pragma comment(lib,"Ws2_32.lib")

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		EventCreation
Input arguments:	HANDLE *newEvent
					LPCTSTR eventName

return:				None

Description-		Creates event with the name "eventName", the intitial state of the event is set to non-signalled and manual reseted.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void EventCreation(HANDLE *newEvent,LPCTSTR eventName)
{
	*newEvent = CreateEvent(NULL,TRUE,FALSE,eventName);  
	if(NULL == *newEvent)
	{	
		printf("Error creating Event Error: %d\n", GetLastError());
		exit(GetLastError());
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		writeMsgToLog
Input arguments:	DataCollection *gameData
					char* logMsg

return:				None

Description-		Secured by mutex - this function writes a string logMsg to the log file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void writeMsgToLog(DataCollection *gameData, char* logMsg)
{
	HANDLE  writeToLog = OpenMutex (MUTEX_ALL_ACCESS,FALSE,WRITE_PROTECT);
	if(WaitForSingleObject(writeToLog,INFINITE) == WAIT_OBJECT_0)
		fwrite(logMsg,sizeof(char),strlen(logMsg) +1,gameData->logFile);
	else
		printf("cant write to log file \n");
	ReleaseMutex(writeToLog);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		searchPlayerByName
Input arguments:	char* playerName
					DataCollection *gameData

return:				int

Description-		This function searches a player by its name and returnes the player index number if exists, else returnes 0.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int	searchPlayerByName(char* playerName,DataCollection *gameData)
{
	int i,retIndex = 0;
	for(i=1; i<=gameData->activePlayersNum; i++)
	{
		if(!strcmp(playerName,gameData->playerInfo[i].playerName ))
		{
			retIndex = i;
			break;
		}
	}
	return retIndex;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		createPlayersList
Input arguments:	DataCollection *gameData
					char *resMessage

return:				None

Description-		This functions build the message needed for the command "players"
					it creates a string of "player name - pawn"
					For example : "John - @,Berry - #"
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void createPlayersList(DataCollection *gameData,char *resMessage)
{
	char message[MAX_MESSAGE_SIZE]={0};
	char playersList[MAXPLAYERSNUMBER+1][MAX_MESSAGE_SIZE]={{0}};
	int i;

	for(i=1; i<=gameData->activePlayersNum ; i++)
	{
		if(i != gameData->activePlayersNum)
			sprintf(playersList[i],"%s-%c, ",gameData->playerInfo[i].playerName,gameData->playerInfo[i].playerPawn);
		else
			sprintf(playersList[i],"%s-%c ",gameData->playerInfo[i].playerName,gameData->playerInfo[i].playerPawn);
	}
	sprintf(resMessage,"%s %s %s %s\n",playersList[1],playersList[2],playersList[3],playersList[4]);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		extractMsg
Input arguments:	char* inputMsg
					char *receiverName
					char* retMsg
					E_MESSAGE_TYPE type

return:				None

Description-		This function exctract the main message from the recieved message from the user.
					Fr example, fr the given string "broadcast hi all" the message to be broadcast will be/
					excreacted "hi all".			
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void extractMsg(char* inputMsg,char *receiverName,char* retMsg,E_MESSAGE_TYPE type)
{
	char command[MAX_MESSAGE_SIZE] = {0};
	char *token;

	switch(type)
	{
	case PLAYER_TO_PLAYER_MESSAGE_E:
		{
			sscanf(inputMsg,"%s %s",command,receiverName);
			token = strtok(inputMsg," ");
			token = strtok(NULL," ");
			token = strtok(NULL,"\0");
			memcpy(retMsg,token,strlen(token)+1);
			break;
		}
	case BROADCAT_MESSAGE_E:
		{
			token = strtok(inputMsg," ");
			token = strtok(NULL,"\0");
			memcpy(retMsg,token,strlen(token)+1);
			break;
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		sendMessageRoutine
Input arguments:	DataCollection *gameData

return:				None

Description-		This functions deal with the parsed message and do as the message type suggests.
					This is the main function of communication between the server and the client.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void sendMessageRoutine(DataCollection *gameData)
{
	HANDLE	sendingMessageEvent =		OpenEvent(EVENT_ALL_ACCESS ,FALSE,SEND_MESSAGE_EVENT);
	HANDLE	doneSendingMessage =		OpenEvent(EVENT_ALL_ACCESS ,FALSE,MESSAGE_DONE_EVENT);
	HANDLE  changeTurnEvent = 0; //MUST BE HERE FOR REASON
	char message			[MAX_MESSAGE_SIZE];
	char msgToCurrentPlayer [MAX_MESSAGE_SIZE];
	char msgToOtherPlayers	[MAX_MESSAGE_SIZE];
	int i,playerIndex;
	Sleep(10);


	while(TRUE)
	{
		if(WaitForSingleObject(sendingMessageEvent,INFINITE) == WAIT_OBJECT_0)
		{
			switch(gameData->msgBox.e_msgType)
			{
			case BROADCAT_MESSAGE_E:
				printf("************** BROADCASTING ****************\n");
				sprintf(message,"Broadcast from %s: %s",gameData->msgBox.playerSenderName,gameData->msgBox.message);
				for(i=1 ;i <= gameData->activePlayersNum ;i++)
				{
					if(gameData->msgBox.playerNumber == i)
						continue;//send broadcast message to all other players else then the sender itself.
					send(gameData->playerInfo[i].playerSocket,message,strlen(message)+1,0);
				}

				sprintf(message,"Broadcast message from user %s: %s\n",gameData->msgBox.playerSenderName,gameData->msgBox.message);
				writeMsgToLog(gameData,message);
				break;

			case PLAYER_TO_PLAYER_MESSAGE_E:
				printf("************** playertoPlayerMessage ****************\n");

				playerIndex = searchPlayerByName(gameData->msgBox.playerRecieverName,gameData);
				if(playerIndex)
				{
					sprintf(message,"Private message from %s: %s",gameData->msgBox.playerSenderName,gameData->msgBox.message);
					send(gameData->playerInfo[playerIndex].playerSocket,message,strlen(message)+1,0);
				}
				else
				{
					sprintf(message,"User %s doesn't exists in the game.\n",gameData->msgBox.playerRecieverName);
					send(gameData->playerInfo[gameData->msgBox.playerNumber].playerSocket,message,strlen(message)+1,0);
				}
				break;

				sprintf(message,"Private message sent from %s to %s: %s\n",gameData->msgBox.playerSenderName,gameData->msgBox.playerRecieverName,gameData->msgBox.message);
				writeMsgToLog(gameData,message);


			case GAME_MESSAGE_E:
				printf("************** sendGameMessage ****************\n");

				printf("%d vs %d\n",gameData->msgBox.msgSize,strlen(gameData->msgBox.message)+1);

				writeMsgToLog(gameData,gameData->msgBox.message);
				if(!strcmp(gameData->msgBox.message,"winner\n"))
				{
					printf("winner is found \n");
					goto WINNER_FOUND;
				}

				for(i=1 ;i <= gameData->activePlayersNum ;i++)
				{
					if(gameData->playerTurn == i)
						continue;
					else
						send(gameData->playerInfo[i].playerSocket,gameData->msgBox.message,strlen(gameData->msgBox.message)+1,0);
				}
				
				gameData->playerPlayed = TRUE;

				break;

			case SINGLE_PLAYER_LIST_MSG_E:
				printf("************** singlePlayerListMessage ****************\n");
				createPlayersList(gameData,message);
				send(gameData->playerInfo[gameData->msgBox.playerNumber].playerSocket,message,strlen(message)+1,0);
				break;

			case BROADCAST_PLAYER_LIST_MSG_E:
				printf("************** broadcastPlayerListMessage *************\n");
				createPlayersList(gameData,message);
				for(i=1 ;i <= gameData->activePlayersNum ;i++)
					send(gameData->playerInfo[i].playerSocket,message,strlen(message)+1,0);	
				printf("game started PlayerListMessage sent\n");
				writeMsgToLog(gameData,"Players' game pieces' selection broadcasted to all users.\n");	
				break;

			case PLAYER_TURN_MSG_E:
				printf("************** sendTurnMsg ****************\n");
				sprintf(msgToCurrentPlayer,"Your turn to play.\n");
				sprintf(msgToOtherPlayers,"It is now %s's turn to play\n",gameData->playerInfo[gameData->playerTurn].playerName);

				for(i=1 ;i <= gameData->activePlayersNum ;i++)
				{
					if(gameData->playerTurn == i)
						send(gameData->playerInfo[i].playerSocket,msgToCurrentPlayer,strlen(msgToCurrentPlayer)+1,0);
					else
						send(gameData->playerInfo[i].playerSocket,msgToOtherPlayers,strlen(msgToOtherPlayers)+1,0);
				}
				writeMsgToLog(gameData,msgToOtherPlayers);
				break;


			case NEW_PLAYER_MESSAGE_E:
				printf("************** newPlayerMessage ****************\n");
				sprintf(msgToCurrentPlayer,"%s your game piece is %c\n",gameData->msgBox.playerSenderName,gameData->msgBox.playerSenderPawn);
				sprintf(msgToOtherPlayers,"New player joined the game:%s %c\n",gameData->msgBox.playerSenderName,gameData->msgBox.playerSenderPawn);

				for(i=1 ;i <= gameData->msgBox.playerNumber ;i++)
				{
					if(gameData->msgBox.playerNumber == i)//send message to the new player
						send(gameData->playerInfo[i].playerSocket,msgToCurrentPlayer,strlen(msgToCurrentPlayer)+1,0);
					else
						send(gameData->playerInfo[i].playerSocket,msgToOtherPlayers,strlen(msgToOtherPlayers)+1,0);
				}

				writeMsgToLog(gameData,msgToCurrentPlayer);
				writeMsgToLog(gameData,msgToOtherPlayers);
				break;

			case EXIT_MESSAGE_E:
				printf("************** exitMessage ****************\n");
				sprintf(message,"Game finnished");
				for(i=1 ;i <= gameData->activePlayersNum ;i++)
					send(gameData->playerInfo[i].playerSocket,message,strlen(message)+1,0);

				for(i=1 ;i <= gameData->activePlayersNum ;i++)
				{
					shutdown(gameData->playerInfo[i].playerSocket,SD_BOTH);
					closesocket(gameData->playerInfo[i].playerSocket);
				}

				gameData->exitRes = ERROR_ECCURED_E;
				gameData->playerPlayed = TRUE;
				break;
			case WINNER_MESSAGE_E:
WINNER_FOUND:
				printf("************** WinnerMessage ****************\n");
				sprintf(message,"Player %s won the game. Congratulations.\n",gameData->msgBox.playerSenderName);
				writeMsgToLog(gameData,message);

				for(i=1 ;i <= gameData->activePlayersNum ;i++)
					send(gameData->playerInfo[i].playerSocket,message,strlen(message)+1,0);

				gameData->exitRes = WINNER_FOUND_E;
				gameData->playerPlayed = TRUE;
				break;
			}
			//done handleing the message - reset all events and get ready for the next message to come.
			memset(message,0,strlen(message)+1);
			memset(msgToOtherPlayers,0,strlen(msgToOtherPlayers)+1);
			memset (msgToCurrentPlayer,0,strlen(msgToCurrentPlayer)+1);
			SetEvent(doneSendingMessage);
			ResetEvent(sendingMessageEvent);
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		buildMessage
Input arguments:	playersInformation *player
					char* msg
					E_MESSAGE_TYPE e_msgType

return:				None

Description-		This function fills all the information needed for the "sendMessageRoutine" function.
					This function fills information at a shared stract between the communication thread and the server.
					The information is excractd from the un-parsed incoming message from the user.
					the function categorize the message for diffrent types.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void buildMessage(playersInformation *player,char* msg,E_MESSAGE_TYPE e_msgType)
{
	char parsedMsg		[MAX_MESSAGE_SIZE] = {0};
	char receiverName	[PLAYER_NAME_SIZE] = {0};
	char messageBuffer	[MAX_MESSAGE_SIZE] = {0};
	int msgSize;
	msgSize = player->msgBox->msgSize;
	ZeroMemory(player->msgBox,sizeof(*player->msgBox));

	/*store general information of the player*/
	memcpy(player->msgBox->playerSenderName,player->playerName,strlen(player->playerName)+1);
	player->msgBox->playerSenderPawn = player->playerPawn;
	player->msgBox->playerNumber = player->playerNumber;
	
	extractMsg(msg,receiverName,parsedMsg,e_msgType);

	switch(e_msgType)
	{
	case BROADCAT_MESSAGE_E:
		player->msgBox->e_msgType = BROADCAT_MESSAGE_E;
		memcpy(player->msgBox->message,parsedMsg,strlen(parsedMsg)+1);
		break;
	case PLAYER_TO_PLAYER_MESSAGE_E:
		player->msgBox->e_msgType = PLAYER_TO_PLAYER_MESSAGE_E;
		memcpy(player->msgBox->playerRecieverName,receiverName,strlen(receiverName)+1);
		memcpy(player->msgBox->message,parsedMsg,strlen(parsedMsg)+1);
		break;
	case GAME_MESSAGE_E:
		player->msgBox->e_msgType = GAME_MESSAGE_E;
		memcpy(player->msgBox->message,msg,strlen(msg)+1);
		player->msgBox->msgSize = msgSize;
		break;
	case NEW_PLAYER_MESSAGE_E:
		player->msgBox->e_msgType = NEW_PLAYER_MESSAGE_E;
		break;
	case PLAYER_TURN_MSG_E:
		player->msgBox->e_msgType = PLAYER_TURN_MSG_E;
		break;
	case SINGLE_PLAYER_LIST_MSG_E:
		player->msgBox->e_msgType = SINGLE_PLAYER_LIST_MSG_E;
		break;
	case BROADCAST_PLAYER_LIST_MSG_E:
		player->msgBox->e_msgType = BROADCAST_PLAYER_LIST_MSG_E;
		break;
	case EXIT_MESSAGE_E:
		player->msgBox->e_msgType = EXIT_MESSAGE_E;
		break;
	case WINNER_MESSAGE_E:
		player->msgBox->e_msgType = WINNER_MESSAGE_E;
		break;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		parseAndSendMessage
Input arguments:	char* msg
					playersInformation *player
					BOOL newPlayer

return:				None

Description-		According to the first word at the message un-parsed message from the client,
					This function calles to "buildMessage" function to fill all the needed data and when done,
					sets an event for the server to start and handle the parsed message.
					After setting the event for the server, this function wait for event that the server set notifing that he has done handeling the message.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void parseAndSendMessage(char* msg,playersInformation *player,BOOL newPlayer)
{
	char command	[MAX_MESSAGE_SIZE] = {0};
	HANDLE	msgInProgress =			OpenMutex(MUTEX_ALL_ACCESS,FALSE,MESSAGE_IN_PROGRESS);
	HANDLE	sendingMessage =		OpenEvent(EVENT_ALL_ACCESS ,FALSE,SEND_MESSAGE_EVENT);
	HANDLE	doneSendingMessage =	OpenEvent(EVENT_ALL_ACCESS ,FALSE,MESSAGE_DONE_EVENT);

	sscanf(msg,"%s",command);
	printf("recieved command %s from %s\n",command,player->playerName);

	if(WaitForSingleObject(msgInProgress,INFINITE) == WAIT_OBJECT_0)
	{
		if(newPlayer)
		{
			buildMessage(player,msg, NEW_PLAYER_MESSAGE_E);
		}
		else if(!strcmp(command,"message"))
			buildMessage(player,msg, PLAYER_TO_PLAYER_MESSAGE_E);

		else if(!strcmp(command,"broadcast"))
			buildMessage(player,msg, BROADCAT_MESSAGE_E);

		else if(!strcmp(command,"play"))
		{
			if((player->gameStarted) && (player->playerTurn == TRUE))
				buildMessage(player,msg, GAME_MESSAGE_E);
			else
			{
				printf("not %s turn to play\n",player->playerName);
				return;
			}
		}
		else if(!strcmp(command,"winner"))
			buildMessage(player,msg, WINNER_MESSAGE_E);

		else if(!strcmp(command,"players"))
			buildMessage(player,msg, SINGLE_PLAYER_LIST_MSG_E);

		else if(!strcmp(command,"exit"))
			buildMessage(player,msg, EXIT_MESSAGE_E);

		else if(!strcmp(command,"Player"))
		{
			printf("recieved %d bytes\n",player->msgBox->msgSize);
			buildMessage(player,msg, GAME_MESSAGE_E);
		}
		else
		{
			printf("unknown command\n");
			return;
		}

		printf("sending message\n");
		SetEvent(sendingMessage);

		if(WaitForSingleObject(doneSendingMessage,INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(doneSendingMessage);
			ReleaseMutex(msgInProgress);
		}
		else
			printf("error eccured in parseMessage %d\n",GetLastError());
	}
	else
	{
		printf(" parseMessage cant recieve mutex error :%d\n",GetLastError());
		return;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		communicationThread
Input arguments:	playersInformation *player
					

return:				None

Description-		This function recieve messages from the client, and handles the situations of errors at the client side.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void communicationThread (playersInformation *player)
{
	char recvMsg[MAX_MESSAGE_SIZE] = {0};
	int iResult;

	Sleep(10);
//	printf("Hello %s...we are ready to communicate %d\n",player->playerName,player->playerNumber);
	parseAndSendMessage(recvMsg,player,TRUE);

	do {

		iResult = recv(player->playerSocket , recvMsg, MAX_MESSAGE_SIZE, 0);
		if ( iResult > 0 )
		{
			player->msgBox->msgSize = iResult;
			parseAndSendMessage(recvMsg,player,FALSE);
			memset(recvMsg,0,sizeof(recvMsg));//zero the buffer for next message
		}
		else if ( iResult == 0 )
		{
			printf("Connection closed %d \n",player->playerNumber);
			parseAndSendMessage("exit",player,FALSE);
			printf("communicationThread of %s exited\n",player->playerName);
			ExitThread(ERROR);
		}
		else
		{
			printf("recv of player %d failed with error: %d\n",player->playerNumber, WSAGetLastError());
			parseAndSendMessage("exit",player,FALSE);
			printf("communicationThread of %s exited\n",player->playerName);
			ExitThread(ERROR);
		}

	} while( iResult > 0 );
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		waitForName
Input arguments:	temporaryData	*tmpData

return:				None

Description-		The server wait for clients to log in in parralel methode,
					meaning that a accepted connection from clients will not be recognized as an "active" player before the client entered his name.
					This function wait for the client who has connected to fill his name.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void waitForName(temporaryData	*tmpData)
{
	char playerName[MAX_MESSAGE_SIZE] = {0},playerNameFullCommand[MAX_MESSAGE_SIZE] = {0};	
	HANDLE	playersUpdateLocker = OpenMutex (MUTEX_ALL_ACCESS,FALSE,PLAYER_UPDATE_LOCK);
	char *token;
	int recBytes;
	Sleep(10);

	do {
		recBytes = recv(tmpData->tmpSocket,playerNameFullCommand,MAX_MESSAGE_SIZE,0);
		if ( recBytes > 0 )
		{
			if(WaitForSingleObject( playersUpdateLocker, INFINITE ) == WAIT_OBJECT_0)
			{
				token = strtok(playerNameFullCommand,"=");
				if(!strcmp(token,"username"))
				{
					token = strtok(NULL, "\n");
					memcpy(tmpData->playerName,token,strlen(token)+1);
					tmpData->isRegistered = TRUE;
					ReleaseMutex (playersUpdateLocker);
					ExitThread(OK);
				}else
				{
					ReleaseMutex (playersUpdateLocker);
					goto EXIT;
				}
			}
		}
		else if ( recBytes == 0 )
		{
			printf("Connection closed\n");
			goto EXIT;
		}
		else
			goto EXIT;

	} while( recBytes > 0 );
EXIT:
	shutdown(tmpData->tmpSocket,SD_BOTH);
	closesocket(tmpData->tmpSocket);
	ExitThread(ERROR);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		isExistingUserName
Input arguments:	char *playerName
					DataCollection *gameData
					int numberOfPlayers

return:				BOOL

Description-		After the client entered his name, before that he can be recognized as "active" player,
					This function checks that the enters username doensn't allready exists at the other "actived" players.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
BOOL isExistingUserName(char *playerName, DataCollection *gameData,int numberOfPlayers)
{
	int i;
	for(i = 1; i <= gameData->activePlayersNum ;i++)
	{
		if(!strcmp(playerName,gameData->playerInfo [i].playerName))
			return TRUE;
	}
	return FALSE;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		UpdatePlayerData
Input arguments:	DataCollection *gameData
					int numberOfPlayers
					int index
					

return:				None

Description-		After the clients has successfuly registerd as "active" player,
					This function set a pawn character to the player and updates the server with the new player data.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void UpdatePlayerData(DataCollection *gameData,int numberOfPlayers,int index)
{
	char pawnCharacters[MAXPLAYERSNUMBER+1] = {' ','@','#','%','*'};

	gameData->playerInfo[numberOfPlayers].playerNumber = numberOfPlayers;
	gameData->playerInfo[numberOfPlayers].playerPawn = pawnCharacters[numberOfPlayers];
	memcpy(gameData->playerInfo[numberOfPlayers].playerName,gameData->tmpData[index].playerName,strlen(gameData->tmpData[index].playerName)+1 );
	gameData->playerInfo[numberOfPlayers].playerSocket = gameData->tmpData[index].tmpSocket;
	gameData->tmpData[index].activePlayer = TRUE;
	gameData->activePlayersNum = numberOfPlayers;
	gameData->playerInfo[numberOfPlayers].msgBox = &gameData->msgBox;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		playersManager
Input arguments:	DataCollection *gameData

return:				None

Description-		This function manages all the process of after reciving a connection until the player can be recognized as "active" player.
					This function checks for the ligitimicy of the players name , counts the "active" players number and notify the server on each
					successfuly registered client.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void playersManager(DataCollection *gameData)
{
	HANDLE	playersUpdateLocker = OpenMutex (MUTEX_ALL_ACCESS,FALSE,PLAYER_UPDATE_LOCK);
	int numberOfPlayers =0;
	DWORD waitRes;
	int i;
	Sleep(10);

	printf("**********************************************************\n");
	while(TRUE)
	{
		for(i=0;i<10;i++)
		{
			if((gameData->tmpData[i].isRegistered) && (gameData->tmpData[i].activePlayer == FALSE))
			{
				waitRes = WaitForSingleObject(playersUpdateLocker,INFINITE);
				CHECK_WAIT_FOR_OBJECT(waitRes);

				if(isExistingUserName(gameData->tmpData[i].playerName,gameData,numberOfPlayers))
				{
					printf("user name alreasy exists\n");
					send(gameData->tmpData[i].tmpSocket ,"Existing user name...exiting\n",strlen("Existing user name...exiting\n")+1,0);
					shutdown(gameData->tmpData[i].tmpSocket,SD_BOTH);
					closesocket(gameData->tmpData[i].tmpSocket);
					gameData->tmpData[i].isRegistered = FALSE;
					ReleaseMutex (playersUpdateLocker);
					break;
				}

				numberOfPlayers++;
				if(numberOfPlayers > MAXPLAYERSNUMBER)
				{
					send(gameData->tmpData[i].tmpSocket,"Cannot accept connection. Game is full.\n",strlen("Cannot accept connection. Game is full.\n")+1,0);
					gameData->tmpData[i].activePlayer = FALSE;
					for(i=0;i<10;i++)
						if(!gameData->tmpData[i].activePlayer)
						{
							shutdown(gameData->tmpData[i].tmpSocket,SD_BOTH);
							closesocket(gameData->tmpData[i].tmpSocket);
						}
						ReleaseMutex(playersUpdateLocker);
						continue;
						/*printf("exiting playersManager\n");
						ExitThread(OK);*/
				}

				if(gameData->gameStarted)
					ExitThread(OK);

				UpdatePlayerData(gameData,numberOfPlayers,i);
				Sleep(5);
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)communicationThread,&gameData->playerInfo[numberOfPlayers],0,&waitRes);

				ReleaseMutex (playersUpdateLocker);

			}
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		getPlayers
Input arguments:	DataCollection *gameData
					

return:				int

Description-		This functiones wait for the clients to connect the server.
					Since the methode the clients can connect is parralel, for each recieved connection this function opens a thread to collect the player's name. 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int getPlayers(DataCollection *gameData)
{
	struct sockaddr_in client;
	int client_len = sizeof(client);
	HANDLE ThreadArray[10],playersManagerHandle;
	DWORD WaitRes,threadID=0;
	int index = 0 ;

	Sleep(10);

	playersManagerHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)playersManager,gameData,0,&WaitRes);

	while(TRUE)
	{
		if(index >= MAX_ACCEPT_INDEX )
			index = MAX_ACCEPT_INDEX;

		gameData->tmpData[index].tmpSocket = accept(gameData->serverSocket,(struct sockaddr*)&client,&client_len);
		ThreadArray[index] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)waitForName,&gameData->tmpData[index],0,&threadID);
		index++;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initiateThreads
Input arguments:	HANDLE *threadsRoutinesArray
					LPTHREAD_START_ROUTINE *threadRoutines
					DataCollection *gameData

return:				None

Description-		This function initiates the threades at the start-up.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initiateThreads(HANDLE *threadsRoutinesArray,LPTHREAD_START_ROUTINE *threadRoutines,DataCollection *gameData)
{
	int i;
	DWORD threadID = 0;

	for(i=0;i<ROUTINE_NUMBER;i++)
	{
		threadsRoutinesArray[i] = CreateThread(NULL,0,threadRoutines[i],gameData,0,&threadID); 
		CHECK_THREAD_CREATION(threadsRoutinesArray[i]);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initiateServer
Input arguments:	DataCollection *gameData
					int port number

return:				None

Description-		This function constracts the server.
					This function sets the enviroment, creates the server socket,
					binds the server to a given address and port, and listen's on the socket.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initiateServer(DataCollection* gameData,int portNumber )
{
	struct sockaddr_in server;
	char *serverAddress = "127.0.0.1";
	WSADATA wsaData;
	int retval;

	if ((retval = WSAStartup(0x202, &wsaData)) != 0)
	{
		printf("Server: WSAStartup() failed with error %d\n", retval);
		WSACleanup();
		exit(EXIT_ERROR);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverAddress);
	server.sin_port = htons(portNumber);

	gameData->serverSocket = socket(AF_INET,SOCK_STREAM,0);
	if (gameData->serverSocket == INVALID_SOCKET)
	{
		printf("Server: socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		exit(EXIT_ERROR);
	}

	if (bind(gameData->serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Server: bind() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		exit (EXIT_ERROR);
	}

	if (listen(gameData->serverSocket,MAXPLAYERSNUMBER) == SOCKET_ERROR)
	{
		fprintf(stderr,"Server: listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		exit(EXIT_ERROR);
	}
//	printf("Server: I'm listening and waiting connection on port %d\n",portNumber);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		resetAllTurns
Input arguments:	DataCollection *gameData

return:				None

Description-		This function reset each  player turne's flags indicated that it is his turn to play.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void resetAllTurns(DataCollection *gameData)
{
	int i;
	for (i=1; i<= gameData->activePlayersNum;i++)
		gameData->playerInfo[i].playerTurn = FALSE;
	gameData->playerTurn = 0;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		createRandomGamePlayOrder
Input arguments:	DataCollection *gameData
					int *playerOrderArray

return:				None

Description-		Before starting the game, This function creates an random order for the "active" players to play and writes it to the server log.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void createRandomGamePlayOrder(DataCollection *gameData,int *playerOrderArray)
{
	int i,randNumber,count = 0, activePlayersNum = gameData->activePlayersNum;
	char gameMsgOrder[MAX_MESSAGE_SIZE];
	BOOL flag = FALSE;
	char playersList[MAXPLAYERSNUMBER][MAX_MESSAGE_SIZE]={{0}};
	time_t t;
	srand((unsigned) time(&t));

	while(count != activePlayersNum)
	{
		randNumber = rand()%activePlayersNum +1;
		flag = FALSE;
		for(i=0; i<MAXPLAYERSNUMBER; i++)
		{
			if(playerOrderArray[i] == randNumber)
			{
				flag = TRUE;
				break;
			}
		}
		if(flag)
			continue;
		playerOrderArray[count] = randNumber;
		count ++;
	}

	for(i=0; i < activePlayersNum ; i++)
	{
		if(i != activePlayersNum -1)
			sprintf(playersList[i]," %s,",gameData->playerInfo[playerOrderArray[i]].playerName);
		else
			sprintf(playersList[i]," %s.",gameData->playerInfo[playerOrderArray[i]].playerName);
	}


	sprintf(gameMsgOrder,"The order of the players' in the game is%s %s %s %s\n",playersList[0],playersList[1],playersList[2],playersList[3]);
	writeMsgToLog(gameData,gameMsgOrder);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		updateGameStarted
Input arguments:	DataCollection *gameData

return:				None

Description-		This function updates each "active" player that the game has started.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void updateGameStarted(DataCollection *gameData)
{
	int i;

	printf("***************game started*****************\n");
	gameData->gameStarted = TRUE;
	for(i=1;i<=gameData->activePlayersNum;i++)
		gameData->playerInfo[i].gameStarted = TRUE;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		gameRoutine
Input arguments:	DataCollection *gameData

return:				None

Description-		This function deliver and switch turn's of the playing players.
					This function get notified from the communication thread that the current player
					has played and the turn need to be changed.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void gameRoutine(DataCollection *gameData)
{
	HANDLE	msgInProgress =			OpenMutex(MUTEX_ALL_ACCESS,FALSE,MESSAGE_IN_PROGRESS);
	HANDLE	sendingMessage =		OpenEvent(EVENT_ALL_ACCESS ,FALSE,SEND_MESSAGE_EVENT);
	HANDLE	doneSendingMessage =	OpenEvent(EVENT_ALL_ACCESS ,FALSE,MESSAGE_DONE_EVENT);

	int playerOrderArray[MAXPLAYERSNUMBER]={0};
	int i = 0,playerPlayingNumber;

	updateGameStarted(gameData);

	createRandomGamePlayOrder(gameData,playerOrderArray);

	if(WaitForSingleObject(msgInProgress,INFINITE) == WAIT_OBJECT_0)
	{
		gameData->msgBox.e_msgType = BROADCAST_PLAYER_LIST_MSG_E;
		ResetEvent(doneSendingMessage);
		SetEvent(sendingMessage);
		WaitForSingleObject(doneSendingMessage,INFINITE);
		ReleaseMutex(msgInProgress);
	}
	else
		printf("cant seize mutex at gameRoutine to announce player list\n");

	while(!gameData->exitRes)
	{
		resetAllTurns(gameData);
		playerPlayingNumber = gameData->playerTurn = playerOrderArray[i];
		printf("player number %d plays\n",playerPlayingNumber);
		gameData->playerInfo[playerPlayingNumber].playerTurn = TRUE;

		gameData->playerPlayed = FALSE;
		if(WaitForSingleObject(msgInProgress,INFINITE) == WAIT_OBJECT_0)
		{
			gameData->msgBox.e_msgType = PLAYER_TURN_MSG_E;
			ResetEvent(doneSendingMessage);
			SetEvent(sendingMessage);
			WaitForSingleObject(doneSendingMessage,INFINITE);
			ReleaseMutex(msgInProgress);
		}
		else
			printf("cant sieze mutex to announce player turn\n");
		
		
		while(gameData->playerPlayed == FALSE);

		printf("changing turned \n");
		if(i == gameData->activePlayersNum -1 )
			i = 0;
		else
			i++;
	}

	if(gameData->exitRes == WINNER_FOUND_E)
	{
		if(WaitForSingleObject(msgInProgress,INFINITE) == WAIT_OBJECT_0)
		{
			printf("exiting for reason: Winner is found \n");
			gameData->msgBox.e_msgType = EXIT_MESSAGE_E;
			ResetEvent(doneSendingMessage);
			SetEvent(sendingMessage);
			WaitForSingleObject(doneSendingMessage,INFINITE);
			ReleaseMutex(msgInProgress);
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		serverRoutine
Input arguments:	int portNumber	- a port number passed from argv
					char* logFile	- a log file name passed from argv	


return:				int

Description-		the main server routine.
					This finction initiates the the database of he game, opens the log file for writing,
					wait for the clients to be registered 60 secounds and then, (if players exists) starts the game.
					at the end of the game this function closes all the handles,sockets,threads and files.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int serverRoutine(int portNumber,char* logFile)
{	
	HANDLE	playersUpdateLocker;
	HANDLE	msgInProgress = 0;
	HANDLE	logWriteMutex;
	HANDLE	sendingMessage;
	HANDLE	doneSendingMessage;
	DataCollection gameData ;
	HANDLE	threadsRoutinesArray[ROUTINE_NUMBER];
	DWORD threadID = 0 ;
	LPTHREAD_START_ROUTINE threadRoutines[] ={(LPTHREAD_START_ROUTINE)getPlayers,(LPTHREAD_START_ROUTINE)sendMessageRoutine};


	ZeroMemory( &gameData, sizeof(gameData));
	if(fopen_s(&gameData.logFile,logFile,"w") != 0)
	{
		printf("cant open log file\n");
		exit(0);
	}

	playersUpdateLocker = CreateMutex(NULL,FALSE,PLAYER_UPDATE_LOCK);
	CHECK_MUTEX_CREATION(playersUpdateLocker);
	msgInProgress = CreateMutex(NULL,FALSE,MESSAGE_IN_PROGRESS);
	CHECK_MUTEX_CREATION(msgInProgress);
	logWriteMutex = CreateMutex(NULL,FALSE,WRITE_PROTECT);
	CHECK_MUTEX_CREATION(logWriteMutex);

	/* INITIATE ALL EVENTS NEEDED */

	EventCreation(&sendingMessage,SEND_MESSAGE_EVENT);
	EventCreation(&doneSendingMessage,MESSAGE_DONE_EVENT);
//	EventCreation(&changeTurnEvent,CHANGE_TURNES_EVENT);

	/*	INITIATE THE SERVER*/

	initiateServer(&gameData, portNumber);

	/*INITIATE ROUTINES*/

	initiateThreads(threadsRoutinesArray,threadRoutines,&gameData);

	Sleep(20000);
	playersUpdateLocker = OpenMutex(0,TRUE,PLAYER_UPDATE_LOCK);

	WaitForSingleObject(playersUpdateLocker,INFINITE);

	if(!TerminateThread(threadsRoutinesArray[GET_PLAYERS_THREAD_E],threadID))
		printf("can't terminate getPlayersThread \n");
	ReleaseMutex(playersUpdateLocker);


//	printf("the number of active players = %d \n",gameData.activePlayersNum);
	if( 0 == gameData.activePlayersNum)
	{
		printf("No players connected, exiting...\n");
		writeMsgToLog(&gameData,"No players connected, exiting...\n");
		exit(0);
	}

	gameRoutine(&gameData);

//	printf("bye bye\n");


	CloseHandle(logWriteMutex);
	CloseHandle(sendingMessage);
	CloseHandle(doneSendingMessage);
	CloseHandle(playersUpdateLocker);
	CloseHandle(msgInProgress);
	fclose(gameData.logFile);
	return OK;
}
