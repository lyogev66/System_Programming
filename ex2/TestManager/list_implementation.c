#include "list_implemetation.h"

extern Process_list *g_Live_process_head;
extern Process_list *g_Dead_process_head;

Process_list* push_element(Process_list* head,Process_list* cell)
{
	Process_list *current = head, *previous=NULL;
	cell ->next=NULL;
	if(head == NULL)// handels empty list
	{
		return cell;
	}
	if(current->procinfo.dwProcessId >= cell->procinfo.dwProcessId)//handle situation that the first node at the list allready has lower process_id then the newely entered cell
	{
		cell->next = current;
		return cell;
	}
	previous = current;
	current = current->next;

	while(current != NULL && current->procinfo.dwProcessId <= cell->procinfo.dwProcessId)
	{
		previous = current;
		current = current->next;
	}

	cell ->next = current;
	previous->next = cell;

	return head;
}

FILETIME SubtractFiletimes(FILETIME Late, FILETIME Early)
{
	typedef unsigned __int64 Unsigned64BitType;
	Unsigned64BitType Late64BitVal;
	Unsigned64BitType Early64BitVal;
	Unsigned64BitType Difference64BitVal;
	FILETIME DifferenceAsFILETIME;
	const Unsigned64BitType Low32BitsMask = 0x00000000FFFFFFFF;

	Late64BitVal = ((Unsigned64BitType)(Late.dwHighDateTime) << 32) +
		Late.dwLowDateTime;
	Early64BitVal = ((Unsigned64BitType)(Early.dwHighDateTime) << 32) +
		Early.dwLowDateTime;
	Difference64BitVal = Late64BitVal - Early64BitVal;

	DifferenceAsFILETIME.dwLowDateTime =
		(DWORD)(Difference64BitVal & Low32BitsMask);
	DifferenceAsFILETIME.dwHighDateTime =
		(DWORD)(Difference64BitVal >> 32);

	return DifferenceAsFILETIME;
}

void print_live_process_list(Process_list* head,FILE* fp)
{
	Process_list* tmp = head;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime,Sys_time, TotalTime;
	SYSTEMTIME UTC_start_time, UTC_total_time,LocalTime;
	if(g_Live_process_head == NULL)
		return;

	fprintf(fp,"List of running processes:\n");
	while (tmp)
	{
		GetProcessTimes(tmp->procinfo.hProcess,&CreationTime,&ExitTime,&KernelTime,&UserTime);
		FileTimeToSystemTime(&CreationTime,&UTC_start_time);

		SystemTimeToTzSpecificLocalTime(NULL, &UTC_start_time, &LocalTime);

		GetSystemTimeAsFileTime (&Sys_time);
		TotalTime = SubtractFiletimes(Sys_time,CreationTime);
		FileTimeToSystemTime(&TotalTime,&UTC_total_time); 
		fprintf(fp,"Process %d running command %s for %d second %d milliseconds\n",	tmp->procinfo.dwProcessId,
			tmp->command,
			UTC_total_time.wSecond,	
			UTC_total_time.wMilliseconds);

		tmp = tmp->next;
	}
}

void print_dead_process_list(Process_list* head,FILE* fp)
{
	Process_list* tmp = head;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime, TotalTime;
	SYSTEMTIME UTC_start_time,UTC_exit_time,UTC_total_time;
	if(g_Dead_process_head == NULL)
		return;
	fprintf(fp,"List of finished processes:\n");
	while (tmp)
	{
		GetProcessTimes(tmp->procinfo.hProcess,&CreationTime,&ExitTime,&KernelTime,&UserTime);
		FileTimeToSystemTime(&CreationTime,&UTC_start_time);
		FileTimeToSystemTime(&ExitTime,&UTC_exit_time);
		TotalTime = SubtractFiletimes(ExitTime,CreationTime);
		FileTimeToSystemTime(&TotalTime,&UTC_total_time);

		//Process 1111 ran command ReadMe.txt and exited with exit code 0x0 after 2
//seconds and 374 milliseconds
		fprintf(fp,"Process %d ran command %s and exited with exit code 0x%x after %d second %d milliseconds\n",	tmp->procinfo.dwProcessId,
			tmp->command,
			tmp->exitcode,
			UTC_total_time.wSecond,
			UTC_total_time.wMilliseconds);
		tmp = tmp->next;
	}
}

Process_list *creat_node(PROCESS_INFORMATION procinfo,char* command_file_name)
{
	Process_list * node;
	node = (Process_list*)malloc(sizeof(Process_list));
	if (node == NULL)
	{
		printf("Failed allocating new Process_list node \n");
		exit(1);
	}
	else
	{
		node->exitcode = 0; 
		node ->next = NULL;
		node->command = (char*) calloc(strlen(command_file_name) + CHARACTER_SIZE, CHARACTER_SIZE);
		if(node->command == NULL)
			exit(1);

		strcpy(node->command,command_file_name);

		if(memcpy((void*)&(node->procinfo), (void*)&procinfo,sizeof(procinfo))==NULL)
			goto FAIL;
	}
	return node;
FAIL:
	printf("ERROR in creat_node \n");
	//my_free(node->command,"node->command");
	free(node->command);
	exit(0);
}

Process_list* pop_node_from_list(Process_list* head, DWORD process_id)
{
	Process_list *previus =NULL;
	Process_list *current = head;
	Process_list *tmp = NULL;

	if(current->procinfo.dwProcessId == process_id)
	{
		tmp =current;
		g_Live_process_head = current->next;
		tmp->next =NULL;
		return tmp;
	}
	while(current)
	{
		if(current->procinfo.dwProcessId == process_id)
		{
			previus->next = current->next;
			current->next =NULL;
			return current;
		}
		previus = current;
		current = current->next;
	}
	return current;
}

void print_list(Process_list *head,char* str)
{
	Process_list *tmp = head;
	if(head ==NULL)
		return;
	while(tmp)
	{
		printf("%p with %d\n",tmp,tmp->procinfo.dwProcessId);
		tmp = tmp->next;
	}
	printf("END OF LIST\n");
}

void free_list(Process_list *head,char* str)
{
	Process_list *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		//my_free(tmp->command,"tmp command");
		free(tmp->command);
		//my_free(tmp,"my_free");
		free(tmp);
	}
}