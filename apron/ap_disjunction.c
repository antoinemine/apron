/* ************************************************************************* */
/* ap_disjunction.c: generic disjunction of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_global0.h"
#include "ap_disjunction.h"

/* ------------------------------------------------------------ */
/* Allocates an abstract value */
/* ------------------------------------------------------------ */

static ap_disjunction_t* ap_disjunction_alloc(size_t size)
{
  size_t i;
  ap_disjunction_t* res = malloc(sizeof(ap_disjunction_t));
  res->size = size;
  res->p = malloc(size * sizeof(void*));
  for (i = 0; i < size; i++)
    res->p[i] = NULL;
  return res;
}
static void ap_disjunction_clear(ap_disjunction_internal_t* intern,
			    ap_disjunction_t* a)
{
  size_t i;
  ap_manager_t* man = intern->manager;
  void (*absfree)(ap_manager_t*, void*) = man->funptr[AP_FUNID_FREE];
  for (i=0; i < a->size; i++) {
    if (a->p[i]!=NULL){
      absfree(man, a->p[i]);
      a->p[i] = NULL;
    }
  }
  free(a->p);
  a->p=NULL;
  a->size = 0;
}
static inline
ap_disjunction_internal_t* get_internal(ap_manager_t* manager)
{
  return (ap_disjunction_internal_t*)manager->internal;
}

/* ============================================================ */
/*   */
/* ============================================================ */

static
void ap_interval_join_with(ap_interval_t* a, ap_interval_t* b)
{
  if (ap_scalar_cmp(a->inf,b->inf)>0){
    ap_scalar_set(a->inf,b->inf);
  }
  if (ap_scalar_cmp(a->sup,b->sup)<0){
    ap_scalar_set(a->sup,b->sup);
  }
}

/*
static void set_bottom(ap_disjunction_internal_t* intern, ap_disjunction_t* a, void* bottom)
{
  ap_manager_t* man = intern->manager;
  ap_dimension_t (*dimension)(ap_manager_t*, ...) = man->funptr[AP_FUNID_DIMENSION];
  void* (*bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_BOTTOM];

  ap_dimension_t dim = dimension(man, a->p[0]);
  assert(a->size>=1);
  ap_disjunction_clear(intern,a);
  a->size = 1;
  a->p[0] = bottom(man, dim.intdim, dim.realdim);
}
*/

static void ap_disjunction_resize(ap_disjunction_t* a)
{
  size_t i,dec;
  i=0; dec=0;
  while (i+dec < a->size) {
    while (i+dec < a->size && a->p[i+dec] == NULL){
      dec++;
    }
    if (dec>0 && i+dec<a->size){
      a->p[i] = a->p[i+dec];
    }
    i++;
  }
  a->size = a->size-dec;
  a->p=realloc(a->p,a->size*sizeof(void*));
}

/* If only bottom values, leaves exactly one such value and frees the other.
   Otherwise, leaves at most one top value and frees the other top values. */
static void ap_disjunction_null_bottom_top(
    ap_disjunction_internal_t* intern,
    ap_disjunction_t* a,
    int* const top,
    bool* const notbottom)
{
  ap_manager_t* man = intern->manager;
  void (*free)(ap_manager_t*, void*) = man->funptr[AP_FUNID_FREE];
  bool (*is_bottom)(ap_manager_t*, void*) = man->funptr[AP_FUNID_IS_BOTTOM];
  bool (*is_top)(ap_manager_t*, void*) = man->funptr[AP_FUNID_IS_TOP];

  int bottom = -1;
  *top = -1;
  *notbottom = false;

  size_t i;
  for (i = 0; i < a->size; i++) {
    if (a->p[i] != NULL){
      if (is_bottom(man,a->p[i])){
	if (bottom>=0) { free(man,a->p[i]); a->p[i]=NULL; }
	else bottom = (int)i;
      }
      else {
	if (is_top(man,a->p[i])){
	  if (*top>=0) { free(man,a->p[i]); a->p[i]=NULL; }
	  else *top = (int)i;
	}
	*notbottom = true;
      }
    }
  }
  if (bottom>=0 && *notbottom){
    free(man,a->p[bottom]);
    a->p[bottom]=NULL;
  }
}


/* applies previous function and remove equal elements */
static void ap_disjunction_elim_redundant(ap_disjunction_internal_t* intern,
					  ap_disjunction_t* a)
{
  ap_manager_t* man = intern->manager;
  void (*free)(ap_manager_t*, void*) = man->funptr[AP_FUNID_FREE];
  bool (*is_eq)(ap_manager_t*,void*,void*) = man->funptr[AP_FUNID_IS_EQ];
  int top;
  bool notbottom;

  size_t i,j;

  ap_disjunction_null_bottom_top(intern,a,&top,&notbottom);
  if (notbottom){
    /* From now on, no bottom and at most one top values */
    for (i = 0; i < a->size; i++) {
      if ((int)i!=top && a->p[i]!=NULL){
	for (j = i+1; j<a->size; j++){
	  if ((int)j!=top && a->p[j]!=NULL){
	    if (is_eq(man,a->p[i],a->p[j])){
	      free(man,a->p[j]);
	      a->p[j]=NULL;
	    }
	  }
	}
      }
    }
  }
  ap_disjunction_resize(a);
}


/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

void ap_disjunction_free(ap_manager_t* manager,
			 ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_disjunction_clear(intern,a);
  free(a);
}

ap_disjunction_t* ap_disjunction_copy(ap_manager_t* manager,
				      ap_disjunction_t* a)
{
  size_t i;
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  void* (*copy)(ap_manager_t*, void*) = man->funptr[AP_FUNID_COPY];

  ap_disjunction_t* res = ap_disjunction_alloc(a->size);
  for (i = 0; i < a->size; i++) {
    res->p[i] = copy(man, a->p[i]);
  }
  return res;
}

size_t ap_disjunction_size(ap_manager_t* manager,
				      ap_disjunction_t* a)
{
  size_t i,res;
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  size_t (*size)(ap_manager_t*, void*) = man->funptr[AP_FUNID_ASIZE];

  res = 0;
  for (i = 0; i < a->size; i++) {
    res += size(man, a->p[i]);
  }
  return res;
}

void ap_disjunction_minimize(ap_manager_t* manager,
			     ap_disjunction_t* a)
{
  size_t i;
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  void* (*minimize)(ap_manager_t*, void*) = man->funptr[AP_FUNID_MINIMIZE];

  ap_disjunction_elim_redundant(intern,a);
  for (i = 0; i < a->size; i++) {
    minimize(man, a->p[i]);
  }
}
void ap_disjunction_canonicalize(ap_manager_t* manager,
				 ap_disjunction_t* a)
{
  size_t i;
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  void* (*canonicalize)(ap_manager_t*, void*) = man->funptr[AP_FUNID_CANONICALIZE];
  ap_disjunction_elim_redundant(intern,a);
  for (i = 0; i < a->size; i++) {
    canonicalize(man, a->p[i]);
  }
}
long ap_disjunction_hash(ap_manager_t* manager,
			 ap_disjunction_t* a)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_HASH, NULL);
  return 0;
}
/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void ap_disjunction_approximate(ap_manager_t* manager, ap_disjunction_t* a,
				int n)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_APPROXIMATE, NULL);
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

/* Print 'a' on the stream */
void ap_disjunction_fprint(FILE* stream, ap_manager_t* manager,
			   ap_disjunction_t* a,
			   char** name_of_dim)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  void (*ptr)(FILE* stream, ap_manager_t*,void*,char**) = man->funptr[AP_FUNID_FPRINT];

  fprintf(stream,"disjunction of library %s\n",man->library);

  size_t i;
  for (i=0;i<a->size;i++){
    ptr(stream, man, a->p[i], name_of_dim);
  }
}

/* print the difference between a1 (old value) and a2 (new value) */
void ap_disjunction_fprintdiff(FILE* stream, ap_manager_t* manager,
			       ap_disjunction_t* a1, ap_disjunction_t* a2,
			       char** name_of_dim)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_FPRINTDIFF, NULL);
}

/* dump the internal representation of a for debugging purposes */
void ap_disjunction_fdump(FILE* stream, ap_manager_t* manager,
			  ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  void (*ptr)(FILE* stream, ap_manager_t*,void*) = man->funptr[AP_FUNID_FDUMP];

  fprintf(stream,"disjunction of library %s\n",man->library);

  size_t i;
  for (i=0;i<a->size;i++){
    ptr(stream,man,a->p[i]);
  }
}

/* ********************************************************************** */
/* II. Constructor, accessory, tests and property extraction */
/* ********************************************************************** */
/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

static ap_disjunction_t* ap_disjunction_of_one(void* abs)
{
  ap_disjunction_t* res = ap_disjunction_alloc(1);
  res->p[0] = abs;
  return res;
}

#define BOTTOM_TOP(NAME,FUNID)						\
  ap_disjunction_t* ap_disjunction_##NAME(ap_manager_t* manager, size_t intdim, size_t realdim) \
  {									\
    ap_disjunction_internal_t* intern = get_internal(manager);	\
    ap_manager_t* man = intern->manager;				\
    void* (*ptr)(ap_manager_t*,size_t,size_t) = man->funptr[FUNID];		\
    void* abs = ptr(man,intdim,realdim);				\
									\
    return ap_disjunction_of_one(abs);					\
  }

BOTTOM_TOP(bottom,AP_FUNID_BOTTOM)
BOTTOM_TOP(top,AP_FUNID_TOP)

ap_disjunction_t* ap_disjunction_of_box(ap_manager_t* manager,
					size_t intdim, size_t realdim,
					ap_interval_t** tinterval)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  void* abs;

  ap_manager_t* man = intern->manager;
  void* (*ptr)(ap_manager_t*, size_t,size_t,ap_interval_t**) = man->funptr[AP_FUNID_OF_BOX];
  abs = ptr(man, intdim, realdim, tinterval);
  return ap_disjunction_of_one(abs);
}

ap_dimension_t ap_disjunction_dimension(ap_manager_t* manager,
					ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  ap_dimension_t (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_DIMENSION];
  return ptr(man, a->p[0]);
}

/* ============================================================ */
/* II.3 Test Functions */
/* ============================================================ */

bool ap_disjunction_is_bottom(ap_manager_t* manager, ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  bool (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_IS_BOTTOM];

  return a->size==1 && ptr(man,a->p[0]);
}

bool ap_disjunction_is_top(ap_manager_t* manager, ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  bool (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_IS_TOP];

  return a->size==1 && ptr(man,a->p[0]);
}

/* ======================================================================= */

bool ap_disjunction_is_eq(ap_manager_t* manager, ap_disjunction_t* a,
			  ap_disjunction_t* b)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  return false;
  /*
  bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_EQ];
  size_t i,j;
  bool eq;
  bool seen[b->size];

  ap_disjunction_elim_redundant(intern,a);
  ap_disjunction_elim_redundant(intern,b);
  if (a->size != b->size)
    return false;
  if ((ap_disjunction_is_bottom(manager,a) &&
       ap_disjunction_is_bottom(manager,b)) ||
      (ap_disjunction_is_top(manager,a) &&
       ap_disjunction_is_top(manager,b))){
    return true;
  }
  eq = true;
  for (j=0;j<b->size;j++) seen[j] = false;
  for (i=0; i<a->size; i++){
    bool ieq = false;
    for (j=0;j<b->size;j++){
      if (!seen[j]){
	ieq = ptr(man,a->p[i],b->p[i]);
	if (ieq){
	  seen[j] = true;
	  break;
	}
      }
    }
    eq = ieq;
    if (!eq) break;
  }
  return eq;
  */
}

bool ap_disjunction_is_leq(ap_manager_t* manager, ap_disjunction_t* a,
			   ap_disjunction_t* b)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  return false;
  /*
  bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_LEQ];
  size_t i,j;
  bool leq;

  if (ap_disjunction_is_bottom(manager,a) || ap_disjunction_is_top(manager,b)){
    return true;
  }
  leq = true;
  for (i=0; i<a->size; i++){
    bool ileq = false;
    for (j=0;j<b->size;j++){
      ileq = ptr(man,a->p[i],b->p[i]);
      if (ileq) break;
    }
    leq = ileq;
    if (!leq) break;
  }
  return leq;
  */
}


bool ap_disjunction_sat_lincons(ap_manager_t* manager, ap_disjunction_t* a,
				ap_lincons0_t* lincons)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  bool (*ptr)(ap_manager_t*, void*, ap_lincons0_t*) = man->funptr[AP_FUNID_SAT_LINCONS];

  size_t i;
  bool res = true;
  for (i = 0; i < a->size; i++) {
    res = res && ptr(man, a->p[i], lincons);
    if (!res)
      break;
  }
  return res;
}

bool ap_disjunction_sat_tcons(ap_manager_t* manager, ap_disjunction_t* a,
			      ap_tcons0_t* tcons)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  bool (*ptr)(ap_manager_t*, void*, ap_tcons0_t*) = man->funptr[AP_FUNID_SAT_TCONS];

  size_t i;
  bool res = true;
  for (i = 0; i < a->size; i++) {
    res = res && ptr(man, a->p[i], tcons);
    if (!res)
      break;
  }
  return res;
}

bool ap_disjunction_sat_interval(ap_manager_t* manager, ap_disjunction_t* a,
				 ap_dim_t dim, ap_interval_t* interval)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  bool (*ptr)(ap_manager_t*, void*, ap_dim_t, ap_interval_t*) = man->funptr[AP_FUNID_SAT_INTERVAL];

  size_t i;
  bool res = true;
  for (i = 0; i < a->size; i++) {
    res = res && ptr(man, a->p[i], dim, interval);
    if (!res)
      break;
  }
  return res;
}

bool ap_disjunction_is_dimension_unconstrained(ap_manager_t* manager,
					       ap_disjunction_t* a, ap_dim_t dim)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
			     AP_FUNID_IS_DIMENSION_UNCONSTRAINED,NULL);
  return false;
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* ap_disjunction_bound_linexpr(
    ap_manager_t* manager, ap_disjunction_t* a, ap_linexpr0_t* expr
)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_interval_t* (*ptr)(ap_manager_t*, void*, ap_linexpr0_t*) = man->funptr[AP_FUNID_BOUND_LINEXPR];

  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_bottom(gres);

  size_t i;
  for (i = 0; i < a->size; i++) {
    ap_interval_t* res = ptr(man, a->p[i], expr);
    ap_interval_join_with(gres, res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))
      break;
  }
  return gres;
}

ap_interval_t* ap_disjunction_bound_texpr(
    ap_manager_t* manager, ap_disjunction_t* a, ap_texpr0_t* expr)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_bottom(gres);

  ap_interval_t* (*ptr)(ap_manager_t*, void*, ap_texpr0_t*) = man->funptr[AP_FUNID_BOUND_TEXPR];

  size_t i;
  for (i = 0; i < a->size; i++) {
    ap_interval_t* res = ptr(man, a->p[i], expr);
    ap_interval_join_with(gres, res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))
      break;
  }
  return gres;
}

ap_interval_t* ap_disjunction_bound_dimension(ap_manager_t* manager,
					      ap_disjunction_t* a, ap_dim_t dim)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_bottom(gres);

  ap_interval_t* (*ptr)(ap_manager_t*, void*, ap_dim_t) =	man->funptr[AP_FUNID_BOUND_DIMENSION];

  size_t i;
  for (i = 0; i < a->size; i++) {
    ap_interval_t* res = ptr(man, a->p[i], dim);
    ap_interval_join_with(gres, res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))   /* ?*/
      break;
  }
  return gres;
}

/* ===================================================================== */
/* Convert abstract value 'a' to a disjunction of linear constraints. */
/* ===================================================================== */

ap_lincons0_array_t ap_disjunction_to_lincons_array(ap_manager_t* manager,
						    ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  ap_lincons0_array_t garray;

  if (a->size==1){
    ap_lincons0_array_t (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];
    garray = ptr(man,a->p[0]);
  }
  else {
    garray.p = NULL;
    garray.size = 0;
    ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
			       AP_FUNID_TO_LINCONS_ARRAY,NULL);
  }
  return garray;
}
ap_tcons0_array_t ap_disjunction_to_tcons_array(ap_manager_t* manager,
						ap_disjunction_t* a)
{
  ap_tcons0_array_t garray;
  garray.p = NULL;
  garray.size = 0;
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
			     AP_FUNID_TO_TCONS_ARRAY,NULL);
  return garray;
}

ap_lincons0_array_t ap_disjunction_to_lincons0_set(ap_manager_t* manager,
						   ap_disjunction_t* a)
{

  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_lincons0_array_t garray;
  garray.p = NULL;
  garray.size = 0;

  ap_lincons0_array_t (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];

  size_t i,j;
  for (i=0;i<a->size;i++){
    ap_lincons0_array_t array = ptr(man,a->p[i]);
    ap_lincons0_array_resize(&garray,garray.size + array.size);
    garray.size -= array.size;
    for (j=0;j<array.size;j++){
      garray.p[garray.size + j] = array.p[j];
    }
    garray.size += array.size;
    free(array.p);
  }
  return garray;
}

/* ====================================================================== */
/* Convert 'a' to an interval/hyper-cube. */
/* ====================================================================== */

ap_interval_t** ap_disjunction_to_box(ap_manager_t* manager,
				      ap_disjunction_t* a)
{

  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_interval_t** gbox = NULL;
  size_t nbdims = 0;

  ap_interval_t** (*ptr)(ap_manager_t*, void*) = man->funptr[AP_FUNID_TO_BOX];
  ap_dimension_t (*ptrdimension)(ap_manager_t*, void*) = man->funptr[AP_FUNID_DIMENSION];
  ap_dimension_t dimension = ptrdimension(man, a->p[0]);
  nbdims = dimension.intdim + dimension.realdim;

  size_t i, j;
  for (i = 0; i < a->size; i++) {
    ap_interval_t** box = ptr(man, a->p[i]);
    if (i == 0) {
      gbox = box;
    } else {
      for (j = 0; j < nbdims; j++) {
	ap_interval_join_with(gbox[j], box[j]);
	if (ap_interval_is_bottom(gbox[j]))
	  break;
      }
      ap_interval_array_free(box, nbdims);
      if (j < nbdims) {
	for (j = 0; j < nbdims; j++)
	  ap_interval_set_bottom(gbox[j]);
	break;
      }
    }
  }
  return gbox;
}

/* ====================================================================== */
/* Convert 'a' to an array of generators. */
/* ====================================================================== */

ap_generator0_array_t ap_disjunction_to_generator_array(ap_manager_t* manager,
							ap_disjunction_t* a)
{
  ap_generator0_array_t array;
  array.p = NULL;
  array.size = 0;
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,	AP_FUNID_TO_GENERATOR_ARRAY, NULL);
  return array;
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */


/* ====================================================================== */
/* Meet and Join of two abstract values */
/* ====================================================================== */

ap_disjunction_t* ap_disjunction_join(ap_manager_t* manager,
				      bool destructive,
				      ap_disjunction_t* a1,
				      ap_disjunction_t* a2)
{
  if (ap_disjunction_is_bottom(manager,a1)){
    if (destructive){
      ap_disjunction_free(manager,a1);
    }
    return ap_disjunction_copy(manager,a2);
  }
  if (ap_disjunction_is_bottom(manager,a2)){
    return destructive ? a1 : ap_disjunction_copy(manager,a1);
  }

  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  void* (*copy)(ap_manager_t*, void*) = man->funptr[AP_FUNID_COPY];
  ap_disjunction_t* res = ap_disjunction_alloc(a1->size+a2->size);
  size_t i,j;
  for (i=0; i<a1->size; i++){
    res->p[i] = destructive ? a1->p[i] : copy(man,a1->p[i]);
  }
  for (i=0; i<a2->size; i++){
    res->p[a1->size+i] = copy(man,a2->p[i]);
  }
  {
    int top;
    bool notbottom;
    ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
    ap_disjunction_resize(res);
  }
  /*
  ap_disjunction_elim_redundant(intern, res);
  */
  return res;
}

ap_disjunction_t* ap_disjunction_join_array(ap_manager_t* manager,
					    ap_disjunction_t** tab, size_t size)
{


  if (size==0){
    ap_manager_raise_exception(manager, AP_EXC_INVALID_ARGUMENT, AP_FUNID_JOIN_ARRAY, "empty array in join_array");
    return NULL;
  }
  else if (size==1){
    return ap_disjunction_copy(manager,tab[0]);
  }
  else if (size==2){
    return ap_disjunction_join(manager,false,tab[0],tab[1]);
  }
  else {
    ap_disjunction_internal_t* intern = get_internal(manager);
    ap_manager_t* man = intern->manager;
    void* (*copy)(ap_manager_t*, void*) = man->funptr[AP_FUNID_COPY];

    size_t length,i,j,l;
    ap_disjunction_t* res;
    int bottom = -1;

    length = 0;
    for (i=0; i<size; i++) length += tab[i]->size;
    res = ap_disjunction_alloc(length);

    l = 0;
    for (i=0; i<size; i++){
      if (ap_disjunction_is_bottom(manager,tab[i])){
	bottom = (int)i;
      }
      else {
	for (j=0; j<tab[i]->size; j++){
	  res->p[l] = copy(man,tab[i]->p[j]);
	  l++;
	}
      }
    }
    if (length==0){
      assert(bottom>=0);
      ap_disjunction_free(manager,res);
      return ap_disjunction_copy(manager,tab[bottom]);
    }
    else {
      {
	int top;
	bool notbottom;
	ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
	ap_disjunction_resize(res);
      }
      /*
	ap_disjunction_elim_redundant(intern, res);
      */
      return res;
    }
  }
}

ap_disjunction_t* ap_disjunction_meet(ap_manager_t* manager, bool destructive,
				      ap_disjunction_t* a1, ap_disjunction_t* a2)
{
  if (ap_disjunction_is_bottom(manager,a1)){
    return destructive ? a1 : ap_disjunction_copy(manager,a1);
  }
  if (ap_disjunction_is_bottom(manager,a2)){
    if (destructive){
      ap_disjunction_free(manager,a1);
    }
    return ap_disjunction_copy(manager,a2);
  }

  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;
  void* (*meet)(ap_manager_t*, bool, void*, void*) = man->funptr[AP_FUNID_MEET];
  void* (*is_bottom)(ap_manager_t*, void*) = man->funptr[AP_FUNID_IS_BOTTOM];

  ap_disjunction_elim_redundant(intern, a1);
  ap_disjunction_elim_redundant(intern, a2);

  ap_disjunction_t* res = ap_disjunction_alloc(a1->size*a2->size);
  size_t i,j,k;
  k = 0;
  for (i=0; i<a1->size; i++){
    for (j=0; j<a2->size; j++){
      void* res1 = meet(man,false,a1->p[i],a2->p[j]);
      if (!is_bottom(man,res1)){
	res->p[k] = res1;
	k++;
      }
    }
  }
  if (k==0){
    assert(res->size>=1);
    ap_dimension_t dim = ap_abstract0_dimension(man,a1->p[0]);
    res->p[0] = ap_abstract0_top(man,dim.intdim,dim.realdim);
    res->size = 1;
  }
  else {
    res->size = k;
    ap_disjunction_elim_redundant(intern, res);
  }
  if (destructive){
    ap_disjunction_free(manager,a1);
  }
  return res;
}

ap_disjunction_t* ap_disjunction_meet_array(ap_manager_t* manager,
					    ap_disjunction_t** tab, size_t size)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_MEET_ARRAY, NULL);
  return NULL;
}

/* ************************************************************************* */
//Meet of the abstract value a with the set of constraints array.
/* ************************************************************************* */

ap_disjunction_t* ap_disjunction_meet_lincons_array(ap_manager_t* manager,
						    bool destructive, ap_disjunction_t* a, ap_lincons0_array_t* array)
{

  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_lincons0_array_t*) = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];

  int top;
  bool notbottom;
  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], array);
  }
  ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
  ap_disjunction_resize(res);
  return res;
}

ap_disjunction_t* ap_disjunction_meet_tcons_array(ap_manager_t* manager,bool destructive,
						  ap_disjunction_t* a, ap_tcons0_array_t* array)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_tcons0_array_t*) = man->funptr[AP_FUNID_MEET_TCONS_ARRAY];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], array);
  }
  {
    int top;
    bool notbottom;
    ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
    ap_disjunction_resize(res);
  }
  return res;
}

//  ????????????????????????
/* ************************************************************************ */
ap_disjunction_t* ap_disjunction_add_ray_array(ap_manager_t* manager,
					       bool destructive, ap_disjunction_t* a, ap_lincons0_array_t* array)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_lincons0_array_t*) = man->funptr[AP_FUNID_ADD_RAY_ARRAY];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], array);
  }
  {
    int top;
    bool notbottom;
    ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
    ap_disjunction_resize(res);
  }
  /*
    ap_disjunction_elim_redundant(intern,res);
  */
  return res;
}

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_asssub_linexpr_array(
    ap_funid_t funid,
    /* either assign or substitute */
    ap_manager_t* manager, bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
    ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dim_t*, ap_linexpr0_t**, size_t, void*) = man->funptr[funid];
  bool (*is_bottom)(ap_manager_t*, void*) =	man->funptr[AP_FUNID_IS_BOTTOM];

  if (dest!=NULL){
    ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, funid, "assign or substitute supported only when dest==NULL");
    return NULL;
  }

  size_t i;
  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], tdim, texpr, size, NULL);
  }
  return res;
}

ap_disjunction_t* ap_disjunction_assign_linexpr_array(
    ap_manager_t* man,
    bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
    ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  return ap_disjunction_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
					     man, destructive, a, tdim, texpr, size, dest);
}

ap_disjunction_t* ap_disjunction_substitute_linexpr_array(ap_manager_t* man,
							  bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
							  ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  return ap_disjunction_asssub_linexpr_array(	AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
						man, destructive, a, tdim, texpr, size, dest);
}

ap_disjunction_t* ap_disjunction_asssub_texpr_array(
    ap_funid_t funid,
    /* either assign or substitute */
    ap_manager_t* manager, bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
    ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dim_t*, ap_texpr0_t**, size_t, void*) = man->funptr[funid];
  bool (*is_bottom)(ap_manager_t*, void*) =	man->funptr[AP_FUNID_IS_BOTTOM];

  if (dest!=NULL){
    ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, funid, "assign or substitute supported only when dest==NULL");
    return NULL;
  }

  size_t i;
  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], tdim, texpr, size, NULL);
  }
  return res;
}

ap_disjunction_t* ap_disjunction_assign_texpr_array(ap_manager_t* man,
						    bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
						    ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  return ap_disjunction_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY, man,
					   destructive, a, tdim, texpr, size, dest);
}

ap_disjunction_t* ap_disjunction_substitute_texpr_array(ap_manager_t* man,
							bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
							ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
  return ap_disjunction_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
					   man, destructive, a, tdim, texpr, size, dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_forget_array(
    ap_manager_t* manager,
    bool destructive, ap_disjunction_t* a, ap_dim_t* tdim, size_t size,
    bool project)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dim_t*, size_t, bool) = man->funptr[AP_FUNID_FORGET_ARRAY];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], tdim, size, project);
  }
  /*
  {
    int top;
    bool notbottom;
    ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
    ap_disjunction_resize(res);
  }
  */
  ap_disjunction_elim_redundant(intern,res);
  return res;
}

ap_disjunction_t* ap_disjunction_add_dimensions(
    ap_manager_t* manager,
    bool destructive, ap_disjunction_t* a, ap_dimchange_t* dimchange,
    bool project)
{
  ap_disjunction_internal_t* intern = get_internal(manager);

  ap_manager_t* man = intern->manager;
  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dimchange_t*, bool) = man->funptr[AP_FUNID_ADD_DIMENSIONS];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], dimchange, project);
  }
  return res;
}

ap_disjunction_t* ap_disjunction_remove_dimensions(ap_manager_t* manager,
						   bool destructive, ap_disjunction_t* a, ap_dimchange_t* dimchange)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dimchange_t*) = man->funptr[AP_FUNID_REMOVE_DIMENSIONS];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], dimchange);
  }
  /*
  {
    int top;
    bool notbottom;
    ap_disjunction_null_bottom_top(intern,res,&top,&notbottom);
    ap_disjunction_resize(res);
  }
  */
  ap_disjunction_elim_redundant(intern,res);
  return res;
}

ap_disjunction_t* ap_disjunction_permute_dimensions(ap_manager_t* manager,
						    bool destructive, ap_disjunction_t* a, ap_dimperm_t* perm)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dimperm_t*) = man->funptr[AP_FUNID_PERMUTE_DIMENSIONS];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], perm);
  }
  return res;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

//expanding
/* ***************************************************************************** */
ap_disjunction_t* ap_disjunction_expand(ap_manager_t* manager,
					bool destructive,
					ap_disjunction_t* a,
					ap_dim_t dim,
					size_t n)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dim_t, size_t) = man->funptr[AP_FUNID_EXPAND];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], dim, n);
  }
  return res;
}

//folding
/* ***************************************************************************** */
ap_disjunction_t* ap_disjunction_fold(ap_manager_t* manager,
				      bool destructive,
				      ap_disjunction_t* a,
				      ap_dim_t* tdim,
				      size_t size)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*, ap_dim_t*, size_t) = man->funptr[AP_FUNID_FOLD];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i], tdim, size);
  }
  /*
  ap_disjunction_elim_redundant(intern,res);
  */
  return res;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

// disjunctive widening
/* *************************************************************************** */
void* ap_disjunction_widening(ap_manager_t* manager,
			      ap_disjunction_t* a1, ap_disjunction_t* a2)
{
  ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_WIDENING, NULL);
  return NULL;
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_closure(ap_manager_t* manager,
					 bool destructive, ap_disjunction_t* a)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

  void* (*ptr)(ap_manager_t*, bool, void*) = man->funptr[AP_FUNID_CLOSURE];

  size_t i;
  for (i = 0; i < a->size; i++) {
    res->p[i] = ptr(man, destructive, a->p[i]);
  }
  return res;
}

/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

void ap_disjunction_internal_free(void* p)
{
  ap_disjunction_internal_t* intern = (ap_disjunction_internal_t*) p;

  ap_manager_free(intern->manager);
  intern->manager = NULL;
  free(intern->library);
  free(intern);
}

ap_manager_t* ap_disjunction_manager_alloc(
    ap_manager_t* underlying,
    void(*merge)(ap_manager_t*, ap_disjunction_t*)
)
{
  ap_disjunction_internal_t* internal;
  ap_manager_t* man;
  ap_funid_t funid;
  void** funptr;

  /* creating internal */
  internal = malloc(sizeof(ap_disjunction_internal_t));
  internal->manager = ap_manager_copy(underlying);
  internal->merge = merge;

  size_t sz = 20+strlen(underlying->library);
  internal->library = malloc(sz);
  snprintf(internal->library,sz,"disjunction of %s",underlying->library);

  /* allocating managers */
  man = ap_manager_alloc(internal->library, underlying->version, internal,
			 &ap_disjunction_internal_free);
  /* default options */
  for (funid = 0; funid < AP_FUNID_SIZE; funid++) {
    man->option.funopt[funid].algorithm = 0x2;
  }
  man->option.funopt[AP_FUNID_COPY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_FREE].algorithm = 0x0;
  man->option.funopt[AP_FUNID_ASIZE].algorithm = 0x0;
  man->option.funopt[AP_FUNID_MINIMIZE].algorithm = 0x1;
  man->option.funopt[AP_FUNID_CANONICALIZE].algorithm = 0x1;
  man->option.funopt[AP_FUNID_HASH].algorithm = 0x1;
  man->option.funopt[AP_FUNID_APPROXIMATE].algorithm = 0x1;
  man->option.funopt[AP_FUNID_IS_BOTTOM].algorithm = 0x1;
  man->option.funopt[AP_FUNID_IS_TOP].algorithm = 0x1;
  man->option.funopt[AP_FUNID_IS_LEQ].algorithm = 0x1;
  man->option.funopt[AP_FUNID_IS_EQ].algorithm = 0x1;
  man->option.funopt[AP_FUNID_SAT_LINCONS].algorithm = 0x1;
  man->option.funopt[AP_FUNID_SAT_TCONS].algorithm = 0x1;
  man->option.funopt[AP_FUNID_SAT_INTERVAL].algorithm = 0x1;
  man->option.funopt[AP_FUNID_IS_DIMENSION_UNCONSTRAINED].algorithm = 0x1;
  man->option.funopt[AP_FUNID_BOUND_LINEXPR].algorithm = 0x1;
  man->option.funopt[AP_FUNID_BOUND_TEXPR].algorithm = 0x1;
  man->option.funopt[AP_FUNID_BOUND_DIMENSION].algorithm = 0x1;
  man->option.funopt[AP_FUNID_TO_LINCONS_ARRAY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_TO_TCONS_ARRAY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_TO_BOX].algorithm = 0x1;
  man->option.funopt[AP_FUNID_TO_GENERATOR_ARRAY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_JOIN].algorithm = 0x1;
  man->option.funopt[AP_FUNID_JOIN_ARRAY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_ADD_RAY_ARRAY].algorithm = 0x1;
  man->option.funopt[AP_FUNID_FORGET_ARRAY].algorithm = 0x3;
  man->option.funopt[AP_FUNID_ADD_DIMENSIONS].algorithm = 0x0;
  man->option.funopt[AP_FUNID_PERMUTE_DIMENSIONS].algorithm = 0x0;
  man->option.funopt[AP_FUNID_EXPAND].algorithm = 0x1;
  man->option.funopt[AP_FUNID_WIDENING].algorithm = 0x0;
  man->option.funopt[AP_FUNID_CLOSURE].algorithm = 0x1;

  /* Virtual table */
  funptr = man->funptr;

  funptr[AP_FUNID_COPY] = &ap_disjunction_copy;
  funptr[AP_FUNID_FREE] = &ap_disjunction_free;
  funptr[AP_FUNID_ASIZE] = &ap_disjunction_size;
  funptr[AP_FUNID_MINIMIZE] = &ap_disjunction_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &ap_disjunction_canonicalize;
  funptr[AP_FUNID_HASH] = &ap_disjunction_hash;
  funptr[AP_FUNID_APPROXIMATE] = &ap_disjunction_approximate;
  funptr[AP_FUNID_BOTTOM] = &ap_disjunction_bottom;
  funptr[AP_FUNID_TOP] = &ap_disjunction_top;
  funptr[AP_FUNID_OF_BOX] = &ap_disjunction_of_box;
  funptr[AP_FUNID_DIMENSION] = &ap_disjunction_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &ap_disjunction_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &ap_disjunction_is_top;
  funptr[AP_FUNID_IS_LEQ] = &ap_disjunction_is_leq;
  funptr[AP_FUNID_IS_EQ] = &ap_disjunction_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED]
    = &ap_disjunction_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &ap_disjunction_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &ap_disjunction_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &ap_disjunction_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &ap_disjunction_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &ap_disjunction_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &ap_disjunction_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &ap_disjunction_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &ap_disjunction_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &ap_disjunction_to_tcons_array;
  funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &ap_disjunction_to_generator_array;
  funptr[AP_FUNID_MEET] = &ap_disjunction_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &ap_disjunction_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &ap_disjunction_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &ap_disjunction_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &ap_disjunction_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &ap_disjunction_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &ap_disjunction_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY]
    = &ap_disjunction_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY]
    = &ap_disjunction_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &ap_disjunction_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY]
    = &ap_disjunction_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &ap_disjunction_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &ap_disjunction_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &ap_disjunction_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &ap_disjunction_forget_array;
  funptr[AP_FUNID_EXPAND] = &ap_disjunction_expand;
  funptr[AP_FUNID_FOLD] = &ap_disjunction_fold;
  funptr[AP_FUNID_WIDENING] = &ap_disjunction_widening;
  funptr[AP_FUNID_CLOSURE] = &ap_disjunction_closure;

  return man;
}

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

/* Decompose a disjunctive abstract value into an array of abstract values */
void** ap_disjunction_decompose(ap_manager_t* manager, bool destructive,
				ap_disjunction_t* a, size_t* psize)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  void* (*copy)(ap_manager_t*, void*) = man->funptr[AP_FUNID_COPY];

  *psize = a->size;

  if (destructive){
    void** res = a->p;
    free(a);
    return res;
  }
  else {
    void** res = (void**) malloc(a->size * sizeof(void*));
    size_t i;
    for (i = 0; i < a->size; i++) {
      res[i] = copy(man, a->p[i]);
    }
    return res;
  }
}

/* Create a disjunctive abstract value from an array */
ap_disjunction_t* ap_disjunction_compose(ap_manager_t* manager,
					 bool destructive,
					 void** tabs, size_t size)
{
  ap_disjunction_internal_t* intern = get_internal(manager);
  ap_manager_t* man = intern->manager;

  ap_disjunction_t* res = ap_disjunction_alloc(size);

  void* (*copy)(ap_manager_t*, void*) = man->funptr[AP_FUNID_COPY];

  size_t i;
  for (i = 0; i < size; i++) {
    res->p[i] = destructive ? tabs[i] : copy(man, tabs[i]);
  }
  return res;
}
