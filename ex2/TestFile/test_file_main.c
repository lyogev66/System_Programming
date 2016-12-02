#include <stdio.h>//for FILE object
#include <string.h> //strlen
#include <windows.h> //for thread and Sleep
#include <io.h>  //_get_osfhandle to cast file to handle
#include "thread_functions.h"

//for memory leak tests
//#define CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

//#define MemoryLeak


#define OK 0
#define ERROR 1



int main(int argc ,char * argv[])
{
	//int memleak;


	char *name_of_tested_file,*address_for_name_of_tested_file;
	char *output_file_path;
	char *temp_name_of_tested_file,*address_for_temp_name_of_tested_file;
	FILE *input_file;
	FILE *output_file;
	size_t input_string_len,output_file_name_size;
	//strings for function return values;

	




	if (argc<3)
		return ERROR;
		

	//input string
	input_string_len=strlen(argv[1]);
	name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len+1);
	if (name_of_tested_file==NULL)
		return ERROR;
	address_for_name_of_tested_file=name_of_tested_file;
	name_of_tested_file=strcpy(name_of_tested_file,argv[1]);


	temp_name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len+5);//1 for \0 and +4 for _log
	if (temp_name_of_tested_file==NULL)
		return ERROR;
	address_for_temp_name_of_tested_file=temp_name_of_tested_file;
	temp_name_of_tested_file=strcpy(temp_name_of_tested_file,name_of_tested_file);
	temp_name_of_tested_file=strtok(temp_name_of_tested_file,".");
	temp_name_of_tested_file=strcat(temp_name_of_tested_file,"_log.txt");


	output_file_name_size=input_string_len+strlen(argv[2])+5; //<outputfile directory> + \+inputfile name with extinsion +_log
	output_file_path=(char*) malloc(sizeof(char)*output_file_name_size+1);
	if (output_file_path==NULL)
		return ERROR;
	output_file_path=strcpy(output_file_path,argv[2]);
	output_file_path=strcat(output_file_path,temp_name_of_tested_file);


	fopen_s(&input_file,name_of_tested_file,"r+");
	if (input_file==NULL)
		return ERROR;
	output_file=fopen(output_file_path,"w");
	if (output_file==NULL)
		return ERROR;


	//main part - calling functions
	
	MainThreadExample(name_of_tested_file);
	/*

	print_file_name(name_of_tested_file,output_file);
	test_file_extension(name_of_tested_file,output_file);
	test_file_size(input_file,output_file);
	test_file_create_time(input_file,output_file);
	test_file_modified_time(input_file,output_file);
	test_file_contant(input_file,output_file);
	*/


	//tidy up - close files and free memory
	fclose(output_file);
	fclose(input_file);
	free(output_file_path);
	free(address_for_name_of_tested_file);
	free(address_for_temp_name_of_tested_file);


	//#ifdef MemoryLeak
	//_CrtDumpMemoryLeaks();  //Call this function to view the unrelease memory
	//#endif
	//memleak=_CrtDumpMemoryLeaks();

	return OK;





}