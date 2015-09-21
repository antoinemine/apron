/* ************************************************************************* */
/* ap_texpr0.c: tree expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_texpr0.h"
#include "ap_linearize.h"

#include <stdarg.h>

/* ====================================================================== */
/* I. Constructors and Destructors */
/* ====================================================================== */

ap_texpr0_t* ap_texpr0_cst(ap_coeff_t* coeff)
{
  ap_texpr0_t* res = malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init_set(&res->val.cst,coeff);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar(ap_scalar_t* scalar)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar(&res->val.cst, scalar);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar_mpq(mpq_t mpq)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar_mpq(&res->val.cst, mpq);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar_mpfr(mpfr_t mpfr)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar_mpfr(&res->val.cst, mpfr);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar_int(long int num)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar_int(&res->val.cst, num);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar_frac(long int num, unsigned long int den)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar_frac(&res->val.cst, num, den);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_scalar_double(double num)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_SCALAR);
  ap_coeff_set_scalar_double(&res->val.cst, num);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval(ap_interval_t* itv)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval(&res->val.cst, itv);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_scalar(ap_scalar_t* inf, ap_scalar_t* sup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_scalar(&res->val.cst, inf, sup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_mpq(mpq_t inf, mpq_t sup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_mpq(&res->val.cst, inf, sup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_mpfr(mpfr_t inf, mpfr_t sup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_mpfr(&res->val.cst, inf, sup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_int(long int inf, long int sup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_int(&res->val.cst, inf, sup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_frac(long int numinf, unsigned long int deninf,
					 long int numsup, unsigned long int densup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_frac(&res->val.cst, numinf, deninf, numsup, densup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_double(double inf, double sup)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_double(&res->val.cst, inf, sup);
  return res;
}
ap_texpr0_t* ap_texpr0_cst_interval_top(void)
{
  ap_texpr0_t* res = (ap_texpr0_t*) malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_CST;
  ap_coeff_init(&res->val.cst, AP_COEFF_INTERVAL);
  ap_interval_set_top(res->val.cst.val.interval);
  return res;
}
ap_texpr0_t* ap_texpr0_dim(ap_dim_t dim)
{
  ap_texpr0_t* res = malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_DIM;
  res->val.dim = dim;
  return res;
}
ap_texpr0_t* ap_texpr0_node(ap_texpr_op_t op, ap_texpr_rtype_t type, ap_texpr_rdir_t dir, ap_texpr0_t* opA, ap_texpr0_t* opB)
{
  ap_texpr0_node_t* node = malloc(sizeof(ap_texpr0_node_t));
  node->op = op;
  node->type = type;
  node->dir = dir;
  node->exprA = opA;
  node->exprB = opB;
  ap_texpr0_t* res = malloc(sizeof(ap_texpr0_t));
  res->discr = AP_TEXPR_NODE;
  res->val.node = node;
  return res;
}
ap_texpr0_t* ap_texpr0_unop(ap_texpr_op_t op, ap_texpr0_t* opA, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  if (!ap_texpr_is_unop(op)){
    fprintf(stderr,"ap_texpr0.c: ap_texpr0_unop: unary operator expected\n");
    abort();
  }
  return ap_texpr0_node(op,type,dir,opA,NULL);
}
ap_texpr0_t* ap_texpr0_binop(ap_texpr_op_t op, ap_texpr0_t* opA, ap_texpr0_t* opB, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  if (!ap_texpr_is_binop(op)){
    fprintf(stderr,"ap_texpr0.c: ap_texpr0_binop: binary operator expected\n");
    abort();
  }
  return ap_texpr0_node(op,type,dir,opA,opB);
}
ap_texpr0_t* ap_texpr0_node_copy(ap_texpr0_node_t* node)
{
  ap_texpr0_t* res = malloc(sizeof(ap_texpr0_t));
  ap_texpr0_node_t* n = malloc(sizeof(ap_texpr0_node_t));
  res->discr = AP_TEXPR_NODE;
  res->val.node = n;
  n->op = node->op;
  n->type = node->type;
  n->dir  = node->dir;
  n->exprA = ap_texpr0_copy(node->exprA);
  n->exprB = ap_texpr0_copy(node->exprB);
  return res;
}
ap_texpr0_t* ap_texpr0_copy(ap_texpr0_t* expr)
{
  if (!expr) return NULL;
  switch (expr->discr){
  case AP_TEXPR_CST:
    return ap_texpr0_cst(&expr->val.cst);
  case AP_TEXPR_DIM:
    return ap_texpr0_dim(expr->val.dim);
  case AP_TEXPR_NODE:
    return ap_texpr0_node_copy(expr->val.node);
  default:
    assert(false);
    return NULL;
  }
}
void ap_texpr0_node_free(ap_texpr0_node_t* node)
{
  ap_texpr0_free(node->exprA);
  ap_texpr0_free(node->exprB);
  free(node);
}
void ap_texpr0_clear(ap_texpr0_t* expr)
{
  switch(expr->discr){
  case AP_TEXPR_CST:
    ap_coeff_clear(&expr->val.cst);
    break;
  case AP_TEXPR_DIM:
    break;
  case AP_TEXPR_NODE:
    ap_texpr0_node_free(expr->val.node);
    break;
  default:
    assert(false);
  }
}
void ap_texpr0_free(ap_texpr0_t* expr)
{
  if (!expr) return;
  ap_texpr0_clear(expr);
  free(expr);
}
ap_texpr0_t* ap_texpr0_from_linexpr0(ap_linexpr0_t* e)
{
  ap_texpr0_t* res = ap_texpr0_cst(&e->cst);
  size_t i;
  ap_dim_t d;
  ap_coeff_t* c;
  ap_linexpr0_ForeachLinterm(e, i, d, c) {
    res = ap_texpr0_binop(AP_TEXPR_ADD,
			  res,
			  ap_texpr0_binop(AP_TEXPR_MUL,
					  ap_texpr0_cst(c), ap_texpr0_dim(d),
					  AP_RTYPE_REAL, AP_RDIR_RND),
			  AP_RTYPE_REAL, AP_RDIR_RND);
  }
  return res;
}

/* ====================================================================== */
/* II. Printing */
/* ====================================================================== */

static const char* ap_texpr_op_name[] =
  { "+", "-", "*", "/", "%", "^", /* binary */
    "-", "cast", "sqrt",          /* unary */
  };

static const int ap_texpr_op_precedence[] =
  { 1, 1, 2, 2, 2, 3, /* binary */
    4, 5, 5           /* unary */
  };

static const char* ap_texpr_rtype_name[] =
  { "", "i", "f", "d", "l", "q", };

static const char* ap_texpr_rdir_name[] =
  { "n", "0", "+oo", "-oo", "?", "", };

/* node induces some rounding (to float or integer) */
static inline bool ap_texpr0_node_exact(ap_texpr0_node_t* a)
{
  if (a->op==AP_TEXPR_NEG || a->op==AP_TEXPR_MOD ||
      a->type==AP_RTYPE_REAL) return true;
  return false;
}

static inline int ap_texpr0_precedence(ap_texpr0_t* a)
{
  if (!a || a->discr!=AP_TEXPR_NODE) return ap_texpr_op_precedence[AP_TEXPR_NEG];
  return ap_texpr_op_precedence[a->val.node->op];
}

static void ap_texpr0_node_fprint(FILE* stream, ap_texpr0_node_t* a,
				  char** name_of_dim)
{
  int prec = ap_texpr_op_precedence[a->op];

  /* left argument (if binary) */
  if (a->exprB) {
    int prec2 = ap_texpr0_precedence(a->exprA);
    if (prec2<prec) fprintf(stream, "(");
    ap_texpr0_fprint(stream, a->exprA, name_of_dim);
    if (prec2<prec) fprintf(stream, ")");
  }

  /* operator & rounding mode */
  if (a->exprB) fprintf(stream, " ");
  fprintf(stream, "%s", ap_texpr_op_name[a->op]);
  if (!ap_texpr0_node_exact(a))
    fprintf(stream, "_%s,%s",
	    ap_texpr_rtype_name[a->type], ap_texpr_rdir_name[a->dir]);

  /* right argument */
  {
    ap_texpr0_t* arg = a->exprB ? a->exprB : a->exprA;
    int prec2 = ap_texpr0_precedence(arg);
    if (a->exprB) fprintf(stream, " ");
    if (prec2<=prec) fprintf(stream, "(");
    ap_texpr0_fprint(stream,arg,name_of_dim);
    if (prec2<=prec) fprintf(stream, ")");
  }
}

void ap_texpr0_fprint(FILE* stream, ap_texpr0_t* a, char** name_of_dim)
{
  if (!a) return;
  switch (a->discr) {
  case AP_TEXPR_CST:
    ap_coeff_fprint(stream, &a->val.cst);
    break;
  case AP_TEXPR_DIM:
    if (name_of_dim) fprintf(stream, "%s", name_of_dim[a->val.dim]);
    else             fprintf(stream, "x%lu", (unsigned long)a->val.dim);
    break;
  case AP_TEXPR_NODE:
    ap_texpr0_node_fprint(stream, a->val.node, name_of_dim);
    break;
  default:
    assert(false);
  }
}

void ap_texpr0_print(ap_texpr0_t* a, char** name_of_dim)
{ ap_texpr0_fprint(stdout, a, name_of_dim); }


/* ====================================================================== */
/* III. Tests, size */
/* ====================================================================== */

size_t ap_texpr0_depth(ap_texpr0_t* a)
{
  int l,r;
  if (!a) return 0;
  switch(a->discr) {
  case AP_TEXPR_CST:
  case AP_TEXPR_DIM:
    return 0;
  case AP_TEXPR_NODE:
    l = ap_texpr0_depth(a->val.node->exprA);
    r = ap_texpr0_depth(a->val.node->exprB);
    return 1 + (l>r ? l : r);
  default:
    assert(0);
    return 0;
  }
}

size_t ap_texpr0_size(ap_texpr0_t* a)
{
  if (!a) return 0;
  switch(a->discr) {
  case AP_TEXPR_CST:
  case AP_TEXPR_DIM:
    return 0;
  case AP_TEXPR_NODE:
    return 1 + ap_texpr0_size(a->val.node->exprA) + ap_texpr0_size(a->val.node->exprB);
  default:
    assert(0);
    return 0;
  }
}

/* maximum between all dimensions and max */
static ap_dim_t ap_texpr0_max_dim_internal(ap_texpr0_t* a, ap_dim_t max)
{
  if (!a) return max;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return max;
  case AP_TEXPR_DIM:
    return (a->val.dim+1) > max ? (a->val.dim+1) : max;
  case AP_TEXPR_NODE:
    return ap_texpr0_max_dim_internal(a->val.node->exprB,
				      ap_texpr0_max_dim_internal(a->val.node->exprA,max));
  default:
    assert(0);
    return max;
  }
}

ap_dim_t ap_texpr0_max_dim(ap_texpr0_t* a)
{
  return ap_texpr0_max_dim_internal(a, 0);
}

bool ap_texpr0_has_dim(ap_texpr0_t* a, ap_dim_t d)
{
  if (!a) return false;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return false;
  case AP_TEXPR_DIM:
    return a->val.dim == d;
  case AP_TEXPR_NODE:
    return
      ap_texpr0_has_dim(a->val.node->exprA, d) ||
      ap_texpr0_has_dim(a->val.node->exprB, d);
  default:
    assert(0);
    return false;
  }
}

/* fill in v, v should be pre-allocated with size max_dim */
static void ap_texpr0_dimlist_internal(ap_texpr0_t* a, char* v)
{
  if (!a) return;
  switch(a->discr) {
  case AP_TEXPR_CST:
    break;
  case AP_TEXPR_DIM:
    v[a->val.dim] = 1;
    break;
  case AP_TEXPR_NODE:
    ap_texpr0_dimlist_internal(a->val.node->exprA, v);
    ap_texpr0_dimlist_internal(a->val.node->exprB, v);
    break;
  default:
    assert(0);
  }
}

ap_dim_t* ap_texpr0_dimlist(ap_texpr0_t* a)
{
  ap_dim_t max,i,nb;
  ap_dim_t* d;
  char* v;

  /* compute occurence vector */
  max = ap_texpr0_max_dim(a);
  if (max==0){
    /* constant expression */
    d = malloc(sizeof(ap_dim_t));
    d[0] = AP_DIM_MAX;
  }
  else {
    /* get number of distinct variables */
    v = malloc(max);
    memset(v, 0, max);
    ap_texpr0_dimlist_internal(a, v);
    for (i=0, nb=0; i<max; i++)
      if (v[i]) nb++;

    /* create & fill list */
    d = malloc(sizeof(ap_dim_t) * (nb+1));
    for (i=0, nb=0; i<max; i++)
      if (v[i]) { d[nb] = i; nb++; }
    d[nb] = AP_DIM_MAX; /* terminator */

    /* clean-up */
    free (v);
  }
  return d;
}

bool ap_texpr0_is_interval_cst(ap_texpr0_t* a)
{
  if (!a) return true;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return true;
  case AP_TEXPR_DIM:
    return false;
  case AP_TEXPR_NODE:
    return
      ap_texpr0_is_interval_cst(a->val.node->exprA) &&
      ap_texpr0_is_interval_cst(a->val.node->exprB);
  default:
    assert(0);
    return false;
  }
}

bool ap_texpr0_is_scalar(ap_texpr0_t* a)
{
  if (!a) return true;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return
      (a->val.cst.discr==AP_COEFF_SCALAR) ||
      (ap_scalar_equal(a->val.cst.val.interval->inf,
		       a->val.cst.val.interval->sup));
  case AP_TEXPR_DIM:
    return true;
  case AP_TEXPR_NODE:
    return
      ap_texpr0_is_scalar(a->val.node->exprA) &&
      ap_texpr0_is_scalar(a->val.node->exprB);
  default:
    assert(0);
    return false;
  }
}

bool ap_texpr0_is_interval_linear(ap_texpr0_t* a)
{
  if (!a) return true;
  switch(a->discr) {
  case AP_TEXPR_CST:
  case AP_TEXPR_DIM:
    return true;
  case AP_TEXPR_NODE:
    switch (a->val.node->op) {
    case AP_TEXPR_NEG:
      return ap_texpr0_is_interval_linear(a->val.node->exprA);
    case AP_TEXPR_CAST:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_linear(a->val.node->exprA);
    case AP_TEXPR_ADD:
    case AP_TEXPR_SUB:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_linear(a->val.node->exprA) &&
	ap_texpr0_is_interval_linear(a->val.node->exprB);
    case AP_TEXPR_MUL:
      return
	ap_texpr0_node_exact(a->val.node) &&
	( (ap_texpr0_is_interval_linear(a->val.node->exprA) &&
	   ap_texpr0_is_interval_cst(a->val.node->exprB)) ||
	  (ap_texpr0_is_interval_linear(a->val.node->exprB) &&
	   ap_texpr0_is_interval_cst(a->val.node->exprA)) );
    case AP_TEXPR_DIV:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_linear(a->val.node->exprA) &&
	ap_texpr0_is_interval_cst(a->val.node->exprB);
    default:
      return false;
    }
  default:
    assert(0);
    return 0;
  }
}

bool ap_texpr0_is_interval_polynomial(ap_texpr0_t* a)
{
  if (!a) return true;
  switch(a->discr) {
  case AP_TEXPR_CST:
  case AP_TEXPR_DIM:
    return true;
  case AP_TEXPR_NODE:
    switch (a->val.node->op) {
    case AP_TEXPR_NEG:
      return ap_texpr0_is_interval_polynomial(a->val.node->exprA);
    case AP_TEXPR_CAST:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_polynomial(a->val.node->exprA);
    case AP_TEXPR_ADD:
    case AP_TEXPR_SUB:
    case AP_TEXPR_MUL:
     return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_polynomial(a->val.node->exprA) &&
	ap_texpr0_is_interval_polynomial(a->val.node->exprB);
    case AP_TEXPR_DIV:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_polynomial(a->val.node->exprA) &&
	ap_texpr0_is_interval_cst(a->val.node->exprB);
    case AP_TEXPR_POW:
      return
	ap_texpr0_node_exact(a->val.node) &&
        ap_texpr0_is_interval_polynomial(a->val.node->exprA) &&
        ap_texpr0_is_interval_cst(a->val.node->exprB); /* check for positivity? */
    default:
      return false;
    }
  default:
    assert(0);
    return 0;
  }
}

bool ap_texpr0_is_interval_polyfrac(ap_texpr0_t* a)
{
  if (!a) return true;
  switch(a->discr) {
  case AP_TEXPR_CST:
  case AP_TEXPR_DIM:
    return true;
  case AP_TEXPR_NODE:
    switch (a->val.node->op) {
    case AP_TEXPR_NEG:
      return ap_texpr0_is_interval_polyfrac(a->val.node->exprA);
    case AP_TEXPR_CAST:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_polyfrac(a->val.node->exprA);
    case AP_TEXPR_ADD:
    case AP_TEXPR_SUB:
    case AP_TEXPR_MUL:
    case AP_TEXPR_DIV:
      return
	ap_texpr0_node_exact(a->val.node) &&
	ap_texpr0_is_interval_polyfrac(a->val.node->exprA) &&
	ap_texpr0_is_interval_polyfrac(a->val.node->exprB);
    case AP_TEXPR_POW:
      return
	ap_texpr0_node_exact(a->val.node) &&
        ap_texpr0_is_interval_polyfrac(a->val.node->exprA) &&
        ap_texpr0_is_interval_cst(a->val.node->exprB);
    default:
      return false;
    }
  default:
    assert(0);
    return 0;
  }
}

static bool ap_texpr0_array_is_template(ap_texpr0_t** texpr, size_t size, bool (*is_template)(ap_texpr0_t* texpr))
{
  size_t i;
  bool res = true;
  for (i=0; i<size; i++){
    res = is_template(texpr[i]);
    if (!res) break;
  }
  return res;
}
bool ap_texpr0_array_is_interval_linear(ap_texpr0_t** texpr, size_t size)
{
  return ap_texpr0_array_is_template(texpr,size,ap_texpr0_is_interval_linear);
}
bool ap_texpr0_array_is_interval_polynomial(ap_texpr0_t** texpr, size_t size)
{
return ap_texpr0_array_is_template(texpr,size,ap_texpr0_is_interval_polynomial);
}
bool ap_texpr0_array_is_interval_polyfrac(ap_texpr0_t** texpr, size_t size)
{
  return ap_texpr0_array_is_template(texpr,size,ap_texpr0_is_interval_polyfrac);
}
bool ap_texpr0_array_is_scalar(ap_texpr0_t** texpr, size_t size)
{
  return ap_texpr0_array_is_template(texpr,size,ap_texpr0_is_scalar);
}

/* ====================================================================== */
/* IV. Operations */
/* ====================================================================== */

ap_texpr0_t* ap_texpr0_substitute(ap_texpr0_t* a, ap_dim_t dim, ap_texpr0_t *dst)
{
  ap_texpr0_t* res = ap_texpr0_copy(a);
  ap_texpr0_substitute_with(res, dim, dst);
  return res;
}

void ap_texpr0_substitute_with(ap_texpr0_t* a, ap_dim_t dim, ap_texpr0_t *dst)
{
  if (!a) return;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return;
  case AP_TEXPR_DIM:
    if (a->val.dim!=dim) return;
    dst = ap_texpr0_copy(dst);
    *a = *dst;
    free(dst);
    return;
  case AP_TEXPR_NODE:
    ap_texpr0_substitute_with(a->val.node->exprA, dim, dst);
    ap_texpr0_substitute_with(a->val.node->exprB, dim, dst);
    break;
  default:
    assert(0);
  }
}

/* ====================================================================== */
/* V. Change of dimensions and permutations */
/* ====================================================================== */

ap_texpr0_t* ap_texpr0_add_dimensions(ap_texpr0_t* expr,
				      ap_dimchange_t* dimchange)
{
  ap_texpr0_t* res = ap_texpr0_copy(expr);
  ap_texpr0_add_dimensions_with(res,dimchange);
  return res;
}
void ap_texpr0_add_dimensions_with(ap_texpr0_t* expr,
				   ap_dimchange_t* dimchange)
{
  if (!expr) return;
  switch(expr->discr){
  case AP_TEXPR_CST:
    return;
  case AP_TEXPR_DIM:
    {
      size_t dimsup = dimchange->intdim+dimchange->realdim;
      size_t k = 0;
      while (k<dimsup && expr->val.dim>=dimchange->dim[k]){
	k++;
      }
      expr->val.dim += k;
    }
    return;
  case AP_TEXPR_NODE:
    ap_texpr0_add_dimensions_with(expr->val.node->exprA,dimchange);
    ap_texpr0_add_dimensions_with(expr->val.node->exprB,dimchange);
    return;
  default:
    assert(false);
  }
}
ap_texpr0_t* ap_texpr0_remove_dimensions(ap_texpr0_t* expr,
					 ap_dimchange_t* dimchange)
{
  ap_texpr0_t* res = ap_texpr0_copy(expr);
  ap_texpr0_remove_dimensions_with(res,dimchange);
  return res;
}
void ap_texpr0_remove_dimensions_with(ap_texpr0_t* expr,
				      ap_dimchange_t* dimchange)
{
  if (!expr) return;
  switch(expr->discr){
  case AP_TEXPR_CST:
    return;
  case AP_TEXPR_DIM:
    {
      size_t dimrem = dimchange->intdim+dimchange->realdim;
      size_t i;
      for (i=0;i<dimrem && expr->val.dim>dimchange->dim[i];i++);
      if (i<dimrem && expr->val.dim==dimchange->dim[i]) {
	/* replace variable with top */
	expr->discr = AP_TEXPR_CST;
	ap_coeff_init(&expr->val.cst, AP_COEFF_INTERVAL);
	ap_interval_set_top(expr->val.cst.val.interval);
      }
      else expr->val.dim -= i;
    }
    return;
  case AP_TEXPR_NODE:
    ap_texpr0_remove_dimensions_with(expr->val.node->exprA,dimchange);
    ap_texpr0_remove_dimensions_with(expr->val.node->exprB,dimchange);
    return;
  default:
    assert(false);
  }
}
ap_texpr0_t* ap_texpr0_permute_dimensions(ap_texpr0_t* expr,
					  ap_dimperm_t* dimperm)
{
  ap_texpr0_t* res = ap_texpr0_copy(expr);
  ap_texpr0_permute_dimensions_with(res,dimperm);
  return res;
}
void ap_texpr0_permute_dimensions_with(ap_texpr0_t* expr,
				       ap_dimperm_t* perm)
{
  if (!expr) return;
  switch(expr->discr){
  case AP_TEXPR_CST:
    return;
  case AP_TEXPR_DIM:
    expr->val.dim = perm->dim[expr->val.dim];
    return;
  case AP_TEXPR_NODE:
    ap_texpr0_permute_dimensions_with(expr->val.node->exprA,perm);
    ap_texpr0_permute_dimensions_with(expr->val.node->exprB,perm);
    return;
  default:
    assert(false);
  }
}

/* ====================================================================== */
/* VI. Hashing, comparisons */
/* ====================================================================== */

long ap_texpr0_hash(ap_texpr0_t* a)
{
  if (!a) return 0;
  switch(a->discr) {
  case AP_TEXPR_CST:
    return ap_coeff_hash(&a->val.cst);
  case AP_TEXPR_DIM:
    return a->val.dim;
  case AP_TEXPR_NODE:
    return
      a->val.node->op * 17 +
      a->val.node->type * 23 +
      a->val.node->dir * 4801 +
      ap_texpr0_hash(a->val.node->exprA) * 17053 +
      ap_texpr0_hash(a->val.node->exprB);
  default:
    assert(0);
    return 0;
  }
}

bool ap_texpr0_equal(ap_texpr0_t* a1, ap_texpr0_t* a2)
{
  if (!a1 && !a2) return true;
  if (!a1 || !a2) return false;
  if (a1->discr!=a2->discr) return false;
  switch(a1->discr) {
  case AP_TEXPR_CST:
    return ap_coeff_equal(&a1->val.cst, &a2->val.cst);
  case AP_TEXPR_DIM:
    return a1->val.dim==a2->val.dim;
  case AP_TEXPR_NODE:
    return
      (a1->val.node->op==a2->val.node->op) &&
      (a1->val.node->type==a2->val.node->type) &&
      (a1->val.node->dir==a2->val.node->dir) &&
      ap_texpr0_equal(a1->val.node->exprA, a2->val.node->exprA) &&
      ap_texpr0_equal(a1->val.node->exprB, a2->val.node->exprB);
  default:
    assert(0);
    return false;
  }
}
