#include <stdio.h>
#include <assert.h>
#include "../util.h"
#include "../z-utils.h"


int main()
{

     /* util.h tests */
     assert(is_equal(1.0, 1.0));
     assert( count_tokens("a\tb\tc") == 3); ;

     /* z-utils tests */
     struct ztf sf;		 /* system transfert function */
     double s_n[] = { 1.0 };     /* numerator */
     unsigned int s_nn = 1;	 /* numerator size */
     double s_d[] = { 1.0, 1.0}; /* denominator */
     unsigned int s_nd= 2;	 /* denominator size */

     ztf_allocate(&sf, s_nn, s_nd);
     ztf_set_num(&sf, s_n, s_nn);
     ztf_set_denom(&sf, s_d, s_nd);

     double initial_value = 0.0;
     ztf_set_buf_input_vals(&sf, initial_value);
     ztf_set_buf_output_vals(&sf, initial_value);

     /* printf("System transfer function\n"); */
     /* ztf_print( &sf ); */
     /* putchar('\n'); */
     /* putchar('\n'); */


     size_t i;

     for (i = 0; i < s_nn; i++) {
	  assert( is_equal(s_n[i], sf.num[i]) );
     }

     for (i = 0; i < s_nd; i++) {
	  assert( is_equal(s_d[i], sf.denom[i]) );
     }

     for (i = 0; i < sf.n_num; i++) {
	  assert( is_equal(sf.buf_inputs[i], initial_value) );
     }

     for (i = 0; i < sf.n_denom; i++) {
	  assert( is_equal(sf.buf_outputs[i], initial_value) );
     }

     assert( is_equal(ztf_compute_output(&sf), 0) );

     double input = 1.0;
     double output;

     ztf_update_inputs(&sf, input);
     output = ztf_compute_output(&sf);
     assert( is_equal(output, 1.0) );

     ztf_update_outputs(&sf, output);

     input = 0.0;
     ztf_update_inputs(&sf, input);
     output =  ztf_compute_output(&sf);
     assert( is_equal(output, -1.0 ) );

     ztf_set_buf_input_vals(&sf, 0.0);
     ztf_set_buf_output_vals(&sf, 0.0);

     input = 2.0;
     output = ztf_update_and_compute(&sf, input);

     assert( is_equal(output, 2.0) );
     ztf_free(&sf);

     struct ztf cf;		       /* controller transfert function */
     double c_n[] = { 1.0, 2.0 };      /* numerator */
     unsigned int c_nn = 2;	       /* numerator size */
     double c_d[] = { 1.0, 3.0, 4.0};  /* denominator */
     unsigned int c_nd= 3;	       /* denominator size */
     double c_i_i[] = {1.0, 2.0};      /* initial input buffer */
     double c_i_o[] = {3.0, 4.0, 5.0}; /* initial output buffer */

     ztf_allocate(&cf, c_nn, c_nd);
     ztf_set_num(&cf, c_n, c_nn);
     ztf_set_denom(&cf, c_d, c_nd);
     ztf_set_buf_inputs(&cf, c_i_i, c_nn);
     ztf_set_buf_outputs(&cf, c_i_o, c_nd);

     /* printf("Controller transfer function\n"); */
     /* ztf_print( &cf ); */
     /* putchar('\n'); */
     /* putchar('\n'); */

     input = 6.0;
     output = ztf_update_and_compute(&cf, input); 
     double answer = -17; 
     assert( is_equal(output, answer) ); 
     ztf_free(&cf);

     FILE *f_tf = NULL; 
     struct ztf tf_imported;
     f_tf = fopen("test_tf.txt","r");
     if (f_tf == NULL ) {
	  fprintf(stderr, "Could not open the file\n");
	  exit(EXIT_FAILURE);
     }
     ztf_import(&tf_imported, f_tf);
     fclose(f_tf);
     ztf_set_buf_input_vals( &tf_imported, 0.0);
     ztf_set_buf_output_vals( &tf_imported, 1.0);

     /* printf("Imported transfer function\n"); */
     /* ztf_print( &tf_imported ); */
     /* putchar('\n'); */
     /* putchar('\n'); */


     input = 1.0;
     output = ztf_update_and_compute(&tf_imported, input); 

     assert( is_equal(output, -1.0) ); 

     ztf_free(&tf_imported); 

     return 0;
}
