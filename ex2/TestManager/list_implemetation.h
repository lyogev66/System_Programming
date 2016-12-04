#ifndef LIST_IMPLEMENTATION_H
#define LIST_IMPLEMENTATION_H

#include "TestManager.h"

typedef struct process_list
{
	PROCESS_INFORMATION procinfo;
	char* command;
	DWORD  exitcode;
	struct process_list *next;
}Process_list;


Process_list *creat_node(PROCESS_INFORMATION procinfo,char* command_file_name);
Process_list * push_element(Process_list* head,Process_list* cell);
void print_live_process_list(Process_list* head,FILE* fp);
Process_list* pop_node_from_list(Process_list* head, DWORD process_id);
void print_dead_process_list(Process_list* head,FILE* fp);
void print_live_process_list(Process_list* head,FILE* fp);
void print_list(Process_list *head,char* str);
void free_list(Process_list *head,char* str);



#endif