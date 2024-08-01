#ifndef SAMCO_H
#define SAMCO_H

//Available Instructions
#define ADD     "0000"
#define SUB     "0001"
#define LSHF    "0010"
#define MUL     "0011"
#define DIV     "0100"
#define GET     "0101"
#define PUT     "0110"
#define JZ      "1000"


//General Purpose Registers
#define r0      "0000"
#define r1      "0001"
#define r2      "0010"
#define r3      "0011"
#define r4      "0100"
#define r5      "0101"
#define r6      "0110"
#define r7      "0111"

//CPU Registers
#define IPR     "1100"
#define IR      "1010"
#define DR      "1001"
#define ZF      "1000"


//16 Bit Instruction Format
struct instruction
{
    char *opcode;
    char *operand1;
    char *operand2;
    char *data;
};
#define INSTRUCTION_SIZE_BITS    16

//Max line size for any line of SamCo Assembly
#define MAX_LINE_SIZE_ASM       2048

//You can switch this with "a" to append or "w" to overwrite
#define OUTPUT_FILE_OPEN_MODE       "w"
#define DEFAULT_OUTPUT_FILENAME     "PROG_memory.EEPROM"

#endif /* SAMCO_H */
