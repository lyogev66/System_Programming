#include <stdio.h>//for FILE object
#include <string.h> //strlen
#include <windows.h> //for thread and Sleep
#include <math.h> //ceil
#include <io.h>  //_get_osfhandle to cast file to handle

//for memory leak tests
//#define CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

//#define MemoryLeak


#define OK 0
#define ERROR 1

void print_file_name(char *inputfile_name,FILE *output_file)
{
	Sleep(10);
	fprintf(output_file,"%s\n",inputfile_name);
	return;
}

void test_file_extension(char *inputfile_name,FILE *output_file)
{
	char *file_extension;
	Sleep(10);
	file_extension=strtok(inputfile_name,".");
	if(file_extension==NULL)
		//wrong input//
		return;
	file_extension=strtok(NULL,".");
	fprintf(output_file,"The file extension of the test file is \".%s\"\n",file_extension);
	return;
}



void test_file_size(FILE *input_file,FILE *output_file)
{
	double file_size=0;


	Sleep(10);
	fseek(input_file,0,SEEK_END);
	file_size=(double)ftell(input_file);
	rewind(input_file);
	if(file_size<1000)
		fprintf(output_file,"The test file size is %.0fB\n",file_size);
	else
		//roundup the value
		fprintf(output_file,"The test file size is %.0fKB\n",ceil(file_size/1000));

	//GetFileSize(input_file,file_size1);
	//fprintf(output_file,"the file size is %d bytes\n",file_size);
}



void test_file_create_time(FILE *input_file,FILE *output_file)
{
	HANDLE file_handle;
	FILETIME created;
	SYSTEMTIME system_time;
	int succes;


	//fclose(input_file);
	//file_handle = CreateFile("my_test.txt", GENERIC_READ, FILE_SHARE_READ,  NULL,  OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);

	file_handle = (HANDLE)_get_osfhandle(fileno(input_file));
	succes=GetFileTime(file_handle, &created, NULL, NULL);
	FileTimeToSystemTime(&created, &system_time);
	fprintf(output_file,"The file was created on %02d/%02d/%d %02d:%02d:%02d\n",
		system_time.wDay, system_time.wMonth, system_time.wYear, system_time.wHour, system_time.wMinute,system_time.wSecond );	
	//CloseHandle(file_handle);




}

void test_file_modified_time(FILE *input_file,FILE *output_file)
{
	HANDLE file_handle;
	FILETIME modified;
	SYSTEMTIME system_time;
	int succes;

	file_handle = (HANDLE)_get_osfhandle(fileno(input_file));
	succes=GetFileTime(file_handle, NULL, NULL, &modified);
	FileTimeToSystemTime(&modified, &system_time);
	fprintf(output_file,"The file was last modified on %02d/%02d/%d %02d:%02d:%02d\n",
		system_time.wDay, system_time.wMonth, system_time.wYear, system_time.wHour, system_time.wMinute,system_time.wSecond);
	//CloseHandle(file_handle);
}



void test_file_contant(FILE *input_file,FILE *output_file)
{

	int i;
	char byte;
	Sleep(10);

	fprintf(output_file,"The files first 5 bytes are:");
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
	//int memleak;


	char *name_of_tested_file,*address_for_name_of_tested_file;
	char *output_file_path;
	char *temp_name_of_tested_file,*address_for_temp_name_of_tested_file;
	FILE *input_file;
	FILE *output_file;
	size_t input_string_len,output_file_name_size;

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
	print_file_name(name_of_tested_file,output_file);
	test_file_extension(name_of_tested_file,output_file);
	test_file_size(input_file,output_file);
	test_file_create_time(input_file,output_file);
	test_file_modified_time(input_file,output_file);
	test_file_contant(input_file,output_file);



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