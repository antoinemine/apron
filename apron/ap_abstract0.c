/* ************************************************************************* */
/* ap_abstract0.c: generic interface */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_abstract0.h"
#include "ap_check0.h"

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_copy(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_COPY,man,a)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_COPY];
    return ap_abstract0_cons(man,ptr(man,a->value));
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    return ap_abstract0_top(man,dimension);
  }
}

void ap_abstract0_free(ap_manager_t* man, ap_abstract0_t* a)
{
  if (a->man==NULL){
    fprintf(stderr,"ap_abstract0_c: ap_abstract0_free: the abstract value has probably already been deallocated !\n");
    abort();
  }
  if (ap_check0_man1(AP_FUNID_FREE,man,a)){
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
size_t ap_abstract0_size(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_ASIZE,man,a)){
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
void ap_abstract0_minimize(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_MINIMIZE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_MINIMIZE];
    ptr(man,a->value);
  }
}
void ap_abstract0_canonicalize(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_CANONICALIZE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_CANONICALIZE];
    ptr(man,a->value);
  }
}
int ap_abstract0_hash(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_CANONICALIZE,man,a)){
    int (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_HASH];
    return ptr(man,a->value);
  }
  else
    return 0;
}
void ap_abstract0_approximate(ap_manager_t* man, ap_abstract0_t* a, int n)
{
  if (ap_check0_man1(AP_FUNID_APPROXIMATE,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_APPROXIMATE];
    ptr(man,a->value,n);
  }
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */
void ap_abstract0_fprint(FILE* stream, ap_manager_t* man,
			 ap_abstract0_t* a,
			 char** name_of_dim)
{
  if (ap_check0_man1(AP_FUNID_FPRINT,man,a)){
    void (*ptr)(FILE*,ap_manager_t*,...) = man->funptr[AP_FUNID_FPRINT];
    ptr(stream,man,a->value,name_of_dim);
  }
  else {
    fprintf(stream,"ap_abstract0_c: ap_abstract0_fprint: INVALID_ARGUMENT\n");
  }
}

void ap_abstract0_fprintdiff(FILE* stream, ap_manager_t* man,
			     ap_abstract0_t* a, ap_abstract0_t* b,
			     char** name_of_dim)
{
  if (ap_check0_man2(AP_FUNID_FPRINTDIFF,man,a,b) &&
      ap_check0_abstract2(AP_FUNID_FPRINTDIFF,man,a,b) ){
    void (*ptr)(FILE*,ap_manager_t*,...) = man->funptr[AP_FUNID_FPRINTDIFF];
    ptr(stream,man,a->value,b->value,name_of_dim);
  }
  else {
    fprintf(stream,"ap_abstract0_c: ap_abstract0_fprintdiff: INVALID ARGUMENT\n");
  }
}
void ap_abstract0_fdump(FILE* stream, ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_FDUMP,man,a)){
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
ap_membuf_t ap_abstract0_serialize_raw(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_SERIALIZE_RAW,man,a)){
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
ap_abstract0_t* ap_abstract0_bottom(ap_manager_t* man, ap_dimension_t dim)
{
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOTTOM];
  return ap_abstract0_cons(man,ptr(man,dim));
}
ap_abstract0_t* ap_abstract0_top(ap_manager_t* man, ap_dimension_t dim)
{
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TOP];
  return ap_abstract0_cons(man,ptr(man,dim));
}
ap_abstract0_t* ap_abstract0_of_box(ap_manager_t* man,
				    ap_dimension_t dim,
				    ap_linexpr0_t box){
  void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_OF_BOX];
  return ap_abstract0_cons(man,ptr(man,dim,box));
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */
ap_dimension_t ap_abstract0_dimension(ap_manager_t* man, ap_abstract0_t* a)
{
  ap_check0_man1(AP_FUNID_DIMENSION,man,a);
  ap_dimension_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_DIMENSION];
  return ptr(a->man,a->value);
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */
bool ap_abstract0_is_bottom(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_IS_BOTTOM,man,a)){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_BOTTOM];
    return ptr(man,a->value);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_is_top(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_IS_TOP,man,a)){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_TOP];
    return ptr(man,a->value);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_is_leq(ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  if (a1==a2){
    return true;
  }
  else if (ap_check0_man2(AP_FUNID_IS_LEQ,man,a1,a2) &&
	   ap_check0_abstract2(AP_FUNID_IS_EQ,man,a1,a2)){
    if (a1->value==a2->value) return true;
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_LEQ];
    return ptr(man,a1->value,a2->value);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_is_eq(ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  if (a1==a2){
    return true;
  }
  if (ap_check0_man2(AP_FUNID_IS_EQ,man,a1,a2) &&
      ap_check0_abstract2(AP_FUNID_IS_EQ,man,a1,a2)){
    if (a1->value==a2->value) return true;
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_EQ];
    return ptr(man,a1->value,a2->value);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_sat_lincons(ap_manager_t* man, ap_abstract0_t* a, ap_lincons0_t lincons)
{
  if (ap_check0_man1(AP_FUNID_SAT_LINCONS,man,a) &&
      ap_check0_lincons(AP_FUNID_SAT_LINCONS,man,_ap_abstract0_dimension(a),lincons) ){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SAT_LINCONS];
    return ptr(man,a->value,lincons);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_sat_tcons(ap_manager_t* man, ap_abstract0_t* a, ap_tcons0_t* tcons)
{
  if (ap_check0_man1(AP_FUNID_SAT_TCONS,man,a) &&
      ap_check0_tcons(AP_FUNID_SAT_TCONS,man,_ap_abstract0_dimension(a),tcons) ){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SAT_TCONS];
    return ptr(man,a->value,tcons);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_sat_interval(ap_manager_t* man, ap_abstract0_t* a,
			       ap_dim_t dim, ap_coeff_t interval)
{
  if (ap_check0_man1(AP_FUNID_SAT_INTERVAL,man,a) &&
      ap_check0_dim(AP_FUNID_SAT_INTERVAL,man,_ap_abstract0_dimension(a),dim)){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_SAT_INTERVAL];
    return ptr(man,a->value,dim,interval);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}
bool ap_abstract0_is_dimension_unconstrained(ap_manager_t* man, ap_abstract0_t* a,
					     ap_dim_t dim)
{
  if (ap_check0_man1(AP_FUNID_IS_DIMENSION_UNCONSTRAINED,man,a) &&
      ap_check0_dim(AP_FUNID_IS_DIMENSION_UNCONSTRAINED,man,_ap_abstract0_dimension(a),dim)){
    bool (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED];
    return ptr(man,a->value,dim);
  }
  else {
    man->result.flag_exact = false;
    return false;
  }
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */
void ap_abstract0_bound_linexpr(ap_manager_t* man,
				ap_coeff_t interval,
				ap_abstract0_t* a, ap_linexpr0_t expr)
{
  if (ap_check0_man1(AP_FUNID_BOUND_LINEXPR,man,a) &&
      ap_check0_linexpr(AP_FUNID_BOUND_LINEXPR,man,_ap_abstract0_dimension(a),expr)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOUND_LINEXPR];
    return ptr(man,interval,a->value,expr);
  }
  else {
    ap_coeff_set_top(interval);
  }
}
void ap_abstract0_bound_texpr(ap_manager_t* man,
			      ap_coeff_t interval,
			      ap_abstract0_t* a, ap_texpr0_t* expr)
{
  if (ap_check0_man1(AP_FUNID_BOUND_TEXPR,man,a) &&
      ap_check0_texpr(AP_FUNID_BOUND_TEXPR,man,_ap_abstract0_dimension(a),expr)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOUND_TEXPR];
    return ptr(man,a->value,expr);
  }
  else {
    ap_coeff_set_top(interval);
  }
}
void ap_abstract0_bound_dimension(ap_manager_t* man,
				  ap_coeff_t interval,
				  ap_abstract0_t* a, ap_dim_t dim)
{
  if (ap_check0_man1(AP_FUNID_BOUND_DIMENSION,man,a) &&
      ap_check0_dim(AP_FUNID_BOUND_DIMENSION,man,_ap_abstract0_dimension(a),dim)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_BOUND_DIMENSION];
    return ptr(man,a->value,dim);
  }
  else {
    ap_coeff_set_top(interval);
  }
}
void ap_abstract0_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t array, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_TO_LINCONS_ARRAY,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];
    ptr(man,array,a->value);
  }
  else {
    ap_lincons0_array_resize(array,0);
  }
}
ap_tcons0_array_t ap_abstract0_to_tcons_array(ap_manager_t* man, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_TO_TCONS_ARRAY,man,a)){
    ap_tcons0_array_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_TCONS_ARRAY];
    return ptr(man,a->value);
  }
  else {
    ap_tcons0_array_t res = { NULL, 0 };
    return res;
  }
}
void ap_abstract0_to_box(ap_manager_t* man, ap_linexpr0_t box, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_TO_BOX,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_BOX];
    return ptr(man,box,a->value);
  }
  else {
    size_t i;
    ap_coeff_t ref;
    ap_dimension_t d = _ap_abstract0_dimension(a);
    size_t size = ap_dimension_size(d);
    ap_linexpr0_resize(box,size);
    for (i=0; i<size; i++){
      ap_linexpr0_coeffref(ref,box,i);
      ap_coeff_set_top(ref);
    }
  }
}
void ap_abstract0_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t array, ap_abstract0_t* a)
{
  if (ap_check0_man1(AP_FUNID_TO_LINGEN_ARRAY,man,a)){
    void (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_LINGEN_ARRAY];
    ptr(man,array,a->value);
  }
  else {
    ap_lingen0_array_resize(array,0);
  }
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */
/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_meetjoin(ap_funid_t funid,
				      ap_manager_t* man, bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  if (ap_check0_man2(funid,man,a1,a2) &&
      ap_check0_abstract2(funid,man,a1,a2)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void* value = ptr(man,destructive,a1->value,a2->value);
    return ap_abstract0_cons2(man,destructive,a1,value);
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a1);
    if (destructive) _ap_abstract0_free(a1);
    return ap_abstract0_top(man,dimension);
  }
}
ap_abstract0_t* ap_abstract0_meet(ap_manager_t* man, bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2){
  return  ap_abstract0_meetjoin(AP_FUNID_MEET,man,destructive,a1,a2);
}
ap_abstract0_t* ap_abstract0_join(ap_manager_t* man, bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2){
  return  ap_abstract0_meetjoin(AP_FUNID_JOIN,man,destructive,a1,a2);
}

ap_abstract0_t* ap_abstract0_meetjoin_array(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t** tab, size_t size)
{
  if (ap_check0_man_array(funid,man,tab,size) &&
      ap_check0_abstract_array(funid,man,tab,size)){
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
    return ap_abstract0_top(man,dimension);
  }
}
ap_abstract0_t* ap_abstract0_meet_array(ap_manager_t* man, ap_abstract0_t** tab, size_t size){
  return ap_abstract0_meetjoin_array(AP_FUNID_MEET_ARRAY,man,tab,size);
}
ap_abstract0_t* ap_abstract0_join_array(ap_manager_t* man, ap_abstract0_t** tab, size_t size){
  return ap_abstract0_meetjoin_array(AP_FUNID_JOIN_ARRAY,man,tab,size);
}
ap_abstract0_t* ap_abstract0_meet_lincons_array(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						ap_lincons0_array_t array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_MEET_LINCONS_ARRAY,man,a) &&
      ap_check0_lincons_array(AP_FUNID_MEET_LINCONS_ARRAY,man,dimension,array) ){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
    void* value = ptr(man,destructive,a->value,array);
    return ap_abstract0_cons2(man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(man,dimension);
  }
}
ap_abstract0_t* ap_abstract0_meet_tcons_array(ap_manager_t* man,
					      bool destructive,
					      ap_abstract0_t* a,
					      ap_tcons0_array_t* array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_MEET_TCONS_ARRAY,man,a) &&
      ap_check0_tcons_array(AP_FUNID_MEET_TCONS_ARRAY,man,dimension,array) ){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_MEET_TCONS_ARRAY];
    void* value = ptr(man,destructive,a->value,array);
    return ap_abstract0_cons2(man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(man,dimension);
  }
}
ap_abstract0_t* ap_abstract0_add_ray_array(ap_manager_t* man,
					   bool destructive,
					   ap_abstract0_t* a,
					   ap_lingen0_array_t array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_ADD_RAY_ARRAY,man,a) &&
      ap_check0_lingen_array(AP_FUNID_ADD_RAY_ARRAY,man,dimension,array)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_ADD_RAY_ARRAY];
    void* value = ptr(man,destructive,a->value,array);
    return ap_abstract0_cons2(man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(man,dimension);
  }
}

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_asssub_linexpr_array(ap_funid_t funid,
						  ap_manager_t* man,
						  bool destructive,
						  ap_abstract0_t* a,
						  ap_dim_t* tdim,
						  ap_linexpr0_array_t array,
						  ap_abstract0_t* dest)
{
  size_t size = ap_linexpr0_array_size(array);
  if (size==0){
    if (dest){
      return ap_abstract0_meet(man,destructive,a,dest);
    }
    else if (destructive){
      return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(funid,man,a) &&
	(dest!=NULL ? (ap_check0_man1(funid,man,dest) && ap_check0_abstract2(funid,man,a,dest)) : true) &&
	ap_check0_dim_array(funid,man,dimension,tdim,size) &&
	ap_check0_linexpr_array(funid,man,dimension,array) ){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
      void* value = ptr(man,destructive,a->value,tdim,array,dest ? dest->value : NULL);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,dimension);
    }
  }
}
ap_abstract0_t* ap_abstract0_assign_linexpr_array(ap_manager_t* man,
						  bool destructive,
						  ap_abstract0_t* a,
						  ap_dim_t* tdim,
						  ap_linexpr0_array_t array,
						  ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
					   man,destructive,a,tdim,array,dest);
}
ap_abstract0_t* ap_abstract0_substitute_linexpr_array(ap_manager_t* man,
						      bool destructive,
						      ap_abstract0_t* a,
						      ap_dim_t* tdim,
						      ap_linexpr0_array_t array,
						      ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
					   man,destructive,a,tdim,array,dest);
}
ap_abstract0_t* ap_abstract0_asssub_texpr_array(ap_funid_t funid,
						ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						ap_dim_t* tdim,
						ap_texpr0_array_t* array,
						ap_abstract0_t* dest)
{
  size_t size = array->size;
  if (size==0){
    if (dest){
      return ap_abstract0_meet(man,destructive,a,dest);
    }
    else if (destructive){
      return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(funid,man,a) &&
	(dest!=NULL ? (ap_check0_man1(funid,man,dest) && ap_check0_abstract2(funid,man,a,dest)) : true) &&
	ap_check0_dim_array(funid,man,dimension,tdim,size) &&
	ap_check0_texpr_array(funid,man,dimension,array) ){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
      void* value = ptr(man,destructive,a->value,tdim,array,dest ? dest->value : NULL);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,dimension);
    }
  }
}
ap_abstract0_t* ap_abstract0_assign_texpr_array(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						ap_dim_t* tdim,
						ap_texpr0_array_t* array,
						ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,
					 man,destructive,a,tdim,array,dest);
}
ap_abstract0_t* ap_abstract0_substitute_texpr_array(ap_manager_t* man,
						    bool destructive,
						    ap_abstract0_t* a,
						    ap_dim_t* tdim,
						    ap_texpr0_array_t* array,
						    ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
					 man,destructive,a,tdim,array,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_forget_array(ap_manager_t* man,
					  bool destructive,
					  ap_abstract0_t* a,
					  ap_dim_t* tdim, size_t size,
					  bool project)
{
  if (size==0){
    if (destructive){
      return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(AP_FUNID_FORGET_ARRAY,man,a) &&
	ap_check0_dim_array(AP_FUNID_FORGET_ARRAY,man,dimension,tdim,size)){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_FORGET_ARRAY];
      void* value = ptr(man,destructive,a->value,tdim,size,project);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,dimension);
    }
  }
}

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_add_dimensions(ap_manager_t* man,
					    bool destructive,
					    ap_abstract0_t* a,
					    ap_dimchange_t* dimchange,
					    bool project)
{
  if (dimchange->dim.intd+dimchange->dim.reald==0){
    if (destructive){
      return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(AP_FUNID_ADD_DIMENSIONS,man,a) &&
	ap_check0_dimchange_add(AP_FUNID_ADD_DIMENSIONS,man,dimension,dimchange)){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_ADD_DIMENSIONS];
      void* value = ptr(man,destructive,a->value,dimchange,project);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,dimension);
    }
  }
}
ap_abstract0_t* ap_abstract0_remove_dimensions(ap_manager_t* man,
					       bool destructive,
					       ap_abstract0_t* a,
					       ap_dimchange_t* dimchange)
{
  if (dimchange->dim.intd+dimchange->dim.reald==0){
    if (destructive){
      return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(AP_FUNID_REMOVE_DIMENSIONS,man,a) &&
	ap_check0_dimchange_remove(AP_FUNID_REMOVE_DIMENSIONS,man,dimension,dimchange)){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_REMOVE_DIMENSIONS];
      void* value = ptr(man,destructive,a->value,dimchange);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      ap_dimension_t dim;
      dim.intd = dimension.intd>=dimchange->dim.intd ? dimension.intd-dimchange->dim.intd : 0;
      dim.reald = dimension.reald>=dimchange->dim.reald ? dimension.reald-dimchange->dim.reald : 0;
      return ap_abstract0_top(man,dim);
    }
  }
}
ap_abstract0_t* ap_abstract0_permute_dimensions(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						ap_dimperm_t* perm)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_PERMUTE_DIMENSIONS,man,a) &&
      ap_check0_dimperm(AP_FUNID_PERMUTE_DIMENSIONS,man,dimension,perm)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_PERMUTE_DIMENSIONS];
    void* value = ptr(man,destructive,a->value,perm);
    return ap_abstract0_cons2(man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(man,dimension);
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
  if (n==0){
    if (destructive){
	return a;
    }
    else {
      return ap_abstract0_copy(man,a);
    }
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a);
    if (ap_check0_man1(AP_FUNID_EXPAND,man,a) &&
	ap_check0_dim(AP_FUNID_EXPAND,man,dimension,dim)){
      void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_EXPAND];
      void* value = ptr(man,destructive,a->value,dim,n);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      ap_dimension_t dimres;
      dimres.intd = dimension.intd + (dim<dimension.intd ? n : 0);
      dimres.reald = dimension.reald + (dim<dimension.intd ? 0 : n);
      return ap_abstract0_top(man,dimres);
    }
  }
}
ap_abstract0_t* ap_abstract0_fold(ap_manager_t* man,
				  bool destructive,
				  ap_abstract0_t* a,
				  ap_dim_t* tdim,
				  size_t size)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_FOLD,man,a) &&
      ap_check0_dim_array(AP_FUNID_FOLD,man,dimension,tdim,size)){
    if (size==0){
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   AP_FUNID_FOLD,
				   "The array of dimension is empty");
	goto _ap_abstract0_fold_exc;
    }
    /* Check also that the array is sorted and contains only integer or real
       dimensions */
    size_t i;
    for (i=1;i<size; i++){
      if (tdim[i-1]>=tdim[i]){
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   AP_FUNID_FOLD,
				   "The array of dimension is not sorted");
	goto _ap_abstract0_fold_exc;
      }
    }
    if (tdim[0]<dimension.intd && tdim[size-1]>=dimension.intd){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_FOLD,
				 "Mixed integer and real dimensions in the array");
      goto _ap_abstract0_fold_exc;
    }
    /* OK now */
    if (size==1){
      if (destructive){
	return a;
      }
      else {
	void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_COPY];
	return ap_abstract0_cons(man,ptr(man,a->value));
      }
    }
    else {
      void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_FOLD];
      void* value = ptr(man,destructive,a->value,tdim,size);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
  }
  else {
  _ap_abstract0_fold_exc:
    if (destructive) _ap_abstract0_free(a);
    ap_dimension_t dim;
    dim.intd = dimension.intd - ( (size>0 && tdim[0]<dimension.intd) ? (size-1) : 0);
    dim.reald = dimension.reald - ( (size>0 && tdim[0]<dimension.intd) ? 0 : (size-1));
    return ap_abstract0_top(man,dim);
  }
}
/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_widening(ap_manager_t* man,
				      ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  if (ap_check0_man2(AP_FUNID_WIDENING,man,a1,a2) &&
      ap_check0_abstract2(AP_FUNID_WIDENING,man,a1,a2)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_WIDENING];
    void* value = ptr(man,a1->value,a2->value);
    return ap_abstract0_cons(man,value);
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a1);
    return ap_abstract0_top(man,dimension);
  }
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */
ap_abstract0_t* ap_abstract0_closure(ap_manager_t* man, bool destructive, ap_abstract0_t* a)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_check0_man1(AP_FUNID_CLOSURE,man,a)){
    void* (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_CLOSURE];
    void* value = ptr(man,destructive,a->value);
    return ap_abstract0_cons2(man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(man,dimension);
  }
}

/* ********************************************************************** */
/* IV. Functions offered by the APRON interface */
/* ********************************************************************** */

/* These functions do not correspond to functions in the underlying library. */


/*
   These two functions implement of_lincons/tcons_array constructors
   using top and meet_lincons/tcons_array operations.
*/
ap_abstract0_t* ap_abstract0_of_lincons_array(ap_manager_t* man,
					      ap_dimension_t dimension,
					      ap_lincons0_array_t array)
{
  ap_abstract0_t* res = ap_abstract0_top(man,dimension);
  res = ap_abstract0_meet_lincons_array(man,true,res,array);
  return res;
}
ap_abstract0_t* ap_abstract0_of_tcons_array(ap_manager_t* man,
					    ap_dimension_t dimension,
					    ap_tcons0_array_t* array)
{
  ap_abstract0_t* res = ap_abstract0_top(man,dimension);
  res = ap_abstract0_meet_tcons_array(man,true,res,array);
  return res;
}

/*
  These four functions implement assignment and substitution of a single
  dimension by a single expression.
*/

ap_abstract0_t* ap_abstract0_asssub_linexpr(ap_funid_t funid,
					    ap_manager_t* man,
					    bool destructive,
					    ap_abstract0_t* a,
					    ap_dim_t dim, ap_linexpr0_t expr,
					    ap_abstract0_t* dest)
{
  ap_abstract0_t* res;
  ap_linexpr0_array_t array;
  ap_linexprD_t exprD;
  ap_linexprMPQ_t exprMPQ;
  ap_linexprMPFR_t exprMPFR;
  ap_linexprD_array_t arrayD;
  ap_linexprMPQ_array_t arrayMPQ;
  ap_linexprMPFR_array_t arrayMPFR;

  array->discr = expr->discr;
  switch (array->discr){
  case AP_SCALAR_D:
    array->linexpr_array.D = arrayD;
    arrayD->size = 1;
    arrayD->p = &exprD;
    *exprD = *(expr->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    array->linexpr_array.MPQ = arrayMPQ;
    arrayMPQ->size = 1;
    arrayMPQ->p = &exprMPQ;
    *exprMPQ = *(expr->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    array->linexpr_array.MPFR = arrayMPFR;
    arrayMPFR->size = 1;
    arrayMPFR->p = &exprMPFR;
    *exprMPFR = *(expr->linexpr.MPFR);
    break;
  default:
    abort();
  }
  return ap_abstract0_asssub_linexpr_array(funid,
					   man,destructive,a,&dim,array,dest);
}
ap_abstract0_t* ap_abstract0_assign_linexpr(ap_manager_t* man,
					    bool destructive,
					    ap_abstract0_t* a,
					    ap_dim_t dim, ap_linexpr0_t expr,
					    ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
				     man,destructive,a,dim,expr,dest);
}
ap_abstract0_t* ap_abstract0_substitute_linexpr(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* a,
						ap_dim_t dim, ap_linexpr0_t expr,
						ap_abstract0_t* dest)
{
  return ap_abstract0_asssub_linexpr(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
				     man,destructive,a,dim,expr,dest);
}
ap_abstract0_t* ap_abstract0_assign_texpr(ap_manager_t* man,
					  bool destructive,
					  ap_abstract0_t* a,
					  ap_dim_t dim, ap_texpr0_t* expr,
					  ap_abstract0_t* dest)
{
  ap_texpr0_array_t array;
  array.size = 1;
  array.p = &expr;
  return ap_abstract0_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,
					 man,destructive,a,&dim,&array,dest);
}
ap_abstract0_t* ap_abstract0_substitute_texpr(ap_manager_t* man,
					      bool destructive,
					      ap_abstract0_t* a,
					      ap_dim_t dim, ap_texpr0_t* expr,
					      ap_abstract0_t* dest)
{
  ap_texpr0_array_t array;
  array.size = 1;
  array.p = &expr;
  return ap_abstract0_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
					 man,destructive,a,&dim,&array,dest);
}

/* Applying an ap_dimchange2_t transformation (dimension adding followed by
   dimension removal/projection).  If project is true, the newly added
   dimension are projected on their 0-hyperplane. */
ap_abstract0_t* ap_abstract0_apply_dimchange2(ap_manager_t* man,
					      bool destructive,
					      ap_abstract0_t* a, ap_dimchange2_t* dimchange2,
					      bool project)
{
  ap_abstract0_t* res;
  if (dimchange2->add){
    res = ap_abstract0_add_dimensions(man,destructive,a,dimchange2->add,project);
    if (dimchange2->remove){
      res = ap_abstract0_remove_dimensions(man,true,res,dimchange2->remove);
    }
  }
  else if (dimchange2->remove){
    res = ap_abstract0_remove_dimensions(man,destructive,a,dimchange2->remove);
  }
  else {
    res = destructive ? a : ap_abstract0_copy(man,a);
  }
  return res;
}

/* This function implements widening with threshold, relying on the
   widening, sat_lincons and meet_lincons_array operations.
*/
ap_abstract0_t* ap_abstract0_widening_threshold(ap_manager_t* man,
						ap_abstract0_t* a1,
						ap_abstract0_t* a2,
						ap_lincons0_array_t array)
{
  void* (*ptr)(ap_manager_t*,...);
  bool (*ptr2)(ap_manager_t*,...);
  void* value;
  size_t i,j,size;
  size_t* psize;
  ap_lincons0_t consi, consj, tmp;

  ap_dimension_t dimension = _ap_abstract0_dimension(a1);
  if (ap_check0_man2(AP_FUNID_WIDENING,man,a1,a2) &&
      ap_check0_abstract2(AP_FUNID_WIDENING,man,a1,a2) &&
      ap_check0_lincons_array(AP_FUNID_WIDENING,man,dimension,array) ){
    ptr = man->funptr[AP_FUNID_WIDENING];
    value = ptr(man,a1->value,a2->value);

    ptr2 = man->funptr[AP_FUNID_SAT_LINCONS];
    psize = NULL;
    switch(array->discr){
    case AP_SCALAR_D:
      psize = &array->lincons_array.D->size;
      break;
    case AP_SCALAR_MPQ:
      psize = &array->lincons_array.MPQ->size;
      break;
    case AP_SCALAR_MPFR:
      psize = &array->lincons_array.MPFR->size;
      break;
    default:
      abort();
    }
    size = *psize;
    i = j = 0;
    while (i<size-j){
      ap_lincons0_array_ref_index(consi,array,i);
      if (ptr2(man,a2->value,consi)){
	i++;
      }
      else {
	j++;
	ap_lincons0_array_ref_index(consj,array,size-j);
	*tmp = *consi; *consi = *consj; *consj = *tmp;
      }
    }
    if (i>0){
      *psize = i;
      ptr = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
      value = ptr(man,true,value,array);
      *psize = size;
    }
    return ap_abstract0_cons(man,value);
  }
  else {
    return ap_abstract0_top(man,dimension);
  }
}
