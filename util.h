#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EQUALITY_THRESHOLD (1e-12)		/* value used to test floats equality */

unsigned int is_equal(double a, double b );
char *get_col(char *line, unsigned int n);


#endif /* UTIL_H */
