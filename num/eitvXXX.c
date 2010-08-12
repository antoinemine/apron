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
bool eitvXXX_canonicalize(itv_internal_t intern,
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
  if (NUMXXX_DIVEXACT && b->eq){
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
  if (NUMXXX_DIVEXACT && b->eq){
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

void eitvXXX_mul(itv_internal_t intern, eitvXXX_t a, eitvXXX_t b, eitvXXX_t c)
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

void eitvXXX_div(itv_internal_t intern, eitvXXX_t a, eitvXXX_t b, eitvXXX_t c)
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
/* Casts */
/* ********************************************************************** */

void eitvXXX_ceil(eitvXXX_t a, eitvXXX_t b)
{
  boundXXX_ceil(a->itv->sup,b->itv->sup);
  if (b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_floor(a->itv->neginf,b->itv->sup);
    a->eq = itvXXX_is_point(a->itv);
  }
}

void eitvXXX_floor(eitvXXX_t a, eitvXXX_t b)
{
  boundXXX_floor(a->itv->sup,b->itv->sup);
  if (b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_ceil(a->itv->neginf,b->itv->sup);
    a->eq = itvXXX_is_point(a->itv);
  }
}

void eitvXXX_trunc(eitvXXX_t a, eitvXXX_t b)
{
  itvXXX_trunc(a->itv,b->itv);
  a->eq = itvXXX_is_point(a->itv);
}

void eitvXXX_to_int(eitvXXX_t a, eitvXXX_t b)
{
  itvXXX_to_int(a->itv,b->itv);
  a->eq = itvXXX_is_point(a->itv);
}

void eitvXXX_to_float(itv_internal_t intern, eitvXXX_t a, eitvXXX_t b)
{
  itvXXX_to_float(intern,a->itv,b->itv);
  a->eq = itvXXX_is_point(a->itv);
}
void eitvXXX_to_double(itv_internal_t intern, eitvXXX_t a, eitvXXX_t b)
{
  itvXXX_to_double(intern,a->itv,b->itv);
  a->eq = itvXXX_is_point(a->itv);
}

/* ********************************************************************** */
/* Conversions */
/* ********************************************************************** */

bool eitvXXX_set_lint(eitvXXX_t a, long int b, num_internal_t intern)
{ a->eq = itvXXX_set_lint(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_llint(eitvXXX_t a, long long int b, num_internal_t intern)
{ a->eq = itvXXX_set_llint(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_mpz(eitvXXX_t a, mpz_t b, num_internal_t intern)
{ a->eq = itvXXX_set_mpz(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_lfrac(eitvXXX_t a, long int i, long int j, num_internal_t intern)
{ a->eq = itvXXX_set_lfrac(a->itv,i,j,intern); return a->eq; }
bool eitvXXX_set_llfrac(eitvXXX_t a, long long int i, long long int j, num_internal_t intern)
{ a->eq = itvXXX_set_llfrac(a->itv,i,j,intern); return a->eq; }
bool eitvXXX_set_mpq(eitvXXX_t a, mpq_t b, num_internal_t intern)
{ a->eq = itvXXX_set_mpq(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_double(eitvXXX_t a, double b, num_internal_t intern)
{ a->eq = itvXXX_set_double(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_ldouble(eitvXXX_t a, long double b, num_internal_t intern)
{ a->eq = itvXXX_set_ldouble(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_mpfr(eitvXXX_t a, mpfr_t b, num_internal_t intern)
{ a->eq = itvXXX_set_mpfr(a->itv,b,intern); return a->eq; }

bool eitvXXX_set_lint2(eitvXXX_t a, long int b, long int c, num_internal_t intern)
{ bool res = itvXXX_set_lint2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_llint2(eitvXXX_t a, long long int b, long long int c, num_internal_t intern)
{ bool res = itvXXX_set_llint2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_mpz2(eitvXXX_t a, mpz_t b, mpz_t c, num_internal_t intern)
{ bool res = itvXXX_set_mpz2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_lfrac2(eitvXXX_t a, long int i, long int j, long int k, long int l, num_internal_t intern)
{ bool res = itvXXX_set_lfrac2(a->itv,i,j,k,l,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_llfrac2(eitvXXX_t a, long long int i, long long int j, long long int k, long long int l, num_internal_t intern)
{ bool res = itvXXX_set_llfrac2(a->itv,i,j,k,l,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_mpq2(eitvXXX_t a, mpq_t b, mpq_t c, num_internal_t intern)
{ bool res = itvXXX_set_mpq2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_double2(eitvXXX_t a, double b, double c, num_internal_t intern)
{ bool res = itvXXX_set_double2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_ldouble2(eitvXXX_t a, long double b, long double c, num_internal_t intern)
{ bool res = itvXXX_set_ldouble2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }
bool eitvXXX_set_mpfr2(eitvXXX_t a, mpfr_t b, mpfr_t c, num_internal_t intern)
{ bool res = itvXXX_set_mpfr2(a->itv,b,c,intern); a->eq = false; if (res) eitvXXX_is_point(a); return res; }

MACRO_ALLZ
bool eitvXXX_set_numZZZ(eitvXXX_t a, numZZZ_t b, num_internal_t intern)
{ a->eq = itvXXX_set_numZZZ(a->itv,b,intern); return a->eq; }
bool eitvXXX_set_numZZZ2(eitvXXX_t a, numZZZ_t b, numZZZ_t c, num_internal_t intern)
{ bool res = itvXXX_set_numZZZ2(a->itv,b,c,intern); if (res) eitvXXX_is_point(a); return res; }
ENDMACRO
