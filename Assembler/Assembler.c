# include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE 300 // The maximum line size of the input file is 300.
#define MAX_LABEL 50 // the maximum lable size is 50.
#define MAX_MEMO_LINE 4096
#define NUM_OF_REG 16
int RegArry[NUM_OF_REG]

enum OPCODE{
		add, sub, mul, and, or, xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt,
};

enum REGISTER{
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
int TranslateOpcode(char *op) {
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
		return or;
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

int GetRegister (char *reg)
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

void Do_R_inst(int op, int rd int rs, int rt )
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


	default:
		// default statements
	}
}

int main(int argc, char* argv[])
{
	FILE *fp;
	int PC = 0;
	FILE *StartFile;
	fp = fopen("fileName","r");
	if (fp == NULL)
	{
		printf("Error: opening file failed\n");
		return 1;
	}

	startFile = fp

	do
	{

	};
	while (!feof(fp));
	fclose(fp);
}
