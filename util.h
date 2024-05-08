#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define EQUALITY_THRESHOLD (1e-12) /* value used to test floats equality */
#define TOKEN_DELIM " \t"	   /* whitespace or tab */
#define ERROR (-1) 
#define SUCCESS (0) 


unsigned int is_equal(double a, double b );
char *get_col(char *line, unsigned int n);

/* counts the number of tokens in string s, tokens are defined in
 * TOKEN_DELIM */
size_t count_tokens(char *s );


/* allocates an array of double da from a string s */
int str_to_darr( char *s, double **da , size_t *nda);



#endif /* UTIL_H */
