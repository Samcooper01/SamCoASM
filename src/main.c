/*
 * Program Name: SamCO Simple Assembly Language
 * Description: Simple Assembly Language for SCC-16 ISA
 * Author: Samuel Cooper
 *
 *
 * Notes:
 *      None.
 *
 * Style:
 *  https://github.com/Samcooper01/StyleGuide/tree/main
 *
 */

//C Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/samco.h"
#include "../include/samco_error.h"

//Input file (SamCO ASM)
FILE *input_asm_fd;
char * input_asm_filename;
//Output file (Binary)
FILE *output_binary_fd;
char * output_binary_filename;

//Global line counter
int line_counter = 0;

static void
open_asm_file()
{
    input_asm_fd = fopen(input_asm_filename, "r");
    if(input_asm_fd == NULL) { fatal_error("Opening asm file");}
}

static void
open_output_file()
{
    output_binary_fd = fopen(output_binary_filename, OUTPUT_FILE_OPEN_MODE);
    if(input_asm_fd == NULL) { fatal_error("Opening asm file"); }
}

static int
is_arthimetic_instruction(char * opcode)
{
    if(strcasecmp(opcode, "ADD") == 0
    || strcasecmp(opcode, "SUB") == 0
    || strcasecmp(opcode, "MUL") == 0
    || strcasecmp(opcode, "DIV") == 0)
    { return 1; }
    return 0;
}

static int
is_mem_access_instruction(char * opcode)
{
    if(strcasecmp(opcode, "GET") == 0
    || strcasecmp(opcode, "PUT") == 0)
    { return 1; }
    return 0;
}

static void
convert_hex_to_binary(char *hex_digits)
{
    char hex_digit0 = hex_digits[1];
    char hex_digit1 = hex_digits[0];
    char binary_string[8];
    for(int i = 0; i < 2; i++)
    {
        if(hex_digits[i] == 'F') memcpy(binary_string + (i*4), "1111", 4);
        else if(hex_digits[i] == 'E') memcpy(binary_string + (i*4), "1110", 4);
        else if(hex_digits[i] == 'D') memcpy(binary_string + (i*4), "1101", 4);
        else if(hex_digits[i] == 'C') memcpy(binary_string + (i*4), "1100", 4);
        else if(hex_digits[i] == 'B') memcpy(binary_string + (i*4), "1011", 4);
        else if(hex_digits[i] == 'A') memcpy(binary_string + (i*4), "1010", 4);
        else if(hex_digits[i] == '9') memcpy(binary_string + (i*4), "1001", 4);
        else if(hex_digits[i] == '8') memcpy(binary_string + (i*4), "1000", 4);
        else if(hex_digits[i] == '7') memcpy(binary_string + (i*4), "0111", 4);
        else if(hex_digits[i] == '6') memcpy(binary_string + (i*4), "0110", 4);
        else if(hex_digits[i] == '5') memcpy(binary_string + (i*4), "0101", 4);
        else if(hex_digits[i] == '4') memcpy(binary_string + (i*4), "0100", 4);
        else if(hex_digits[i] == '3') memcpy(binary_string + (i*4), "0011", 4);
        else if(hex_digits[i] == '2') memcpy(binary_string + (i*4), "0010", 4);
        else if(hex_digits[i] == '1') memcpy(binary_string + (i*4), "0001", 4);
        else if(hex_digits[i] == '0') memcpy(binary_string + (i*4), "0000", 4);
    }
    strcpy(hex_digits, binary_string);
}

static int
write_instruction(struct instruction *instruct)
{
    char *instruction_16_bit = (char *)malloc(INSTRUCTION_SIZE_BITS * sizeof(char));
    //opcode
    if(instruct->opcode == NULL) fatal_error("Opcode not recognized");
    else if(strcasecmp(instruct->opcode, "ADD") == 0) memcpy(instruction_16_bit, ADD, 4);
    else if(strcasecmp(instruct->opcode, "SUB") == 0) memcpy(instruction_16_bit, SUB, 4);
    else if(strcasecmp(instruct->opcode, "LSHF") == 0) memcpy(instruction_16_bit, LSHF, 4);
    else if(strcasecmp(instruct->opcode, "MUL") == 0) memcpy(instruction_16_bit, MUL, 4);
    else if(strcasecmp(instruct->opcode, "DIV") == 0) memcpy(instruction_16_bit, DIV, 4);
    else if(strcasecmp(instruct->opcode, "GET") == 0) memcpy(instruction_16_bit, GET, 4);
    else if(strcasecmp(instruct->opcode, "PUT") == 0) memcpy(instruction_16_bit, PUT, 4);
    else if(strcasecmp(instruct->opcode, "JZ") == 0) memcpy(instruction_16_bit, JZ, 4);
    else fatal_error("Opcode not recognized");

    //operand1
    if(instruct->operand1 == NULL) fatal_error("Operand1 not recognized");
    else if(strcasecmp(instruct->operand1, "r0") == 0) memcpy(instruction_16_bit + 4, r0, 4);
    else if(strcasecmp(instruct->operand1, "r1") == 0) memcpy(instruction_16_bit + 4, r1, 4);
    else if(strcasecmp(instruct->operand1, "r2") == 0) memcpy(instruction_16_bit + 4, r2, 4);
    else if(strcasecmp(instruct->operand1, "r3") == 0) memcpy(instruction_16_bit + 4, r3, 4);
    else if(strcasecmp(instruct->operand1, "r4") == 0) memcpy(instruction_16_bit + 4, r4, 4);
    else if(strcasecmp(instruct->operand1, "r5") == 0) memcpy(instruction_16_bit + 4, r5, 4);
    else if(strcasecmp(instruct->operand1, "r6") == 0) memcpy(instruction_16_bit + 4, r6, 4);
    else if(strcasecmp(instruct->operand1, "r7") == 0) memcpy(instruction_16_bit + 4, r7, 4);
    else if(strcasecmp(instruct->operand1, "IPR") == 0) memcpy(instruction_16_bit + 4, IPR, 4);
    else if(strcasecmp(instruct->operand1, "IR") == 0) memcpy(instruction_16_bit + 4, IR, 4);
    else if(strcasecmp(instruct->operand1, "DR") == 0) memcpy(instruction_16_bit + 4, DR, 4);
    else if(strcasecmp(instruct->operand1, "ZF") == 0) memcpy(instruction_16_bit + 4, ZF, 4);
    else fatal_error("Operand1 not recognized");

    //operand2
    if(instruct->operand2 == NULL)
    {
        if(strcasecmp(instruct->opcode, "JZ") == 0) //JZ only has one operand
        {
            memcpy(instruction_16_bit + 8, "0000", 4);
        }
        else
        {
            fatal_error("Operand2 not recognized");
        }
    }
    else if(strcasecmp(instruct->opcode, "LSHF") == 0)
    {
        if(sizeof(instruct->operand2) != 8) fatal_error("Operand2 not recognized");
        char *hex_digits = instruct->operand2 + 2;
        convert_hex_to_binary(hex_digits);
        memcpy(instruction_16_bit + 8, hex_digits, 8);
        fprintf(output_binary_fd, "0x%04x ", line_counter);
        fprintf(output_binary_fd, "%s\n", instruction_16_bit);
        free(instruction_16_bit);
        return 0;
    }
    else if(strcasecmp(instruct->operand2, "r0") == 0) memcpy(instruction_16_bit + 8, r0, 4);
    else if(strcasecmp(instruct->operand2, "r1") == 0) memcpy(instruction_16_bit + 8, r1, 4);
    else if(strcasecmp(instruct->operand2, "r2") == 0) memcpy(instruction_16_bit + 8, r2, 4);
    else if(strcasecmp(instruct->operand2, "r3") == 0) memcpy(instruction_16_bit + 8, r3, 4);
    else if(strcasecmp(instruct->operand2, "r4") == 0) memcpy(instruction_16_bit + 8, r4, 4);
    else if(strcasecmp(instruct->operand2, "r5") == 0) memcpy(instruction_16_bit + 8, r5, 4);
    else if(strcasecmp(instruct->operand2, "r6") == 0) memcpy(instruction_16_bit + 8, r6, 4);
    else if(strcasecmp(instruct->operand2, "r7") == 0) memcpy(instruction_16_bit + 8, r7, 4);
    else if(strcasecmp(instruct->operand2, "IPR") == 0) memcpy(instruction_16_bit + 8, IPR, 4);
    else if(strcasecmp(instruct->operand2, "IR") == 0) memcpy(instruction_16_bit + 8, IR, 4);
    else if(strcasecmp(instruct->operand2, "DR") == 0) memcpy(instruction_16_bit + 8, DR, 4);
    else if(strcasecmp(instruct->operand2, "ZF") == 0) memcpy(instruction_16_bit + 8, ZF, 4);
    else fatal_error("Operand 2 not recognized");
    //data
    memcpy(instruction_16_bit + 12, "0000", 4);
    fprintf(output_binary_fd, "0x%04x ", line_counter);
    fprintf(output_binary_fd, "%s\n", instruction_16_bit); //Even last instruction gets a newline
    free(instruction_16_bit);
    return 0;
}

static int
write_line(char * tokens[])
{
    struct instruction new_instruct;
    //Build instruction
    char * opcode = tokens[0];
    if(is_arthimetic_instruction(opcode)
    || is_mem_access_instruction(opcode))
    {
        new_instruct.opcode = tokens[0];
        new_instruct.operand1 = tokens[1];
        new_instruct.operand2 = tokens[2];
        new_instruct.data = tokens[3];
    }
    else if(strcasecmp(opcode, "LSHF") == 0)
    {
        new_instruct.opcode = tokens[0];
        new_instruct.operand1 = tokens[1];
        new_instruct.operand2 = tokens[2];
    }
    else if(strcasecmp(opcode, "JZ") == 0)
    {
        new_instruct.opcode = tokens[0];
        new_instruct.operand1 = tokens[1];
    }
    else
    {
        fatal_error("Cannot find Opcode");
    }

    write_instruction(&new_instruct);

}

static void
removeNewline(char *str) {
    size_t len = strlen(str);
    // Check if the last character is a newline
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0'; // Replace newline with null terminator
    }
}

void
assemble()
{
    char line_buffer[MAX_LINE_SIZE_ASM];

    while(fgets(line_buffer, MAX_LINE_SIZE_ASM, input_asm_fd) != NULL)
    {
        if(strcmp(line_buffer, "\n") == 0)
        {
            continue;
        }
        removeNewline(line_buffer);
        char *tokens[4];
        int token_index = 0;
        char *first_token = strtok(line_buffer, " ");
        tokens[token_index] = first_token;
        char *next_token;
        while((next_token = strtok(NULL, " ")) != NULL)
        {
            token_index++;
            if(token_index >= 3) {error_at_line(line_counter);} //max args is 3
            tokens[token_index] = next_token;
        }

        int successful_line = write_line(tokens);

        line_counter++;
    }

}

void
open_files()
{
    open_asm_file();
    open_output_file();
}

void
close_files()
{
    if(output_binary_fd != NULL) fclose(output_binary_fd);
    if(input_asm_fd != NULL) fclose(input_asm_fd);
}

void
print_usage()
{
    printf("USAGE:\n\n");
    printf("      ./ASM_SamCO <SamCo_ASM_file> <Output_File>\n\n");
    printf("*Output filename is optional. Default output filename is: %s\n", DEFAULT_OUTPUT_FILENAME);
}

int
main(int argc, char **argv)
{
    if(argc != 2 && argc != 3)
    {
        print_usage();
        exit(EXIT_SUCCESS);
    }
    input_asm_filename = argv[1];
    if(argc == 3)
    {
        output_binary_filename = argv[2];
    }
    else if (argc == 2)
    {
        output_binary_filename = DEFAULT_OUTPUT_FILENAME;
    }

    open_files();

    //Main functionality
    assemble();

    close_files();

    exit(EXIT_SUCCESS);
}

/* End of file: main.c */
