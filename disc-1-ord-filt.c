#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"


#define MIN_ARGC (3)				/* minimal number of arguments */
#define COL_TIME (1)
#define COL_INPUT (2)

static void
usage()
{
     fprintf(stderr, "Usage: step [-a alpha_value] [-i initial_value ]\n");
}

long double
filter ( long double input, long double out_prev , long double a )
{
     long double output;

     output = input * ( 1 - a ) + a * out_prev;

     return output;
}

int
main(int argc, char *argv[])
{

     long double alpha = 0;	/* pole of the filter */
     long double out_prev = 0.0;
     long double out_cur;
     long double time;
     long double input;

     int opt;
     size_t linesize = 0;
     char *line = NULL;
     char *coltime = NULL, *colinput = NULL;

     if ( argc < MIN_ARGC ) {
	  usage();
	  exit(EXIT_FAILURE);
     }

     while ((opt = getopt(argc, argv, "a:i:")) != -1) {
	  switch (opt) {
	  case 'a':
	       alpha = strtold(optarg, NULL);
	       break;
	  case 'i':
	       out_prev = strtold(optarg, NULL);
	       break;
	  default: /* '?' */
	       usage();
	       exit(EXIT_FAILURE);
	  }
     }

     /* Input validation */

     if ( fabsl(alpha) > 1 ) {
	  fprintf(stderr, "Alpha constraint not respected: 0 < alpha < 1\n");
	  exit(EXIT_FAILURE);
     }

     while ( getline(&line, &linesize, stdin) > 0 ) {
	  coltime  = get_col(line, COL_TIME);
	  colinput = get_col(line, COL_INPUT);

	  if ( coltime  == NULL || colinput == NULL ) {
	       fprintf(stderr, "Could not allocate memory for column selection");
	       free(line);
	       exit(EXIT_FAILURE);
	  }

	  time = strtold(coltime, NULL);
	  input = strtold(colinput, NULL);

	  out_cur = filter ( input, out_prev , alpha );
	  out_prev = out_cur;

	  printf("%Lf\t%Lf\t%Lf\n", time, input, out_cur);


	  free(coltime);
	  free(colinput);
     }

     free(line);
     exit(EXIT_SUCCESS);
}
