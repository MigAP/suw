#include "util.h"

/* Equality between floats: returns 1 if a == b, otherwise returns 0 */
unsigned int
is_equal(double a, double b )
{
     if ( fabsl(a - b) < EQUALITY_THRESHOLD ) {
	  return 1; 
     }
     return 0; 
}

/* Returns the n-th column of a string. Columns are assumed to be
 * separated by a tab and are 1-based indexed.*/
char *
get_col(char *line, unsigned int n)
{
     size_t i = 0, cnt = 0;
     unsigned int first = 0, last = 0; 
     char *col = NULL; 
     size_t len = 0;
     
     if ( n < 1 ) {
	  fprintf(stderr,"Columns are 1-based indexed\n"); 
	  return col;
     }

     /* find the starting and ending position of the n-th column  */
     while( cnt != n  ) {

	  if ( line[i] == '\t' || line[i] == '\n' ) {
	       cnt++; 

	       if ( cnt == n ) {
		    last = i - 1; 
	       } else {
		    first = i + 1; 
	       }
	  }
	  i++; 
     }

     len = last - first + 1; 
     col = (char *) malloc( (len+1)  * sizeof(char) ); /* includes '\0' */

     if ( col == NULL ) {
	  fprintf(stderr, "Could not allocate memory for column selection");
	  return col; 
     }
     /* Copy n-th column using from first to last inclusive*/
     for (i = 0; i < len; i++) {
	  col[i] = line[i + first];
     }
     col[len] = '\0'; 

     return col;
}

/* counts the number of tokens in string s, tokens are defined in
 * TOKEN_DELIM */
size_t
count_tokens(char *s )
{
     char *stok = NULL;  
     char *tok = NULL; 
     size_t ntok = 0;  

     stok = strdup(s); /* necessary because strtok modifies its input */
     tok = strtok(stok, TOKEN_DELIM);

     /* count number of double */
     while ( tok != NULL ) {
	  ntok++; 
	  /* find next token */
	  tok = strtok(NULL, TOKEN_DELIM);
     }

     free(stok); 

     return ntok; 
}

/* allocates an array of double da from a string s. It assumes that
 * *da == NULL */
int
str_to_darr( char *s, double **da , size_t *nda)
{

     char *stok = NULL;  	/* s duplicate necessary to use strtok()*/
     char *tok = NULL; 		/* pointer to tokens */
     char *endptr = NULL; 	/* used for strtod */
     size_t i = 0; 
     double val; 

     *nda = count_tokens( s );

     /* allocate array of doubles of size nda */
     if ( *nda != 0 && *da == NULL) {
	  *da = (double *) malloc( (*nda) * sizeof(double) ); 

	  if ( *da == NULL ) {
	       fprintf(stderr, "str_to_darr: unable to allocate array of doubles\n"); 
	       return ERROR; 
	  }

     } else {
	  fprintf(stderr, "str_to_darr: string has no tokens to parse\n");
	  return ERROR; 
     }

     /* duplicate s because strtok modifies its input */
     stok = strdup(s); 

     /* Look for the tokens, convert them to double and store them in
      * da */
     tok = strtok(stok, TOKEN_DELIM); 
     while ( tok != NULL ) {

	  errno = 0;    /* To distinguish success/failure after call */
	  val = strtod(tok, &endptr);

	  /* Check for various possible errors. */
	  if (errno != 0) {
	       fprintf(stderr, "str_to_darr: could not convert to double\n");
	       return ERROR;
	  }

	  if (endptr == tok) {
	       fprintf(stderr, "str_to_darr: no digits were found\n");
	       return ERROR;
	  }

	  *( (*da) + i ) = val;
	  /* printf("val: %f, **va: %f\n", val, *((*da) + i));  */
	  i++; 

	  /* find next token */
	  tok = strtok(NULL, TOKEN_DELIM); 
     }

     free(stok); 

     return SUCCESS; 
}
