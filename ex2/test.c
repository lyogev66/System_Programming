#include <stdio.h>
#include <Windows.h>

void main(int argc, char*argv[])
{
	FILE* file;
	file=fopen("testfile.txt", "w");

	printf("test\n");
	fprintf(file,"hello world");

	Sleep(10000);
}
