#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <assert.h>

#define PRECISION (17) 

int main(int argc, char *argv[])
{
     double a = 1.23403;
     double b = 5.678; 

     assert( a > b );
     /* printf("a: %.*g\nb: %.*g", PRECISION, a, b); */
     printf("a: %2$.*1$g\n", PRECISION, a);
     printf("a: %2$.*1$g\tb: %3$.*1$g", PRECISION, a, b);
     putchar('\n');

     return 0;
}
