/*
 * File name: samco_error.c
 * Description: Error functions
 *
 * Notes:
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE *output_binary_fd;
extern char *output_binary_filename;

void fatal_error(char * message)
{
    printf("ERROR: %s\n", message);
    fclose(output_binary_fd);
    output_binary_fd = fopen(output_binary_filename, "w");
    exit(EXIT_FAILURE);
}

void error_at_line(int line)
{
    printf("ERROR: at line %d\n", line);
    exit(EXIT_FAILURE);
}

/* End of file: samco_error.c */
