/* ********************************************************************** */
/* eitvXXX.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "eitvXXX.h"
#include "math.h"

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool eitvXXX_canonicalize(itvXXX_internal_t* intern,
		      eitvXXX_t a, bool integer)
{
  bool exc,cmp;

  if (integer){
    boundXXX_floor(a->itv->neginf,a->itv->neginf);
    boundXXX_floor(a->itv->sup,a->itv->sup);
    a->eq = false;
  }
  if (boundXXX_infty(a->itv->neginf) || boundXXX_infty(a->itv->sup)) return false;

  /* Check that it is not bottom */
  numXXX_neg(intern->canonicalize_num,boundXXX_numref(a->itv->neginf));
  cmp = boundXXX_cmp_num(a->itv->sup,intern->canonicalize_num);
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

void eitvXXX_mul_num(eitvXXX_t a, eitvXXX_t b, numXXX_t c)
{
  boundXXX_mul_num(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_mul_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (numXXX_sgn(c)<0){
    boundXXX_swap(a->itv->neginf,a->itv->sup);
    boundXXX_neg(a->itv->sup,a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->neginf);
  }
}

void eitvXXX_mul_bound(eitvXXX_t a, eitvXXX_t b, boundXXX_t c)
{
  assert (c!=a->itv->neginf && c!=a->itv->sup && c!=b->itv->neginf && c!=b->itv->sup);
  boundXXX_mul(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_mul(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (boundXXX_sgn(c)<0){
    boundXXX_swap(a->itv->neginf,a->itv->sup);
    boundXXX_neg(a->itv->sup,a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->neginf);
  }
}

void eitvXXX_div_num(eitvXXX_t a, eitvXXX_t b, numXXX_t c)
{
  boundXXX_div_num(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_div_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (numXXX_sgn(c)<0){
    boundXXX_swap(a->itv->neginf,a->itv->sup);
    boundXXX_neg(a->itv->sup,a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->neginf);
  }
}
void eitvXXX_div_bound(eitvXXX_t a, eitvXXX_t b, boundXXX_t c)
{
  assert (c!=a->itv->neginf && c!=a->itv->sup && c!=b->itv->neginf && c!=b->itv->sup);
  boundXXX_div(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_div(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
  if (boundXXX_sgn(c)<0){
    boundXXX_swap(a->itv->neginf,a->itv->sup);
    boundXXX_neg(a->itv->sup,a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->neginf);
  }
}
void eitvXXX_sub(eitvXXX_t a, eitvXXX_t b, eitvXXX_t c)
{
  if (a!=c){
    boundXXX_add(a->itv->sup,b->itv->sup,c->itv->neginf);
    if (NUMXXX_EXACT && b->eq && c->eq){
      boundXXX_neg(a->itv->neginf,a->itv->sup);
      a->eq = true;
    }
    else {
      boundXXX_add(a->itv->neginf,b->itv->neginf,c->itv->sup);
      a->eq = false;
    }
  } else {
    boundXXX_swap(a->itv->neginf,a->itv->sup);
    eitvXXX_add(a,b,c);
  }
}
void eitvXXX_abs(eitvXXX_t a, eitvXXX_t b)
{
  if (boundXXX_sgn(b->itv->neginf)<=0)
    /* positive interval */
    eitvXXX_set(a,b);
  else if (boundXXX_sgn(b->itv->sup)<=0)
    /* negative interval */
    eitvXXX_neg(a,b);
  else {
    boundXXX_max(a->itv->sup,b->itv->neginf,b->itv->sup);
    boundXXX_set_int(a->itv->neginf,0);
    a->eq = (boundXXX_sgn(a->itv->sup)==0);
  }
}


/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

void eitvXXX_mul(itvXXX_internal_t* intern, eitvXXX_t a, eitvXXX_t b, eitvXXX_t c)
{
  if (b->eq)
    eitvXXX_mul_bound(a,c,b->itv->sup);
  else if (c->eq){
    eitvXXX_mul_bound(a,b,c->itv->sup);
  }
  else {
    itvXXX_mul(intern,a->itv,b->itv,c->itv);
    a->eq = false;
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

void eitvXXX_div(itvXXX_internal_t* intern, eitvXXX_t a, eitvXXX_t b, eitvXXX_t c)
{
  if (b->eq)
    eitvXXX_div_bound(a,c,b->itv->sup);
  else if (c->eq)
    eitvXXX_div_bound(a,b,c->itv->sup);
  else {
    itvXXX_mul(intern,a->itv,b->itv,c->itv);
    a->eq = true;
  }
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */
