#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACK_COUNT 196

int main(int argc, char *argv[])
{
	char * file_in_name = NULL;
	char * file_out_name = NULL;
	int file_name_len = 0;
	unsigned __int32 count = 0; 
	unsigned __int32 value = 0;
	unsigned __int32 file_count = 0;
	int i=0;
	FILE * fp_in;
	FILE * fp_out;

	if(argc != 2)
	{
		printf("<file name>\n");
		return 0;
	}
	
	//input file
	file_in_name = argv[1];
		
	//output file
	file_out_name = malloc(strlen(argv[1]) + 10);
	memset(file_out_name, '\0', strlen(argv[1]) + 10);
	strcpy(file_out_name, argv[1]);
	file_name_len = strlen(file_out_name);
	for(i=file_name_len-1; i>=0; i--)
	{
		if(file_out_name[i] == '.')
		{
			break;
		}	
	} 
	if(i>=0)
	{
		strcpy(file_out_name+i, ".ota.bin");		
	}
	
	printf("Input  file name: %s\n", file_in_name);
	printf("Output file name: %s\n", file_out_name);
	
	fp_in = fopen(file_in_name, "rb");
	if(fp_in == NULL)
	{	
		printf("open file %s error!\n", file_in_name);
		return 0;
	}

	int ch;
	unsigned __int8 ch1; 
	value = 0;
	i = 0;
	file_count = 0;
	while(EOF != (ch = fgetc(fp_in)))
	{
		ch1 = ch; 
		file_count ++;
		
		value <<= 8*i;
		value |= ch1;	
		
		i++; 
		if(i >= 4)
		{
			i=0;
			
			count += value;
			value = 0;
		}
	}
	fclose(fp_in);

	printf("file count = %d\n", file_count);
	printf("file EO check value = %.8X\n", count);

	fp_in = fopen(file_in_name, "rb");			
	if(fp_in == NULL)
	{	
		printf("open file %s error!\n", file_in_name);
		return 0;
	}

	fp_out = fopen(file_out_name, "wb");
	if(fp_out == NULL)
	{	
		printf("open file %s error!\n", file_out_name);
		return 0;
	}
	
	//write odd-even check
	fwrite(&count, 4, 1, fp_out);	
	
	//write file count
	fwrite(&file_count, 4, 1, fp_out);
	
	//write other
	for(i=0; i<PACK_COUNT-8; i++)
	{
		fputc(0x00, fp_out);	
	}
	
	//write file value
	while(EOF != (ch = fgetc(fp_in)))
	{
		fputc(ch, fp_out);
	}
	
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}
