#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>

#include "z-tf.h"

#define COL_TIME (1)
#define COL_REF (2)

static void
usage()
{
     fprintf(stderr, "Usage: z-cl [-c controller transfer function]\n [-s system transfer function]\n [-i system initial output value]\n [-j controller initial output value ]\n");
}


double
compute_error(double ref, double output)
{
     return ref - output; 
}

int
main(int argc, char *argv[])
{

     /* system and controller transfer functions */
     struct ztf sf; 
     struct ztf cf; 
     size_t linesize; 
     char *line = NULL; 
     char *time_str = NULL, *reference_str = NULL;
     double s_init_v = 0.0, c_init_v = 0.0; 	/* intial output values */

     int opt;
     char* sfname = NULL, *cfname = NULL; 
     FILE *sfile = NULL, *cfile = NULL; 

     while ((opt = getopt(argc, argv, "c:s:i:j:")) != -1) {
	  switch (opt) {
	  case 'c':
	       /* TODO: free fname */
	       cfname = strdup(optarg); 
	       /* printf("File name passed by argument: %s\n", cfname);  */
	       break;
	  case 's':
	       /* TODO: free sfname */
	       sfname = strdup(optarg); 
	       /* printf("File name passed by argument: %s\n", sfname);  */
	       break;
	  case 'i':
	       s_init_v = strtod(optarg, NULL); 
	       /* printf("System initial value argument passed\n");  */
	       break;
	  case 'j':
	       c_init_v = strtod(optarg, NULL); 
	       /* printf("controller initial value argument passed\n");  */
	       break;
	  default: /* '?' */
	       usage();
	       fprintf(stderr, "Usage error\n"); 
	       exit(EXIT_FAILURE);
	  }
     }    
     /* Import system and controller transfer functions */
     if ( strlen(sfname) != 0 ) {
	  sfile = fopen(sfname, "r"); 
     }

     if ( sfile == NULL ) {
	  fprintf(stderr, "Unable to open system transfer function file\n"); 
     }
     ztf_import(&sf, sfile);
     fclose(sfile);
     free(sfname); 

     if ( strlen(cfname) != 0 ) {
	  cfile = fopen(cfname, "r"); 
     }

     if ( cfile == NULL ) {
	  fprintf(stderr, "Unable to open controller transfer function file\n"); 
     }
     ztf_import(&cf, cfile);
     fclose(cfile);
     free(cfname); 

     /* Initialize transfer function with null values for the inputs
      * and outputs */
     ztf_set_buf_input_vals(&sf, 0.0); 
     ztf_set_buf_output_vals ( &sf, s_init_v );

     ztf_set_buf_input_vals(&cf, 0.0); 
     ztf_set_buf_output_vals ( &cf, c_init_v );

     /* printf("Controller transfer function: \n"); */
     /* ztf_print(&cf); */
     /* printf("\n\n");  */

     /* printf("System transfer function: \n"); */
     /* ztf_print(&sf); */
     /* printf("\n\n");  */


     double time = 0.0, reference = 0.0, error = 0, control = 0;
     double output = s_init_v; 	/* intial system output */

     while ( getline(&line, &linesize, stdin) > 0 ) {
	  time_str = get_col(line, COL_TIME); 
	  reference_str = get_col(line, COL_REF); 

	  if ( time_str  == NULL || reference_str == NULL ) {
	       fprintf(stderr, "Unable to allocate memory for column selection");
	       free(line);
	       exit(EXIT_FAILURE);
	  }
	  time = strtod(time_str, NULL);
	  reference = strtod(reference_str, NULL);

	  error = compute_error(reference, output);
	  control = ztf_update_and_compute(&cf, error);
	  output = ztf_update_and_compute(&sf, control);

	  printf("%f\t%f\t%f\t%f\t%f\n", time, reference, error, control, output);

	  free(time_str);
	  free(reference_str);
     }

     ztf_free(&sf);
     ztf_free(&cf);

     return 0;
}
