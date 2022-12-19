#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define True 1;
#define False 0;
#define MAX_LINE 300// The maximum line size of the input file is 300.
#define MAX_LABEL 50 // the maximum lable size is 50.
#define MAX_MEMO_LINE 4096
#define NUM_OF_REG 16
#define MAX_LINE_LEN 500

int PC = 0;
int RegArry[NUM_OF_REG] = { 0 };
typedef struct Rfromat
{
	uint32_t op : 6;
	uint32_t rd : 3;
	uint32_t rs : 3;
	uint32_t rt : 3;
}Rfromat;

typedef struct Ifromat
{
	uint64_t op : 6;
	uint64_t rd : 3;
	uint64_t rs : 3;
	uint64_t rt : 3;
	uint64_t imm : 20;
}Ifromat;

typedef struct Instraction
{
	uint64_t op : 6;
	uint64_t rd : 3;
	uint64_t rs : 3;
	uint64_t rt : 3;
	uint64_t imm : 20;
}inst;


enum OPCODE {
	add, sub, mul, and, or , xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt,
};

enum REGISTER {
	$zero, $imm, $v0, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $s0, $s1, $s2, $gp, $sp, $ra,
};

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
}I_Format;


typedef struct
{
	int op; // R_FORMAT, I_FORMAT, J_FORMAT 
	int rs;
	int rt;
	int rd;
	int func; // R_FORMAT
	int imm; // I_FORMAT
	int targetAddress; // J_FORMAT

}Instraction;


// help function for write
int count_bits(uint8_t x)
{
	int bits = 0;
	while (x) {
		if (x & 1U) bits++;
		x >>= 1;
	}
	return bits;
}

int int_bits(void)
{
	return count_bits(~0U);
}

void print_nbits(uint8_t x, uint8_t n, FILE *machine)
{
	int i = int_bits();
	i = (n < i) ? n - 1 : i - 1;
	for (; i >= 0; i--)
		putc((((x >> i) & 1U) ? '1' : '0'), machine); // needs to be putc(char,fp1) to write to file
}

/*
 * @function    TranslateOpcode
 *
 * @brief		This function gets string of opcode and tranlate it into its binary value
 *
 * input params
 * @param       op pointer to the opcode string
 *
 * output params
 * @result      binary value of opcode
*/
int GetOpcode(char *op) {
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

int GetRegister(char *reg)
{
	if (strcmp(reg, "$zero") == 0) {
		return $zero;
	}
	if (strcmp(reg, "imm") == 0) {
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

/*****************************************************************
						first run
*****************************************************************/

// first run on the asembly code detects the Lables and immdiates and store them in a list of Lable struck. 
typedef struct label
{
	// name of the label
	char name[MAX_LABEL];
	// val the immediate value related
	int val;
	// a pointer to the next label
	struct label* next;
} label;

// the function creates a label 
label *create_label(char name[50], int val)
{
	// allocate memory for the label and create a pointer to it
	label* new_label = (label*)malloc(sizeof(label));

	// if the allocation was successful.
	if (new_label != NULL)
	{
		strcpy(new_label->name, name);
		new_label->val = val;
		new_label->next = NULL;
	}
	return new_label;
}

// adds a label to the start of the labels list.
label* addLabel(label* head, char name[50], int val) {
	// create a new lable
	label* new_label = create_label(name, val);
	// error in allocating memory
	if (new_label == NULL)
		return NULL;
	// the next pointer will point to the original head
	new_label->next = head;
	return new_label;
}



// destroys the list and frees the memory
void destroyLabel(label* head)
{
	label* temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}



label* firstRun(FILE *asembly)
{
	int RowInd = 0;
	char line[MAX_LINE_LEN]; // a place to store the line  
	label* head = NULL; // set the label list to null = empty
	char label_name[MAX_LABEL];
	int i = 0;

	while (!feof(asembly))
	{
		fgets(line, MAX_LINE_LEN, asembly); // reads a line from asembly
		if (strcmp(line, "\n") == 0) continue;  //empty line
		if (line[0] == '#') continue;
		if (strstr(line, ":") != NULL)  //If there are ':' in the line which means it contains a lable 
		{
			if (strstr(line, "#") != NULL)
			{   // chacks if the ':' are in a comment
				if ((strstr(line, ":")) > (strstr(line, "#"))) continue;
			}

			while (line[i] != ':')
			{
				label_name[i] = line[i];
				i++;
			}
			label_name[i] = '\0';
		}
		head = addLabel(head, label_name, RowInd);  // finally we add the label to label list
		RowInd++;
	}
	return head; // returns the list
}

/*****************************************************************
						second run
*****************************************************************/


// this will be used in the second run to read a label val
int read_Label(label* head, char name[50])
{
	if (head == NULL)
		return -1;

	while (strcmp(head->name, name) != 0)
	{
		// go over the labels
		head = head->next;
		// if the label was not found the function returns -1
		if (head == NULL) {
			return -1;
		}
	}
	// return the value of the label.
	return head->val;
}



// second run on the asbembly file excutes the instruction and write to the ,machine code file.
// add, sub, mul, and, or , xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt
void Do_R_inst(int op, int rd, int rs, int rt)
{
	switch (op)
	{
	case add:
		RegArry[rd] = RegArry[rs] + RegArry[rt];
		break;
	case sub:
		RegArry[rd] = RegArry[rs] - RegArry[rt];
		break;
	case mul:
		RegArry[rd] = RegArry[rs] * RegArry[rt];
		break;
	case and:
		// b.w 
		RegArry[rd] = RegArry[rs] & RegArry[rt];
		break;
	case or :
		// b.w 
		RegArry[rd] = RegArry[rs] | RegArry[rt];
		break;
	case xor:
		// b.w 
		RegArry[rd] = RegArry[rs] ^ RegArry[rt];
		break;
	case sll:
		RegArry[rd] = RegArry[rs] << RegArry[rt];
		break;
		// ??????????????????????????????
	case sra:
		RegArry[rd] = RegArry[rs] >> RegArry[rt];
		break;
	case srl:
		RegArry[rd] = RegArry[rs] >> RegArry[rt];
		break;
	case beq:
		if (RegArry[rs] == RegArry[rt])
			PC = RegArry[rd];
		break;
	case bne:
		if (RegArry[rs] != RegArry[rt])
			PC = RegArry[rd];
		break;
	case blt:
		if (RegArry[rs] < RegArry[rt])
			PC = RegArry[rd];
		break;
	case bgt:
		if (RegArry[rs] > RegArry[rt])
			PC = RegArry[rd];
		break;
	case ble:
		if (RegArry[rs] <= RegArry[rt])
			PC = RegArry[rd];
		break;
	case bge:
		if (RegArry[rs] >= RegArry[rt])
			PC = RegArry[rd];
		break;
	case jal:
		RegArry[rd] = PC + 1; // +1 0r +4?? nedd to think if pc is relative.
		PC = RegArry[rs];
		break;
		// left 
		// lw, sw, reti, in, out, halt

	// default:
		// default statements
	}
}


void print_inst(FILE *machine, inst *instraction, int isR)
{
	if (isR == 1) // Rformat
	{
		print_nbits(instraction->op, 6, machine);
		print_nbits(instraction->rd, 3, machine);
		print_nbits(instraction->rs, 3, machine);
		print_nbits(instraction->rt, 3, machine);
	}
	else // Iformat
	{
		print_nbits(instraction->op, 6, machine);
		print_nbits(instraction->rd, 3, machine);
		print_nbits(instraction->rs, 3, machine);
		print_nbits(instraction->rt, 3, machine);
		putc('\n', machine);
		print_nbits(instraction->imm, 20, machine);
	}
}



void SecondRun(FILE* asembly, FILE *machine)
{

	int RowInd = 0;
	int R_format = False;
	int pc = 0;
	char line[MAX_LINE_LEN]; // a place to store the line
	char op[6];
	char reg[6];
	int opcode;
	inst instraction;
	int j;
	while (!feof(asembly))
	{
		fgets(line, MAX_LINE_LEN, asembly);
		if (strstr(line, ".word") != NULL)
			break;
		if (strcmp(line, "\n") == 0)
		{
			pc++;
			continue;  //empty line
		}
		if (line[0] == '#')
		{
			pc++;
			continue;
		}
		if (strstr(line, ":") != NULL)  //If there are ':' in the line which means it contains a lable 
		{
			pc++;
			continue;
		}

		int i = 0;
		while (line[i] != ' ')
		{
			op[i] = line[i];
			i++;
		}
		op[i] = '\0';
		instraction.op = GetOpcode(op);
		i++; // skip the space

		// set rd
		j = 0;
		while (line[i] != ',')
		{
			reg[j] = line[i];
			i++;
			j++;
		}
		reg[j] = '\0';
		instraction.rd = GetRegister(reg);
		i++; // skip the space

		// set rs
		j = 0;
		while (line[i] != ',')
		{
			reg[j] = line[i];
			i++;
			j++;
		}
		reg[j] = '\0';
		instraction.rs = GetRegister(reg);
		i++; // skip the space

		// set rt 
		j = 0;
		while (line[i] != ',')
		{
			reg[j] = line[i];
			i++;
			j++;
		}
		reg[j] = '\0';
		instraction.rs = GetRegister(reg);
		i++; // skip the space

		if (line[i] == ' ' || line[i] == '#')
			R_format = True;


		if (R_format == 1)
		{
			print_inst(machine, &instraction, 1);
			Do_R_inst(instraction.op, instraction.rd, instraction.rs, instraction.rt);
		}

		else // Iformat
		{
			continue;
		}


	}
}






int main(int argc, char* argv[])
{
	/*
	FILE *fp;

	FILE *StartFile;
	fp = fopen("fileName", "r");
	if (fp == NULL)
	{
		printf("Error: opening file failed\n");
		return 1;
	}

	StartFile = fp;

		do
		{
			// read line , split to tokens , do instraction
		}
	while (!feof(fp));
	fclose(fp);
	*/
	typedef struct Rfromat
	{
		uint32_t op : 6;
		uint32_t rd : 3;
		uint32_t rs : 3;
		uint32_t rt : 3;
	}Rfromat;

	Rfromat b;
	b.op = 0b1111100001;


	// print_nbits(b.op, 6);
	printf("\n%6u", b.op);

}
