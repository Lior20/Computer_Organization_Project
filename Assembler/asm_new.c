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
#define  NUM_OF_REG 16
#define  MAX_VAR_IN_INST 7 

/*****************************************************************
						Globals
*****************************************************************/
enum OPCODE {
	add, sub, mul, and, or , xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt,
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
int words[MAX_MEMO_LINES] = { 0 };
int word_addrs_count = 0;

uint16_t PC = 0; // cant be nagative maximum 12 bits
int RegArry[NUM_OF_REG] = { 0 }; // 32 bits but we need to use 20


/*****************************************************************
						help function declarations
*****************************************************************/
void remove_white_spaces(char *str);
Rfromat *read_instraction_R(char *line);
Ifromat *read_instraction_I(char *line);
int is_instraction(char *line);
char GetOpcode(char *op);
char GetRegister(char *reg);
int read_label_imm(char *str);
void print_inst_I(FILE *machine, Ifromat *I);
void print_inst_R(FILE *machine, Rfromat *R);






/*****************************************************************
						first run
*****************************************************************/

// fill the lable array so we can use them
void firstRun(FILE *assembly)
{
	int pc = 0;
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
				labels[label_counter].val = pc;
				strcpy(labels[label_counter].name , label_name);
				label_counter++;
			}

		}
		
		// if the line has "$" or ".word" its an instraction if its is not after a "#"
		if (is_instraction(line))
			pc++;
	}
}

/*****************************************************************
						second run
*****************************************************************/
void second_run(FILE* assembly, FILE *machine)
{
	int RowInd = 0;
	int R_format = FALSE;
	int pc = 0;
	char line[MAX_LINE_LEN]; // a place to store the line
	// char op[6];
	// char reg[6];
	// int opcode;
	Rfromat *Rinst;
	Ifromat *Iinst;
	//inst instraction;
	// int j;
	while (!feof(assembly))
	{
		fgets(line, MAX_LINE_LEN, assembly); // reads a line from assembly
		
		remove_white_spaces(line);  // removes spaces and tabes.

		if (strcmp(line, "\n") == 0) continue;  // empty line
		if (line[0] == '#') continue; // all row is comment

		if (is_instraction(line))  // a label will be skipped 
		{
			/*if ((strstr(line, ".word") != NULL)
			{

			}*/

			/*else*/ if (strstr(line, "$imm") != NULL) // Iformat
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

int is_instraction(char *line)
{
	if ((strchr(line, '$') != NULL))
	{
		if (strstr(line, "#") == NULL)
			return TRUE;
		if ((strstr(line, "$")) < (strstr(line, "#")))
			return TRUE;		
	}
	if (strstr(line, ".word") != NULL) 
	{
		if (strstr(line, "#") == NULL)
			return TRUE;
		if ((strstr(line, ".word")) < (strstr(line, "#")))
			return TRUE;
	}
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
	if (strstr(str, "0x")) // the imm is hexadecimal
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
	/*
	fprintf(machine, "%x", I->op);
	fprintf(machine, "%x", I->rd);
	fprintf(machine, "%x", I->rs);
	fprintf(machine, "%x\n", I->rt);
	fprintf(machine, "%x\n", I->imm);
	*/
	printf("%x ", I->op);
	printf("%x ", I->rd);
	printf("%x ", I->rs);
	printf("%x\n", I->rt);
	printf("%x\n", I->imm);

}

void print_inst_R(FILE *machine, Rfromat *R)
{
	/*
	fprintf(machine, "%x", R->op);
	fprintf(machine, "%x", R->rd);
	fprintf(machine, "%x", R->rs);
	fprintf(machine, "%x\n", R->rt);
	*/
	printf("%x ", R->op);
	printf("%x ", R->rd);
	printf("%x ", R->rs);
	printf("%x\n", R->rt);
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
	if (strcmp(op, "reti") == 0) {
		return reti;
	}
	if (strcmp(op, "in") == 0) {
		return in;
	}
	if (strcmp(op, "out") == 0) {
		return out;
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
	printf("%d", assembly);

	StartFile = assembly;
	fclose(assembly);
	fclose(machine);

}
