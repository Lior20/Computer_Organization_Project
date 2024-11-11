🚀 RISC Processor Simulator and Assembler

## Overview
This project is a C program that implements an assembler and a simulator for a RISC processor called SIMP. The assembler translates assembly code written in text format into machine code, while the simulator can read the machine code and simulate the execution of the program on the SIMP processor.

## Table of Contents
- [Features](#features)
- [SIMP Processor Architecture](#simp-processor-architecture)
- [Usage](#usage)
  - [Assembler](#assembler)
  - [Simulator](#simulator)
- [Folder Structure](#folder-structure)
- [Contributing](#contributing)
- [License](#license)

## Features
- 📝 Assembler that translates assembly code into machine code
- 🖥️ Simulator that can execute the machine code and simulate the SIMP processor
- 🔢 Supports 18 instructions, including arithmetic, logical, branch, and memory operations
- 🔍 Handles both R-format and I-format instructions
- 📚 Provides detailed documentation and comments throughout the code

## SIMP Processor Architecture
The SIMP processor has the following features:
- 💾 16 registers, each 32 bits wide
- ⏱️ Clock frequency of 512 Hz
- 🔢 Supports two instruction formats: R-format and I-format
- 🔣 18 instructions, including add, sub, mul, and, or, xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, and halt

## Usage

### Assembler
To use the assembler, run the following command:
```
asm.exe <input_file.asm> <output_file.txt>
```
The input file should contain the assembly code, and the output file will contain the corresponding machine code.

### Simulator
To use the simulator, run the following command:
```
sim.exe <input_file.txt> <output_file.txt> <regout_file.txt> <trace_file.txt> <cycles_file.txt>
```
The input file should contain the machine code, and the output files will contain the following:
- `output_file.txt`: The final state of the memory
- `regout_file.txt`: The final state of the registers
- `trace_file.txt`: A trace of the executed instructions
- `cycles_file.txt`: The total number of clock cycles executed

## Folder Structure
```
RISC/
├── asm/
│   └── Debug/
├── sim/
│   └── Debug/
├── fibo/
│   ├── memin.txt
│   └── README.md
├── Project_computer.sln
└── README.md
```
- `asm/`: Contains the assembler code and executable.
- `sim/`: Contains the simulator code and executable.
- `fibo/`: Contains a sample Fibonacci sequence program and its input/output files.
- `Project_computer.sln`: The Visual Studio solution file.
- `README.md`: This file.

## Contributing
🤝 Contributions to this project are welcome! If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

## License
This project is licensed under the [MIT License](LICENSE).
