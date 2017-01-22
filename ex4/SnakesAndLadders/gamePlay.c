/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		gamePlay.h
			gamePlay.h
	
Description -	The board handling implementation
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "gamePlay.h"
#include "common.h"

#define DEBUG 0

#define NUMBER_OF_LADDERS 8
#define NUMBER_OF_SNAKES 8
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		convertArrayToNum
Input arguments:	BOOL pawn_array[NUMBER_OF_PAWNS]


return:				int - the number represents the current state

Description-		this function converts the array of bool values to a value that is later
					represent which pawn is standing in the current cell
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/	
int convertArrayToNum(BOOL pawn_array[NUMBER_OF_PAWNS])
{
	int i,num = 0;

	for(i=0;i<NUMBER_OF_PAWNS;i++)
	{
		//create a binary representation of pawn_array
		if (pawn_array[i])
		{
			num = num + (int)(pow((double)2,i));
		}
	}
	return num;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		convertNumToString
Input arguments:	BOOL pawn_array[NUMBER_OF_PAWNS]


return:				char * - the returned string that was represented by the number

Description-		this function converts a number to a string that represent the current pawn state in the cell
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
char *convertNumToString(int num)
{
	char *pawnStringsList[] = {		"   @  ",
									"   #  ",
									"  @#  ",
									"   %  ",
									"  @%  ",
									"  #%  ",
									"  @#% ",
									"   *  ",
									"  @*  ",
									"  #*  ",
									"  @#* ",
									"  %*  ",
									"  @%* ",
									"  #%* ",
									" @#%* ",                                          
											};


	return pawnStringsList[num-1];
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		getPawnString
Input arguments:	matrixCell cell -the current cell


return:				char * - the returned string that represents the cell state

Description-		this function converts the bool array to a string represents the current pawn state in the cell
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
char *getPawnString(matrixCell cell)                  
{
	int num;

	num = convertArrayToNum(cell.pawn_array);
	return convertNumToString(num);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		CountPawnInCell
Input arguments:	matrixCell cell -the current cell


return:				int				- the number of pawns standing in the current cell

Description-		this function counts the number of pawns in the current cell
					helps later in figuring out if we need to print the matrix normaly or with a pawn display
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int CountPawnInCell(matrixCell cell)
{
	int i = 0,counter = 0;

	for(i = 0; i < NUMBER_OF_PAWNS ; i++)
	{
		if(cell.pawn_array[i] == TRUE)
			counter++;
	}
	return counter;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		updateCellDisplay
Input arguments:	matrixCell cell -the current cell
					int cellIndex	- the index of the cell

return:				None

Description-		this function updates the display in the current cell according to the instructions
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void updateCellDisplay(matrixCell *cell,int cellIndex)                              
{
	int numberOfPawns = CountPawnInCell(*cell);

	if( !numberOfPawns)			// normal display
	{
		if (cellIndex < 10)
			sprintf(cell->display,"  0%d  ",cellIndex);
		else if(cellIndex < 100)
			sprintf(cell->display,"  %d  ",cellIndex);
		else
			sprintf(cell->display,"  %d ",cellIndex);

		switch (cell->type)
		{
		case STARTLADDER_CELL:
			{
				cell->display[1] = '^';
				break;
			}
		case ENDLADDER_CELL:
			{
				cell->display[1] = '=';
				break;
			}
		case STARTSNAKE_CELL:
			{
				cell->display[4] = 'v';
				break;
			}
		case ENDSNAKE_CELL:
			{
				cell->display[4] = '_';
				break;
			}
		}
	}
	else
	{
		strcpy(cell->display,getPawnString(*cell));			// pawn display
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		updateBoardDisplay
Input arguments:	matrixCell gameBoard[GAME_ARRAY_SIZE]	-the complete matrix

return:				None

Description-		this function updates the display in all the cells in the board
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void updateBoardDisplay(matrixCell gameBoard[GAME_ARRAY_SIZE])
{
	int i;

	for(i = 0; i < GAME_ARRAY_SIZE ; i++)
		updateCellDisplay(&gameBoard[i],i);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		setBoard
Input arguments:	matrixCell gameBoard[GAME_ARRAY_SIZE]	-the complete matrix

return:				None

Description-		this function defines the inital state of the board
					and defines the position of the ladders and snakes
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void setBoard(matrixCell gameBoard[GAME_ARRAY_SIZE])
{
	//locations for starts and ends
	int ladderStartArr[NUMBER_OF_LADDERS] = {4 , 9 , 20, 28, 40, 51, 63, 71};	
	int ladderEndArr[NUMBER_OF_LADDERS] =	{14, 31, 38, 84, 59, 67, 81, 91};
	int snakeStartArr[NUMBER_OF_SNAKES] =	{17, 54, 62, 64, 87, 93, 96, 99};
	int snakeEndArr[NUMBER_OF_SNAKES] =		{7 , 34, 19, 60, 24, 73, 75, 78};
	int i;

	for(i = 0 ; i < NUMBER_OF_LADDERS ; i++)
	{
		gameBoard[ladderStartArr[i]].type = STARTLADDER_CELL;
		gameBoard[ladderStartArr[i]].endLocation = ladderEndArr[i];
		gameBoard[ladderEndArr[i]].type = ENDLADDER_CELL;
	}
	for(i = 0 ; i < NUMBER_OF_SNAKES ; i++)
	{
		gameBoard[snakeStartArr[i]].type = STARTSNAKE_CELL;
		gameBoard[snakeStartArr[i]].endLocation = snakeEndArr[i];
		gameBoard[snakeEndArr[i]].type = ENDSNAKE_CELL;
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initCell
Input arguments:	None

return:				matrixCell	- an initialized cell

Description-		this function creates an initialed cell and returns is
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
matrixCell initCell()
{
	matrixCell cell;
	int i;

	cell.type = NORMAL_CELL;
	cell.endLocation = 0;

	for(i = 0 ; i < DISPLAY_SIZE ; i++)
		cell.display[i] = '\0';

	for( i = 0 ; i < NUMBER_OF_PAWNS ; i++)
		cell.pawn_array[i]=FALSE;

	return cell;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		printBroad
Input arguments:	matrixCell gameBoard[GAME_ARRAY_SIZE]	- the gameboard matrix
					HANDLE writingLock						- the lock for protected writing to screen

return:				None

Description-		this function prints the board to the screen according to instructions
					using a lock 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void printBroad(matrixCell gameBoard[GAME_ARRAY_SIZE],HANDLE writingLock)
{
	int i, j, currentIndex;

	WaitForSingleObject(writingLock,INFINITE);

	for( i = 9 ; i >= 0 ; i--)
	{
		if( i == 9 )
			printf("\n-----------------------------------------------------------------------\n");
		for( j = 10 ; j >= 1 ; j-- )
		{
			if( j == 10 )
				printf("|");
			if ( i%2 == 1 )//odd rows
				currentIndex = i*10 + j;
			else			//even rows
				currentIndex = i*10 + 11-j;
			printf("%s|",gameBoard[currentIndex].display);
		}
		printf("\n-----------------------------------------------------------------------\n");
	}
	ReleaseMutex(writingLock);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		initBoard
Input arguments:	boardStruct *borad	- the board and pawn locations sturct


return:				None

Description-		this function initiates the board struct
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void initBoard(boardStruct *borad)
{
	int i;

	for( i = 0 ; i < GAME_ARRAY_SIZE ; i++ )
		borad->gameBoard[i] = initCell();

	for( i = 0 ; i < NUMBER_OF_PAWNS ; i++)
		borad->pawnLocation[i] = 0;

	setBoard(borad->gameBoard);
	updateBoardDisplay(borad->gameBoard);

}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		movePlayerPawn
Input arguments:	int pawnIndex		- the index of the current pawn
					int stepSize		- the number of steps to be done by the pawn
					boardStruct *borad	- the board and pawn locations sturct

return:				int - a value indicates that the player hit the 100 cell

Description-		this function moves the pawn in pawn index by the requiered number of steps 
					and returns a RETURN WINNER if the pawn hit the 100 cell
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int movePlayerPawn(int pawnIndex,int stepSize,boardStruct *borad)
{
	int currentLocation = borad->pawnLocation[pawnIndex];
	int nextLocation = currentLocation + stepSize;
	int nextLocationType = -1;

	//printf("current location %d\n");
	//printf("nextLocation location %d\n");

	if ( nextLocation >= 100 ){			//arrived to the 100 cell
		return RETURN_WINNER;
	}
	nextLocationType = borad->gameBoard[nextLocation].type;
	borad->gameBoard[currentLocation].pawn_array[pawnIndex] = 0;

	switch (nextLocationType)	//go up the ladder or down the snake
	{
	case	STARTLADDER_CELL:
	case    STARTSNAKE_CELL:
		nextLocation = borad->gameBoard[nextLocation].endLocation;
		break;
	}

	borad->gameBoard[nextLocation].pawn_array[pawnIndex] = 1;
	borad->pawnLocation[pawnIndex] = nextLocation;

	return 0;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		findPawnIndex
Input arguments:	char pawnType		- the pawn character


return:				int - the index for the inserted pawn

Description-		this function returns the representing index for the inserted pawntype charecter
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int findPawnIndex(char pawnType)
{			
	if ( pawnType == '@' ){
		return 0;
	}else if( pawnType == '#' ){
		return 1;
	}else if( pawnType == '%' ){
		return 2;
	}else if( pawnType == '*' ){
		return 3;
	}else {
		printf("Unknown pawn type\n");
		exit(EXIT_ERROR);
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		playGame
Input arguments:	char pawnType		- the pawn character
					int stepSize		- the size of the step to be done by the above pawn
					boardStruct *borad	- the gameboard struct
					HANDLE writingLock	- a handle to a writing protection mutex


return:				int - a value indicates that the player hit the 100 cell

Description-		this function conducts the game routine, moves the pawn to it's new location
					and gives an indication if it hits the 100 cell
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int playGame(char pawnType,int stepSize,boardStruct *borad,HANDLE writingLock)
{
	int pawnIndex = 1;

	pawnIndex = findPawnIndex(pawnType);

	if (movePlayerPawn(pawnIndex,stepSize,borad) != RETURN_WINNER)
	{
		updateBoardDisplay(borad->gameBoard);
		printBroad(borad->gameBoard,writingLock);	
		return 0;
	}else	
		return RETURN_WINNER;
}