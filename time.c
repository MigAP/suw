#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"


#define DF_TIMESTEP (1e-6)		/* default timestep */
#define MIN_ARGC (4)				/* minimal number of arguments */

static void
usage()
{
     fprintf(stderr, "Usage: step [-i init_time] [-f final_time] [-s timestep]\n");
}



int
main(int argc, char *argv[])
{

     int opt;
     double ti=0.0, tf=1.0, ts=0.0;
     double t;

     if ( argc < MIN_ARGC ) {
	  usage();
	  exit(EXIT_FAILURE);
     }

     while ((opt = getopt(argc, argv, "i:f:s:")) != -1) {
	  switch (opt) {
	  case 'i':
	       ti = strtod(optarg, NULL);
	       break;
	  case 'f':
	       tf = strtod(optarg, NULL);
	       break;
	  case 's':
	       ts = strtod(optarg, NULL);
	       break;

	  default: /* '?' */
	       usage();
	       exit(EXIT_FAILURE);
	  }
     }

     /* Input validation */
     if ( tf <= ti ) {
	  fprintf(stderr, "final time must be superior to initial time\n");
	  exit(EXIT_FAILURE);
     }

     if ( is_equal(ts, 0.0) ) {
	  printf("Default timestep  %f is used\n", (double) DF_TIMESTEP);
	  ts = DF_TIMESTEP;
     }

     for (t=ti; !is_equal(t, tf+ts) ; t += ts) {
	  printf("%.*g\n",PRECISION, t);
     }
     exit(EXIT_SUCCESS);
}
