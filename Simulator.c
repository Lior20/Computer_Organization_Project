#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#define True 1;
#define False 0;
#define MAX_LINE 5 // maximum line length in memin.txt.
#define MAX_LABEL 50 // the maximum lable size is 50.
#define MAX_MEMO_LINES 4096 // maximum number of lines in the memory.
#define NUM_OF_REG 16 // number of registers.


typedef struct Instruction {
	int opcode;
	int rd;
	int rs;
	int rt;
	int32_t imm;
}instruction;


int main(int argc, char* argv[]);

// Declearation of functions:
void read_file(FILE* file_name);
int read_instructions(int PC, instruction* inst);
void execute(instruction* inst);

// Define Variables:
int PC = 0; // program counter
int imm_check = 0; // check if the instruction is I-type or not.
int cycles_count = 0; // count the number of cycles.
unsigned int memory[MAX_MEMO_LINES]; // memory
int reg[NUM_OF_REG] = { 0 }; // registers 
char memin_array[MAX_MEMO_LINES][MAX_LINE] = { "00000" }; // array of memory input.
instruction inst = { 0 }; // instruction
instruction* instr = &inst; // instruction

// Main Function:
int main(int argc, char* argv[]) {
	FILE* memin = NULL;
	FILE* memout = NULL;
	FILE* regout = NULL;
	FILE* trace = NULL;
	FILE* cycles = NULL;


	// open files: 
	memin = fopen(argv[1], "r");
	// check if memin file was opened:
	if (memin == NULL) {
		printf("Error: memin file was not opened");
		exit(1);
	}

	memout = fopen(argv[2], "w");
	// check if memout file was opened:
	if (memout == NULL) {
		printf("Error: memout file was not opened");
		exit(1);
	}

	regout = fopen(argv[3], "w");
	// check if regout file was opened:
	if (regout == NULL) {
		printf("Error: regout file was not opened");
		exit(1);
	}

	trace = fopen(argv[4], "w");
	// check if memin file was opened:
	if (trace == NULL) {
		printf("Error: Cannot open file\n");
		return False;
	}

	cycles = fopen(argv[5], "w");
	// check if memin file was opened:
	if (cycles == NULL) {
		printf("Error: Cannot open file\n");
		return False;
	}

	// read memin file into memin_array:
	read_file(memin);


	while (PC > -1 && PC <= MAX_MEMO_LINES) {

		imm_check = read_instructions(PC, instr);
		// write to trace.txt file:
		fprintf(trace, "%03X %s 00000000 %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", PC, memin_array[PC], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7], reg[8], reg[9], reg[10], reg[11], reg[12], reg[13], reg[14], reg[15]);

		execute(instr);

		// update PC & cycles_count:
		if (imm_check == 1) {
			PC = PC + 2;
			cycles_count += 1; // add 1 cycle to the cycles counter due to constant loading
			imm_check = 0;
		}
		else {
			PC = PC + 1;
		}
		if ((instr->opcode == 0x10) || (instr->opcode == 0x11)) cycles_count++; // add 1 cycle to the cycles counter due to lw / sw instructions
		cycles_count++;

	}

	// print memory to memout.txt file:
	for (int i = 0; i < MAX_MEMO_LINES; i++) {
		fprintf(memout, "%08x\n", memory[i]);
	}

	// print registers to regout.txt file:
	for (int i = 2; i < NUM_OF_REG; i++) {
		fprintf(regout, "%08x\n", reg[i]);
	}

	// print cycles to cycles.txt file:
	fprintf(cycles, "%d\n", cycles_count);


	// close files:
	fclose(memin);
	fclose(memout);
	fclose(regout);
	fclose(trace);
	fclose(cycles);


	// return 0 if no errors:
	return False;
}


// Functions Implementation:

void read_file(FILE* file) {
	char line[MAX_LINE + 1];
	int i = 0;
	char temp;
	// read the file line by line:
	while (!feof(file)) {
		fgets(line, MAX_LINE + 1, file); // get line from file.
		strncpy(memin_array[i], line, MAX_LINE); // copy the line into the array of memin_array.
		temp = getc(file); // remove '\n' from the end of the line.
		i++; // increase the index of the array.
	}
}

int read_instructions(int PC, instruction* instr) {
	int imm_check = 0;
	char op_code[3] = { memin_array[PC][0], memin_array[PC][1], '\0' }; // read the opcode
	char rd[2] = { memin_array[PC][2], '\0' }; // read the rd
	char rs[2] = { memin_array[PC][3], '\0' }; // read the rs
	char rt[2] = { memin_array[PC][4], '\0' }; // read the rt

	instr->imm = 0; // initialize imm 
	instr->opcode = (int)strtol(op_code, NULL, 16); // convert the opcode to decimal
	instr->rd = (int)strtol(rd, NULL, 16); // convert the rd to decimal
	instr->rs = (int)strtol(rs, NULL, 16); // convert the rs to decimal
	instr->rt = (int)strtol(rt, NULL, 16); // convert the rt to decimal.

	// check if the instruction is R-type or I-type:
	if (instr->rs == 1) { // rs is immidiate
		imm_check = 1; // indicate that the instruction is I-type.
		cycles_count += 1; // add 1 cycle to the cycles counter due to constant loading
		//strncpy(imm, memin_array[PC + 1], 5); // get the imm
		char imm[6] = { memin_array[PC + 1][0], memin_array[PC + 1][1], memin_array[PC + 1][2], memin_array[PC + 1][3], memin_array[PC + 1][4], '\0' };
		instr->imm = (int32_t)strtol(imm, NULL, 16); // convert the imm to decimal and sign extend it.
		reg[instr->rs] = instr->imm;
	}

	if (instr->rt == 1) { // rt is immidiate
		imm_check = 1;
		cycles_count += 1; // add 1 cycle to the cycles counter due to constant loading
		//strncpy(imm, memin_array[PC + 1], 5); // get the imm
		char imm[6] = { memin_array[PC + 1][0], memin_array[PC + 1][1], memin_array[PC + 1][2], memin_array[PC + 1][3], memin_array[PC + 1][4], '\0' };
		instr->imm = (int32_t)strtol(imm, NULL, 16); // convert the imm to decimal and sign extend it.
		reg[instr->rt] = instr->imm;
	}

	return imm_check;
}


//printf("%05x\n", instr->imm);
void execute(instruction* instr) {
	switch (instr->opcode) {
	case 0x00: // add
		reg[instr->rd] = reg[instr->rs] + reg[instr->rt];
		break;
	case 0x01: // sub
		reg[instr->rd] = reg[instr->rs] - reg[instr->rt];
		break;
	case 0x02: // mul
		reg[instr->rd] = reg[instr->rs] * reg[instr->rt];
		break;
	case 0x03: // and
		reg[instr->rd] = reg[instr->rs] & reg[instr->rt];
		break;
	case 0x04: // or
		reg[instr->rd] = reg[instr->rs] | reg[instr->rt];
		break;
	case 0x05: // xor
		reg[instr->rd] = reg[instr->rs] ^ reg[instr->rt];
		break;
	case 0x06: // sll
		reg[instr->rd] = reg[instr->rs] << reg[instr->rt];
		break;
	case 0x07: // sra
		reg[instr->rd] = reg[instr->rs] >> reg[instr->rt];
		break;
	case 0x08: // srl
		reg[instr->rd] = reg[instr->rs] >> reg[instr->rt];
		break;
	case 0x09: // beq
		if (reg[instr->rs] == reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0A: // bne
		if (reg[instr->rs] != reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0B: // blt
		if (reg[instr->rs] < reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0C: // bgt
		if (reg[instr->rs] > reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0D: // ble
		if (reg[instr->rs] <= reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0E: // bge
		if (reg[instr->rs] >= reg[instr->rt]) {
			PC = PC + (instr->rd << 2);
		}
		break;
	case 0x0F: // jal
		reg[31] = PC;
		PC = PC + (instr->rd << 2);
		break;
	case 0x10: // lw
		reg[instr->rd] = memory[reg[instr->rs] + instr->rt];
		break;
	case 0x11: // sw
		memory[reg[instr->rs] + instr->rt] = reg[instr->rd];
		break;
	case 0x12: // halt
		PC = -5;
		break;
	}
}
/*


case 0x12: // reti
	PC = IORegister[7];
	break;
case 0x13: // in
	reg[instr.rd] = IORegister[instr.rs + instr.rt];
	break;
case 0x14: // out
	IORegister[instr.rs + instr.rt] = reg[instr.rd];
	break;


 ALL OF THOSE INSTRUCTIONS ABOVE ARE FOR THE BONUS PART - ADD THEM IF WE DECIDE TO DO THE BONUS PART!!!!!!!!!!


 */



