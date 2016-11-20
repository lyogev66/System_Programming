#include <stdio.h>
#include<Windows.h>
#include<tchar.h>

typedef struct node_cell
{
	int process_id;
	struct node_cell* next;
}node_cell;


node_cell* push_element(node_cell* head,node_cell* cell)
{
	node_cell* tmp = head;
	if(head ==NULL)
		return cell;
	while(tmp->process_id >= cell->process_id && tmp !=NULL)
		tmp = tmp->next;

	if(tmp != NULL)
		cell ->next = tmp->next;

	tmp->next = cell;
	return head;
}

void print_list(node_cell* head)
{
	node_cell* tmp = head;
	while (tmp !=NULL)
	{
		printf("process_id : %d\n",tmp->process_id);
		tmp = tmp->next;
	}
	printf("up to here \n");
}

node_cell *creat_node(int process_id)
{
	node_cell * ptr;
	ptr=(node_cell*)malloc(sizeof(node_cell));
	if (ptr == NULL)
		exit(1);
	else
		ptr ->next = NULL;
		ptr->process_id=process_id;
		return ptr;

}
void main()
{
	node_cell *first=NULL,*secound=NULL,*head = NULL;
	first=creat_node(5);
	secound=creat_node(6);
	head = push_element(head,first);
	head = push_element(head,secound);
	print_list(head);
}

//	PROCESS_INFORMATION procinfo,procinfo2;
//	char *str1="proj2.exe 3"
//	TCHAR str[] =_T(str1);
//
//	//wchar_t wtext[20];
//	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
//	//LPWSTR ptr ;
////	mbstowcs(wtext, text, strlen(text)+1);//Plus null
//
//	printf("manager\n");
//	CreateProcess(NULL,
//		str,
//		NULL,
//		NULL,
//		FALSE,
//		NORMAL_PRIORITY_CLASS,
//		NULL,
//		NULL,
//		&startinfo,
//		&procinfo);
//
//
//		CreateProcess(NULL,
//		str,
//		NULL,
//		NULL,
//		FALSE,
//		NORMAL_PRIORITY_CLASS,
//		NULL,
//		NULL,
//		&startinfo,
//		&procinfo2);
//
//	printf("procinfo.dwProcessId %d \n",procinfo.dwProcessId);
//	printf("procinfo2.dwProcessId %d \n",procinfo2.dwProcessId);
//	printf("procinfo.dwThreadId %d \n",procinfo.dwThreadId);
//	printf("procinfo2.dwThreadId %d \n",procinfo2.dwThreadId);
//
//
//	printf("string is %c \n",str);
//	//printf("procinfo2.dwThreadId %s \n",procinfo2.hProcess);
//

