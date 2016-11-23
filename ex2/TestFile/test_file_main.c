#include <stdio.h>//for FILE object

#include <windows.h> //for thread and Sleep

void test_file_extension(char *inputfile_name,FILE *output_file)
{
	char *file_extension;
	Sleep(10);
	file_extension=strtok(inputfile_name,".");
	if(file_extension==NULL)
		//wrong input//
		return;
	file_extension=strtok(NULL,".");
	fprintf(output_file,"the file extension of the test file is \".%s\"\n",file_extension);
		return;



}

void test_file_size(FILE *input_file,FILE *output_file)
{
	Sleep(10);
	//fprintf(output_file,"the file size is %s\n",file_size);

}

void test_file_timing(FILE *input_file,FILE *output_file)
{
	Sleep(10);
	//fprintf(output_file,"the file was created on   foramt it hereeeeeeeee %s\n",file_created);
	//fprintf(output_file,"the file was last modified foramt it hereeeeeeeee %s\n",file_last_modified);
}

void test_file_contant(FILE *input_file,FILE *output_file)
{

	int i;
	char byte;
	Sleep(10);

	fprintf(output_file,"the file first 5 bytes are:");
	for( i=0;i<5;i++)
	{
		byte=getc(input_file);
		if (byte==EOF)
		{
			fprintf(output_file,"\n");
			return;
		}

		else
			fprintf(output_file,"%c",byte);
	}
	fprintf(output_file,"\n");
}


void main()//int argc ,char * argv[])
{
	//char *name_of_tested_file;
	//char *output_file_path;
	//	name_of_tested_file=argv[1];
	//	output_file_path=argv[2];
	char inputfile_name[]="test1.txt";
	//char outputfilepath[]="C:\Users\yogev\Documents\Visual Studio 2010\Projects\system_programming\ex2\TestFile";
	char outputfile_name[]="test1_log.txt";
	FILE *input_file;
	FILE *output_file;


	input_file=fopen(inputfile_name,"r");
	output_file=fopen(outputfile_name,"w");



	test_file_extension(inputfile_name,output_file);
	test_file_size(input_file,output_file);
	test_file_timing(input_file,output_file);
	test_file_contant(input_file,output_file);


	fclose(output_file);
	fclose(input_file);





}