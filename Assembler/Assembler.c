# include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE 300 // The maximum line size of the input file is 300.
#define MAX_LABEL 50 // the maximum lable size is 50.
#define MAX_MEMO_LINE 4096

enum {
	add, sub, and, or , sll, sra, mac, branch, jal, lw, sw, jr, halt,
} opcode;

enum {
	$zero, $at, $v0, $a0, $a1, $t0, $t1, $t2, $t3, $s0, $s1, $s2, $gp, $sp, $fp, $ra,
} reg;

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



int main(int argc, char* argv[])
{
	
}
