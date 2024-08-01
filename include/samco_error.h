#ifndef SAMCO_ERROR_H
#define SAMCO_ERROR_H

#define EXIT_FAIL       1
#define EXIT_SUCCESS    0

void fatal_error(char * message);
void error_at_line(int line_index);

#endif /* SAMCO_ERROR_H */
