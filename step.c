#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

#define MIN_ARGC (7)				/* minimal number of arguments */

static void
usage()
{
     fprintf(stderr, "Usage: step [-i initial_value] [-f final_value] [-t step time]\n");
}

int
main(int argc, char *argv[])
{

     double vi = 0.0, vf = 0.0;     /* intial and final values */
     double st = 0.0;		 /* step time */
     double time; 

     int opt; 
     size_t linesize = 0; 
     char *line = NULL; 

     if ( argc < MIN_ARGC ) {
	  usage();
	  exit(EXIT_FAILURE);
     }
     
     while ((opt = getopt(argc, argv, "i:f:t:")) != -1) {
	  switch (opt) {
	  case 'i':
	       vi = strtod(optarg, NULL);
	       break;
	  case 'f':
	       vf = strtod(optarg, NULL);
	       break;
	  case 't':
	       st = strtod(optarg, NULL);
	       break;

	  default: /* '?' */
	       usage();
	       exit(EXIT_FAILURE);
	  }
     }

     while ( getline(&line, &linesize, stdin) > 0 ) {
	  time = strtod(line, NULL); 

	  if ( time < st ) {
	       /* printf("%2$.*1$g\t%3$.*1$g\n", PRECISION, time, vi); */
	       printf("%.*g\t", PRECISION, time); 
	       printf("%.*g\n", PRECISION, vi); 
	  } else {
	       /* printf("%2$.*1$g\t%3$.*1$g\n", PRECISION, time, vf); */
	       printf("%.*g\t", PRECISION, time); 
	       printf("%.*g\n", PRECISION, vf); 
	  }
     }

     free(line);
     exit(EXIT_SUCCESS);
}
