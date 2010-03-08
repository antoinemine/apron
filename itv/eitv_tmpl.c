/* ********************************************************************** */
/* itv.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "eitv.h"
#include "math.h"

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool eitv_canonicalize(itv_internal_t* intern,
		      eitv_t a, bool integer)
{
  bool exc,cmp;

  if (integer){
    bound_floor(a->itv->neginf,a->itv->neginf);
    bound_floor(a->itv->sup,a->itv->sup);
    a->eq = false;
  }
  if (bound_infty(a->itv->neginf) || bound_infty(a->itv->sup)) return false;

  /* Check that it is not bottom */
  num_neg(intern->canonicalize_num,bound_numref(a->itv->neginf));
  cmp = bound_cmp_num(a->itv->sup,intern->canonicalize_num);
  if (cmp<0){
    a->eq = false;
    exc = true;
  }
  else {
    a->eq = (cmp==0);
    exc = false;
  }
  return exc;
}


/* ********************************************************************** */
/* Arithmetic operations */
/* ********************************************************************** */

/* We assume no aliasing between 

   - an itv and a num or a bound, 
*/

void eitv_mul_num(eitv_t a, eitv_t b, num_t c)
{
  bound_mul_num(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else { 
    bound_mul_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (num_sgn(c)<0){
    bound_swap(a->itv->neginf,a->itv->sup);
    bound_neg(a->itv->sup,a->itv->sup);
    bound_neg(a->itv->neginf,a->itv->neginf);
  }
}

void eitv_mul_bound(eitv_t a, eitv_t b, bound_t c)
{
  assert (c!=a->itv->neginf && c!=a->itv->sup && c!=b->itv->neginf && c!=b->itv->sup);
  bound_mul(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else { 
    bound_mul(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (bound_sgn(c)<0){
    bound_swap(a->itv->neginf,a->itv->sup);
    bound_neg(a->itv->sup,a->itv->sup);
    bound_neg(a->itv->neginf,a->itv->neginf);
  }
}

void eitv_div_num(eitv_t a, eitv_t b, num_t c)
{
  bound_div_num(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else { 
    bound_div_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (num_sgn(c)<0){
    bound_swap(a->itv->neginf,a->itv->sup);
    bound_neg(a->itv->sup,a->itv->sup);
    bound_neg(a->itv->neginf,a->itv->neginf);
  }
}
void eitv_div_bound(eitv_t a, eitv_t b, bound_t c)
{
  assert (c!=a->itv->neginf && c!=a->itv->sup && c!=b->itv->neginf && c!=b->itv->sup);
  bound_div(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else { 
    bound_div(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (bound_sgn(c)<0){
    bound_swap(a->itv->neginf,a->itv->sup);
    bound_neg(a->itv->sup,a->itv->sup);
    bound_neg(a->itv->neginf,a->itv->neginf);
  }
}
void eitv_sub(eitv_t a, eitv_t b, eitv_t c)
{
  if (a!=c){
    bound_add(a->itv->sup,b->itv->sup,c->itv->neginf);
    if (NUM_EXACT && b->eq && c->eq){
      bound_neg(a->itv->neginf,a->itv->sup);
      a->eq = true;
    }
    else { 
      bound_add(a->itv->neginf,b->itv->neginf,c->itv->sup);
      a->eq = false;
    }
  } else {
    bound_swap(a->itv->neginf,a->itv->sup);
    eitv_add(a,b,c);
  }
}
void eitv_abs(eitv_t a, eitv_t b)
{
  if (bound_sgn(b->itv->neginf)<=0)
    /* positive interval */
    eitv_set(a,b);
  else if (bound_sgn(b->itv->sup)<=0)
    /* negative interval */
    eitv_neg(a,b);
  else {
    bound_max(a->itv->sup,b->itv->neginf,b->itv->sup);
    bound_set_int(a->itv->neginf,0);
    a->eq = (bound_sgn(a->itv->sup)==0);
  }
}


/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

void eitv_mul(itv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c)
{
  if (b->eq)
    eitv_mul_bound(a,c,b->itv->sup);
  else if (c->eq){
    eitv_mul_bound(a,b,c->itv->sup);
  }
  else {
    itv_mul(intern,a->itv,b->itv,c->itv);
    a->eq = false;
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

void eitv_div(itv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c)
{
  if (b->eq)
    eitv_div_bound(a,c,b->itv->sup);
  else if (c->eq)
    eitv_div_bound(a,b,c->itv->sup);
  else {
    itv_mul(intern,a->itv,b->itv,c->itv);
    a->eq = true;
  }
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */

