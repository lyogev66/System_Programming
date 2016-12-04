#include "thread_functions.h"
HANDLE g_file_handle;

double round_to_2_digit(double file_size)
{
	file_size=(file_size*100)+0.5;
	file_size=floor(file_size);
	return file_size/100;
}

void CheckForThreadError(DWORD ec)
{
	switch (ec)
	{
	case EXIT_CODE_TIME_THREAD:
		exit(EXIT_CODE_TIME_THREAD);
		break;
	case EXIT_CODE_SIZE_FILE_THREAD :
		exit(EXIT_CODE_SIZE_FILE_THREAD);
		break;
	case EXIT_CODE_FIRST_5_CHARS_THREAD:
		exit(EXIT_CODE_FIRST_5_CHARS_THREAD);
		break;
	case EXIT_CODE_EXTENTION_FILE_THREAD:
		exit(EXIT_CODE_EXTENTION_FILE_THREAD);
		break;
	case EXIT_CODE_OK:
		break;
	default:
		printf("AN ERROR %d ACCURED \n", ec);
		exit(ec);
	}
}

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

	ExitThread(EXIT_CODE_OK);
FAIL:
	ExitThread (EXIT_CODE_TIME_THREAD);
}

void test_file_size(SIZE_INFORMATION *si)
{
	DWORD dword_file_size = 0;
	char unit_index = 0;
	Sleep(10);
	dword_file_size= GetFileSize(g_file_handle,NULL);
	if(si->file_size == INVALID_FILE_SIZE)
		ExitThread( EXIT_CODE_SIZE_FILE_THREAD);
	si->file_size=(double)dword_file_size;
	while(si->file_size > SIZE_QUANT)
	{
		si->file_size /= SIZE_QUANT;
		unit_index ++ ;
	}
	si->file_size=round_to_2_digit(si->file_size);
	si->units =(FILE_UNITS) unit_index;
	ExitThread( EXIT_CODE_OK);
}

void test_file_contant(char* ff)
{
	BOOL success = TRUE;
	DWORD get_error, NumRead;
	Sleep(10);
	success = ReadFile(g_file_handle,ff,NumberOfBytesToRead,&NumRead,NULL);
	ff[NumberOfBytesToRead] = '\0';
	if (!success)
	{
		printf("last error %d\n",GetLastError());
		ExitThread( EXIT_CODE_FIRST_5_CHARS_THREAD);
	}
	ExitThread( EXIT_CODE_OK);
}

void test_file_extension(char* str)
{
	char *str_tmp = NULL;
	Sleep(10);
	if(str == NULL)
	{
		printf("FILE NAME DOESNT EXISTS\n");
		ExitThread( EXIT_CODE_EXTENTION_FILE_THREAD);
	}

	str_tmp = (char*)memchr(str, '.', strlen(str));
	str_tmp = str_tmp + 1;
	strcpy(str,str_tmp);

	ExitThread( EXIT_CODE_OK);
}

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

int main(int argc ,char * argv[])
{
	int index;
	FILE_TIMES ft;
	STATUS_DATA sd; 
	SIZE_INFORMATION si;
	FILE *output_file = NULL;
	HANDLE Thread_handle_array[THREAD_NUMBER];
	size_t input_string_len,output_file_name_size;
	char FirstFive[NumberOfBytesToRead + 1] = {0},* str_tmp = NULL ;
	DWORD file_size, ThreadID[THREAD_NUMBER], exitCodesArr[THREAD_NUMBER] = {0};
	char *temp_name_of_tested_file = NULL,*address_for_temp_name_of_tested_file = NULL;
	char *name_of_tested_file = NULL ,*address_for_name_of_tested_file = NULL ,*output_file_path = NULL;

	if (argc < NUMBER_OF_ARGUMENTS)
		return ERROR;

	input_string_len=strlen(argv[1]);
	name_of_tested_file=(char*) malloc(sizeof(char)*input_string_len+1);
	if (name_of_tested_file==NULL)
		return ERROR;


	address_for_name_of_tested_file = name_of_tested_file;
	name_of_tested_file = strcpy(name_of_tested_file,argv[1]);

	temp_name_of_tested_file = (char*) malloc(sizeof(char)*input_string_len+ strlen("_log") +1);//1 for \0 and +4 for _log
	if (temp_name_of_tested_file==NULL)
		return ERROR;
	address_for_temp_name_of_tested_file = temp_name_of_tested_file;
	temp_name_of_tested_file = strcpy(temp_name_of_tested_file,name_of_tested_file);
	temp_name_of_tested_file = strtok(temp_name_of_tested_file,".");
	temp_name_of_tested_file = strcat(temp_name_of_tested_file,"_log.txt");

	output_file_name_size = strlen(argv[2])+ strlen ("\\") + input_string_len+strlen("_log");
	output_file_path=(char*) malloc(sizeof(char)*output_file_name_size + 1);
	if (output_file_path==NULL)
		return ERROR;
	output_file_path[0] = 0;
	output_file_path = strcpy(output_file_path,argv[2]);
	output_file_path = strcat(output_file_path,"\\");
	output_file_path = strcat(output_file_path,temp_name_of_tested_file);

	output_file=fopen(output_file_path,"w");
	if (output_file==NULL)
		return ERROR;


	memset(&sd,0,sizeof(sd));
	g_file_handle = CreateFile(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY ,NULL);
	 
	if(g_file_handle == INVALID_HANDLE_VALUE)
		exit(MY_ERROR);

	Thread_handle_array[TIME_THREAD]			= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_get_times,&ft,0,&ThreadID[TIME_THREAD]);
	Thread_handle_array[SIZE_FILE_THREAD]		= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_size,&si,0,&ThreadID[SIZE_FILE_THREAD]);
	Thread_handle_array[FIRST_5_CHARS_THREAD]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_contant,FirstFive,0,&ThreadID[FIRST_5_CHARS_THREAD]);
	Thread_handle_array[EXTENTION_FILE_THREAD]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)test_file_extension,name_of_tested_file,0,&ThreadID[EXTENTION_FILE_THREAD]);
	WaitForMultipleObjects(THREAD_NUMBER,Thread_handle_array,TRUE,INFINITE);
	
	while(sd.c_ts != ALL_THREAD_FINNISHED)
	{

		for(index = 0; index <THREAD_NUMBER; index++)
		{
			if(GetExitCodeThread(Thread_handle_array[index],&exitCodesArr[index]) == MY_ERROR)
				exit(MY_ERROR);
			if(STILL_ACTIVE  != exitCodesArr[index])
			{
				CheckForThreadError(exitCodesArr[index]);
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
	fprintf(output_file,"The file extension of the test file is \".%s\"\n",name_of_tested_file);
	fprintf(output_file,"The test file size is %.2f%s\n",si.file_size,size_enum_to_string(si.units));
	fprintf(output_file,"The file was created on %02d/%02d/%d %02d:%02d:%02d\n",
								ft.UTC_Israel_CreationTime.wDay,
								ft.UTC_Israel_CreationTime.wMonth,
								ft.UTC_Israel_CreationTime.wYear,
								ft.UTC_Israel_CreationTime.wHour,
								ft.UTC_Israel_CreationTime.wMinute,
								ft.UTC_Israel_CreationTime.wSecond );	

	fprintf(output_file,"The file was last modified on %02d/%02d/%d %02d:%02d:%02d\n",
								ft.UTC_Isreal_lastmodified.wDay,
								ft.UTC_Isreal_lastmodified.wMonth,
								ft.UTC_Isreal_lastmodified.wYear,
								ft.UTC_Isreal_lastmodified.wHour,
								ft.UTC_Isreal_lastmodified.wMinute,
								ft.UTC_Isreal_lastmodified.wSecond);

	fprintf(output_file,"The file's first 5 bytes are:%s\n",FirstFive);


	CloseHandle(Thread_handle_array[TIME_THREAD]);
	CloseHandle(Thread_handle_array[SIZE_FILE_THREAD]);
	CloseHandle(Thread_handle_array[FIRST_5_CHARS_THREAD]);
	CloseHandle(Thread_handle_array[EXTENTION_FILE_THREAD]);

	fclose(output_file);
	free(output_file_path);
	free(address_for_name_of_tested_file);
	free(address_for_temp_name_of_tested_file);
	CloseHandle(g_file_handle);
	return EXIT_CODE_OK;
}




