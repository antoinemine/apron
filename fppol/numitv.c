/*
 * numitv.c
 *
 * Outward interval arithmetic with floating point arithmetic.
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

/*
 * In our implementation, to be more efficient, we switch once the rounding mode
 * and for all to rounding towards +\infty, that is, FE_UPWARD.
 * Details are depicted in Page 229 (Sect.7.5.1) of Antoine Mine's Phd thesis
 *    @phdthesis{mine:phd,
 *      author  = {Min\'e, A{.}},
 *      title   = {Weakly Relational Numerical Abstract Domains},
 *      school  = {\'Ecole Polytechnique},
 *      address = {Palaiseau, France},
 *      month   = {December},
 *      year    = {2004},
 *      note    = {\url{http://www.di.ens.fr/~mine/these/these-color.pdf}},
 *    }
 *
 * Note: The rounding mode FE_UPWARD must be set before calling the numitv_t functions.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */
#include <math.h>

#include "fpp_config.h"
#include "fpp_internal.h"
#include "numitv.h"

#if   defined ( NUM_DOUBLE )
#include "numflt_native.h" /* overflow are ok (stick to +oo), type not closed by + and / 2 */
#elif defined ( NUM_LONGDOUBLE )
#include "numflt_native.h"  /* overflow are ok (stick to +oo), type not closed by + and / 2 */
#endif

/* -a */
numitv_t numitv_neg(numitv_t a)
{
   numitv_t c;
   c.inf = -a.sup;
   c.sup = -a.inf;
   return c;
}

/* a+b */
numitv_t numitv_add(numitv_t a, numitv_t b)
{
   /* roundup now; */
   numitv_t c;
   c.inf = num_add_downward(a.inf,b.inf);
   c.sup = a.sup + b.sup;
   return c;
}

/* a-b */
numitv_t numitv_sub(numitv_t a,numitv_t b)
{
   /* roundup now; */
   numitv_t c;
   c.inf=num_sub_downward(a.inf,b.sup);
   c.sup=a.sup-b.inf;
   return c;
}

/* a*b */
numitv_t numitv_mul(numitv_t a,numitv_t b)
{
   /* roundup now; */
   numitv_t c;
   numdbl_t infc,supc,d;

   infc=num_mul_downward(a.inf,b.inf);
   d=num_mul_downward(a.inf,b.sup);
   infc=fmin(infc,d);
   d=num_mul_downward(a.sup,b.inf);
   infc=fmin(infc,d);
   d=num_mul_downward(a.sup,b.sup);
   infc=fmin(infc,d);
   c.inf=infc;

   supc=a.inf*b.inf;
   d=a.inf*b.sup;
   supc=fmax(supc,d);
   d=a.sup*b.inf;
   supc=fmax(supc,d);
   d=a.sup*b.sup;
   supc=fmax(supc,d);
   c.sup=supc;

   return c;
}

/* a/b: Assume 0.0 is not contained in b, otherwise overflow happens */
numitv_t numitv_div(numitv_t a,numitv_t b)
{
   /* roundup now; */
   numitv_t c;
   numdbl_t infc,supc,d;

   if(b.inf>0.0 || b.sup<-0.0){
     infc=num_div_downward(a.inf,b.inf);
     d=num_div_downward(a.inf,b.sup);
     infc=fmin(infc,d);
     d=num_div_downward(a.sup,b.inf);
     infc=fmin(infc,d);
     d=num_div_downward(a.sup,b.sup);
     infc=fmin(infc,d);
     c.inf=infc;

     supc=a.inf/b.inf;
     d=a.inf/b.sup;
     supc=fmax(supc,d);
     d=a.sup/b.inf;
     supc=fmax(supc,d);
     d=a.sup/b.sup;
     supc=fmax(supc,d);
     c.sup=supc;
     return c;
   }
   else{
     fprintf(stderr,"Error in numitv_Div(): 0.0 in contained in divisor interval.\n"); fflush(stdout);
     c.inf = -NUMFLT_MAX;
     c.sup = NUMFLT_MAX;
     return c;
   }
}

/* a+b */
numitv_t numitv_add_number(numdbl_t a,numdbl_t b)
{
   /* roundup now; */
   numitv_t c;
   c.inf=num_add_downward(a,b);
   c.sup=a+b;
   return c;
}

/* a-b */
numitv_t numitv_sub_number(numdbl_t a,numdbl_t b)
{
   /* roundup now; */
   numitv_t c;
   c.inf=num_sub_downward(a,b);
   c.sup=a-b;
   return c;
}

/* a*b */
numitv_t numitv_mul_number(numdbl_t a,numdbl_t b)
{
   /* roundup now; */
   numitv_t c;
   c.inf=num_mul_downward(a,b);
   c.sup=a*b;
   return c;
}

/* a/b: Assume 0.0 is not contained in b, otherwise overflow happens */
numitv_t numitv_div_number(numdbl_t a,numdbl_t b)
{
   /* roundup now; */
   numitv_t c;
   numdbl_t infc,supc,d;

   if(b>0.0 || b<-0.0){
     c.inf=num_div_downward(a,b);
     c.sup=a/b;
     return c;
   }
   else{
     fprintf(stderr,"Error in numitv_Div(): 0.0 in contained in divisor interval.\n");
     c.inf=-NUMFLT_MAX;
     c.sup=NUMFLT_MAX;
     return c;
   }
}

/* compute the interval ranges of a linear expression with $dim$ variables  */
numitv_t numitv_lexp_range(int dim, numdbl_t *lexp_coeff, numdbl_t *bounds)
{
   numitv_t sum;
   numitv_t ai,bnd;
   int i;

   sum.inf=0.0;
   sum.sup=0.0;
   for(i=0;i<dim;i++){
      if( *(lexp_coeff+i)!= 0.0  &&  *(lexp_coeff+i)!= -0.0){
         ai.inf=ai.sup=*(lexp_coeff+i);
         bnd.inf=*(bounds+2*i);
         bnd.sup=*(bounds+2*i+1);
         sum=numitv_add(sum,numitv_mul(ai,bnd));
      }
   }
   if(sum.inf==-0.0) sum.inf=0.0;
   if(sum.sup==-0.0) sum.sup=0.0;
   return sum;
}

numitv_t numitv_singleton(numdbl_t a)
{
   numitv_t c;
   c.inf=a;
   c.sup=a;
   return c;
}

void numitv_print(numitv_t a)
{
   fprintf(stdout,"[%.40f, %.40f]\n",a.inf,a.sup);
}

numdbl_t __attribute__ ((noinline)) num_add_downward(numdbl_t a, numdbl_t b)
{
   int rmode = fegetround();
   fesetround(FE_DOWNWARD);
   numdbl_t m = a + b;
   fesetround(rmode);
   return m;
}

numdbl_t __attribute__ ((noinline)) num_sub_downward(numdbl_t a, numdbl_t b)
{
   int rmode = fegetround();
   fesetround(FE_DOWNWARD);
   numdbl_t m = a - b;
   fesetround(rmode);
   return m;
}

numdbl_t __attribute__ ((noinline)) num_mul_downward(numdbl_t a, numdbl_t b)
{
   int rmode = fegetround();
   fesetround(FE_DOWNWARD);
   numdbl_t m = a*b;
   fesetround(rmode);
   return m;
}

numdbl_t __attribute__ ((noinline)) num_div_downward(numdbl_t a, numdbl_t b)
{
   int rmode = fegetround();
   fesetround(FE_DOWNWARD);
   numdbl_t m = a/b;
   fesetround(rmode);
   return m;
}

bool numitv_is_leq(numitv_t a,numitv_t b)
{
   if(a.inf>=b.inf && a.sup<=b.sup)
	    return true;
   else return false;
}

numdbl_t numitv_width(numitv_t a)
{
   if(a.sup==NUMFLT_MAX || a.inf==-NUMFLT_MAX)
	    return NUMFLT_MAX;
   else return (a.sup-a.inf);
}
