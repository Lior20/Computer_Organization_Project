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

typedef struct {
	unsigened int opcode;
	unsigened int rd;
	unsigened int rs;
	unsigened int rt;
}instruction;


// Declearation of functions:
void read_file(char* file_name);
void read_instruction(char* line);
instruction fetch();
void execute(instruction inst);

// Define Variables:
unsigned int PC = 0; // program counter
unsigned int memory[MAX_MEMO_LINE]; // memory
unsigned int reg[NUM_OF_REG]; // registers

int main(int argc, char* argv[]) {
	FILE* memin = NULL;
	FILE* memout = NULL;
	//FILE* regout = NULL;
	char line[MAX_LINE];
	
	memin = fopen(argv[1], "r");
	if (memin == NULL) {
		printf("Error: Cannot open file\n");
		return False;
	} // check if the file is open.
	memout = fopen(argv[2], "w"); 
	if (memout == NULL) { 
		printf("Error: Cannot open file\n");
		return False;
	} // check if memout created.
	
	while (!foef(memin)) {
		fgets(line, MAX_LINE, memin);
		// get hexa number and turn it into binary.
		
		
	}
	else

		
	fclose(memin);
	fclose(memout);
	fclose(regout);
	return True;
	
		
}






// Functions Implementation:

void read_file(char* file_name) {
	FILE* file = NULL;
	char line[MAX_LINE];
	file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Error: Cannot open file\n");
		return 1;
	} // check if the file is open.
	while (!feof(file)) {
		fgets(line, MAX_LINE, file);
		// get hexa number and turn it into binary.
	}
}

void read_instruction(char* line) {

}

instruction fetch() {
	instruction inst;
	inst.opcode = memory[PC] << 24 | memory[PC + 1] << 16 | memory[PC + 2] << 8 | memory[PC + 3];
	inst.rd = memory[PC + 4] << 24 | memory[PC + 5] << 16 | memory[PC + 6] << 8 | memory[PC + 7];
	inst.rs = memory[PC + 8] << 24 | memory[PC + 9] << 16 | memory[PC + 10] << 8 | memory[PC + 11];
	inst.rt = memory[PC + 12] << 24 | memory[PC + 13] << 16 | memory[PC + 14] << 8 | memory[PC + 15];
	inst.imm = memory[PC + 16] << 24 | memory[PC + 17] << 16 | memory[PC + 18] << 8 | memory[PC + 19];
	PC += 16;
	return inst;


	// read line from memin.txt and stores it char by char in inst.
	
}

void execute(instruction instr) {
	switch (instr.opcode) {
	case 0x00: // add
		reg[instr.rd] = reg[instr.rs] + reg[instr.rt];
		break;
	case 0x01: // sub
		reg[instr.rd] = reg[instr.rs] - reg[instr.rt];
		break;
	case 0x02: // mul
		reg[instr.rd] = reg[instr.rs] * reg[instr.rt];
		break;
	case 0x03: // and
		reg[instr.rd] = reg[instr.rs] & reg[instr.rt];
		break;
	case 0x04: // or
		reg[instr.rd] = reg[instr.rs] | reg[instr.rt];
		break;
	case 0x05: // xor
		reg[instr.rd] = reg[instr.rs] ^ reg[instr.rt];
		break;
	case 0x06: // sll
		reg[instr.rd] = reg[instr.rs] << reg[instr.rt];
		break;
	case 0x07: // sra
		reg[instr.rd] = reg[instr.rs] >> reg[instr.rt];
		break;
	case 0x08: // srl
		reg[instr.rd] = reg[instr.rs] >> reg[instr.rt];
		break;
	case 0x09: // beq
		if (reg[instr.rs] == reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0A: // bne
		if (reg[instr.rs] != reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0B: // blt
		if (reg[instr.rs] < reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0C: // bgt
		if (reg[instr.rs] > reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0D: // ble
		if (reg[instr.rs] <= reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0E: // bge
		if (reg[instr.rs] >= reg[instr.rt]) {
			PC = PC + (instr.rd << 2);
		}
		break;
	case 0x0F: // jal
		reg[31] = PC;
		PC = PC + (instr.rd << 2);
		break;
	case 0x10: // lw
		reg[instr.rd] = memory[reg[instr.rs] + instr.rt];
		break;
	case 0x11: // sw
		memory[reg[instr.rs] + instr.rt] = reg[instr.rd];
		break;
	case 0x12: // reti
		PC = IORegister[7];
		break;
	case 0x13: // in
		reg[instr.rd] = IORegister[instr.rs + instr.rt];
		break;
	case 0x14: // out
		IORegister[instr.rs + instr.rt] = reg[instr.rd];
		break;
	case 0x15: // halt
		PC = 0;
		break;
	}
}
