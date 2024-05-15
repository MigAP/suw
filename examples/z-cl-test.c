#include <stdio.h>

#include "z-tf.h"

double
compute_error(double ref, double output)
{
     return ref - output; 
}

int
main(int argc, char *argv[])
{

     /* system transfer function */
     struct ztf sf; 
     double s_n[] = { 0.05 }; /* numerator */
     unsigned int s_nn = 1;	  /* numerator size */
     double s_d[] = { 1.0, -0.95}; /* denominator */
     unsigned int s_nd= 2;	      /* denominator size */

     ztf_allocate(&sf, s_nn, s_nd); 
     ztf_set_num(&sf, s_n, s_nn); 
     ztf_set_denom(&sf, s_d, s_nd); 
     ztf_set_buf_input_vals(&sf, 0.0); 
     ztf_set_buf_output_vals(&sf, 0.0); 

     printf("System transfer function\n"); 
     ztf_print( &sf );
     putchar('\n');
     putchar('\n');

     /* controller transfer function */
     struct ztf cf; 
     double c_n[] = { 2.0 }; /* numerator */
     unsigned int c_nn = 1;	  /* numerator size */
     double c_d[] = { 1.0 }; /* denominator */
     unsigned int c_nd= 1;	      /* denominator size */

     ztf_allocate(&cf, c_nn, c_nd); 
     ztf_set_num(&cf, c_n, c_nn); 
     ztf_set_denom(&cf, c_d, c_nd); 
     ztf_set_buf_input_vals(&cf, 0.0); 
     ztf_set_buf_output_vals(&cf, 0.0); 

     printf("System transfer function\n"); 
     ztf_print( &cf );
     putchar('\n');
     putchar('\n');

     /* Given a new reference we must:
      * 1. update and compute controller input and output
      * 2. update and compute system input and output 
      */
     unsigned int i, steps = 10; 
     double reference = 1; 
     double output = 0.0; 
     double error = 0, control = 0; 

     for (i = 0; i < steps; i++) {
	  error = compute_error(reference, output);  
	  control = ztf_update_and_compute(&cf, error); 
	  output = ztf_update_and_compute(&sf, control); 
	  printf("step: %d, reference: %f, error: %f, control: %f, output: %f\n",
		 i, reference, error, control, output); 

     }

     ztf_free(&sf);
     ztf_free(&cf);

     return 0;
}
