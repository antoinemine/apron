/* ************************************************************************* */
/* ap_abstract0_c: generic interface */
/* ************************************************************************* */

#include "ap_abstract0.h"


/* ********************************************************************** */
/* 0. Utility and checking functions */
/* ********************************************************************** */

/* Constructor for ap_abstract0_t */

static inline
ap_abstract0_t* ap_abstract0_cons(ap_manager_t* man, void* value)
{
  ap_abstract0_t* res = malloc(sizeof(ap_abstract0_t));
  res->value = value;
  res->man = ap_manager_copy(man);
  return res;
}

/* ====================================================================== */
/* 0.1 Checking typing w.r.t. manager */
/* ====================================================================== */

/*
These functions return true if everything is OK, otherwise they raise an
exception in the manager and return false.
*/

/* One abstract value */

static
void ap_abstract0_checkman1_raise(ap_funid_t funid, ap_manager_t* man, const ap_abstract0_t* a)
{
  char str[160];
  snprintf(str,159,"\
The abstract value of type %s is not of the type %s expected by the manager\
",
	   a->man->library,man->library);
  ap_manager_raise_exception(man,
			  AP_EXC_INVALID_ARGUMENT,
			  funid,
			  str);
}
static inline
bool ap_abstract0_checkman1(ap_funid_t funid, ap_manager_t* man, const ap_abstract0_t* a)
{
  if (man->library != a->man->library){
    ap_abstract0_checkman1_raise(funid,man,a);
    return false;
  }
  else
    return true;
}

/* Two abstract values */
static
bool ap_abstract0_checkman2(ap_funid_t funid,
			 ap_manager_t* man, const ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
  bool res;
  char str[160];

  res = true;
  if (man->library != a1->man->library){
    snprintf(str,159,"\
The first abstract value of type %s is not of the type %s expected by the manager\
",
	   a1->man->library,man->library);
    res = false;
  }
  else if (man->library != a2->man->library){
    snprintf(str,159,"\
The second abstract value of type %s is not of the type %s expected by the manager\
",
	     a2->man->library,man->library);
    res = false;
  }
  if (!res){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,
			    str);
  }
  return res;
}
/* Array of abstract values */
static
bool ap_abstract0_checkman_array(ap_funid_t funid,
			      ap_manager_t* man, const ap_abstract0_t*const* tab, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    if (man->library != tab[i]->man->library){
      char str[160];
      snprintf(str,159,"\
The %dth abstract value of the array is of type %s and not of the type %s expected by the manager\
",
	   i,tab[i]->man->library,man->library);
      ap_manager_raise_exception(man,
			      AP_EXC_INVALID_ARGUMENT,
			      funid,
			      str);
      return false;
    }
  }
  return true;
}

/* ====================================================================== */
/* 0.2 Checking compatibility of arguments: abstract values */
/* ====================================================================== */

/* Getting dimensions without checks */
static inline
ap_dimension_t _ap_abstract0_dimension(const ap_abstract0_t* a)
{
  ap_dimension_t (*ptr)(ap_manager_t*,...) = a->man->funptr[AP_FUNID_DIMENSION];
  return ptr(a->man,a->value);
}

/* Check that the 2 abstract values have the same dimensionality */
static
bool ap_abstract0_check_abstract2(ap_funid_t funid, ap_manager_t* man,
			       const ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
  ap_dimension_t dim1 = _ap_abstract0_dimension(a1);
  ap_dimension_t dim2 = _ap_abstract0_dimension(a2);
  if ( (dim1.intdim != dim2.intdim) || (dim1.realdim != dim2.realdim) ){
    char str[160];

    snprintf(str,159,"\
incompatible dimensions for the two arguments:\n\
first abstract0:  (%3d,%3d)\n\
second abstract0: (%3d,%3d)",
	     dim1.intdim,
	     dim1.realdim,
	     dim2.intdim,
	     dim2.realdim);
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,str);
    return false;
  } else {
    return true;
  }
}

/* Check that the array of abstract values have the same dimensionality.*/
static
bool ap_abstract0_check_abstract_array(ap_funid_t funid, ap_manager_t* man,
			       const ap_abstract0_t*const* tab, size_t size)
{
  size_t i=0;
  ap_dimension_t dim;
  ap_dimension_t dim0;
  bool res;

  res = true;

  if (size==0){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,"array of abstract values of size 0");
    res = false;
  }
  else {
    dim0 = _ap_abstract0_dimension(tab[0]);
    for (i=1; i<size; i++){
      dim = _ap_abstract0_dimension(tab[i]);
      if ( (dim.intdim != dim0.intdim) || (dim.realdim != dim0.realdim) ){
	res = false;
	break;
      }
    }
  }
  if (!res){
    char str[160];
    if (size==0){
      snprintf(str,159,"array of size 0");
    }
    else {
      snprintf(str,159,"\
incompatible dimensions for the array of polyhedra:\n\
abstract0 0: (%3d,%3d)\n\
abstract0 %d: (%3d,%3d)\
",
	       dim0.intdim,dim0.realdim,
	       i,
	       dim.intdim,dim.realdim
	       );
    }
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,str);
  }
  return res;
}

/* ====================================================================== */
/* 0.3 Checking compatibility of arguments: dimensions */
/* ====================================================================== */

/* Check that the dimension makes sense in the given dimensionality */
static
void ap_abstract0_check_dim_raise(ap_funid_t funid, ap_manager_t* man,
			       ap_dimension_t dimension, ap_dim_t dim,
			       char* prefix)
{
   char str[160];

   snprintf(str,159,"\
%s:\n\
abstract0:  (%3d,%3d)\n\
dimension:  %3d\n",
	    prefix,
	    dimension.intdim,
	    dimension.realdim,
	    dim);
   ap_manager_raise_exception(man,
			   AP_EXC_INVALID_ARGUMENT,
			   funid,str);
}

static inline
bool ap_abstract0_check_dim(ap_funid_t funid, ap_manager_t* man,
			 ap_dimension_t dimension, ap_dim_t dim)
{
  if (dim>dimension.intdim+dimension.realdim){
    ap_abstract0_check_dim_raise(funid,man,dimension,dim,
			      "incompatible dimension for the abstract value");
    return false;
  } else {
    return true;
  }
}

/* Check that the array of dimensions make sense in the given dimensionality */
static
bool ap_abstract0_check_dim_array(ap_funid_t funid, ap_manager_t* man,
			       ap_dimension_t dimension, const ap_dim_t* tdim, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    ap_dim_t dim = tdim[i];
    if (dim>dimension.intdim+dimension.realdim){
      char str[80];
      sprintf(str,"incompatible %dth dimension in the array for the abstract value",i);
      ap_abstract0_check_dim_raise(funid,man,dimension,dim,str);
      return false;
    }
  }
  return true;
}

/* ====================================================================== */
/* 0.4 Checking compatibility of arguments: linear expressions */
/* ====================================================================== */

/* Check that the linear expression makes sense in the given dimensionality */
static
ap_dim_t ap_abstract0_check_linexpr_check(ap_dimension_t dimension,
				   const ap_linexpr0_t* expr)
{
  int i;
  size_t nbdims;
  ap_dim_t dim;

  nbdims = dimension.intdim+dimension.realdim;
  dim = 0;
  switch (expr->discr){
  case AP_LINEXPR_DENSE:
    if (expr->size > nbdims){
      dim = expr->size-1;
    }
    else {
      dim = AP_DIM_MAX;
    }
    break;
  case AP_LINEXPR_SPARSE:
    /* Assumed to be sorted (this is not checked */
    for (i=expr->size-1; i>=0; i--){
      dim = expr->p.linterm[i].dim;
      if (dim!=AP_DIM_MAX){
	if (dim < nbdims)
	  dim = AP_DIM_MAX;
	;
	break;
      }
    }
    break;
  default:
    abort();
  }
  return dim;
}

static
void ap_abstract0_check_linexpr_raise(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension,
				   ap_dim_t dim,
				   char* prefix)
{
  char str[160];
  snprintf(str,159,"\
%s:\n\
abstract0: (%3d,%3d)\n\
dimension: %3d\
",
	   prefix,
	   dimension.intdim,dimension.realdim,dim);
  ap_manager_raise_exception(man,
			  AP_EXC_INVALID_ARGUMENT,
			  funid,str);
}

static
bool ap_abstract0_check_linexpr(ap_funid_t funid, ap_manager_t* man,
			   ap_dimension_t dimension,
			   const ap_linexpr0_t* expr)
{
  ap_dim_t dim = ap_abstract0_check_linexpr_check(dimension,expr);
  if (dim!=AP_DIM_MAX){
    ap_abstract0_check_linexpr_raise(funid,man,dimension,dim,
				  "incompatible dimension in the linear expression for the bastract value");
    return false;
  } else {
    return true;
  }
}

/* ====================================================================== */
/* 0.5 Checking compatibility of arguments: array of expressions/constraints/generators */
/* ====================================================================== */

/* Check that array of linear expressions makes sense in the given dimensionality */
bool ap_abstract0_check_linexpr_array(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension,
				   const ap_linexpr0_t*const* texpr, size_t size)
{
  size_t i;

  for (i=0;i<size; i++){
    ap_dim_t dim = ap_abstract0_check_linexpr_check(dimension,texpr[i]);
    if (dim!=AP_DIM_MAX){
      char str[80];
      sprintf(str,"incompatible dimension in the %dth expression of the array",i);
      ap_abstract0_check_linexpr_raise(funid,man,dimension,dim,str);
      return false;
    }
  }
  return true;
}
/* Check that array of linear constraint makes sense in the given dimensionality */
bool ap_abstract0_check_lincons_array(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension,
				   const ap_lincons0_array_t* array)
{
  size_t i;

  for (i=0;i<array->size; i++){
    ap_dim_t dim = ap_abstract0_check_linexpr_check(dimension,array->p[i].linexpr0);
    if (dim!=AP_DIM_MAX){
      char str[80];
      sprintf(str,"incompatible dimension in the %dth constraint of the array",i);
      ap_abstract0_check_linexpr_raise(funid,man,dimension,dim,str);
      return false;
    }
  }
  return true;
}

/* Check that array of generator makes sense in the given dimensionality */
bool ap_abstract0_check_generator_array(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension, const ap_generator0_array_t* array)
{
  size_t i;

  for (i=0;i<array->size; i++){
    ap_dim_t dim = ap_abstract0_check_linexpr_check(dimension,array->p[i].linexpr0);
    if (dim!=AP_DIM_MAX){
      char str[80];
      sprintf(str,"incompatible dimension in the %dth generator of the array",i);
      ap_abstract0_check_linexpr_raise(funid,man,dimension,dim,str);
      return false;
    }
  }
  return true;
}

/* ====================================================================== */
/* 0.6 Checking compatibility of arguments: dimchange */
/* ====================================================================== */

bool ap_abstract0_check_ap_dimchange_add(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension, ap_dimchange_t* dimchange)
{
  size_t i,size;
  bool res = true;
  /* Check consistency between intdim and the dimensions in the array */
  for (i=0; i<dimchange->intdim; i++){
    if (dimchange->dim[i]>dimension.intdim){
      res = false;
      break;
    }
  }
  size = dimchange->intdim+dimchange->realdim;
  if (res && size>0){
    /* Check sortedness */
    ap_dim_t dim = 0;
    for (i=1; i<size; i++){
      if (dim>dimchange->dim[i]){
	res = false;
	break;
      } else {
	dim = dimchange->dim[i];
      }
    }
    res = res && dimchange->dim[size-1]<=dimension.intdim+dimension.realdim;
  }
  if (!res){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,
			    "dimchange->dim is not sorted or is inconsistent wrt dimchange->intdim or abstract0");
  }
  return res;
}

bool ap_abstract0_check_ap_dimchange_remove(ap_funid_t funid, ap_manager_t* man,
				      ap_dimension_t dimension, ap_dimchange_t* dimchange)
{
  size_t i,size;
  bool res = true;
  /* Check consistency between intdim and the dimensions in the array */
  for (i=0; i<dimchange->intdim; i++){
    if (dimchange->dim[i]>=dimension.intdim){
      res = false;
      break;
    }
  }
  size = dimchange->intdim+dimchange->realdim;
  if (res && size>0){
    /* Check sortedness */
    ap_dim_t dim = 0;
    for (i=1; i<dimchange->intdim+dimchange->realdim; i++){
      if (dim>=dimchange->dim[i]){
	res = false;
	break;
      } else {
	dim = dimchange->dim[i];
      }
    }
    res = res && dimchange->dim[size-1]<dimension.intdim+dimension.realdim;
  }
  if (!res){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,
			    "dimchange->dim is not sorted, contains duplicatas, or is inconsistent wrt dimchange->intdim or abstract0");
  }
  return res;
}
bool ap_abstract0_check_dimperm(ap_funid_t funid, ap_manager_t* man,
			     ap_dimension_t dimension, const ap_dimperm_t* perm)
{
  size_t i;
  size_t size = dimension.intdim+dimension.realdim;
  bool res = (dimension.intdim+dimension.realdim==perm->size);
  if (res){
    for (i=0; i<perm->size; i++){
      if (perm->dim[i]>=size){
	res = false;
	break;
      }
    }
  }
  if (!res){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    funid,
			    "permutation is not valid");
  }
  return res;
}

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_copy(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_COPY,man,a)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_COPY];
    return ap_abstract0_cons(man,ptr(man,a->value));
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}

void ap_abstract0_free(ap_manager_t* man, ap_abstract0_t* a)
{
  if (a->man==NULL){
    fprintf(stderr,"ap_abstract0_c: ap_abstract0_free: the abstract value has probably already been deallocated !\n");
    abort();
  }
  if (ap_abstract0_checkman1(AP_FUNID_FREE,man,a)){
    void (*ptr)(ap_manager_t*,ap_abstract0_t*) = man->funptr[AP_FUNID_FREE];
    ptr(man,a->value);
  }
  else {
    void (*ptr)(ap_manager_t*,ap_abstract0_t*) = a->man->funptr[AP_FUNID_FREE];
    ptr(a->man,a->value);
  }
  ap_manager_free(a->man);
  a->man = NULL;
  a->value = NULL;
  free(a);
}
size_t ap_abstract0_size(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_ASIZE,man,a)){
    size_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_ASIZE];
    return ptr(man,a->value);
  }
  else {
    return 0;
  }
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */
void ap_abstract0_minimize(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_MINIMIZE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_MINIMIZE];
    ptr(man,a->value);
  }
}
void ap_abstract0_canonicalize(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_CANONICALIZE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_CANONICALIZE];
    ptr(man,a->value);
  }
}
void ap_abstract0_approximate(ap_manager_t* man, ap_abstract0_t* a, int n)
{
  if (ap_abstract0_checkman1(AP_FUNID_APPROXIMATE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_APPROXIMATE];
    ptr(man,a->value,n);
  }
}
tbool_t ap_abstract0_is_minimal(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_IS_MINIMAL,man,a)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_MINIMAL];
    return ptr(man,a->value);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_is_canonical(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_IS_CANONICAL,man,a)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_CANONICAL];
    return ptr(man,a->value);
  }
  else {
    return tbool_top;
  }
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */
void ap_abstract0_fprint(FILE* stream, ap_manager_t* man,
		      const ap_abstract0_t* a,
		      char** name_of_dim)
{
  if (ap_abstract0_checkman1(AP_FUNID_FPRINT,man,a)){
    void (*ptr)(FILE*,ap_manager_t*,...) = man->funptr[AP_FUNID_FPRINT];
    ptr(stream,man,a->value,name_of_dim);
  }
  else {
    fprintf(stream,"ap_abstract0_c: ap_abstract0_fprint: INVALID_ARGUMENT\n");
  }
}

void ap_abstract0_fprintdiff(FILE* stream, ap_manager_t* man,
			   const ap_abstract0_t* a, const ap_abstract0_t* b,
			   char** name_of_dim)
{
  if (ap_abstract0_checkman2(AP_FUNID_FPRINTDIFF,man,a,b) &&
      ap_abstract0_check_abstract2(AP_FUNID_FPRINTDIFF,man,a,b) ){
    void (*ptr)(FILE*,ap_manager_t*,...) = man->funptr[AP_FUNID_FPRINTDIFF];
    ptr(stream,man,a->value,b->value,name_of_dim);
  }
  else {
    fprintf(stream,"ap_abstract0_c: ap_abstract0_fprintdiff: INVALID ARGUMENT\n");
  }
}
void ap_abstract0_fdump(FILE* stream, ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_FDUMP,man,a)){
    void (*ptr)(FILE*,ap_manager_t*,...) = man->funptr[AP_FUNID_FDUMP];
    ptr(stream,man,a->value);
  }
  else {
    fprintf(stream,"ap_abstract0_c: ap_abstract0_fdump: INVALID_ARGUMENT\n");
  }
}

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */
ap_membuf_t ap_abstract0_serialize_raw(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_SERIALIZE_RAW,man,a)){
    ap_membuf_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SERIALIZE_RAW];
    return ptr(man,a->value);
  }
  else {
    ap_membuf_t res = { NULL, 0 };
    return res;
  }
}
ap_abstract0_t* ap_abstract0_deserialize_raw(ap_manager_t* man, void* p, size_t* size)
{
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_DESERIALIZE_RAW];
  return ap_abstract0_cons(man,ptr(man,p,size));
}

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */
/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOTTOM];
  return ap_abstract0_cons(man,ptr(man,intdim,realdim));
}
ap_abstract0_t* ap_abstract0_top(ap_manager_t* man, size_t intdim, size_t realdim){
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TOP];
  return ap_abstract0_cons(man,ptr(man,intdim,realdim));
}
ap_abstract0_t* ap_abstract0_of_box(ap_manager_t* man,
			    size_t intdim, size_t realdim,
			    const ap_interval_t*const* tinterval){
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_OF_BOX];
  return ap_abstract0_cons(man,ptr(man,intdim,realdim,tinterval));
}
ap_abstract0_t* ap_abstract0_of_lincons_array(ap_manager_t* man,
					size_t intdim, size_t realdim,
					const ap_lincons0_array_t* array)
{
  ap_dimension_t dimension;
  dimension.intdim = intdim;
  dimension.realdim = realdim;
  if (ap_abstract0_check_lincons_array(AP_FUNID_OF_LINCONS_ARRAY,man,dimension,array)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_OF_LINCONS_ARRAY];
    return ap_abstract0_cons(man,ptr(man,intdim,realdim,array));
  }
  else {
    return ap_abstract0_top(man,intdim,realdim);
  }
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */
ap_dimension_t ap_abstract0_dimension(ap_manager_t* man, const ap_abstract0_t* a)
{
  ap_abstract0_checkman1(AP_FUNID_DIMENSION,man,a);
  ap_dimension_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_DIMENSION];
  return ptr(a->man,a->value);
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */
tbool_t ap_abstract0_is_bottom(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_IS_BOTTOM,man,a)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_BOTTOM];
    return ptr(man,a->value);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_is_top(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_IS_TOP,man,a)){
  tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_TOP];
  return ptr(man,a->value);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_is_leq(ap_manager_t* man, const ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
  if (ap_abstract0_checkman2(AP_FUNID_IS_LEQ,man,a1,a2) &&
      ap_abstract0_check_abstract2(AP_FUNID_IS_EQ,man,a1,a2)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_LEQ];
    return ptr(man,a1->value,a2->value);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_is_eq(ap_manager_t* man, const ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
  if (ap_abstract0_checkman2(AP_FUNID_IS_EQ,man,a1,a2) &&
      ap_abstract0_check_abstract2(AP_FUNID_IS_EQ,man,a1,a2)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_EQ];
    return ptr(man,a1->value,a2->value);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_sat_lincons(ap_manager_t* man, const ap_abstract0_t* a, const ap_lincons0_t* lincons)
{
  if (ap_abstract0_checkman1(AP_FUNID_SAT_LINCONS,man,a) &&
      ap_abstract0_check_linexpr(AP_FUNID_SAT_LINCONS,man,_ap_abstract0_dimension(a),lincons->linexpr0) ){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SAT_LINCONS];
    return ptr(man,a->value,lincons);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_sat_interval(ap_manager_t* man, const ap_abstract0_t* a,
			      ap_dim_t dim, const ap_interval_t* interval)
{
  if (ap_abstract0_checkman1(AP_FUNID_SAT_INTERVAL,man,a) &&
      ap_abstract0_check_dim(AP_FUNID_SAT_INTERVAL,man,_ap_abstract0_dimension(a),dim)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SAT_INTERVAL];
    return ptr(man,a->value,dim,interval);
  }
  else {
    return tbool_top;
  }
}
tbool_t ap_abstract0_is_dimension_unconstrained(ap_manager_t* man, const ap_abstract0_t* a,
					     ap_dim_t dim)
{
  if (ap_abstract0_checkman1(AP_FUNID_IS_DIMENSION_UNCONSTRAINED,man,a) &&
      ap_abstract0_check_dim(AP_FUNID_IS_DIMENSION_UNCONSTRAINED,man,_ap_abstract0_dimension(a),dim)){
    tbool_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED];
    return ptr(man,a->value,dim);
  }
  else {
    return tbool_top;
  }
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */
ap_interval_t* ap_abstract0_bound_linexpr(ap_manager_t* man,
				    const ap_abstract0_t* a, const ap_linexpr0_t* expr)
{
  if (ap_abstract0_checkman1(AP_FUNID_BOUND_LINEXPR,man,a) &&
      ap_abstract0_check_linexpr(AP_FUNID_BOUND_LINEXPR,man,_ap_abstract0_dimension(a),expr)){
    ap_interval_t* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOUND_LINEXPR];
    return ptr(man,a->value,expr);
  }
  else {
    ap_interval_t* itv = ap_interval_alloc();
    ap_interval_reinit(itv,man->option.scalar_discr);
    ap_interval_set_top(itv);
    return itv;
  }
}
ap_interval_t* ap_abstract0_bound_dimension(ap_manager_t* man,
				      const ap_abstract0_t* a, ap_dim_t dim)
{
  if (ap_abstract0_checkman1(AP_FUNID_BOUND_DIMENSION,man,a) &&
      ap_abstract0_check_dim(AP_FUNID_BOUND_DIMENSION,man,_ap_abstract0_dimension(a),dim)){
    ap_interval_t* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOUND_DIMENSION];
    return ptr(man,a->value,dim);
  }
  else {
    ap_interval_t* itv = ap_interval_alloc();
    ap_interval_reinit(itv,man->option.scalar_discr);
    ap_interval_set_top(itv);
    return itv;
  }
}
ap_lincons0_array_t ap_abstract0_to_lincons_array(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_TO_LINCONS_ARRAY,man,a)){
    ap_lincons0_array_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];
    return ptr(man,a->value);
  }
  else {
    ap_lincons0_array_t res = { NULL, 0 };
    return res;
  }
}
ap_interval_t** ap_abstract0_to_box(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_TO_BOX,man,a)){
    ap_interval_t** (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_BOX];
    return ptr(man,a->value);
  }
  else {
    size_t i;
    ap_dimension_t d = _ap_abstract0_dimension(a);
    ap_interval_t** titv = ap_interval_array_alloc(d.intdim+d.realdim);
    for (i=0; i<d.intdim+d.realdim; i++){
      ap_interval_reinit(titv[i],man->option.scalar_discr);
      ap_interval_set_top(titv[i]);
    }
    return titv;
  }
}
ap_generator0_array_t ap_abstract0_to_generator_array(ap_manager_t* man, const ap_abstract0_t* a)
{
  if (ap_abstract0_checkman1(AP_FUNID_TO_GENERATOR_ARRAY,man,a)){
    ap_generator0_array_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_GENERATOR_ARRAY];
    return ptr(man,a->value);
  }
  else {
    ap_generator0_array_t res = { NULL, 0 };
    return res;
  }
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */
/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_meetjoin(ap_funid_t funid,
				ap_manager_t* man, bool destructive, ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
  if (ap_abstract0_checkman2(funid,man,a1,a2) &&
      ap_abstract0_check_abstract2(funid,man,a1,a2)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void* value1 = a1->value;
    void* value = ptr(man,destructive,a1->value,a2->value);
    if (value==value1){
      assert(destructive);
      return a1;
    }
    else if (destructive){
      ap_manager_free(a1->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a1);
    if (destructive) ap_abstract0_free(a1->man,a1->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
ap_abstract0_t* ap_abstract0_meet(ap_manager_t* man, bool destructive, ap_abstract0_t* a1, const ap_abstract0_t* a2){
  return  ap_abstract0_meetjoin(AP_FUNID_MEET,man,destructive,a1,a2);
}
ap_abstract0_t* ap_abstract0_join(ap_manager_t* man, bool destructive, ap_abstract0_t* a1, const ap_abstract0_t* a2){
  return  ap_abstract0_meetjoin(AP_FUNID_JOIN,man,destructive,a1,a2);
}

ap_abstract0_t* ap_abstract0_meetjoin_array(ap_funid_t funid, ap_manager_t* man, const ap_abstract0_t*const* tab, size_t size)
{
  if (ap_abstract0_checkman_array(funid,man,tab,size) &&
      ap_abstract0_check_abstract_array(funid,man,tab,size)){
    size_t i;
    ap_abstract0_t* res;
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void** ntab = malloc(size*sizeof(void*));
    for (i=0;i<size;i++) ntab[i] = tab[i]->value;
    res = ap_abstract0_cons(man,ptr(man,ntab,size));
    free(ntab);
    return res;
  }
  else {
    ap_dimension_t dimension = { 0, 0};
    if (size>0){
      dimension = _ap_abstract0_dimension(tab[0]);
    }
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
ap_abstract0_t* ap_abstract0_meet_array(ap_manager_t* man, const ap_abstract0_t*const* tab, size_t size){
  return ap_abstract0_meetjoin_array(AP_FUNID_MEET_ARRAY,man,tab,size);
}
ap_abstract0_t* ap_abstract0_join_array(ap_manager_t* man, const ap_abstract0_t*const* tab, size_t size){
  return ap_abstract0_meetjoin_array(AP_FUNID_JOIN_ARRAY,man,tab,size);
}
ap_abstract0_t* ap_abstract0_meet_lincons_array(ap_manager_t* man,
					  bool destructive,
					  ap_abstract0_t* a,
					  const ap_lincons0_array_t* array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_MEET_LINCONS_ARRAY,man,a) &&
      ap_abstract0_check_lincons_array(AP_FUNID_MEET_LINCONS_ARRAY,man,dimension,array) ){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,array);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
ap_abstract0_t* ap_abstract0_add_ray_array(ap_manager_t* man,
				     bool destructive,
				     ap_abstract0_t* a,
				     const ap_generator0_array_t* array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_ADD_RAY_ARRAY,man,a) &&
      ap_abstract0_check_generator_array(AP_FUNID_ADD_RAY_ARRAY,man,dimension,array)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_ADD_RAY_ARRAY];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,array);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_asssub_linexpr(ap_funid_t funid, 
				      /* either assign or substitute */
				      ap_manager_t* man,
				      bool destructive,
				      ap_abstract0_t* a,
				      ap_dim_t dim, const ap_linexpr0_t* expr,
				      const ap_abstract0_t* dest)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(funid,man,a) &&
      (dest!=NULL ? (ap_abstract0_checkman1(funid,man,dest) && ap_abstract0_check_abstract2(funid,man,a,dest)) : true) &&
      ap_abstract0_check_dim(funid,man,dimension,dim) &&
      ap_abstract0_check_linexpr(funid,man,dimension,expr) ){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,dim,expr,dest);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
ap_abstract0_t* ap_abstract0_assign_linexpr(ap_manager_t* man,
				      bool destructive,
				      ap_abstract0_t* a,
				      ap_dim_t dim, const ap_linexpr0_t* expr,
				      const ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr(AP_FUNID_ASSIGN_LINEXPR,
				  man,destructive,a,dim,expr,dest);
}
ap_abstract0_t* ap_abstract0_substitute_linexpr(ap_manager_t* man,
				      bool destructive,
				      ap_abstract0_t* a,
				      ap_dim_t dim, const ap_linexpr0_t* expr,
				      const ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr(AP_FUNID_SUBSTITUTE_LINEXPR,
				  man,destructive,a,dim,expr,dest);
}
ap_abstract0_t* ap_abstract0_asssub_linexpr_array(ap_funid_t funid,
						  ap_manager_t* man,
						  bool destructive,
						  ap_abstract0_t* a,
						  const ap_dim_t* tdim,
						  const ap_linexpr0_t*const* texpr,
						  size_t size,
						  const ap_abstract0_t* dest)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(funid,man,a) &&
      (dest!=NULL ? (ap_abstract0_checkman1(funid,man,dest) && ap_abstract0_check_abstract2(funid,man,a,dest)) : true) &&
      ap_abstract0_check_dim_array(funid,man,dimension,tdim,size) &&
      ap_abstract0_check_linexpr_array(funid,man,dimension,texpr,size) ){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,tdim,texpr,size,dest);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
ap_abstract0_t* ap_abstract0_assign_linexpr_array(ap_manager_t* man,
					    bool destructive,
					    ap_abstract0_t* a,
					    const ap_dim_t* tdim,
					    const ap_linexpr0_t*const* texpr,
					    size_t size,
					    const ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
					man,destructive,a,tdim,texpr,size,dest);
}
ap_abstract0_t* ap_abstract0_substitute_linexpr_array(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						const ap_dim_t* tdim,
						const ap_linexpr0_t*const* texpr,
						size_t size,
						const ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
					man,destructive,a,tdim,texpr,size,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_forget_array(ap_manager_t* man,
				    bool destructive,
				    ap_abstract0_t* a,
				    const ap_dim_t* tdim, size_t size,
				    bool project)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_FORGET_ARRAY,man,a) &&
      ap_abstract0_check_dim_array(AP_FUNID_FORGET_ARRAY,man,dimension,tdim,size)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_FORGET_ARRAY];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,tdim,size,project);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_add_dimensions(ap_manager_t* man,
				      bool destructive,
				      ap_abstract0_t* a,
				      ap_dimchange_t* dimchange)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_ADD_DIMENSIONS,man,a) &&
      ap_abstract0_check_ap_dimchange_add(AP_FUNID_ADD_DIMENSIONS,man,dimension,dimchange)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_ADD_DIMENSIONS];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,dimchange);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim+dimchange->intdim,
			 dimension.realdim+dimchange->realdim);
  }
}
ap_abstract0_t* ap_abstract0_remove_dimensions(ap_manager_t* man,
					 bool destructive,
					 ap_abstract0_t* a,
					 ap_dimchange_t* dimchange)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_REMOVE_DIMENSIONS,man,a) &&
      ap_abstract0_check_ap_dimchange_remove(AP_FUNID_REMOVE_DIMENSIONS,man,dimension,dimchange)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_REMOVE_DIMENSIONS];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,dimchange);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim-dimchange->intdim >= 0 ? dimension.intdim-dimchange->intdim : 0,
			 dimension.realdim-dimchange->realdim >= 0 ? dimension.realdim-dimchange->realdim : 0);
  }
}
ap_abstract0_t* ap_abstract0_permute_dimensions(ap_manager_t* man,
					  bool destructive,
					  ap_abstract0_t* a,
					  const ap_dimperm_t* perm)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_PERMUTE_DIMENSIONS,man,a) &&
      ap_abstract0_check_dimperm(AP_FUNID_PERMUTE_DIMENSIONS,man,dimension,perm)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_PERMUTE_DIMENSIONS];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,perm);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_expand(ap_manager_t* man,
			      bool destructive,
			      ap_abstract0_t* a,
			      ap_dim_t dim,
			      size_t n)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_EXPAND,man,a) &&
      ap_abstract0_check_dim(AP_FUNID_EXPAND,man,dimension,dim)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_EXPAND];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,dim,n);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim + (dim<dimension.intdim ? n : 0),
			 dimension.realdim + (dim<dimension.intdim ? 0 : n));
  }
}
ap_abstract0_t* ap_abstract0_fold(ap_manager_t* man,
			    bool destructive,
			    ap_abstract0_t* a,
			    const ap_dim_t* tdim,
			    size_t size)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_FOLD,man,a) &&
      size>0 &&
      ap_abstract0_check_dim_array(AP_FUNID_FOLD,man,dimension,tdim,size)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_FOLD];
    void* valueold = a->value;
    void* value = ptr(man,destructive,a->value,tdim,size);
    if (value==valueold){
      assert(destructive);
      return a;
    }
    else if (destructive){
      ap_manager_free(a->man);
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    if (destructive) ap_abstract0_free(a->man,a->value);
    return ap_abstract0_top(man,
			 dimension.intdim - ( (size>0 && tdim[0]<dimension.intdim) ? (size-1) : 0),
			 dimension.realdim - ( (size>0 && tdim[0]<dimension.intdim) ? 0 : (size-1)));
  }
}
/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_widening(ap_manager_t* man,
				const ap_abstract0_t* a1, const ap_abstract0_t* a2)
{
    if (ap_abstract0_checkman2(AP_FUNID_WIDENING,man,a1,a2) &&
	ap_abstract0_check_abstract2(AP_FUNID_WIDENING,man,a1,a2)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_WIDENING];
    void* value = ptr(man,a1->value,a2->value);
    return ap_abstract0_cons(man,value);
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a1);
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_closure(ap_manager_t* man, bool destructive, ap_abstract0_t* a)
{
ap_dimension_t dimension = _ap_abstract0_dimension(a);
 if (ap_abstract0_checkman1(AP_FUNID_CLOSURE,man,a)){
   void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_CLOSURE];
   void* valueold = a->value;
   void* value = ptr(man,destructive,a->value);
   if (value==valueold){
     assert(destructive);
     return a;
   }
   else if (destructive){
     ap_manager_free(a->man);
   }
   return ap_abstract0_cons(man,value);
 }
 else {
   if (destructive) ap_abstract0_free(a->man,a->value);
   return ap_abstract0_top(man,
			dimension.intdim,
			dimension.realdim);
 }
}

/* ********************************************************************** */
/* ********************************************************************** */
/* Additional generic functions */
/* ********************************************************************** */
/* ********************************************************************** */

ap_abstract0_t* ap_abstract0_widening_threshold(ap_manager_t* man,
					  const ap_abstract0_t* a1, const ap_abstract0_t* a2,
					  ap_lincons0_array_t* array)
{
  void* (*ptr)(ap_manager_t*,...);
  tbool_t (*ptr2)(ap_manager_t*,...);
  void* value;
  size_t i,j,size;
  ap_lincons0_t tmp;

  ap_dimension_t dimension = _ap_abstract0_dimension(a1);
  if (ap_abstract0_checkman2(AP_FUNID_WIDENING,man,a1,a2) &&
      ap_abstract0_check_abstract2(AP_FUNID_WIDENING,man,a1,a2) &&
      ap_abstract0_check_lincons_array(AP_FUNID_WIDENING,man,dimension,array) ){
    ptr = man->funptr[AP_FUNID_WIDENING];
    value = ptr(man,a1->value,a2->value);

    ptr2 = man->funptr[AP_FUNID_SAT_LINCONS];
    size = array->size;
    i = j = 0;
    while (i<size-j){
      if (ptr2(man,a2->value,array->p[i])==tbool_true){
	i++;
      } 
      else {
	j++;
	tmp = array->p[i]; 
	array->p[i] = array->p[array->size-j];
	array->p[array->size-j] = tmp;
      }
    }
    if (i>0){
      array->size = i;
      ptr = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
      value = ptr(man,true,value,array);
      array->size = size;
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    return ap_abstract0_top(man,
			 dimension.intdim,
			 dimension.realdim);
  }
}
