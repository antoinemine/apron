/* -*- mode: c -*- */

/* ********************************************************************** */
/* ap_lingenXXX.c: */
/* ********************************************************************** */

#include "ap_lingenXXX.h"

#define _ITVXXX_MARK_

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

ap_lingenXXX_ptr ap_lingenXXX_alloc(size_t size)
{
  ap_lingenXXX_ptr res = (ap_lingenXXX_ptr)malloc(sizeof(ap_lingenXXX_struct));
  ap_lingenXXX_init(res,size);
  return res;
}
ap_lingenXXX_ptr ap_lingenXXX_alloc_set(ap_lingenXXX_t cons)
{
  ap_lingenXXX_ptr res = (ap_lingenXXX_ptr)malloc(sizeof(ap_lingenXXX_struct));
  ap_lingenXXX_init_set(res,cons);
  return res;
}
ap_lingenXXX_ptr ap_lingenXXX_alloc_set_linexpr(ap_linexprXXX_t expr, ap_gentyp_t gentyp)
{
  ap_lingenXXX_ptr res = (ap_lingenXXX_ptr)malloc(sizeof(ap_lingenXXX_struct));
  ap_lingenXXX_init_set_linexpr(res,expr,gentyp);
  return res;
}
void ap_lingenXXX_free(ap_lingenXXX_ptr e)
{
  ap_lingenXXX_clear(e);
  free(e);
}

void ap_lingenXXX_fprint(FILE* stream, ap_lingenXXX_t gen, char** name)
{
  char* str = NULL;
  switch(gen->gentyp){
  case AP_GEN_LINE:
    str = "LINE:    ";
    break;
  case AP_GEN_RAY:
    str = "RAY:     ";
    break;
  case AP_GEN_VERTEX:
    str = "VERTEX:  ";
    break;
  case AP_GEN_LINEMOD:
    str = "LINEMOD: ";
    break;
  case AP_GEN_RAYMOD:
    str = "RAYMOD:  ";
    break;
  default:
    abort();
  }
  fprintf(stream,"%s",str);
  ap_linexprXXX_fprint(stream,gen->linexpr,name);
}

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

bool ap_lingenXXX_equal(ap_lingenXXX_t gen1,ap_lingenXXX_t gen2)
{
  return gen1->gentyp==gen2->gentyp &&
    ap_linexprXXX_equal(gen1->linexpr,gen2->linexpr);
}

/* Lexicographic ordering, terminating by constant coefficients */
int ap_lingenXXX_compare(ap_lingenXXX_t gen1, ap_lingenXXX_t gen2)
{
  int res;
  res =
    gen1->gentyp > gen2->gentyp ? 1 :
    (gen1->gentyp==gen2->gentyp ? 0 : -1);
  if (!res){
    res = ap_linexprXXX_compare(gen1->linexpr,gen2->linexpr);
  }
  return res;
}

#undef _ITVXXX_MARK_
