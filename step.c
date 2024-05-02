#define _POSIX_C_SOURCE 200809L

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

     long double vi = 0.0, vf = 0.0;     /* intial and final values */
     long double st = 0.0;		 /* step time */
     long double time; 

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
	       vi = strtold(optarg, NULL);
	       break;
	  case 'f':
	       vf = strtold(optarg, NULL);
	       break;
	  case 't':
	       st = strtold(optarg, NULL);
	       break;

	  default: /* '?' */
	       usage();
	       exit(EXIT_FAILURE);
	  }
     }

     while ( getline(&line, &linesize, stdin) > 0 ) {
	  time = strtold(line, NULL); 

	  if ( time < st ) {
	       printf("%Lf\t%Lf\n", time, vi); 
	  } else {
	       printf("%Lf\t%Lf\n", time, vf);
	  }
     }

     free(line);
     exit(EXIT_SUCCESS);
}
