#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define  TRUE 1;
#define  FALSE 0;
#define  MAX_LINE_LEN 301// The maximum line size of the input file is 300.
#define  MAX_LABEL_LEN 51 // the maximum lable size is 50.
#define  MAX_MEMO_LINES 4096
#define  DATA_MEMO_START 2048
#define  NUM_OF_REG 16
#define  MAX_VAR_IN_INST 7 

/*****************************************************************
						Globals
*****************************************************************/
enum OPCODE {
	add, sub, mul, and, or , xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, halt,
};

enum REGISTER {
	$zero, $imm, $v0, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $s0, $s1, $s2, $gp, $sp, $ra,
};

typedef struct Rfromat
{
	char op; // 8 bits
	char rd; // 4 bits
	char rs; // 4 bits
	char rt; // 4 bits
}Rfromat;

typedef struct Ifromat
{
	char op; // 8 bits
	char rd; // 4 bits
	char rs; // 4 bits
	char rt; // 4 bits
	int imm; // 20 bits.
}Ifromat;


typedef struct word
{
	int address;
	int data;
}word;

typedef struct label // a struct to organize labels informanrion
{
	// name of the label
	char name[MAX_LABEL_LEN];
	// val the immediate value related
	int val;
} label;

// an array to store the labels in order to use then in the 2nd run.
label labels[MAX_MEMO_LINES];
int label_counter = 0; // number of labels

// we need to store also the word memory - .word
int words[MAX_MEMO_LINES / 2] = {0};
int word_addrs_count = 0;
int max_word_address = 0;
uint16_t PC = 0; // cant be nagative maximum 12 bits


/*****************************************************************
						help function declarations
*****************************************************************/
void remove_white_spaces(char *str); // remove " " , "\t", "\n" 
void remove_tabs(char* str);
Rfromat *read_instraction_R(char *line); //set the R struct according to the line instruction 
Ifromat *read_instraction_I(char *line); //set the I struct according to the line instruction 
int is_instraction(char *line); //return True if the line is an instruction and false otherwise. 
int is_I_instraction(char *line); // return True if the line is an I type instruction and false otherwise. 
char GetOpcode(char *op); // translate the opcode to a number 
char GetRegister(char *reg); // translate the register to a number
int read_label_imm(char *str); // translate the label to an immediate 
void print_inst_I(FILE *machine, Ifromat *I); //print an I type instruction to memin.txt 
void print_inst_R(FILE *machine, Rfromat *R); //print an R type instruction to memin.txt 
word *read_instraction_word(char *line); //set the word struct according to the line instruction
void print_inst_words(FILE *machine, word *words); // print the value to memin.txt the data part. 





/*****************************************************************
						first run
*****************************************************************/

// fill the lable array so we can use them
void firstRun(FILE *assembly)
{
	char line[MAX_LINE_LEN]; // a place to store the line  
	char* label_name;
	int i = 0;

	while (!feof(assembly))
	{
		fgets(line, MAX_LINE_LEN, assembly); // reads a line from assembly
		remove_white_spaces(line); // removes spaces and tabes.
		if (strcmp(line, "\n") == 0) continue;  // empty line
		if (line[0] == '#') continue; // all row is comment
		if (strstr(line, ":") != NULL)  // If there are ':' in the line which means it contains a lable 
		{
			if (strstr(line, "#") != NULL)
			{   // chacks if the ':' are in a comment
				if ((strstr(line, ":")) > (strstr(line, "#"))) continue;
			}

			
			label_name = strtok(line, ":"); // sparates the label name before the ":"
			if (label_name != NULL)
			{
				labels[label_counter].val = PC;
				strcpy(labels[label_counter].name , label_name);
				label_counter++;
			}

		}
		
		// if the line has "$" its an instraction if its is not after a "#"
		if (is_instraction(line))
			PC++;
		if (is_I_instraction(line))
			PC++;
	}
}

/*****************************************************************
						second run
*****************************************************************/
void second_run(FILE* assembly, FILE *machine)
{
	int RowInd = 0;
	int R_format = FALSE;
	char line[MAX_LINE_LEN]; // a place to store the line
	// char op[6];
	// char reg[6];
	// int opcode;
	Rfromat *Rinst;
	Ifromat *Iinst;
	word *w;
	//inst instraction;
	// int j;
	while (!feof(assembly))
	{
		fgets(line, MAX_LINE_LEN, assembly); // reads a line from assembly
		

		if (strcmp(line, "\n") == 0) continue;  // empty line
		if (line[0] == '#') continue; // all row is comment
		
		if (strstr(line, ".word") != NULL)
		{
			if ((strstr(line, "#") == NULL) ||
				(strstr(line, "#") != NULL) && (strstr(line, ".word") < (strstr(line, "#"))))
			{

				w = read_instraction_word(line);
				words[w->address] = w->data;
				if (w->address > max_word_address)
				{
					max_word_address = w->address;			// update max address
				}
				continue;
			}
		}

		else if (is_instraction(line))  // a label will be skipped 
		{
			remove_white_spaces(line);  // removes spaces and tabes.
			
			if (strstr(line, "$imm") != NULL) // Iformat
			{
				Iinst = read_instraction_I(line); // ignore the comment and read the instraction into a Iformat struct
				print_inst_I(machine, Iinst);
			}
			else //Rformat
			{
				Rinst = read_instraction_R(line); // ignore the comment and read the instraction into a Rformat struct
				print_inst_R(machine, Rinst);
			}			
		}

	}
	int line_num = PC;
	for (line_num; line_num <= 2048 ; line_num++)
	{
		fprintf(machine, "%05X\n", 0);
		printf("%05X\n", 0);
	}

	print_inst_words(machine, words);
}

/*****************************************************************
						help functions 
*****************************************************************/


void remove_white_spaces(char* str)
{
	char* d = str;
	do
	{
		while (*d == ' ' || *d == '\t' || *d == '\n')
		{
			++d;
		}
	} while (*str++ = *d++);
}

void remove_tabs(char* str)
{
	char* d = str;
	do
	{
		while (*d == '\t' || *d == '\n')
		{
			++d;
		}
	} while (*str++ = *d++);
}

Ifromat *read_instraction_I(char *line)
{
	char op[7];
	char *token;
	int i = 1;
	int j = 0;
	char* inst[MAX_VAR_IN_INST];
	Ifromat *I = (Ifromat*) malloc (sizeof(Ifromat));
	char *label;
	
	char* comment = strchr(line, '#');	// check if contains comment
	if (comment)
	{
		*comment = '\0'; // if the line has a comment, then it's replaced with '\0'
	}

	while (line[j] != '$')
	{
		op[j] = line[j];
		j++;
	}
	op[j] = '\0';

	line = line+j;
	
	inst[0] = op;

	token = strtok(line, ",");
	while (token != NULL)
	{
		inst[i] = token;
		token = strtok(NULL, ",");
		i++;
	}

	I -> op = GetOpcode(inst[0]);
	I -> rd = GetRegister(inst[1]);	
	I -> rs = GetRegister(inst[2]);
	I -> rt = GetRegister(inst[3]); 
	I -> imm = read_label_imm(inst[4]);
	return I;
}


Rfromat *read_instraction_R(char *line)
{
	char op[7];
	char *token;
	int i = 1;
	int j = 0;
	char* inst[MAX_VAR_IN_INST];
	Rfromat *R = (Rfromat*)malloc(sizeof(Rfromat));
	char *label;

	char* comment = strchr(line, '#');	// check if contains comment
	if (comment)
	{
		*comment = '\0'; // if the line has a comment, then it's replaced with '\0'
	}

	while (line[j] != '$')
	{
		op[j] = line[j];
		j++;
	}
	op[j] = '\0';

	line = line + j;

	inst[0] = op;

	token = strtok(line, ",");
	while (token != NULL)
	{
		inst[i] = token;
		token = strtok(NULL, ",");
		i++;
	}

	R->op = GetOpcode(inst[0]);
	R->rd = GetRegister(inst[1]);
	R->rs = GetRegister(inst[2]);
	R->rt = GetRegister(inst[3]);
	return R;
}

word *read_instraction_word(char *line) 
{
	word *w = (word*)malloc(sizeof(word));
	char *token;
	remove_tabs(line);
	
	token = strtok(line, " "); // first one is .word
	
	token = strtok(NULL, " "); // second is address 
	if (strstr(token, "0x") || strstr(token, "0X")) // the address is hexadecimal
		w -> address =  (int)strtol(token, NULL, 16);
	else // the address is decimal
		w -> address = atoi(token);

	token = strtok(NULL, " "); // 3rd is the value
	if (strstr(token, "0x") || strstr(token, "0X")) // the data is hexadecimal
		w->data = (int)strtol(token, NULL, 16);
	else // the data is decimal
		w->data = atoi(token);
	return w;

}

int is_instraction(char *line)
{
	if ((strchr(line, '$') != NULL))
	{
		if (strstr(line, "#") == NULL)
			return TRUE;
		if ((strstr(line, "$")) < (strstr(line, "#")))
			return TRUE;		
	}
	/*if (strstr(line, ".word") != NULL) 
	{
		if (strstr(line, "#") == NULL)
			return TRUE;
		if ((strstr(line, ".word")) < (strstr(line, "#")))
			return TRUE;///
	}*/
	return FALSE;
}

int is_I_instraction(char *line)
{
	if (!is_instraction(line))
		return FALSE;
	if (strstr(line, "$imm") != NULL)
		return TRUE;
	return FALSE;
}

int read_label_imm(char *str)
{
	int num;
	int neg = FALSE;
	for (int i = 0; i < label_counter; i++)
	{
		if (strcmp(str, labels[i].name) == 0)
		{
			return labels[i].val;
		}
	}
	if (strstr(str, "0x")|| strstr(str, "0X")) // the imm is hexadecimal
	{
		return ((int)strtol(str, NULL, 16));		
	}
	 // negative
	if (strchr(str, '-') != NULL)
	{
		neg = TRUE;
		str++;
	}
	num = atoi(str);

	if ( neg == 1 ) 
		return (-1 * num);
	return num;

}

void print_inst_I(FILE *machine, Ifromat *I)
{
	
	fprintf(machine, "%02X", I->op);
	fprintf(machine, "%X", I->rd);
	fprintf(machine, "%X", I->rs);
	fprintf(machine, "%X\n", I->rt);
	fprintf(machine, "%05X\n", I->imm & 0xFFFFF);
	
	
	printf("%02X", I->op);
	printf("%X", I->rd);
	printf("%X", I->rs);
	printf("%X\n", I->rt);
	printf("%05X\n", I->imm & 0xFFFFF);
	

}

void print_inst_R(FILE *machine, Rfromat *R)
{
	
	fprintf(machine, "%02X", R->op);
	fprintf(machine, "%X", R->rd);
	fprintf(machine, "%X", R->rs);
	fprintf(machine, "%X\n", R->rt);
	
	printf("%02X", R->op);
	printf("%X", R->rd);
	printf("%X", R->rs);
	printf("%X\n", R->rt);
	
}

void print_inst_words(FILE *machine, int *words)
{
	for (int a = 0; a <= max_word_address; a++)		// loop over .word list
	{
		fprintf(machine, "%05X\n", words[a]);	// print data in dmemin (if empty, print zeros)
		printf("%05X\n", words[a]);
	}
}

char GetOpcode(char *op) {
	if (strcmp(op, "add") == 0) {
		return add;
	}
	if (strcmp(op, "sub") == 0) {
		return sub;
	}
	if (strcmp(op, "mul") == 0) {
		return mul;
	}
	if (strcmp(op, "and") == 0) {
		return and;
	}
	if (strcmp(op, "or") == 0) {
		return or ;
	}
	if (strcmp(op, "xor") == 0) {
		return xor;
	}
	if (strcmp(op, "sll") == 0) {
		return sll;
	}
	if (strcmp(op, "sra") == 0) {
		return sra;
	}
	if (strcmp(op, "srl") == 0) {
		return srl;
	}
	if (strcmp(op, "beq") == 0) {
		return beq;
	}
	if (strcmp(op, "bne") == 0) {
		return bne;
	}
	if (strcmp(op, "blt") == 0) {
		return blt;
	}
	if (strcmp(op, "bgt") == 0) {
		return bgt;
	}
	if (strcmp(op, "ble") == 0) {
		return ble;
	}
	if (strcmp(op, "bge") == 0) {
		return bge;
	}
	if (strcmp(op, "jal") == 0) {
		return jal;
	}
	if (strcmp(op, "lw") == 0) {
		return lw;
	}
	if (strcmp(op, "sw") == 0) {
		return sw;
	}
	return halt;
}

char GetRegister(char *reg)
{
	if (strcmp(reg, "$zero") == 0) {
		return $zero;
	}
	if (strcmp(reg, "$imm") == 0) {
		return $imm;
	}
	if (strcmp(reg, "$v0") == 0) {
		return $v0;
	}
	if (strcmp(reg, "$a0") == 0) {
		return $a0;
	}
	if (strcmp(reg, "$a1") == 0) {
		return $a1;
	}
	if (strcmp(reg, "$a3") == 0) {
		return $a3;
	}
	if (strcmp(reg, "$t0") == 0) {
		return $t0;
	}
	if (strcmp(reg, "$t1") == 0) {
		return $t1;
	}
	if (strcmp(reg, "$t2") == 0) {
		return $t2;
	}
	if (strcmp(reg, "$s0") == 0) {
		return $s0;
	}
	if (strcmp(reg, "$s1") == 0) {
		return $s1;
	}
	if (strcmp(reg, "$s2") == 0) {
		return $s2;
	}
	if (strcmp(reg, "$gp") == 0) {
		return $gp;
	}
	if (strcmp(reg, "$sp") == 0) {
		return $sp;
	}
	if (strcmp(reg, "$ra") == 0) {
		return $ra;
	}
}

int main(int argc, char* argv[])
{
	
	FILE *assembly;
	FILE *machine;
	FILE *StartFile;
	assembly = fopen(argv[1], "r");
	if (assembly == NULL)
	{
		printf("Error: opening file failed\n");
		return 1;
	}
	machine = fopen(argv[2], "w");
	if (machine == NULL)
	{
		printf("Error: opening file failed\n");
		return 1;
	}

	firstRun(assembly); 
	rewind(assembly);
	second_run(assembly, machine);

	StartFile = assembly;
	fclose(assembly);
	fclose(machine);

}
