#include "TestManager.h"

Process_list *g_Live_process_head = NULL;
Process_list *g_Dead_process_head = NULL;
int Processcount = 0;

void pad_argument(char *str)
{
	int length;
	char *padded_str =NULL;

	length = strlen(str);

	padded_str = (char*) calloc(length+3*CHARACTER_SIZE,CHARACTER_SIZE);
	if(padded_str ==  NULL)
		exit(0);

	strcat(padded_str," ");
	strcat(padded_str,str);

	str = (char*)realloc(str,strlen(padded_str) +1);
	if(str ==NULL)
	{
		printf("reallocating str failed\n");
		exit(0);
	}

	memset(str,0,strlen(padded_str));
	strcpy(str,padded_str);
	free(padded_str);
}

char* Create_Process_Argument(char *target_process_name,char *file_name,char *output_directory_name)
{
	char* ret =NULL ,*argument_command=NULL;
	argument_command = (char*) calloc(strlen(output_directory_name)+ strlen(file_name)+ strlen(target_process_name) + CHARACTER_SIZE,CHARACTER_SIZE);
	if(argument_command == NULL)
	{
		printf("ERROR at argument_command \n");
		exit(0);
	}
	ret = strcat(argument_command,target_process_name);
	if (ret == NULL)
		goto FAIL;
	ret =strcat(argument_command,file_name);
	if (ret == NULL)
		goto FAIL;
	ret = strcat(argument_command,output_directory_name);
	if (ret == NULL)
		goto FAIL;
	return argument_command;

FAIL:
	printf("Create_Process_Argument failed\n");
	free(argument_command);
	exit(0);
}

char* create_output_folder_path(char *path,int flag)
{
	char* tmp=NULL, *ret=NULL;
	if(!flag)
	{
		tmp = (char*) calloc(strlen(path) + 3,CHARACTER_SIZE);
		if(tmp == NULL)
			exit(ERROR_CODE);
		strcat(tmp,"./");
		strcat(tmp,path);
		return tmp;
	}
	else
	{
		tmp = (char*) calloc (strlen(path)+ strlen("runtime_logfile.txt") + 4,CHARACTER_SIZE);
		strcat(tmp,"./");
		strcat(tmp,path);
		strcat(tmp,"/");
		strcat(tmp,"runtime_logfile.txt");
	}
	return tmp;
}

void process_lists_management()
{
	Process_list *live_tmp = g_Live_process_head , *dead_tmp = g_Dead_process_head, *tmp=NULL;
	DWORD	exitcode,waitcode;

	while(live_tmp != NULL)
	{
		waitcode = WaitForSingleObject(live_tmp->procinfo.hProcess, 0);

		if(waitcode == WAIT_OBJECT_0) 
		{	
			GetExitCodeProcess(live_tmp->procinfo.hProcess,&(live_tmp->exitcode));
			tmp = pop_node_from_list(g_Live_process_head,live_tmp->procinfo.dwProcessId);
			tmp->next =NULL;
			g_Dead_process_head = push_element(g_Dead_process_head,tmp);
			Processcount--;
			if(g_Live_process_head !=NULL)
				live_tmp = g_Live_process_head;	
			else
				break;
		}
		live_tmp = live_tmp->next;
	}
}

BOOL InitiateProcess(LPTSTR CommandLine,FILE *fp,char* file_name)
{
	PROCESS_INFORMATION procinfo ;
	STARTUPINFO    startinfo = { sizeof(STARTUPINFO), NULL, 0 }; 
	Process_list *node = NULL;
	DWORD               waitcode;

	if(CreateProcess(NULL,		/*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		&procinfo				/*  Pointer to PROCESS_INFORMATION structure. */
		))
	{
		fprintf(fp,"Successfully created a process with ID %d to execute %s\n",procinfo.dwProcessId,file_name);
		node = creat_node(procinfo,file_name);
		g_Live_process_head = push_element(g_Live_process_head,node);
		Processcount++;
		return OK;
	}
	else
	{
		fprintf(fp,"!!! Failed to create new process to run %s. Error code: %d !!!\n",CommandLine,GetLastError());
		exit(0);
	}

}

void free_processes()
{
	Process_list *tmp = g_Dead_process_head;
	while(tmp!=NULL)
	{
		CloseHandle(tmp->procinfo.hProcess);
		CloseHandle(tmp->procinfo.hThread);
		tmp = tmp->next;
	}
}

int main(int argc,char* argv[])
{
	char ch ,BOOL= TRUE;
	FILE *instream, *outstream;
	char *file_name = NULL,*output_directory_name = NULL,target_process_name[] =TARGET_NAME,*complete_arguments_command =NULL,*output_dir =NULL;
	int length=0;
	output_directory_name = (char*) malloc(strlen(argv[2]) + CHARACTER_SIZE);
	if(output_directory_name == NULL)
		exit(0);
	strcpy(output_directory_name,argv[2]);
	output_dir = create_output_folder_path(argv[2],CREATE_DIRECTORY_FLAG);

	if(!CreateDirectory(output_dir,NULL))
	{
		printf("%s %s \n",argv[2], GetLastError()==ERROR_ALREADY_EXISTS? "ERROR_ALREADY_EXISTS":"ERROR_PATH_NOT_FOUND");
		exit(0);
	}
	free(output_dir);
	output_dir = NULL;

	output_dir = create_output_folder_path(argv[2],CREATE_FILE_INSIDE_DIRECTORY_PATH);
	if(fopen_s(&outstream,output_dir,"w+")!= OK)
		exit(0);

	if(fopen_s(&instream,argv[1],"r+") != OK)
		exit(0);

	pad_argument(output_directory_name);


	file_name = (char*)calloc(CHARACTER_SIZE,CHARACTER_SIZE);
	if(NULL==file_name)
	{
		printf("Error allocating string for file name \n");
		exit(ERROR_CODE);
	}


	while (1)
	{
		ch = getc(instream);
		if ((ch != '\r') && (ch!='\n') && (ch !='\0') && (ch!=EOF))
		{
			length++;
			file_name = (char*)realloc(file_name, length + CHARACTER_SIZE);
			if(NULL == file_name)
			{
				printf("Error parsing file name \n");
				exit(ERROR_CODE);
			}
			file_name[length - CHARACTER_SIZE] = ch;
		}
		else
		{
			if(0 == length)
			{
				printf("Empty file!\n");
				exit(ERROR_CODE);
			}
			file_name[length] = '\0';
			pad_argument(file_name);

			complete_arguments_command = Create_Process_Argument(target_process_name,file_name,output_directory_name);
			InitiateProcess(complete_arguments_command,outstream,file_name);
			free(complete_arguments_command);
			length = 0;
			if(EOF == ch)
			{
				BOOL = FALSE;
				break;
			}
		}
	}

	while(Processcount)
	{
		process_lists_management();
		print_live_process_list(g_Live_process_head,outstream);

		print_dead_process_list(g_Dead_process_head,outstream);

		if(g_Live_process_head == NULL)
			goto EXIT_PROGRAM;
		
		Sleep(atoi(argv[3]));
	}


EXIT_PROGRAM:

	printf("exiting program\n");
	fprintf(outstream,"All the processes have finished running. Exiting program.");
	free_processes();
	free_list(g_Live_process_head,"g_Live_process_head");
	free_list(g_Dead_process_head,"g_Dead_process_head");
	free(output_directory_name);
	fclose(instream);
	fclose(outstream);
	free(output_dir);
	free(file_name);

}


