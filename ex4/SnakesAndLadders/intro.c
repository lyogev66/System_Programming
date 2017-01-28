/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		common.h
			server.h
			client.h
	
Description -	The intro file contains calls to the main server and client routines 
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "common.h"
#include "server.h"
#include "client.h"

#define SERVERARGUMENTSCOUNT			4
#define	CLIENTARMUNENTSCOUNT			5

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main
Input arguments:	int argc		- number of input arguments
					char *argv[]	- inputed string


return:				int - the return value

Description-		the main function which calls the server or client routine
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/	
int main(int argc, char *argv[])
{
	if(!strcmp(argv[1],"server"))
	{//check for correct number of arguments
		if(argc < SERVERARGUMENTSCOUNT)
		{
			printf("incorrect number of arguments to initiate server interface\n");
			printf("SnakesAndLadders.exe <server|client> <logfile> <server port> <username> \n");
			return 0;
		}

		serverRoutine(atoi(argv[3]),argv[2]);

	}
	else if(!strcmp(argv[1],"client"))
	{
		if(argc < CLIENTARMUNENTSCOUNT)
		{
			printf("incorrect number of arguments to initiate client interface\n");
			printf("SnakesAndLadders.exe <server|client> <logfile> <server port> <username> \n");
			return 0;
		}
		clientRoutine(argv[4],atoi(argv[3]),argv[2]);
	}
}