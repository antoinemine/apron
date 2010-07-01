
#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true  (int)1
#define false (int)0

#define NUM_LONGDOUBLE
#include "num.h"
#include "bound.h"

int main(int argc, char**argv)
{
  bound_t a;
  double b,c,d;
  bound_init_set_int(a,-3);

  b = (double)1.0/(double)0.0;
  c = b;
  d = b+c;
  printf ("b=%.20g\nc=%.20g\nd=%.20g\n",b,c,d);
  c = 0;
  d = b*c;
  printf ("b=%.20g\nc=%.20g\nd=%.20g\n",b,c,d);
  
  return 0;
}

