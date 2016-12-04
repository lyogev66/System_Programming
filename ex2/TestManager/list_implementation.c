/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
Author= Ziv Belahsan 201567278 Yogev laks=200344729
Project=Exercise 2
Using -			list_implemetation.h	 
	
Description -	handels the list implementation part of the project
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
#include "list_implemetation.h"

extern Process_list *g_Live_process_head;
extern Process_list *g_Dead_process_head;
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		push_element
Input arguments:	Process_list* head - the head of the list
					Process_list* cell - a cell to be added to the list

return:				Process_list * -a pointer to the process list

Description-		pushes an elemnt to the process list so it will be sorted by process id
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		SubtractFiletimes
Input arguments:	FILETIME Late	- a struct which contains the value of the late time
					FILETIME Early	- a struct which contains the value of the early time

return:				FILETIME - a filetime object 

Description-		substracts the value of the early filetime struct from the late
					struct according to needed fields
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		print_live_process_list
Input arguments:	Process_list* head	- a pointer to the list head
					FILE* fp			- a file object to be printed to

return:				None

Description-		prints the process list of the running processes into a file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		print_dead_process_list
Input arguments:	Process_list* head	- a pointer to the list head
					FILE* fp			- a file object to be printed to

return:				None

Description-		prints the process list of the finished processes into a file
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		creat_node
Input arguments:	PROCESS_INFORMATION procinfo	- a process information struct to be added to the node
					char* command_file_name			-the command line argument to be added to the node  

return:				Process_list * - a pointer to a process list

Description-		creates a new node of type process cell and adds it to the list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		pop_node_from_list
Input arguments:	Process_list* head	- a pointer to the head of the list 
					DWORD process_id	- the process id to be poped from the list

return:				Process_list * - a pointer to a process list

Description-		removes a process from the process list according to the process_id
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		print_list
Input arguments:	Process_list* head	- a pointer to the head of the list 
					char* str			-	???????????????

return:				None

Description-		prints the list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
function name :		print_list
Input arguments:	Process_list* head	- a pointer to the head of the list 
					char* str			-	???????????????

return:				None

Description-		frees the list
oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
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