#include <stdio.h>//for FILE object
#include <string.h> //strlen
#include <windows.h> //for thread and Sleep

#define OK 0
#define ERROR 1


void test_file_extension(char *inputfile_name,FILE *output_file)
{
	char *file_extension;
	Sleep(10);
	file_extension=strtok(inputfile_name,".");
	if(file_extension==NULL)
		//wrong input//
		return;
	file_extension=strtok(NULL,".");
	fprintf(output_file,"the file extension of the test file is \".%s\"\n",file_extension);
		return;



}

void test_file_size(FILE *input_file,FILE *output_file)
{
	Sleep(10);
	//fprintf(output_file,"the file size is %s\n",file_size);

}

void test_file_timing(FILE *input_file,FILE *output_file)
{
	Sleep(10);
	//fprintf(output_file,"the file was created on   foramt it hereeeeeeeee %s\n",file_created);
	//fprintf(output_file,"the file was last modified foramt it hereeeeeeeee %s\n",file_last_modified);
}

void test_file_contant(FILE *input_file,FILE *output_file)
{

	int i;
	char byte;
	Sleep(10);

	fprintf(output_file,"the file first 5 bytes are:");
	for( i=0;i<5;i++)
	{
		byte=getc(input_file);
		if (byte==EOF)
		{
			fprintf(output_file,"\n");
			return;
		}

		else
			fprintf(output_file,"%c",byte);
	}
	fprintf(output_file,"\n");
}

void output_file_name();

int main(int argc ,char * argv[])
{
	char *name_of_tested_file;
	char *output_file_path;
	char *temp_name_of_tested_file;
	//	name_of_tested_file=argv[1];
	//	output_file_path=argv[2];
	//char inputfile_name[]="test1.txt";
	//char outputfilepath[]="C:\Users\yogev\Documents\Visual Studio 2010\Projects\system_programming\ex2\TestFile";
	//char outputfile_name[]="test1_log.txt";
	FILE *input_file;
	FILE *output_file;
	size_t input_string_len,output_file_name_size;

	if (argc<3)
		return ERROR;


	//input string
	input_string_len=strlen(argv[1]);
	name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len);
	if (name_of_tested_file==NULL)
		return ERROR;


	name_of_tested_file=strcpy(name_of_tested_file,argv[1]);
	temp_name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len);
	if (temp_name_of_tested_file==NULL)
		return ERROR;

	temp_name_of_tested_file=strcpy(temp_name_of_tested_file,argv[1]);
	temp_name_of_tested_file=strtok(temp_name_of_tested_file,".");
	temp_name_of_tested_file=strcat(temp_name_of_tested_file,"_log.txt");
	//printf("inname=%s\n",temp_name_of_tested_file);

	//need to handle the file extantion length
	output_file_name_size=input_string_len+strlen(argv[2])+5; //<outputfile directory> + \+inputfile name with extinsion +_log
	output_file_path=(char*) malloc(sizeof(char)*output_file_name_size);
	if (output_file_path==NULL)
		return ERROR;
	output_file_path=strcpy(output_file_path,argv[2]);
	output_file_path=strcat(output_file_path,temp_name_of_tested_file);
	//printf("outname=%s\n",output_file_path);
	


	input_file=fopen(name_of_tested_file,"r");
	if (input_file==NULL)
		return ERROR;
	output_file=fopen(output_file_path,"w");
	if (output_file==NULL)
		return ERROR;


	test_file_extension(name_of_tested_file,output_file);
	test_file_size(input_file,output_file);
	test_file_timing(input_file,output_file);
	test_file_contant(input_file,output_file);


	fclose(output_file);
	fclose(input_file);

	//problem with free
	//free(output_file_path);
	//free(name_of_tested_file);
	//free(temp_name_of_tested_file);




	return OK;



}