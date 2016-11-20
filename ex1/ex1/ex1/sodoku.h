#ifndef SODOKU_H
#define SODOKU_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

	
#define OK						0
#define ERROR					1
#define ARR_SIZE				10
#define MAT_SIZE				9
#define SUB_MAT_SIZE			3	
#define CHAR_TO_DIGIT			48
#define INSUFFICIENT_ARGUMENTS	2
#define MAX_ARGUMENTS_NUMBER	4
#define ONLY_ONE_OPTION			1

typedef int opt_array[ARR_SIZE];

int text_to_mat(char* input_file_name,int arr[MAT_SIZE][MAT_SIZE]);
int main_solver(int mat[MAT_SIZE][MAT_SIZE],FILE * output_file);
void main_checker(int arr[MAT_SIZE][MAT_SIZE],FILE * output_file);
FILE* create_output_file(char* output_file_name);
void create_output_file_name(char* output_file_name);

#endif