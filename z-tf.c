#include "z-tf.h"

/* allocates space for the numerator and denominator arrays */
int
ztf_allocate( struct ztf *fc, size_t n_num, size_t n_denom )
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
ztf_set_num ( struct ztf *fc, double num[], size_t n_num )
{
     size_t i;

     if ( num == NULL ) {
	  fprintf(stderr, "num[] points to NULL");
	  return ERROR;
     }

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
ztf_set_denom ( struct ztf *fc, double denom[], size_t n_denom )
{
     size_t i;
     if ( denom == NULL) {
	  fprintf(stderr, "denom points to NULL");
	  return ERROR;
     }

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
ztf_set_buf_inputs ( struct ztf *fc, double num[], size_t n_num )
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
ztf_set_buf_outputs( struct ztf *fc, double denom[], size_t n_denom)
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
ztf_set_buf_input_vals ( struct ztf *fc, double val )
{
     size_t i;
     for (i = 0; i < fc->n_num ; i++) {
	  fc->buf_inputs[i] = val;
     }
}

void
ztf_set_buf_output_vals( struct ztf *fc, double val)
{
     size_t i;

     for (i = 0; i < fc->n_denom; i++) {
	  fc->buf_outputs[i] = val;
     }

}


void
ztf_print( struct ztf *fc )
{
     size_t i;

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
ztf_free( struct ztf *fc )
{
     free(fc->num);
     free(fc->denom);
     free(fc->buf_inputs);
     free(fc->buf_outputs);
}

/* updates the input buffer of "fc" by adding "input" at the beginning
 * of the buffer. */
void
ztf_update_inputs( struct ztf *fc, double input )
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
ztf_update_outputs( struct ztf *fc, double input )
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
ztf_compute_output(struct ztf *fc)
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
ztf_update_and_compute( struct ztf *fc, double input )
{
     double output;

     ztf_update_inputs(fc, input );
     output = ztf_compute_output(fc);
     ztf_update_outputs(fc, output );

     return output;
}





int
ztf_import(struct ztf *fc, FILE *f )
{
     char *lineptr = NULL;
     size_t line_size;
     double *num = NULL, *denom = NULL; 
     size_t nnum = 0, ndenom = 0; 
     int rval = SUCCESS; 	/* return value */

     /* import the numerator and deminator coefficients from FILE */
     while ( getline(&lineptr, &line_size, f) != EOF ) {

	  /* remove end of line '\n' character */
	  lineptr[strcspn(lineptr, "\n")] = '\0';

	  if ( *lineptr == NUMERATOR_CHAR ) {
	       if( str_to_darr( (lineptr + 1), &num , &nnum) ) {
		    fprintf(stderr, "ztf_import: could not parse line from file\n"); 
		    rval =  ERROR;
	       }
	  } else if ( *lineptr == DENOMINATOR_CHAR ) {
	       if( str_to_darr( (lineptr + 1), &denom , &ndenom) ) {
		    fprintf(stderr, "ztf_import: could not parse line from file\n"); 
		    rval =  ERROR;
	       }
	  } else {
	       fprintf(stderr, "ztf_import: unknown line identifier %s: \n", lineptr); 
	       rval =  ERROR;
	  }

     }
     free(lineptr);

     if ( ztf_allocate(fc, nnum, ndenom) ) {
	  fprintf(stderr, "ztf_import: unable to allocate transfer function\n"); 
	  return ERROR; 
     }

     ztf_set_num(fc, num, nnum);
     ztf_set_denom(fc, denom, ndenom);

     free(num);
     free(denom); 

     return rval; 
}
