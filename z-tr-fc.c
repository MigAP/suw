#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define NUMERATOR_CHAR 'N'
#define DENOMINATOR_CHAR 'D'
#define TOKEN_DELIM " \t"	/* whitespace + tab */
#define ERROR (-1) 
#define SUCCESS (0) 


struct ztrfc {
     /* numerator and denominator of the transfert function */
     double *num;
     size_t n_num;

     double *denom;
     size_t n_denom;

     /* buffer of previous input and output values */
     double *buf_inputs;
     double *buf_outputs;
};

/* allocates space for the numerator and denominator arrays */
int
ztrfc_allocate( struct ztrfc *fc, size_t n_num, size_t n_denom )
{

     /* allocate numerator and denominator*/
     fc->num = (double *) malloc ( n_num * sizeof(double) );

     if ( fc->num == NULL ) {
	  return ERROR; 
     }
     fc->n_num = n_num;

     fc->denom = (double *) malloc ( n_denom * sizeof(double) );
     if ( fc->denom == NULL ) {
	  return ERROR; 
     }
     fc->n_denom = n_denom;

     /* allocate output and input buffers */
     fc->buf_inputs = (double *) malloc ( n_num * sizeof(double) );
     if ( fc->buf_inputs == NULL ) {
	  return ERROR; 
     }

     fc->buf_outputs = (double *) malloc ( n_denom * sizeof(double) );
     if ( fc->buf_outputs == NULL ) {
	  return ERROR; 
     }

     return SUCCESS;
}

int
ztrfc_set_num ( struct ztrfc *fc, double num[], size_t n_num )
{
     size_t i;

     if ( n_num != fc->n_num ) {
	  fprintf(stderr, "fc->n_num and n_num are not equal\n");
	  return ERROR;
     }
     for (i = 0; i < n_num; i++) {
	  fc->num[i] = num[i];
     }

     return SUCCESS;
}

int
ztrfc_set_denom ( struct ztrfc *fc, double denom[], size_t n_denom )
{
     size_t i;
     if ( n_denom != fc->n_denom ) {
	  fprintf(stderr, "fc->n_denom and n_denom are not equal\n");
	  return ERROR;
     }
     for (i = 0; i < n_denom; i++) {
	fc->denom[i] = denom[i];
     }
     return SUCCESS;
}

int
ztrfc_set_buf_inputs ( struct ztrfc *fc, double num[], size_t n_num )
{
     size_t i;

     if ( n_num != fc->n_num ) {
	  fprintf(stderr, "fc->n_num and num should match\n");
	  return ERROR;
     }

     for (i = 0; i < n_num; i++) {
	  fc->buf_inputs[i] = num[i];
     }

     return SUCCESS;
}


int
ztrfc_set_buf_outputs( struct ztrfc *fc, double denom[], size_t n_denom)
{
     size_t i;

     if (n_denom != fc->n_denom ) {
	  fprintf(stderr, "fc->n_denom and n_denom should match\n");
	  return ERROR;
     }

     for (i = 0; i < n_denom; i++) {
	  fc->buf_outputs[i] = denom[i];
     }

     return SUCCESS;
}

void
ztrfc_set_buf_input_vals ( struct ztrfc *fc, double val )
{
     size_t i;
     for (i = 0; i < fc->n_num ; i++) {
	  fc->buf_inputs[i] = val;
     }
}

void
ztrfc_set_buf_output_vals( struct ztrfc *fc, double val)
{
     size_t i;

     for (i = 0; i < fc->n_denom; i++) {
	  fc->buf_outputs[i] = val;
     }

}


void
ztrfc_print( struct ztrfc *fc )
{
     int i;

     printf("N:\t");
     for (i = 0; i < fc->n_num; i++) {
	  printf("%f\t", fc->num[i]);
     }

     printf("\nD:\t");

     for (i = 0; i < fc->n_denom; i++) {
	  printf("%f\t", fc->denom[i]);
     }

     printf("\nInput buffer:\t");

     for (i = 0; i < fc->n_num; i++) {
	  printf("%f\t", fc->buf_inputs[i]);
     }

     printf("\nOuput buffer:\t");

     for (i = 0; i < fc->n_denom; i++) {
	  printf("%f\t", fc->buf_outputs[i]);
     }
     putchar('\n'); 
}

void
ztrfc_free( struct ztrfc *fc )
{
     free(fc->num);
     free(fc->denom);
     free(fc->buf_inputs);
     free(fc->buf_outputs);
}

/* updates the input buffer of "fc" by adding "input" at the beginning
 * of the buffer. */
void
ztrfc_update_inputs( struct ztrfc *fc, double input )
{
     size_t i;

     for (i = (fc->n_num -1); i > 0; i--) {
	  fc->buf_inputs[i] = fc->buf_inputs[i-1];
     }

     fc->buf_inputs[0] = input;
}

/* updates the output buffer of "fc" by adding "input" at the
 * beginning of the buffer. */
void
ztrfc_update_outputs( struct ztrfc *fc, double input )
{
     size_t i;

     for (i = (fc->n_denom -1); i > 0; i--) {
	  fc->buf_outputs[i] = fc->buf_outputs[i-1];
     }

     fc->buf_outputs[0] = input;
}

/* the new input is assumed to be already in fc, so there is no need
 * to update fc buffers */
double
compute_output(struct ztrfc *fc, double input)
{
     size_t i;
     double output = 0.0;

     for (i = 0; i < fc->n_num; i++) {
	  output += fc->num[i] * fc->buf_inputs[i];
     }

     /* buf_outputs[i-1] is used because it has not been updated
      * yet */
     for (i = 1; i < fc->n_denom; i++) {
	  output -= fc->denom[i] * fc->buf_outputs[i-1];
     }


     output = output / fc->denom[0];

     return output;
}

/* updates fc with the new input, computes the output and updates fc
 * with the new output */
double
update_and_compute( struct ztrfc *fc, double input )
{
     double output;

     ztrfc_update_inputs(fc, input );
     output = compute_output(fc, input);
     ztrfc_update_outputs(fc, output );

     return output;
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

/* allocates an array of double da from a string s */
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
     if ( *nda != 0 ) {
	  *da = (double *) malloc( (*nda) * sizeof(double) ); 

	  if ( *da == NULL ) {
	       fprintf(stderr, "Could not allocate double array\n"); 
	       return ERROR; 
	  }

     } else {
	  fprintf(stderr, "String has no tokens to parse\n");
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
	       fprintf(stderr, "Could not convert to double\n");
	       return ERROR;
	  }

	  if (endptr == tok) {
	       fprintf(stderr, "No digits were found\n");
	       return ERROR;
	  }

	  *( (*da) + i ) = val;
	  /* printf("val: %f, **va: %f\n", val, *((*da) + i));  */
	  i++; 

	  /* find next token */
	  tok = strtok(NULL, TOKEN_DELIM); 
     }
     return SUCCESS; 
}

/* TODO: finish function */
int
ztrfc_import(struct ztrfc *fc, FILE *f )
{
     char *lineptr = NULL;
     size_t line_size;
     double *num = NULL, *denom = NULL; 
     size_t nnum, ndenom; 

     /* import the numerator and deminator coefficients from FILE */
     while ( getline(&lineptr, &line_size, f) != EOF ) {

	  /* remove end of line '\n' character */
	  lineptr[strcspn(lineptr, "\n")] = '\0'; 

	  if ( *lineptr == NUMERATOR_CHAR ) {
	       if( str_to_darr( ++lineptr, &num , &nnum) ) {
		    /* TODO: print file name */
		    fprintf(stderr, "Could not parse line from file ... \n"); 
		    return ERROR;
	       }
	  } else if ( *lineptr == DENOMINATOR_CHAR ) {
	       /* printf("Denominator: \n");  */
	       /* printf("size: %ld, string: %s\n", line_size, (lineptr+1)); */

	       if( str_to_darr( ++lineptr, &denom , &ndenom) ) {
		    /* TODO: print file name */
		    fprintf(stderr, "Could not parse line from file ... \n"); 
		    return ERROR;
	       }
	  } else {
	       fprintf(stderr, "Unknown line identifier %s: \n", lineptr); 
	       return ERROR;
	  }

     }

     if ( ztrfc_allocate(fc, nnum, ndenom) ) {
	  fprintf(stderr, "Could not allocate transfer function\n"); 
	  return ERROR; 
     }

     ztrfc_set_num(fc, num, nnum);
     ztrfc_set_denom(fc, denom, ndenom);

     return SUCCESS; 
}

int
main(int argc, char *argv[])
{

     struct ztrfc fc;
     double output;
     double input = 42.0;

     /* double num[] = { 1.0, 2.0 };  */
     /* unsigned int n_num = 2;   */
     /* double denom[] = { 1.0, 4.0, 5.0};  */
     /* unsigned int n_denom = 3;   */

     /* double num_buf[] = { 6.0, 7.0 };  */
     /* double denom_buf[] = { 8.0, 9.0 , 10.0};  */

     /* /\* Initialization of the transfert function *\/ */
     /* ztrfc_allocate( &fc, 2, 3); */
     /* ztrfc_set_num(&fc, num, n_num);  */
     /* ztrfc_set_denom(&fc, denom, n_denom);  */
     /* ztrfc_set_buf_inputs ( &fc, num_buf, n_num ); */
     /* ztrfc_set_buf_outputs( &fc, denom_buf, n_denom); */

     /* ztrfc_print( fc ); */
     /* putchar('\n');  */
     /* putchar('\n');  */

     /* ztrfc_free(&fc); */

     /* import z-transfer function from file */
     FILE *f_fc;
     f_fc = fopen("z_fct.txt","r");
     if (f_fc == NULL ) {
	  fprintf(stderr, "Could not open the file\n");
	  exit(EXIT_FAILURE);
     }
     ztrfc_import(&fc, f_fc);
     fclose(f_fc);
     printf("Imported transfer function: \n"); 
     ztrfc_print( &fc );
     putchar('\n');
     putchar('\n');


     /* initialize input and output buffer */
     ztrfc_set_buf_inputs_val( &fc, 1.0);
     ztrfc_set_buf_outputs_val( &fc, 2.0);

     printf("Input and output buffers initialization \n"); 
     ztrfc_print( &fc );
     putchar('\n');
     putchar('\n');

     /* Measurement update  */
     ztrfc_update_buf_inputs( &fc, input );

     /* Ouput computation and transfert function update */
     output = compute_output(&fc, input);
     ztrfc_update_buf_outputs( &fc, output );
     printf("New state:\n");


     ztrfc_print( &fc );
     putchar('\n');
     putchar('\n');

     /* Everything done before but in one function call */
     printf("New new state:\n");

     update_and_compute( &fc, 54.0 );

     ztrfc_print( &fc );
     putchar('\n');
     putchar('\n');

     ztrfc_free(&fc);

     putchar('\n');
     return 0;
}
