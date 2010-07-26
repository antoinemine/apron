#ifndef _ITVCONFIG_H_
#define _ITVCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAS_TBOOL
#define HAS_TBOOL
/* Boolean with a third value */
typedef enum tbool_t {
  tbool_false=0,
  tbool_true=1,
  tbool_top=2,   /* don't know */
} tbool_t;

static inline tbool_t tbool_of_int(int a);
static inline tbool_t tbool_of_bool(bool a);
static inline tbool_t tbool_of_or(tbool_t a, tbool_t b);
static inline tbool_t tbool_of_and(tbool_t a, tbool_t b);
static inline tbool_t tbool_of_not(tbool_t a);

typedef enum itvlinexpr_type_t {
  ITV_LINEXPR_INTLINEAR,
  ITV_LINEXPR_QUASILINEAR,
  ITV_LINEXPR_LINEAR
} itvlinexpr_type_t;

typedef enum itvcoefftag_t {
  ITV_COEFF,          /* waiting for 1 ap_coeff_t* and a dimension */
  ITV_EITV,           /* waiting for 1 eitv_t and a dimension */
  ITV_NUM,            /* waiting for 1 num_t and a dimension */
  ITV_NUM2,           /* waiting for 2 num_t and a dimension */
  ITV_LINT,           /* waiting for 1 long int and a dimension */
  ITV_LINT2,          /* waiting for 2 long int and a dimension */
  ITV_LLINT,          /* waiting for 1 long long int and a dimension */
  ITV_LLINT2,         /* waiting for 2 long long int and a dimension */
  ITV_MPZ,            /* waiting for 1 mpz_t and a dimension */
  ITV_MPZ2,           /* waiting for 2 mpz_t and a dimension */
  ITV_LFRAC,          /* waiting for 2 long int and a dimension */
  ITV_LFRAC2,         /* waiting for 4 long int and a dimension */
  ITV_LLFRAC,         /* waiting for 2 long long int and a dimension */
  ITV_LLFRAC2,        /* waiting for 4 long long int and a dimension */
  ITV_MPQ,            /* waiting for 1 mpq_t and a dimension */
  ITV_MPQ2,           /* waiting for 2 mpq_t and a dimension */
  ITV_DOUBLE,         /* waiting for 1 double and a dimension */
  ITV_DOUBLE2,        /* waiting for 2 double and a dimension */
  ITV_LDOUBLE,        /* waiting for 1 long double and a dimension */
  ITV_LDOUBLE2,       /* waiting for 2 long double and a dimension */
  ITV_MPFR,           /* waiting for 1 mpfr_t and a dimension */
  ITV_MPFR2,          /* waiting for 2 mpfr_t double and a dimension */
  ITV_END
} itvcoefftag_t;

/* Datatype for type of constraints */
typedef enum itvconstyp_t {
  ITV_CONS_EQ,    /* equality constraint */
  ITV_CONS_SUPEQ, /* >= constraint */
  ITV_CONS_SUP,   /* > constraint */
  ITV_CONS_EQMOD, /* congruence equality constraint */
  ITV_CONS_DISEQ  /* disequality constraint */
} itvconstyp_t;

static inline tbool_t tbool_of_int(int a)
{ return ((a) ? tbool_true : tbool_false); }
static inline tbool_t tbool_of_bool(bool a)
{ return ((a) ? tbool_true : tbool_false); }
static inline tbool_t tbool_or(tbool_t a, tbool_t b)
{  return
    ( (a==tbool_true || b==tbool_true) ? tbool_true :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
	tbool_false ) ); }
static inline tbool_t tbool_and(tbool_t a, tbool_t b)
{
  return
    ( (a==tbool_false || b==tbool_false) ? tbool_false :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
	tbool_true ) );
}
static inline tbool_t tbool_not(tbool_t a)
{
  return
    a==tbool_false ? tbool_true :
    a==tbool_true  ? tbool_false : a;
}
#endif

#ifdef __cplusplus
}
#endif

#endif
