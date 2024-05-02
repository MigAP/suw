#define _POSIX_C_SOURCE 200809L

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
     long double ti=0.0, tf=0.0, ts=0.0;
     long double t;

     if ( argc < MIN_ARGC ) {
	  usage();
	  exit(EXIT_FAILURE);
     }

     while ((opt = getopt(argc, argv, "i:f:s:")) != -1) {
	  switch (opt) {
	  case 'i':
	       ti = strtold(optarg, NULL);
	       break;
	  case 'f':
	       tf = strtold(optarg, NULL);
	       break;
	  case 's':
	       ts = strtold(optarg, NULL);
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
	  printf("Default timestep  %Lf is used\n", (long double) DF_TIMESTEP);
	  ts = DF_TIMESTEP;
     }

     for (t=ti; !is_equal(t, tf+ts) ; t += ts) {
	  printf("%Lf\n", t);
     }
     exit(EXIT_SUCCESS);
}
