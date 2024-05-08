#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#include "util.h"
#include "z-tf.h"

#define COL_TIME (1)
#define COL_INPUT (2)

int
main(int argc, char *argv[])
{

     struct ztf fc;
     double time, input, output;  
     size_t linesize = 0; 
     char *line = NULL; 
     char *time_str = NULL, *input_str = NULL;
     double i_out = 0.0;  		/* intial output */

     int opt; 
     char *fname = NULL; 
     FILE *f_fc = NULL;

     while ((opt = getopt(argc, argv, "f:i:")) != -1) {
	  switch (opt) {
	  case 'f':
	       fname = strdup(optarg); 
	       printf("File name passed by argument: %s\n", fname); 
	       break;
	  case 'i':
	       i_out = strtod(optarg, NULL);
	       break;

	  default: /* '?' */
	       /* TODO: create usage function */
	       fprintf(stderr, "Usage error\n"); 
	       exit(EXIT_FAILURE);
	  }
     }

     /* Import transfer function */
     if ( strlen(fname) != 0 ) {
	  f_fc = fopen(fname,"r");
     }

     if (f_fc == NULL ) {
	  fprintf(stderr, "Could not open the file\n");
	  exit(EXIT_FAILURE);
     }
     ztf_import(&fc, f_fc);
     free(fname); 
     fclose(f_fc);
     printf("Imported transfer function: \n"); 
     ztf_print( &fc );
     putchar('\n');
     putchar('\n');


     /* Initialize transfer function: initial input and output values
      * are assumed to be equal to zero */
     ztf_set_buf_input_vals( &fc, 0.0);
     ztf_set_buf_output_vals( &fc, i_out);

     printf("Input and output buffers initialization \n"); 
     ztf_print( &fc );
     putchar('\n');
     putchar('\n');

     while ( getline(&line, &linesize, stdin) > 0 ) {
	  time_str  = get_col(line, COL_TIME);
	  input_str = get_col(line, COL_INPUT);

	  if ( time_str  == NULL || input_str == NULL ) {
	       fprintf(stderr, "Could not allocate memory for column selection");
	       free(line);
	       exit(EXIT_FAILURE);
	  }

	  /* TODO: See z-tr-fc for error handling */
	  time = strtod(time_str, NULL);
	  input = strtod(input_str, NULL);
	  output = ztf_update_and_compute( &fc, input );

	  printf("%f\t%f\t%f\n", time, input, output);

	  free(input_str); 
	  free(time_str);
     }
     free(line); 

     ztf_free(&fc);

     exit(EXIT_SUCCESS);
}
