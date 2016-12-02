#include <stdio.h>//for FILE object
#include <string.h> //strlen
#include <windows.h> //for thread and Sleep
#include <math.h> //ceil
#include <io.h>  //_get_osfhandle to cast file to handle

#include <tchar.h>
#include <stdlib.h>
#include "thread_functions.h"

typedef struct objectp{
	char *inputfile_name;
	HANDLE input_hfile;
	char *file_extension;
	char *file_size;
	char *file_create_time;
	char *file_modified_time;
	char *file_contant;

}objectp;

//void print_file_name(objectp *arg_p)//char *inputfile_name,FILE *output_file)
//{
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
//	return;
//}

void test_file_extension(objectp *arg_p)//char *inputfile_name,FILE *output_file)
{
	int malloc_size=strlen("The file extension of the test file is \n")+5;
	char *file_extension=arg_p->inputfile_name;
	Sleep(10);
	file_extension=strtok(file_extension,".");
	if(file_extension==NULL)
		//wrong input//
		return;
	file_extension=strtok(NULL,".");

	
	arg_p->file_extension=(char*)malloc(sizeof(char)*malloc_size);										//leak_here
	sprintf(arg_p->file_extension,"The file extension of the test file is \".%s\"\n",file_extension);
	return;
}

void test_file_size(objectp *arg_p)//FILE *input_file,FILE *output_file)
{
	DWORD file_size;
	char* Units[] = { "B", "KB", "MB", "GB", "TB"};
	int unit_index=0;
	int malloc_size=strlen("The test file size is KB\n")+5;
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
	DWORD nNumberOfBytesToRead=5;
	DWORD lpNumberOfBytesRead=0;
	int malloc_size=strlen("The files first 5 bytes are:aaaaa\n")+1;
	int i;
	char byte;
	char *buffer;
	Sleep(10);

	buffer= (char*)malloc(sizeof(char)*6);
	ReadFile(arg_p->input_hfile,buffer,nNumberOfBytesToRead,&lpNumberOfBytesRead,NULL);
	//ReadFile(arg_p->input_hfile,

	arg_p->file_contant=(char*)malloc(sizeof(char)*malloc_size);

	sprintf(arg_p->file_contant,"The files first 5 bytes are:%s\n",buffer);
	free(buffer);
}



//void output_file_name();


#define NUM_OF_THREADS 5
#define FILETIME_UNITS_PER_MILLISECOND 10000 

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

/**
* <ISP> Example of using threads.
*/

HANDLE create_thread_simple(LPTHREAD_START_ROUTINE StartAddress,
	LPVOID ParameterPtr,
	LPDWORD ThreadIdPtr)
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
	HANDLE ThreadHandles[NUM_OF_THREADS];   /* An array of thread handles */
	DWORD ThreadIDs[NUM_OF_THREADS];        /* An array of threadIDs */
	DWORD exitcode;
	//GuestSeating_t guest0, guest1, guest2; /* Items to be compared with the GuestDB */
	int i;
	char out_name[]="C:\Users\yogev\Documents\Visual Studio 2010\Projects\system_programming\ex2\TestFile\out.txt";
	DWORD dwBytesToWrite = (DWORD)strlen(out_name);
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	HANDLE out_hfile;
	HANDLE in_hfile;




	objectp arg_p;
	arg_p.inputfile_name=inputfile_name;


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




	//opening a file handle for output
	    out_hfile = CreateFile(out_name,                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_ALWAYS,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (out_hfile == INVALID_HANDLE_VALUE) 
    { 
        //DisplayError(TEXT("CreateFile"));
        _tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), out_name);
        return;
    }


	arg_p.input_hfile=in_hfile;





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

	ThreadHandles[4] = create_thread_simple(
		(LPTHREAD_START_ROUTINE)test_file_contant,
		&arg_p,
		&ThreadIDs[4]);
		
	/* Wait for all threads to end: */
	WaitForMultipleObjects(
		NUM_OF_THREADS,
		ThreadHandles,
		TRUE,       /* wait until all threads finish */
		INFINITE);

	Sleep(10);

	for (i = 0; i < NUM_OF_THREADS; i++)
	{
		GetExitCodeThread(ThreadHandles[i], &exitcode);
		printf("Thread number %d returned exit code %d\n", i, exitcode);
		CloseHandle(ThreadHandles[i]);
	}
	printf("%s\n",arg_p.inputfile_name);
	printf(arg_p.file_extension);
	printf(arg_p.file_size);
	printf(arg_p.file_create_time);
	printf(arg_p.file_modified_time);
	printf(arg_p.file_contant);




	//handle the free over here !!!!!!!!!!!!!!1
	//free(arg_p.file_extension); leak_here
	//free(arg_p.file_size); leak_here
	//free(arg_p.file_create_time); leak_here
	//free(arg_p.file_modified_time); leak_here
	//free(arg_p.file_contant); leak_here

}
