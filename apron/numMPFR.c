/* ********************************************************************** */
/* numMPFR.c */
/* ********************************************************************** */

#include "numMPFR.h"
#include "num_internal.h"

/* NOTES:
   - use mpfr_set_default_prec to change the precision (global)
   - rounding is always towards +oo
*/

int numMPFR_snprint(char* s, int size, const numMPFR_t a)
{
  double d;
  /* special cases */
  if (mpfr_nan_p(a)) return ap_snprintf(s,size,"NaN");
  if (mpfr_inf_p(a)) return ap_snprintf(s,size,"%coo",mpfr_sgn(a)>0?'+':'-');
  if (mpfr_zero_p(a)) return ap_snprintf(s,size,"0");
  d = mpfr_get_d(a,GMP_RNDU);
  if (mpfr_cmp_d(a,d)==0) return ap_snprintf(s,size,"%.*g",NUMFLT_PRINT_PREC,d);
  else {
    /* general case */
    char* tmp;
    mp_exp_t e;
    int x,i;
    tmp = mpfr_get_str(NULL,&e,10,NUMFLT_PRINT_PREC,a,GMP_RNDU);
    if (!tmp) { return 0; }
    if (tmp[0]=='-' || tmp[0]=='+')
      x=ap_snprintf(s,size,"%c.%se+%ld",tmp[0],tmp+1,(long int)e);
    else
      x=ap_snprintf(s,size,".%se%+ld",tmp,(long int)e);
    mpfr_free_str(tmp);
    return x;
  }
}
void numMPFR_fprint(FILE* stream, const numMPFR_t a)
{
  char buf[256];
  char* str = buf;
  int n = numMPFR_snprint(str,256,a);
  if (n>=256){
    str = malloc(n);
    numMPFR_snprint(str,n+1,a);
  }
  fputs(str,stream);
  if (n>=256) free(str);
}

size_t numMPFR_serialize(void* dst, const numMPFR_t src)
{
  size_t count = 0;
  mp_exp_t e;
  mpz_t z;
  /* special cases */
  if (mpfr_nan_p(src)) { *((char*)dst) = 0; return 1; }
  if (mpfr_inf_p(src)) { *((char*)dst) = mpfr_sgn(src)>0?1:2 ; return 1; }
  /* normal case */
  mpz_init(z);
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(z,src);
  *((char*)dst) = mpz_sgn(z)>=0?3:4;
  mpz_export((char*)dst+9,&count,1,1,1,0,z);
  mpz_clear(z);
  /* XXX fails if count or exponent is more than 4-byte wide */
  assert(((unsigned)count)==count);
  assert(((int)e)==e);
  num_dump_word32((char*)dst+1,(unsigned)count);
  num_dump_word32((char*)dst+5,(unsigned)count);
  return count+9;

}

size_t numMPFR_deserialize(numMPFR_t dst, const void* src)
{
  size_t count;
  mp_exp_t e;
  mpz_t z;
  switch (*((const char*)src)) {
  case 0: mpfr_set_nan(dst); return 1;
  case 1: mpfr_set_inf(dst,1); return 1;
  case 2: mpfr_set_inf(dst,-1); return 1;
  case 3:
  case 4:
    count = num_undump_word32((const char*)src+1);
    e = (int)num_undump_word32((const char*)src+5);
    mpz_init(z);
    mpz_import(z,count,1,1,1,0,(const char*)src+9);
    mpfr_set_z(dst,z,GMP_RNDU);
    mpfr_mul_2si(dst,dst,e,GMP_RNDU);
    if (*((const char*)src)==4) mpfr_neg(dst,dst,GMP_RNDU);
    mpz_clear(z);
    return count+9;
  default:
    assert(0);
    return 1;
  }
}

 size_t numMPFR_serialize_array(void* dst, numMPFR_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_serialize((char*)dst+n,src[i]);
  return n;
}

 size_t numMPFR_deserialize_array(numMPFR_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_deserialize(dst[i],(const char*)src+n);
  return n;
}

 size_t numMPFR_serialized_size_array(numMPFR_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_serialized_size(src[i]);
  return n;
}
