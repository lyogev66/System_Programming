/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 1
Using - 
	checker.c
	create_output_file.c
	solver.c
	text_to_mat.c
	all of these file are rediricted via "sodoku.h" header.

Description- The main function of project sodoku.
the main function recieves the command line arguments and initiats the required operation
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "sodoku.h"


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main
Input arguments:	int argc - number of arguments sent from command line
					char * argv[] - array of strings recieved from command line

return:				int OK - program run without errors 
						ERROR - program failed 

Description-		The main function of project sodoku.
					the main function recieves the command line arguments and initiats the required operation	
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc,char *argv[])
{
	FILE *output_file = NULL;
	int mat[MAT_SIZE][MAT_SIZE];// create 2D matrix for storing the numbers from the imput file
	int solver_or_checker_flag =-1;//flag to check or solve operation

	if ((argc < INSUFFICIENT_ARGUMENTS)||(argc > MAX_ARGUMENTS_NUMBER)) //check number of arguments from main
	{
		printf("ARGUMENTS NUMBER INCORRECT\n");
		return(ERROR);						
	}

	solver_or_checker_flag = atoi(argv[1]);// get flag for solver/checker from command line
	if(text_to_mat(argv[2],mat) == ERROR)//convert the input file to a matrix 
		return ERROR;

	if (argc == MAX_ARGUMENTS_NUMBER)// we have got an output file name from main
		output_file = create_output_file(argv[3]);	//create_output_file with argv[3] name
	else
	{
		create_output_file_name(argv[2]);// reformat input file name
		output_file = create_output_file(argv[2]);// creates a file for writing with the new file name
	}

	if(solver_or_checker_flag)//0 - solver, 1 - checker
		main_checker(mat,output_file);
	else
		main_solver(mat,output_file);
	
	fclose(output_file);
}