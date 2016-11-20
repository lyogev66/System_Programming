/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 1
Using -			sodoku.h	 
	
Description -	parsing a text file into 2D array and returning it to main fuction	
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "sodoku.h"



/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		text_to_mat
Input arguments:	char*	input_file_name -  the name of the input file
					int		arr[MAT_SIZE][MAT_SIZE] - pointer to 2D array to fill accordingly
return:				int OK - function run without errors 
						ERROR - function failed 

Description-		The function is parsing a text file into 2D array and fills the 2D array and returns
					it to the main fuction
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int text_to_mat(char* input_file_name,int arr[MAT_SIZE][MAT_SIZE])
{
	FILE *fp = NULL;
	int digit=0 ,i=0, j=0;
	fp = fopen (input_file_name,"r+");
	if (fp == NULL)
	{
		printf("FILE NOT FOUND \n");
		return ERROR;
	}

	while (! feof(fp))				//while we havnt reached end of file
	{
		if(isdigit(digit))
			arr[i][j++] = digit - CHAR_TO_DIGIT; // convert to digit from ascii value
		else if(digit == '.')
			arr[i][j++] = 0;

		digit = fgetc(fp); // get the next char from stream file.
	}
	fclose(fp);
	return OK;
}
