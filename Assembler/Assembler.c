# include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE 300 // The maximum line size of the input file is 300.
#define MAX_LABEL 50 // the maximum lable size is 50.

typedef struct 
{
	int opcode;
	int rd;
	int rs;
	int rt;
}R_Format;

typedef struct
{
	int opcode;
	int rd;
	int rs;
	int rt;
	int imm;
}J_Format;





int main(int argc, char* argv[])
{
	
}
