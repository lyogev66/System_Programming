#include "client.h"



int clientRoutine(char userName[31], int portNumber,char* logFile)
{
	printf("username = %s\n",userName);
	printf("portNumber = %d\n",portNumber);
	printf("logfile = %s\n",logFile);


	return OK;
}
