/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 2
Using -			thread_functions.h	 
	
Description -	runs the TestFile project
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "thread_functions.h"

HANDLE g_file_handle;	// a global for the file handle				????????????
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		round_to_2_digit
Input arguments:	double file_size - a file size to be rounded down to 2 digits

return:				double			 - a rounded file size		-

Description-		Rounds the file size down to 2 digits after the decimal point
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
double round_to_2_digit(double file_size)
{
	file_size=(file_size*100)+0.5;
	file_size=floor(file_size);
	return file_size/100;
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		test_get_times
Input arguments:	FILE_TIMES *ft - a file time pointer to be filled

return:				None		

Description-		a Thread function to be called and handle the file time test
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void test_get_times(FILE_TIMES *ft)
{
	Sleep(10);
	if(GetFileTime(g_file_handle,&ft->CreationTime, NULL, &ft->lastmodified) == MY_ERROR)
		goto FAIL;
	if(FileTimeToSystemTime(&ft->lastmodified, &ft->UTC_lastmodified) == MY_ERROR)
		goto FAIL;
	if(SystemTimeToTzSpecificLocalTime(NULL,&(ft->UTC_lastmodified),&(ft->UTC_Isreal_lastmodified)) == MY_ERROR)
		goto FAIL;
	if(FileTimeToSystemTime(&ft->CreationTime, &ft->UTC_CreationTime) == MY_ERROR)
		goto FAIL;
	if(SystemTimeToTzSpecificLocalTime(NULL,&(ft->UTC_CreationTime),&(ft->UTC_Israel_CreationTime)) == MY_ERROR)
		goto FAIL;

	ExitThread(STATUS_OK);
FAIL:
	printf("test_get_times failed with 0x%x\n",GetLastError());
	ExitThread (GetLastError());
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		test_file_size
Input arguments:	SIZE_INFORMATION *si - a size information pointer to be filled

return:				None		

Description-		a Thread function to be called and handle the file size test
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void test_file_size(SIZE_INFORMATION *si)
{
	DWORD dword_file_size = 0;
	char unit_index = 0;
	Sleep(10);
	dword_file_size= GetFileSize(g_file_handle,NULL);
	if(si->file_size == INVALID_FILE_SIZE)
	{
		printf("test_file_size failed with 0x%x\n",GetLastError());
		ExitThread (GetLastError());
	}
	si->file_size=(double)dword_file_size;
	while(si->file_size > SIZE_QUANT)
	{
		si->file_size /= SIZE_QUANT;
		unit_index ++ ;
	}
	si->file_size=round_to_2_digit(si->file_size);
	si->units =(FILE_UNITS) unit_index;

	ExitThread(STATUS_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		test_file_contant
Input arguments:	char* ff - a pointer to a the bytes read from the file

return:				None		

Description-		a Thread function to be called and reads the first five bytes from the file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void test_file_contant(char* ff)
{
	BOOL success = TRUE;
	DWORD NumRead;
	Sleep(10);
	success = ReadFile(g_file_handle,ff,NumberOfBytesToRead,&NumRead,NULL);
	ff[NumberOfBytesToRead] = '\0';
	if (!success)
	{
		printf("test_file_contant failed with 0x%x\n",GetLastError());
		ExitThread (GetLastError());
	}
	ExitThread(STATUS_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		test_file_extension
Input arguments:	char* str - a pointer to a the file name to be changed to the file extansion

return:				None		

Description-		a Thread function to be called which gets the file name and moves the pointer
					of the string to the point where the file extension is
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void test_file_extension(char* str)
{
	char *str_tmp = NULL;
	Sleep(10);

	str_tmp = (char*)memchr(str, '.', strlen(str));
	if(str_tmp == NULL)
	{
		printf("the token '.' does not exists \n");
		ExitThread (NO_TOKEN_FOUND);
	}
	str_tmp = str_tmp + 1;
	strcpy(str,str_tmp);

	ExitThread(STATUS_OK);
}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		size_enum_to_string
Input arguments:	FILE_UNITS fu - a file unit struct 

return:				char * - the matching string that represent the file_units struct		

Description-		a function that gets a file units struct and returns the matching string of file units
					this function helps the file size thread
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
char* size_enum_to_string(FILE_UNITS fu)
{
	switch(fu)
	{
	case B:
		return "B";
	case KB:
		return "KB";
	case MB:
		return "MB";
	case GB:
		return "GB";
	case TB:
		return "TB";
	default:
		"UNKNOWN_FILE_SIZE";
	}

}
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		main
Input arguments:	int argc	-command line argument count
					char* argv[]-a pointer to command line argument strings

return:				int - the exit code in which the program finished

Description-		handles the project and returns the exit code
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc ,char * argv[])
{
	int index;
	FILE_TIMES ft;
	STATUS_DATA sd; 
	SIZE_INFORMATION si;
	int exit_codes_array [THREAD_NUMBER] ={0} ;
	FILE *output_file = NULL;
	HANDLE Thread_handle_array[THREAD_NUMBER];
	size_t input_string_len,output_file_name_size;
	char FirstFive[NumberOfBytesToRead + 1] = {0},* str_tmp = NULL ;
	DWORD file_size, ThreadID[THREAD_NUMBER], exitCodesArr[THREAD_NUMBER] = {0}, exit_code =0;
	char *temp_name_of_tested_file = NULL,*address_for_temp_name_of_tested_file = NULL;
	char *name_of_tested_file = NULL ,*address_for_name_of_tested_file = NULL ,*output_file_path = NULL;

	if (argc < NUMBER_OF_ARGUMENTS)
		exit(ERROR);

	input_string_len=strlen(argv[1]);
	name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len+1);
	if (name_of_tested_file == NULL)
	{
		printf("Memory Allocation failed \n");
		exit(ERROR);
	}
	address_for_name_of_tested_file = name_of_tested_file;
	name_of_tested_file = strcpy(name_of_tested_file,argv[1]);

	temp_name_of_tested_file = (char*) malloc(sizeof(char)*input_string_len+ strlen("_log") +1);
	if (temp_name_of_tested_file==NULL)
	{
		printf("Memory Allocation failed \n");
		free (name_of_tested_file);
		exit(ERROR);
	}

	address_for_temp_name_of_tested_file = temp_name_of_tested_file;
	temp_name_of_tested_file = strcpy(temp_name_of_tested_file,name_of_tested_file);
	temp_name_of_tested_file = strtok(temp_name_of_tested_file,".");
	temp_name_of_tested_file = strcat(temp_name_of_tested_file,"_log.txt");

	output_file_name_size = strlen(argv[2])+ strlen ("\\") + input_string_len+strlen("_log");
	output_file_path=(char*) malloc(sizeof(char)*output_file_name_size + 1);
	if (output_file_path == NULL)
	{
		printf("Memory Allocation failed \n");
		free (name_of_tested_file);
		free(address_for_temp_name_of_tested_file);
		exit(ERROR);
	}
	output_file_path[0] = 0;
	output_file_path = strcpy(output_file_path,argv[2]);
	output_file_path = strcat(output_file_path,"\\");
	output_file_path = strcat(output_file_path,temp_name_of_tested_file);

	output_file=fopen(output_file_path,"w");
	if (output_file == NULL)
	{
		printf("File open failed \n");
		free (name_of_tested_file);
		free(address_for_temp_name_of_tested_file);
		free(output_file_path);
		exit(ERROR);
	}

	memset(&sd,0,sizeof(sd));
	g_file_handle = CreateFile(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY ,NULL);
	if(g_file_handle == INVALID_HANDLE_VALUE)
	{
		printf("File handler open failed \n");
		free (name_of_tested_file);
		free(address_for_temp_name_of_tested_file);
		free(output_file_path);
		fclose(output_file);
		exit(ERROR);
	}

	Thread_handle_array[TIME_THREAD] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_get_times,&ft,0,&ThreadID[TIME_THREAD]);
	if(Thread_handle_array[TIME_THREAD] == NULL)
	{
		printf("Create test_get_times thread failed \n");
		exit_code = GetLastError();
		goto EXIT_1;
	}
	Thread_handle_array[SIZE_FILE_THREAD] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_size,&si,0,&ThreadID[SIZE_FILE_THREAD]);
	if(Thread_handle_array[SIZE_FILE_THREAD] == NULL)
	{
		printf("Create test_file_size thread failed \n");
		exit_code = GetLastError();
		goto EXIT_2;
	}
	Thread_handle_array[FIRST_5_CHARS_THREAD] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_contant,FirstFive,0,&ThreadID[FIRST_5_CHARS_THREAD]);
	if(Thread_handle_array[FIRST_5_CHARS_THREAD] == NULL)
	{
		printf("Create test_file_contant thread failed \n");
		exit_code = GetLastError();
		goto EXIT_3;
	}
	Thread_handle_array[EXTENTION_FILE_THREAD] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_extension,name_of_tested_file,0,&ThreadID[EXTENTION_FILE_THREAD]);
	if(Thread_handle_array[EXTENTION_FILE_THREAD]== NULL)
	{
		printf("Create test_file_extension thread failed \n");
		exit_code = GetLastError();
		goto EXIT_4;
	}

	WaitForMultipleObjects(THREAD_NUMBER,Thread_handle_array,TRUE,INFINITE);


	while(sd.c_ts != ALL_THREAD_FINNISHED)
	{
		for(index = 0; index <THREAD_NUMBER; index++)
		{
			if(GetExitCodeThread(Thread_handle_array[index],&exitCodesArr[index]) == MY_ERROR)
			{
				goto EXIT_5;
				exit_code = GetLastError();
			}
			if(STILL_ACTIVE  != exitCodesArr[index])
			{
				switch(index)
				{
				case TIME_THREAD:
					sd.ts.TIME_THREAD_STATUS = 1;
					break;
				case SIZE_FILE_THREAD:
					sd.ts.SIZE_FILE_THREAD_STATUS = 1;
					break;
				case FIRST_5_CHARS_THREAD:
					sd.ts.FIRST_5_CHARS_THREAD_STATUS = 1;
					break;
				case EXTENTION_FILE_THREAD :
					sd.ts.EXTENTION_FILE_THREAD_STATUS = 1;
					break;
				}
			}
		}
	}

	fprintf(output_file,"%s \n", argv[1]);

	if(exitCodesArr[EXTENTION_FILE_THREAD] == STATUS_OK)
		fprintf(output_file,"The file extension of the test file is \".%s\"\n",name_of_tested_file);
	else
	{
		fprintf(output_file,"Thread EXTENTION_FILE_THREAD failed with error code :0x%x\n",exitCodesArr[EXTENTION_FILE_THREAD]);
		exit_code = exitCodesArr[EXTENTION_FILE_THREAD];
	}

	if(exitCodesArr[SIZE_FILE_THREAD] == STATUS_OK)
		fprintf(output_file,"The test file size is %.2f%s\n",si.file_size,size_enum_to_string(si.units));
	else
	{
		fprintf(output_file,"Thread SIZE_FILE_THREAD failed with error code :0x%x\n",exitCodesArr[SIZE_FILE_THREAD]);
		exit_code = exitCodesArr[SIZE_FILE_THREAD];
	}

	if(exitCodesArr[TIME_THREAD] == STATUS_OK)
	{
		fprintf(output_file,"The file was created on %02d/%02d/%d, %02d:%02d:%02d\n",
							ft.UTC_Israel_CreationTime.wDay,
							ft.UTC_Israel_CreationTime.wMonth,
							ft.UTC_Israel_CreationTime.wYear,
							ft.UTC_Israel_CreationTime.wHour,
							ft.UTC_Israel_CreationTime.wMinute,
							ft.UTC_Israel_CreationTime.wSecond );	

		fprintf(output_file,"The file was last modified on %02d/%02d/%d, %02d:%02d:%02d\n",
							ft.UTC_Isreal_lastmodified.wDay,
							ft.UTC_Isreal_lastmodified.wMonth,
							ft.UTC_Isreal_lastmodified.wYear,
							ft.UTC_Isreal_lastmodified.wHour,
							ft.UTC_Isreal_lastmodified.wMinute,
							ft.UTC_Isreal_lastmodified.wSecond);
	}
	else
	{
		fprintf(output_file,"Thread TIME_THREAD failed with error code :0x%x\n",exitCodesArr[TIME_THREAD]);
		exit_code = exitCodesArr[TIME_THREAD];
	}
	
	if(exitCodesArr[EXTENTION_FILE_THREAD] == STATUS_OK)
		fprintf(output_file,"The file's first 5 bytes are:%s\n",FirstFive);
	else
	{
		fprintf(output_file,"Thread EXTENTION_FILE_THREAD failed with error code :0x%x\n",exitCodesArr[EXTENTION_FILE_THREAD]);
		exit_code = exitCodesArr[EXTENTION_FILE_THREAD];
	}



EXIT_5:
	CloseHandle(Thread_handle_array[EXTENTION_FILE_THREAD]);
EXIT_4:
	CloseHandle(Thread_handle_array[FIRST_5_CHARS_THREAD]);
EXIT_3:
	CloseHandle(Thread_handle_array[SIZE_FILE_THREAD]);
EXIT_2:
	CloseHandle(Thread_handle_array[TIME_THREAD]);
EXIT_1:
	fclose(output_file);
	free(output_file_path);
	free(address_for_name_of_tested_file);
	free(address_for_temp_name_of_tested_file);
	CloseHandle(g_file_handle);
	exit(exit_code);
}




