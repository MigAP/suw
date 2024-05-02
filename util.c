#include "util.h"

/* Equality between floats: returns 1 if a == b, otherwise returns 0 */
unsigned int
is_equal(long double a, long double b )
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
