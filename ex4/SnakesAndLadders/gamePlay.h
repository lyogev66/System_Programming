/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project= Exercise 4
Using -		gamePlay.h
			gamePlay.h
	
Description -	The gamePlay header file contains all the routines that controls the game board
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "common.h"
#include <math.h>          //for power
#include <stdarg.h>        //implement printf + fprintf

#define NUMBER_OF_PAWNS			4
#define GAME_ARRAY_SIZE			101
#define DISPLAY_SIZE			7
#define RETURN_WINNER			100
#define RETURN_OTHER_WINNER		101



typedef enum cellType{
                NORMAL_CELL,
                STARTSNAKE_CELL,
                ENDSNAKE_CELL,
                STARTLADDER_CELL,
                ENDLADDER_CELL,
}cellType;

//struct for each cell in the matrix
typedef struct matrixCell{
                cellType type;
                int endLocation;
                char display[DISPLAY_SIZE];
                BOOL pawn_array[NUMBER_OF_PAWNS];        
}matrixCell;

//struct for the board matrix
typedef struct boardStruct{
				matrixCell gameBoard[GAME_ARRAY_SIZE];
                int pawnLocation[NUMBER_OF_PAWNS];        
}boardStruct;


void initBoard(boardStruct *borad);
int playGame(char pawnType,int stepSize,boardStruct *borad,HANDLE writingLock);

#endif