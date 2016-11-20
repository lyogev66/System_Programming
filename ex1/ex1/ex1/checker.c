/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 1
Using -			sodoku.h	 
	
Description -	checks a given solution for a sodoku
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "sodoku.h"


int gErrorFound=0;	//global variable that indicates first orrurance of errors
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		error_found
Input arguments:	FILE *output_file	- The output file handler

return:				None

Description-		The function prints the error header only once according to the global Error flag
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void error_found(FILE *output_file)
{
	if(!gErrorFound)
	{
		gErrorFound =1;
		fprintf(output_file,"Found errors in given Sudoku puzzle.\nThe errors are:\n");
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		check_rows
Input arguments:	int arr[MAT_SIZE][MAT_SIZE]		- The input matrix
					FILE *output_file				- The output file handler

return:				None

Description-		The function prints the errors that are found in the rows
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void check_rows(int arr[MAT_SIZE][MAT_SIZE],FILE *output_file)
{
	int i,j,k,tmp;
	for (i=0; i<MAT_SIZE; i++)
	{
		for(j=0; j<MAT_SIZE; j++)
		{
			tmp = arr[i][j];
			for(k=j+1;k<MAT_SIZE;k++)
			{
				if(arr[i][k] == tmp)
				{
					error_found(output_file);
					fprintf(output_file,"Line error: digit %d appears at (%d,%d) and (%d,%d)\n",arr[i][k],j+1,i+1,k+1,i+1);
				}
			}
		}

	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		check_colomn
Input arguments:	int arr[MAT_SIZE][MAT_SIZE]		- The input matrix
					FILE *output_file				- The output file handler

return:				None

Description-		The function prints the errors that are found in the colomns
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void check_colomn(int arr[MAT_SIZE][MAT_SIZE],FILE *output_file)
{
	int i,j,k,tmp;
	for (i=0; i<MAT_SIZE; i++)
	{
		for(j=0; j<MAT_SIZE; j++)
		{
			tmp = arr[j][i];
			for(k=j+1;k<MAT_SIZE;k++)
			{
				if(arr[k][i] == tmp)
				{
					error_found(output_file);
					fprintf(output_file,"Column error: digit %d appears at (%d,%d) and (%d,%d)\n",arr[k][i],i+1,j+1,i+1,k+1);
				}
			}
		}

	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		check_box
Input arguments:	int arr[MAT_SIZE][MAT_SIZE]		- The input matrix
					FILE *output_file				- The output file handler

return:				None

Description-		The function gets a subgrid and prints the errors that are found in it.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void check_box(int arr[MAT_SIZE][MAT_SIZE],int row,int col,FILE *output_file)
{
	int i,j,x,y;
	int repeating_numbers_array[ARR_SIZE]={0};

	for(i=0;i<SUB_MAT_SIZE;i++)
	{
		for(j=0;j<SUB_MAT_SIZE;j++)
		{
			for(x=0;x<SUB_MAT_SIZE;x++)
			{
				for(y=0;y<SUB_MAT_SIZE;y++)
				{
					if(x!=i || y!=j)//make sure that we don't double check a value (i,j) and (j,i)
						if(arr[i][j] == arr[x][y])
							if(! repeating_numbers_array[arr[i][j]])//repeating_numbers_array managers the already found repeating numbers
							{
								error_found(output_file);
								fprintf(output_file,"Subgrid error: digit %d appears at (%d,%d) and (%d,%d)\n",arr[i][j],j+col,i+row,y+col,x+row);
								repeating_numbers_array[arr[i][j]]++;
							}
				}
			}
		}
	}
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		check_Subgrid
Input arguments:	int arr[MAT_SIZE][MAT_SIZE]		- The input matrix
					FILE *output_file				- The output file handler

return:				None

Description-		The function calls the check_box function with the needed sub grid in order to find
					the errors in it.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void check_Subgrid(int arr[MAT_SIZE][MAT_SIZE],FILE *output_file)
{
	int row ,col,k=0;
	for(row =0 ; row < MAT_SIZE ; row +=3)
	{
		for(col = 0; col <MAT_SIZE; col+=3)
		{
			check_box((int (*)[MAT_SIZE])&arr[row][col],row+1,col+1,output_file);
		}
	}

}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main_checker
Input arguments:	int arr[MAT_SIZE][MAT_SIZE]		- The input matrix
					FILE *output_file				- The output file handler

return:				None

Description-		The function calls sub routines in order the go over the input matrix and check it.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void main_checker(int arr[MAT_SIZE][MAT_SIZE],FILE * output_file)
{
	int row =0, col=0;
	int backet[MAT_SIZE]={0};
	check_rows(arr,output_file);
	check_colomn(arr,output_file);
	check_Subgrid(arr,output_file);

	if(!gErrorFound)
		fprintf(output_file,"No errors found in given Sudoku puzzle.");

}