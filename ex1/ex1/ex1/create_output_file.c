/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 1
Using -			sodoku.h	 
	
Description -	handles the output file
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "sodoku.h"



/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		create_output_file
Input arguments:	char*	output_file_name -  the name of the output file
					
return:				FILE* fp - A pointer to an output file
					the function kills the program if the output file cannot be opened

Description-		The function is creating an output file according to output_file_name and returns
					a pointer to it
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
FILE* create_output_file(char* output_file_name)
{
	FILE *fp;
	fp = fopen (output_file_name,"w");
	if (fp == NULL)
	{
		printf("FILE NOT FOUND \n");// if cannot open file - kill program
		exit(1) ;
	}
	return fp;
}


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		create_output_file_name
Input arguments:	char*	output_file_name -  the name of the output file
return:				None

Description-		The function reformat the input file name to get the correct output file name
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void create_output_file_name(char* output_file_name)
{
	output_file_name=strtok(output_file_name,".");
	strcat(output_file_name,"_sol.txt");
}