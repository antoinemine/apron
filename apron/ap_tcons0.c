/* ************************************************************************* */
/* ap_tcons0.c: tree constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_tcons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void ap_tcons0_fprint(FILE* stream, ap_tcons0_t* cons, char** name_of_dim)
{
  ap_texpr0_fprint(stream,cons->texpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	     "\"ERROR in ap_tcons0_fprint\"")));
  if (cons->constyp == AP_CONS_EQMOD){
    fprintf(stream," mod ");
    mpq_out_str(stream,10,cons->mpq);
  }
}
void ap_tcons0_print(ap_tcons0_t* a, char** name_of_dim)
{ ap_tcons0_fprint(stdout,a,name_of_dim); }

ap_tcons0_t* ap_tcons0_make_unsat()
{
  ap_texpr0_t* expr = malloc(sizeof(ap_texpr0_t));
  expr->discr = AP_TEXPR_CST;
  ap_coeff_init(expr->val.cst,AP_SCALAR_D);
  ap_coeff_set_int(expr->val.cst,-1);
  return ap_tcons0_make(expr,AP_CONS_SUPEQ,NULL);
}

ap_tcons0_t* ap_tcons0_from_lincons0(ap_lincons0_t cons)
{
  ap_linexpr0_t linexpr0ref;
  ap_texpr0_t* texpr0;
  ap_constyp_t* constyp;
  mpq_ptr mpq;
  ap_lincons0_linexpr0ref(linexpr0ref,cons);
  texpr0 = ap_texpr0_from_linexpr0(linexpr0ref);
  constyp = ap_lincons0_constypref(cons);
  mpq =  ap_lincons0_mpqref(cons);
  return ap_tcons0_make(texpr0,*constyp,mpq);
}

bool ap_tcons0_equal(ap_tcons0_t* a1, ap_tcons0_t* a2)
{
  return
    a1==a2 ||
    (
	a1->constyp==a2->constyp &&
	(a1->constyp==AP_CONS_EQMOD ? mpq_equal(a1->mpq,a2->mpq) : true) &&
	ap_texpr0_equal(a1->texpr0,a2->texpr0)
    );
}
