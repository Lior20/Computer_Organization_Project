# include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE 300 // The maximum line size of the input file is 300.
#define MAX_LABEL 50 // the maximum lable size is 50.
#define MAX_MEMO_LINE 4096


int PC = 0;

int main(int argc, char* argv[]) {
	FILE* memin = NULL;
	FILE* memout = NULL;
	FILE* regout = NULL;
	char line[MAX_LINE];
	
	memin = fopen(argv[1], "r");
	if (memin == NULL) {
		printf("Error: Cannot open file\n");
		return 1;
	} // check if the file is open.
	memout = fopen(argv[2], "w"); 
	if (memout == NULL) { 
		printf("Error: Cannot open file\n");
		return 1;
	} // check if memout created.
	
	while (!foef(memin)) {
		// read
	}
	else
		
}