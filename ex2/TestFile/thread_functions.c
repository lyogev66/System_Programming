#include <stdio.h>//for FILE object
#include <string.h> //strlen
#include <windows.h> //for thread and Sleep
#include <math.h> //ceil
//#include <io.h>  //_get_osfhandle to cast file to handle

#include <tchar.h>
#include <stdlib.h>
#include "thread_functions.h"

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

/**
* <ISP> Example of using threads.
*/
#define NUM_OF_THREADS 5
#define FILETIME_UNITS_PER_MILLISECOND 10000 

typedef struct objectp{
	char *inputfile_name;
	HANDLE input_hfile;
	char *file_extension;
	char *file_size;
	char *file_create_time;
	char *file_modified_time;
	char *file_contant;

}objectp;

void print_file_name(objectp *arg_p)//char *inputfile_name,FILE *output_file)
{
//	DWORD dwBytesToWrite = (DWORD)strlen(arg_p->inputfile_name);
//	DWORD dwBytesWritten = 0;
//	BOOL bErrorFlag = FALSE;
//	Sleep(10);
//
//	bErrorFlag = WriteFile( 
//					arg_p->output_hfile,        // open file handle
//                    arg_p->inputfile_name,		// start of data to write
//                    dwBytesToWrite,				// number of bytes to write
//                    &dwBytesWritten,			// number of bytes that were written
//                    NULL);						// no overlapped structure
//
	return;
}

void test_file_extension(objectp *arg_p)//char *inputfile_name,FILE *output_file)
{
	int malloc_size=strlen("The file extension of the test file is \n")+10;
	char *file_extension,*file_name_copy;
	int input_name_len=strlen(arg_p->inputfile_name);
	Sleep(10);

	file_name_copy=(char*)malloc(sizeof(char)*(input_name_len+1));	

	strcpy(file_name_copy,arg_p->inputfile_name);
	file_extension=strtok(file_name_copy,".");
	if(file_extension==NULL)
		//wrong input//
		return;
	file_extension=strtok(NULL,".");

	
	arg_p->file_extension=(char*)malloc(sizeof(char)*malloc_size);										
	sprintf(arg_p->file_extension,"The file extension of the test file is \".%s\"\n",file_extension);
	free(file_name_copy);
	return;
}

void test_file_size(objectp *arg_p)//FILE *input_file,FILE *output_file)
{
	DWORD file_size;
	char* Units[] = { "B", "KB", "MB", "GB", "TB"};
	int unit_index=0;
	int malloc_size=strlen("The test file size is KB\n")+10;
	//FILE *input_file=p->input_file;
	//FILE *output_file=p->output_file;
	//double file_size=0;

	Sleep(10);
	GetFileSize(arg_p->input_hfile,&file_size);

	arg_p->file_size=(char*)malloc(sizeof(char)*malloc_size);

	while(file_size>1024)
	{
		file_size/=1024;
		unit_index++;
	}

	sprintf(arg_p->file_size,"The test file size is %ldKB\n",file_size);

	//GetFileSize(input_file,file_size1);
	//fprintf(output_file,"the file size is %d bytes\n",file_size);
}

void test_file_create_time(objectp *arg_p)
{
	int malloc_size=strlen("The file was created on \n")+30;
	FILETIME created;
	SYSTEMTIME system_time;
	int succes;

	arg_p->file_create_time=(char*)malloc(sizeof(char)*malloc_size);

	succes=GetFileTime(arg_p->input_hfile, &created, NULL, NULL);
	FileTimeToSystemTime(&created, &system_time);
	sprintf(arg_p->file_create_time,"The file was created on %02d/%02d/%d %02d:%02d:%02d\n",
		system_time.wDay, system_time.wMonth, system_time.wYear, system_time.wHour, system_time.wMinute,system_time.wSecond );	
	return;
}

void test_file_modified_time(objectp *arg_p)
{
	int malloc_size=strlen("The file was last modified on \n")+30;
	FILETIME modified;
	SYSTEMTIME system_time;
	int succes;

	arg_p->file_modified_time=(char*)malloc(sizeof(char)*malloc_size);

	succes=GetFileTime(arg_p->input_hfile, NULL, NULL, &modified);
	FileTimeToSystemTime(&modified, &system_time);
	sprintf(arg_p->file_modified_time,"The file was last modified on %02d/%02d/%d %02d:%02d:%02d\n",
		system_time.wDay, system_time.wMonth, system_time.wYear, system_time.wHour, system_time.wMinute,system_time.wSecond);
}

void test_file_contant(objectp *arg_p)
{
	BOOL success;
	DWORD nNumberOfBytesToRead=5;
	DWORD get_error;
	//DWORD lpNumberOfBytesRead=0;
	int malloc_size=strlen("The files first 5 bytes are:aaaaa\n")+1;
	int i;
	char byte;
	char *buffer;
	Sleep(10);

	buffer= (char*)malloc(sizeof(char)*6);
	success=ReadFile(arg_p->input_hfile,buffer,nNumberOfBytesToRead,NULL,NULL);
	if (!success)
		get_error=GetLastError();
	//ReadFile(arg_p->input_hfile,

	arg_p->file_contant=(char*)malloc(sizeof(char)*malloc_size);

	sprintf(arg_p->file_contant,"The files first 5 bytes are:%s\n",buffer);
	free(buffer);
}



HANDLE create_thread_simple(LPTHREAD_START_ROUTINE StartAddress,LPVOID ParameterPtr,LPDWORD ThreadIdPtr)
{
	return CreateThread(
		NULL,            /*  default security attributes */
		0,                /*  use default stack size */
		StartAddress,    /*  thread function */
		ParameterPtr,    /*  argument to thread function */
		0,                /*  use default creation flags */
		ThreadIdPtr);    /*  returns the thread identifier */
}

void MainThreadExample(char *inputfile_name)
{

	int i;
	HANDLE in_hfile;
	HANDLE ThreadHandles[NUM_OF_THREADS];   /* An array of thread handles */
	DWORD ThreadIDs[NUM_OF_THREADS];        /* An array of threadIDs */
	DWORD exitcode;
	objectp arg_p;



	//opening a file handle for output
	in_hfile = CreateFile(inputfile_name,                // name of the write
                       READ_CONTROL,          // open for reading
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (in_hfile == INVALID_HANDLE_VALUE) 
    { 
        //DisplayError(TEXT("CreateFile"));
        _tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for read.\n"), inputfile_name);
        return;
    }

	//inserting the values into the struct
	arg_p.inputfile_name=inputfile_name;
	arg_p.input_hfile=in_hfile;
	arg_p.file_extension=NULL;
	arg_p.file_size=NULL;
	arg_p.file_create_time=NULL;
	arg_p.file_modified_time=NULL;
	arg_p.file_contant=NULL;


	ThreadHandles[0] = create_thread_simple(
		(LPTHREAD_START_ROUTINE)test_file_extension,  /*  thread function */
		&arg_p,								/*  argument to thread function */
		&ThreadIDs[0]);                    /*  returns the thread identifier */
	ThreadHandles[1] = create_thread_simple(
		(LPTHREAD_START_ROUTINE)test_file_size,
		&arg_p,
		&ThreadIDs[1]);

	
	ThreadHandles[2] = create_thread_simple(
		(LPTHREAD_START_ROUTINE)test_file_create_time,
		&arg_p,
		&ThreadIDs[2]);

	ThreadHandles[3] = create_thread_simple(
		(LPTHREAD_START_ROUTINE)test_file_modified_time,
		&arg_p,
		&ThreadIDs[3]);

	//ThreadHandles[4] = create_thread_simple(
	//	(LPTHREAD_START_ROUTINE)test_file_contant,
	//	&arg_p,
	//	&ThreadIDs[4]);
		
	/* Wait for all threads to end: */
	WaitForMultipleObjects(
		NUM_OF_THREADS,
		ThreadHandles,
		TRUE,       /* wait until all threads finish */
		INFINITE);

	Sleep(10);

	for (i = 0; i < NUM_OF_THREADS; i++)
	{
		if (i==4)
		{
			continue;
		}
		GetExitCodeThread(ThreadHandles[i], &exitcode);
		printf("Thread number %d returned exit code %d\n", i, exitcode);
		CloseHandle(ThreadHandles[i]);
	}

	printf("%s\n",arg_p.inputfile_name);
	printf(arg_p.file_extension);
	printf(arg_p.file_size);
	printf(arg_p.file_create_time);
	printf(arg_p.file_modified_time);
	//printf(arg_p.file_contant);




	//handle the free over here !!!!!!!!!!!!!!1 leak_here
	free(arg_p.file_extension); 
	free(arg_p.file_size); 
	free(arg_p.file_create_time); 
	free(arg_p.file_modified_time); 
	//free(arg_p.file_contant); problem here !!!

}
