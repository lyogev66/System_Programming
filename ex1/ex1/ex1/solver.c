/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 1
Using -			sodoku.h	 

Description -	solves the sodoku if possible
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "sodoku.h"


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fill_row_optional_numbers
Input arguments:	int mat[MAT_SIZE][MAT_SIZE] -	2D array containing the current sodoku matrix
						arr[ARR_SIZE]			-	an array of flags determing the available row values for the current cell
													arr[ARR_SIZE] is sent with all the values in it initialized to '1' 
						row						-	The current row number

return:				None

Description-		The function use the arr[ARR_SIZE] to indicate which values are already filled in the current row
					it goes over the specific row at the matrix and zeroes the value in the arr[ARR_SIZE] index if it is
					present at the 2D matrix row
					
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

void fill_row_optional_numbers(int mat[MAT_SIZE][MAT_SIZE],int arr[ARR_SIZE],int row)
{
	int i;
	for(i=0;	i<MAT_SIZE;	i++)
	{
		if(arr[mat[row][i]])
		{
			arr[mat[row][i]]--;
		}
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fill_column_optional_numbers
Input arguments:	int mat[MAT_SIZE][MAT_SIZE] -	2D array containing the current sodoku matrix
						arr[ARR_SIZE]			-	an array of flags determing the available column values for the current cell
													arr[ARR_SIZE] is sent with all the values in it initialized to '1' 
						col						-	The current column number

return:				None

Description-		The function use the arr[ARR_SIZE] to indicate which values are already filled in the current column
					it goes over the specific column at the matrix and zeroes the value in the arr[ARR_SIZE] index if it is
					present at the 2D matrix column
					
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

void fill_column_optional_numbers(int mat[MAT_SIZE][MAT_SIZE],int arr[ARR_SIZE],int col)
{
	int i;
	for(i=0;	i<MAT_SIZE;	i++)
	{
		if(arr[mat[i][col]])
		{
			arr[mat[i][col]]--;
		}
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fill_subgrid_optional_numbers
Input arguments:	int mat[MAT_SIZE][MAT_SIZE] -	2D array containing the current sodoku matrix
						arr[ARR_SIZE]			-	an array of flags determing the available sub grid values for the current cell
													arr[ARR_SIZE] is sent with all the values in it initialized to '1' 
return:				None

Description-		The function use the arr[ARR_SIZE] to indicate which values are already filled in the current sub grid 
					it goes over the specific sub grid at the matrix and zeroes the value in the arr[ARR_SIZE] index if it is
					present at the 2D matrix sub grid
					
					
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void fill_subgrid_optional_numbers(int mat[MAT_SIZE][MAT_SIZE],int arr[ARR_SIZE])
{
	int i,j;
	for(i=0;i<SUB_MAT_SIZE;i++)
	{
		for(j=0;j<SUB_MAT_SIZE;j++)
		{
			if(arr[mat[i][j]])
			{
				arr[mat[i][j]]--;
			}
		}
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fill_subgrid_optional_numbers
Input arguments:	int arr_row[ARR_SIZE]		-	1D array contaning the available row values for the current cell 
						arr_col[ARR_SIZE]		-	1D array contaning the available column values for the current cell 
						arr_subgrid[ARR_SIZE]	-	1D array contaning the available sub grid values for the current cell 
return:				int *total_optional_numbers_for_cell	-	a dinamic allocated array indicating all the available value in the 
																sudoko matrix.

Description-		The function allocates a dinamic array for each cell that was previosly empty, that array indicates
					which values are optional for that cell according to the row, column and sub grid.
					the first cell of the dinamic array - arr[0]- contains the number of available values. 
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int *total_optional_numbers_for_cell(int arr_row[ARR_SIZE],int arr_col[ARR_SIZE],int arr_subgrid[ARR_SIZE])
{
	int i,count =0,flag=1;
	int *final_arr;
	final_arr = (int*) malloc(ARR_SIZE*sizeof(int));
	if(final_arr ==NULL)
		exit(ERROR);
	for(i=1	;i<ARR_SIZE;	i++)
	{
		final_arr[i] = arr_row[i] & arr_col[i] & arr_subgrid[i];
		if(final_arr[i])
			count ++;
	}
	final_arr [0] = count;

	for(i=0;i<ARR_SIZE;i++)
	{
		if(final_arr[i])
			flag =0;
	}
	if(flag)
		exit(ERROR); //No optional values available for the current cell , no go situation 
	return final_arr;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		return_single_option
Input arguments:	int	arr[ARR_SIZE]			-	an array of flags determing the available values for the current cell
													
return:				int i - the single value at the array or OK if the array is empty
Description-		The function returned the single value available for the current cell
						
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int return_single_option(int arr[ARR_SIZE])
{
	int i;
	for(i = 1; i<ARR_SIZE; i++)
		if(arr[i])
			return i;
	return OK;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		init_arr
Input arguments:	int arr[ARR_SIZE] -	1D array
						num			-	Initial value 
return:				None

Description-		The function initialize all the values in the array to num
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

void init_arr(int arr[ARR_SIZE],int num)
{
	int i;
	for (i=0;i<ARR_SIZE;i++)
		arr[i] =num;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		found_in_array
Input arguments:	int k				-	The value to be searched
						arr[ARR_SIZE]	-	1D array
return:				int OK				-	if the value is found
						ERROR			-	if the value is not found

Description-		The function search for k in the array and returns OK if it is there
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int found_in_array(int k,int arr[ARR_SIZE])
{
	if(arr[k])
		return OK;
	return ERROR;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		find_single_in_sub_grid
Input arguments:	int mat[MAT_SIZE][MAT_SIZE]		-	The value to be searched
						row							-	Current row
						col							-	Current column
					int *opt_mat[MAT_SIZE][MAT_SIZE]-	The matrix of all the available option

return:				int flag	-	If the cell had his value changed

Description-		The function operates when there is more than one value available for each cell in a sub grid
					it goes over the cells, and finds if there is a value that can be placed only in one cell.
					If such a case happens it place the value in the cell. 

oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int find_single_in_sub_grid(int mat[MAT_SIZE][MAT_SIZE],int *opt_mat[MAT_SIZE][MAT_SIZE],int row, int col)
{
	int arr[ARR_SIZE];
	int i,j,k;
	int flag =0;
	init_arr(arr,0);
	for(i=0;i<SUB_MAT_SIZE;i++)
	{
		for(j=0;j<SUB_MAT_SIZE;j++)
		{
			if(mat[i][j] == 0)
			{
				for(k=1;k<ARR_SIZE;k++)
				{
					if(opt_mat[i+row][j+col][k])
						arr[k]++;
				}
			}
		}
	}

	for(k=1;k<ARR_SIZE;k++)
	{
		if(arr[k] == 1)
		{
			for(i=0;i<SUB_MAT_SIZE;i++)
			{
				for(j=0;j<SUB_MAT_SIZE;j++)
				{
					if(mat[i][j] == 0)
					{
						if(!found_in_array(k,opt_mat[i+row][j+col]))
						{
							mat[i][j] = k;
							flag=1;
							free(opt_mat[i+row][j+col]);
						}
					}
				}

			}
		}
	}
	return flag;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		fprint_mat
Input arguments:	int mat[MAT_SIZE][MAT_SIZE]	- The complete sodoku matrix
					FILE * output_file			- The output file handler

return:				None

Description-		The function prints the matrix to a output file according to exercise the instructions 

oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void fprint_mat(int mat[MAT_SIZE][MAT_SIZE],FILE * output_file)
{
	int i,j;
	for(i=0;i<MAT_SIZE; i++)
	{
		for(j=0;j<MAT_SIZE; j++)
		{
			if ((j %SUB_MAT_SIZE == 0) && (j>1))
				fprintf(output_file," |");
			fprintf(output_file," %d",mat[i][j]);

		}

		if ((i %SUB_MAT_SIZE == 2) && (i>1) && (i<8))
			fprintf(output_file,"\n-------+-------+-------");
		fprintf(output_file,"\n");
	}
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		check_if_done
Input arguments:	int mat[MAT_SIZE][MAT_SIZE]	- The complete sodoku matrix
					*opt_mat[MAT_SIZE][MAT_SIZE]- The option matrix
					FILE * output_file			- The output file handler

return:				int OK if the matrix is solved ERROR else.

Description-		The function checks if the mtrix is full, if it is not full it prints a message to the output file
					furthermore, the function frees any leftover dinamic allocated arrays.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int check_if_done(int mat[MAT_SIZE][MAT_SIZE],int *opt_mat[MAT_SIZE][MAT_SIZE],FILE * output_file)
{
	int i,j,error_flag = 0;
	for(i=0;i<MAT_SIZE; i++)
	{
		for(j=0;j<MAT_SIZE; j++)
		{
			if(mat[i][j] == 0)
			{	
				if(!error_flag)
				{
					fprintf(output_file,"Sudoku puzzele is too hard for me to solve");
					error_flag ++;
				}
				free(opt_mat[i][j]);
			}
		}
	}
	if(error_flag)
		return ERROR;
	return OK;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main_solver
Input arguments:	int mat[MAT_SIZE][MAT_SIZE]	- The complete sodoku matrix
					FILE * output_file			- The output file handler

return:				int OK if the matrix is solved ERROR else.

Description-		The function calls the sub routines in order to solve the sudoko.
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main_solver(int mat[MAT_SIZE][MAT_SIZE],FILE * output_file)
{
	int row,col;
	int arr_row[ARR_SIZE],arr_col[ARR_SIZE],arr_subgrid[ARR_SIZE];
	int flag =1;
	int *options_matrix[MAT_SIZE][MAT_SIZE];
	while(flag)//flag goes up if there is any change in the matrix in a single loop 
	{
		flag = 0;
		// START - fill options per cell
		for(row = 0; row< MAT_SIZE; row ++)
		{
			for(col = 0;	col<MAT_SIZE;	col++)
			{
				init_arr(arr_row,1);
				init_arr(arr_col,1);
				init_arr(arr_subgrid,1);
				if(0 == mat[row][col])//the cell needs to be filled
				{
					fill_row_optional_numbers(mat,arr_row,row);
					fill_column_optional_numbers(mat,arr_col,col);
					fill_subgrid_optional_numbers((int (*)[MAT_SIZE])
						&mat[(row/SUB_MAT_SIZE)*SUB_MAT_SIZE][(col/SUB_MAT_SIZE)*SUB_MAT_SIZE],arr_subgrid);
					//this function recives a sub matrix according to the cell cordinations 
					options_matrix[row][col] = total_optional_numbers_for_cell(arr_row,arr_col,arr_subgrid);
				}
			}
		}
		// END - fill options per cell
		//START - fill cells with only one option
		for(row=0; row< MAT_SIZE; row ++)
		{
			for(col=0;	col<MAT_SIZE;	col++)
			{
				if(mat[row][col] == 0)
				{
					if(*options_matrix[row][col] == ONLY_ONE_OPTION)// if there is only one option than set it as the value of the cell
					{
						mat[row][col] = return_single_option(options_matrix[row][col]);
						flag =1;
						free(options_matrix[row][col]);
					}
				}
			}
		}//if flag is zero at this point either we finnished or we have a problem of at least 2 options available for a cell

		//END - fill cells with only one option
		//START - try to fill cells with more than one option
		if(!flag)
		{
			for(row = 0; row< MAT_SIZE; row+=3)
			{
				for(col = 0;	col<MAT_SIZE;	col+=3)
				{
					if(find_single_in_sub_grid((int (*)[MAT_SIZE])&mat[row][col],options_matrix,row,col))
						flag=1;
				}
			}
		}
		//END - try to fill cells with more than one option
	}//End of while

	if(check_if_done(mat,options_matrix,output_file) == ERROR)
		return ERROR;

	fprint_mat(mat,output_file);//prints matrix to the output file

	return OK;

}

