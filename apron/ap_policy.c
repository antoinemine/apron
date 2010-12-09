/* ************************************************************************* */
/* ap_policy.c */
/* ************************************************************************* */

#include "ap_policy.h"

/* ********************************************************************** */
/* I. Policy managers */
/* ********************************************************************** */

ap_policy_manager_t* ap_policy_manager_alloc(ap_manager_t* man,
					     void* internal,
					     void (*internal_free)(void*))
{
  size_t i;
  ap_policy_manager_t* pman;

  pman = (ap_policy_manager_t*)malloc(sizeof(ap_policy_manager_t));
  pman->man = ap_manager_copy(man);
  pman->internal = internal;
  pman->internal_free = internal_free;
  pman->count = 1;
  for (i=0; i<AP_FUNPOLICYID_SIZE; i++) pman->funptr[i]=NULL;
  return pman;
}
void ap_policy_manager_free(ap_policy_manager_t* pman)
{
  if (pman->count>1){
    pman->count--;
  }
  else {
    ap_manager_free(pman->man);
    if (pman->internal != NULL){
      pman->internal_free(pman->internal);
      pman->internal = NULL;
    }
    free(pman);
  }
}

/* ********************************************************************** */
/* II. Policy, general */
/* ********************************************************************** */

void ap_policy_free(ap_policy_manager_t* man, ap_policy_t* policy)
{
  typedef void (*funptr)(ap_policy_manager_t* man, ap_policy_t* policy);
  funptr ptr = (funptr)man->funptr[AP_FUNPOLICYID_FREE];
  (*ptr)(man,policy);
}
ap_policy_t* ap_policy_copy(ap_policy_manager_t* man, ap_policy_t* policy)
{
  typedef ap_policy_t* (*funptr)(ap_policy_manager_t* man, ap_policy_t* policy);
  funptr ptr = (funptr)man->funptr[AP_FUNPOLICYID_COPY];
  return (*ptr)(man,policy);
}
void ap_policy_fprint(FILE* stdout, ap_policy_manager_t* man, ap_policy_t* policy)
{
  typedef void (*funptr)(FILE* stdout, ap_policy_manager_t* man, ap_policy_t* policy);
  funptr ptr = (funptr)man->funptr[AP_FUNPOLICYID_FPRINT];
  return (*ptr)(stdout,man,policy);
}
bool ap_policy_equal(ap_policy_manager_t* man, ap_policy_t* policy1, ap_policy_t* policy2)
{
  typedef bool (*funptr)(ap_policy_manager_t* man, ap_policy_t* policy1, ap_policy_t* policy2);
  funptr ptr = (funptr)man->funptr[AP_FUNPOLICYID_EQUAL];
  return (*ptr)(man,policy1,policy2);
}

/* ********************************************************************** */
/* III. Policy, level 0 */
/* ********************************************************************** */

ap_policy_t* ap_abstract0_policy_alloc(ap_policy_manager_t* man, ap_funid_t funid, size_t nbdims)
{
  typedef ap_policy_t* (*funpolicyptr)(ap_policy_manager_t* man, ap_funid_t funid, size_t nbdims);
  funpolicyptr ptr = (funpolicyptr)man->funptr[AP_FUNPOLICYID_ALLOC];
  return (*ptr)(man,funid,nbdims);
}

ap_abstract0_t*
ap_abstract0_policy_meetjoin(ap_funpolicyid_t funid,
			     /* either meet or join */
			     ap_policy_manager_t* pman, 
			     ap_policy_t* policy, ap_policy_mode_t mode, 
			     bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  if (ap_abstract0_checkman2(funid,pman->man,a1,a2) &&
      ap_abstract0_check_abstract2(funid,pman->man,a1,a2)){
    void* (*ptr)(ap_policy_manager_t*,...) = pman->funptr[funid];
    void* value = ptr(pman,policy,mode,destructive,a1->value,a2->value);
    return ap_abstract0_cons2(pman->man,destructive,a1,value);
  }
  else {
    ap_dimension_t dimension = _ap_abstract0_dimension(a1);
    if (destructive) _ap_abstract0_free(a1);
    return ap_abstract0_top(pman->man,
			    dimension.intdim,
			    dimension.realdim);
  }
}
ap_abstract0_t* 
ap_abstract0_policy_meetjoin_array(ap_funpolicyid_t funid, 
				   ap_policy_manager_t* pman, 
				   ap_policy_t* policy, ap_policy_mode_t mode,
				   ap_abstract0_t** tab, size_t size)
{
  if (ap_abstract0_checkman_array(funid,pman->man,tab,size) &&
      ap_abstract0_check_abstract_array(funid,pman->man,tab,size)){
    size_t i;
    ap_abstract0_t* res;
    void* (*ptr)(ap_policy_manager_t*,...) = pman->funptr[funid];
    void** ntab = malloc(size*sizeof(void*));
    for (i=0;i<size;i++) ntab[i] = tab[i]->value;
    res = ap_abstract0_cons(pman->man,ptr(pman,policy,mode,ntab,size));
    free(ntab);
    return res;
  }
  else {
    ap_dimension_t dimension = { 0, 0};
    if (size>0){
      dimension = _ap_abstract0_dimension(tab[0]);
    }
    return ap_abstract0_top(pman->man,
			    dimension.intdim,
			    dimension.realdim);
  }
}
ap_abstract0_t*
ap_abstract0_policy_meet(ap_policy_manager_t* pman, 
			 ap_policy_t* policy, ap_policy_mode_t mode, 
			 bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  return ap_abstract0_policy_meetjoin(AP_FUNPOLICYID_MEET,
				      pman,policy,mode,
				      destructive,a1,a2);
}
ap_abstract0_t*
ap_abstract0_policy_meet_array(ap_policy_manager_t* pman, 
			       ap_policy_t* policy, ap_policy_mode_t mode, 
			       ap_abstract0_t** tab, size_t size)
{
  return ap_abstract0_policy_meetjoin_array(AP_FUNPOLICYID_MEET_ARRAY,pman,policy,mode,tab,size);
}
ap_abstract0_t*
ap_abstract0_policy_meet_lincons_array(ap_policy_manager_t* pman, 
				       ap_policy_t* policy, ap_policy_mode_t mode, 
				       bool destructive, ap_abstract0_t* a, ap_lincons0_array_t* array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_MEET_LINCONS_ARRAY,pman->man,a) &&
      ap_abstract0_check_lincons_array(AP_FUNID_MEET_LINCONS_ARRAY,pman->man,dimension,array) ){
    void* (*ptr)(ap_policy_manager_t*,...) = pman->funptr[AP_FUNPOLICYID_MEET_LINCONS_ARRAY];
    void* value = ptr(pman,policy,mode,destructive,a->value,array);
    return ap_abstract0_cons2(pman->man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(pman->man,
			    dimension.intdim,
			    dimension.realdim);
  }
}
ap_abstract0_t*
ap_abstract0_policy_meet_tcons_array(ap_policy_manager_t* pman, 
				     ap_policy_t* policy, ap_policy_mode_t mode,
				     bool destructive, ap_abstract0_t* a, ap_tcons0_array_t* array)
{
  ap_dimension_t dimension = _ap_abstract0_dimension(a);
  if (ap_abstract0_checkman1(AP_FUNID_MEET_TCONS_ARRAY,pman->man,a) &&
      ap_abstract0_check_tcons_array(AP_FUNID_MEET_TCONS_ARRAY,pman->man,dimension,array) ){
    void* (*ptr)(ap_policy_manager_t*,...) = pman->funptr[AP_FUNPOLICYID_MEET_TCONS_ARRAY];
    void* value = ptr(pman,policy,mode,destructive,a->value,array);
    return ap_abstract0_cons2(pman->man,destructive,a,value);
  }
  else {
    if (destructive) _ap_abstract0_free(a);
    return ap_abstract0_top(pman->man,
			    dimension.intdim,
			    dimension.realdim);
  }
}


/*
ap_abstract0_t*
ap_abstract0_policy_asssub_linexpr_array(ap_funpolicyid_t funid,
					 ap_manager_t* man,
					 ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
					 bool destructive,
					 ap_abstract0_t* a,
					 ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
					 ap_abstract0_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract0_policy_meet(man,policy_man,policy,mode,destructive,a,dest);
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
    if (ap_abstract0_checkman1(funid,man,a) &&
	(dest!=NULL ? (ap_abstract0_checkman1(funid,man,dest) && ap_abstract0_check_abstract2(funid,man,a,dest)) : true) &&
	ap_abstract0_check_dim_array(funid,man,dimension,tdim,size) &&
	ap_abstract0_check_linexpr_array(funid,man,dimension,texpr,size) ){
      void* (*ptr)(ap_manager_t*,...) = pman->funptr[funid];
      void* value = ptr(man,policy_man,policy,destructive,a->value,tdim,texpr,size,dest ? dest->value : NULL);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,
			      dimension.intdim,
			      dimension.realdim);
    }
  }
}
ap_abstract0_t*
ap_abstract0_policy_asssub_texpr_array(ap_funpolicyid_t funid,
				       ap_manager_t* man,
				       ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				       bool destructive,
				       ap_abstract0_t* a,
				       ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				       ap_abstract0_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract0_policy_meet(man,policy_man,policy,mode,destructive,a,dest);
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
    if (ap_abstract0_checkman1(funid,man,a) &&
	(dest!=NULL ? (ap_abstract0_checkman1(funid,man,dest) && ap_abstract0_check_abstract2(funid,man,a,dest)) : true) &&
	ap_abstract0_check_dim_array(funid,man,dimension,tdim,size) &&
	ap_abstract0_check_texpr_array(funid,man,dimension,texpr,size) ){
      void* (*ptr)(ap_manager_t*,...) = pman->funptr[funid];
      void* value = ptr(man,policy_man,policy,destructive,a->value,tdim,texpr,size,dest ? dest->value : NULL);
      return ap_abstract0_cons2(man,destructive,a,value);
    }
    else {
      if (destructive) _ap_abstract0_free(a);
      return ap_abstract0_top(man,
			      dimension.intdim,
			      dimension.realdim);
    }
  }
}
ap_abstract0_t*
ap_abstract0_policy_asssub_linexpr(ap_funpolicyid_t funid,
				   ap_manager_t* man,
				   ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				   bool destructive,
				   ap_abstract0_t* org,
				   ap_dim_t dim, ap_linexpr0_t* expr,
				   ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_linexpr_array(funid,man,policy_man,policy,mode,destructive,org,&dim,&expr,1,dest);
}
ap_abstract0_t*
ap_abstract0_policy_asssub_texpr(ap_funpolicyid_t funid,
				 ap_manager_t* man,
				 ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				 bool destructive,
				 ap_abstract0_t* org,
				 ap_dim_t dim, ap_texpr0_t* expr,
				 ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_texpr_array(funid,man,policy_man,policy,mode,destructive,org,&dim,&expr,1,dest);
}
ap_abstract0_t*
ap_abstract0_policy_assign_linexpr_array(ap_manager_t* man,
					 ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
					 bool destructive,
					 ap_abstract0_t* org,
					 ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
					 ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_linexpr_array(AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY,
						  man,policy_man,policy,mode,
						  destructive,org,tdim,texpr,size,dest);
}
ap_abstract0_t*
ap_abstract0_policy_assign_texpr_array(ap_manager_t* man,
				       ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				       bool destructive,
				       ap_abstract0_t* org,
				       ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				       ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_texpr_array(AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY,
						man,policy_man,policy,mode,
						destructive,org,tdim,texpr,size,dest);
}
ap_abstract0_t*
ap_abstract0_policy_assign_linexpr(ap_manager_t* man,
				   ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				   bool destructive,
				   ap_abstract0_t* org,
				   ap_dim_t dim, ap_linexpr0_t* expr,
				   ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_linexpr(AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY,man,policy_man,policy,mode,destructive,org,dim,expr,dest);
}
ap_abstract0_t*
ap_abstract0_policy_assign_texpr(ap_manager_t* man,
				 ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode, 
				 bool destructive,
				 ap_abstract0_t* org,
				 ap_dim_t dim, ap_texpr0_t* expr,
				 ap_abstract0_t* dest)
{
  return ap_abstract0_policy_asssub_texpr(AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY,man,policy_man,policy,mode,destructive,org,dim,expr,dest);
}
*/

/* ********************************************************************** */
/* IV. Policy, level 1 */
/* ********************************************************************** */

ap_policy_t* ap_abstract1_policy_alloc(ap_policy_manager_t* man, 
				       ap_funid_t funid, ap_environment_t* env)
{ return ap_abstract0_policy_alloc(man,funid,env->intdim+env->realdim); }

ap_abstract1_t
ap_abstract1_policy_meetjoin(ap_funpolicyid_t funid,
			     ap_policy_manager_t* pman, 
			     ap_policy_t* policy, ap_policy_mode_t mode,
			     bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  ap_abstract1_t res;
  if (ap_abstract1_check_env2(funid,pman->man,a1,a2)){
    ap_abstract0_t* value = ap_abstract0_policy_meetjoin(funid,pman,policy,mode,destructive,a1->abstract0,a2->abstract0);
    res = ap_abstract1_consres(destructive, a1, value);
  }
  else {
    res = ap_abstract1_top(pman->man,a1->env);
    if (destructive) ap_abstract1_clear(pman->man,a1);
  }
  return res;
}
ap_abstract1_t
ap_abstract1_policy_meetjoin_array(ap_funpolicyid_t funid,
				   ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				   ap_abstract1_t* tab, size_t size)
{
  ap_abstract1_t res;
  if (ap_abstract1_checkman_array(funid,pman->man,tab,size) &&
      ap_abstract1_check_env_array(funid,pman->man,tab,size)){
    size_t i;
    ap_abstract0_t* res0;
    void* (*ptr)(ap_policy_manager_t*,...) = pman->funptr[funid];
    void** ntab = malloc(size*sizeof(void*));
    for (i=0;i<size;i++) ntab[i] = tab[i].abstract0->value;
    res0 = malloc(sizeof(ap_abstract0_t));
    res0->man = ap_manager_copy(pman->man);
    res0->value = ptr(pman,policy,mode,ntab,size);
    res.abstract0 = res0;
    res.env = ap_environment_copy(tab[0].env);
    free(ntab);
  }
  else {
    res = ap_abstract1_top(pman->man, size==0 ? ap_environment_alloc_empty() : tab[0].env);
  }
  return res;
}

ap_abstract1_t
ap_abstract1_policy_meet(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
			 bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  return ap_abstract1_policy_meetjoin(AP_FUNPOLICYID_MEET,pman,policy,mode,destructive,a1,a2);
}
ap_abstract1_t
ap_abstract1_policy_meet_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
			       ap_abstract1_t* tab, size_t size)
{
  return ap_abstract1_policy_meetjoin_array(AP_FUNPOLICYID_MEET_ARRAY,pman,policy,mode,tab,size);
}
ap_abstract1_t
ap_abstract1_policy_meet_lincons_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				       bool destructive,
				       ap_abstract1_t* a,
				       ap_lincons1_array_t* array)
{
  ap_abstract1_t res;
  ap_lincons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    array0 = array->lincons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(pman->man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_LINCONS_ARRAY,
				 "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? *a : ap_abstract1_copy(pman->man,a);
      return res;
    }
    array0 = ap_lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  ap_abstract0_t* value = ap_abstract0_policy_meet_lincons_array(pman,policy,mode,destructive,a->abstract0,&array0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lincons0_array_clear(&array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

ap_abstract1_t
ap_abstract1_policy_meet_tcons_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				     bool destructive, ap_abstract1_t* a, ap_tcons1_array_t* array)
{
  ap_abstract1_t res;
  ap_tcons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    array0 = array->tcons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(pman->man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_TCONS_ARRAY,
				 "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? *a : ap_abstract1_copy(pman->man,a);
      return res;
    }
    array0 = ap_tcons0_array_add_dimensions(&array->tcons0_array,dimchange);
  }
  ap_abstract0_t* value = ap_abstract0_policy_meet_tcons_array(pman,policy,mode,destructive,a->abstract0,&array0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_tcons0_array_clear(&array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}


/*
ap_abstract1_t ap_abstract1_policy_asssub_linexpr_array(ap_funpolicyid_t funid,
							ap_manager_t* man,
							ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
							bool destructive, ap_abstract1_t* a,
							ap_var_t* tvar, ap_linexpr1_t* linexpr, size_t size,
							ap_abstract1_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract1_policy_meet(pman,policy,mode,destructive,a,dest);
    }
    else if (destructive){
      return *a;
    }
    else {
      return ap_abstract1_copy(pman->man,a);
    }
  }
  else {
    ap_dim_t* tdim;
    ap_linexpr0_t** tlinexpr0;
    ap_abstract1_t res;
    ap_dimchange_t* dimchange;
    size_t i;

    i = 0;
    tdim = NULL;
    tlinexpr0 = NULL;
    if ( dest==NULL || ap_abstract1_check_env2(funid,pman->man,a,dest)){
      tdim = malloc(size*sizeof(ap_dim_t));
      tlinexpr0 = malloc(size*sizeof(ap_linexpr0_t*));
      for (i=0; i<size; i++){
	tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
	if (tdim[i]==AP_DIM_MAX){
	  ap_abstract1_raise_invalid_var(pman->man,funid,tvar[i]);
	  goto ap_abstract1_asssub_linexpr_array_exit;
	}
	if (ap_environment_is_eq(a->env,linexpr[i].env)){
	  dimchange = NULL;
	  tlinexpr0[i] = linexpr[i].linexpr0;
	}
	else {
	  dimchange = ap_environment_dimchange(linexpr[i].env,a->env);
	  if (dimchange==NULL){
	    ap_abstract1_raise_invalid_expr(pman->man,funid);
	    goto ap_abstract1_asssub_linexpr_array_exit;
	  }
	  tlinexpr0[i] = ap_linexpr0_add_dimensions(linexpr[i].linexpr0,dimchange);
	  ap_dimchange_free(dimchange);
	}
      }
      ap_abstract0_t* value = ap_abstract0_policy_asssub_linexpr_array(funid,pman,policy,mode,
								       destructive,a->abstract0,
								       tdim, tlinexpr0, size,
								       (dest!=NULL) ? dest->abstract0 : NULL);
      res = ap_abstract1_consres(destructive, a, value);
    }
    else {
    ap_abstract1_asssub_linexpr_array_exit:
      res = ap_abstract1_top(pman->man,a->env);
      if (destructive) ap_abstract1_clear(pman->man,a);
    }
    if (tlinexpr0!=NULL){
      size_t j;
      for (j=0; j<i; j++){
	if (tlinexpr0[j]!=linexpr[j].linexpr0)
	  ap_linexpr0_free(tlinexpr0[j]);
      }
      free(tlinexpr0);
      free(tdim);
    }
    return res;
  }
}
ap_abstract1_t
ap_abstract1_policy_asssub_texpr_array(ap_funid_t funid,
				       ap_manager_t* man,
				       ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				       bool destructive, ap_abstract1_t* a,
				       ap_var_t* tvar, ap_texpr1_t* texpr, size_t size,
				       ap_abstract1_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract1_policy_meet(pman,policy,mode,destructive,a,dest);
    }
    else if (destructive){
      return *a;
    }
    else {
      return ap_abstract1_copy(pman->man,a);
    }
  }
  else {
    ap_dim_t* tdim;
    ap_texpr0_t** ttexpr0;
    ap_abstract1_t res;
    ap_dimchange_t* dimchange;
    size_t i;

    i = 0;
    tdim = NULL;
    ttexpr0 = NULL;
    if ( dest==NULL || ap_abstract1_check_env2(funid,pman->man,a,dest)){
      tdim = malloc(size*sizeof(ap_dim_t));
      ttexpr0 = malloc(size*sizeof(ap_texpr0_t*));
      for (i=0; i<size; i++){
	tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
	if (tdim[i]==AP_DIM_MAX){
	  ap_abstract1_raise_invalid_var(pman->man,funid,tvar[i]);
	  goto ap_abstract1_asssub_texpr_array_exit;
	}
	if (ap_environment_is_eq(a->env,texpr[i].env)){
	  dimchange = NULL;
	  ttexpr0[i] = texpr[i].texpr0;
	}
	else {
	  dimchange = ap_environment_dimchange(texpr[i].env,a->env);
	  if (dimchange==NULL){
	    ap_abstract1_raise_invalid_expr(pman->man,funid);
	    goto ap_abstract1_asssub_texpr_array_exit;
	  }
	  ttexpr0[i] = ap_texpr0_add_dimensions(texpr[i].texpr0,dimchange);
	  ap_dimchange_free(dimchange);
	}
      }
      ap_abstract0_t* value = ap_abstract0_policy_asssub_texpr_array(funid,pman,policy,mode,
							      destructive,a->abstract0,
							      tdim, ttexpr0, size,
							      (dest!=NULL) ? dest->abstract0 : NULL);
      res = ap_abstract1_consres(destructive, a, value);
    }
    else {
    ap_abstract1_asssub_texpr_array_exit:
      res = ap_abstract1_top(pman->man,a->env);
      if (destructive) ap_abstract1_clear(pman->man,a);
    }
    if (ttexpr0!=NULL){
      size_t j;
      for (j=0; j<i; j++){
	if (ttexpr0[j]!=texpr[j].texpr0)
	  ap_texpr0_free(ttexpr0[j]);
      }
      free(ttexpr0);
      free(tdim);
    }
    return res;
  }
}
ap_abstract1_t
ap_abstract1_policy_assign_linexpr_array(ap_manager_t* man,
					 ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
					 bool destructive, ap_abstract1_t* a,
					 ap_var_t* tvar, ap_linexpr1_t* texpr, size_t size,
					 ap_abstract1_t* dest)
{
  return ap_abstract1_policy_asssub_linexpr_array(AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY,pman,policy,mode,destructive,a,tvar,texpr,size,dest);
}
ap_abstract1_t
ap_abstract1_policy_assign_texpr_array(ap_manager_t* man,
				       ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				       bool destructive, ap_abstract1_t* a,
				       ap_var_t* tvar, ap_texpr1_t* texpr, size_t size,
				       ap_abstract1_t* dest)
{
  return ap_abstract1_policy_asssub_texpr_array(AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY,pman,policy,mode,destructive,a,tvar,texpr,size,dest);
}
ap_abstract1_t
ap_abstract1_policy_assign_linexpr(ap_manager_t* man,
				   ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				   bool destructive, ap_abstract1_t* a,
				   ap_var_t var, ap_linexpr1_t* expr,
				   ap_abstract1_t* dest)
{
  return ap_abstract1_policy_asssub_linexpr_array(AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY,pman,policy,mode,destructive,a,&var,expr,1,dest);
}
ap_abstract1_t
ap_abstract1_policy_assign_texpr(ap_manager_t* man, ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				 bool destructive, ap_abstract1_t* a,
				 ap_var_t var, ap_texpr1_t* expr,
				 ap_abstract1_t* dest)
{
  return ap_abstract1_policy_asssub_texpr_array(AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY,pman,policy,mode,destructive,a,&var,expr,1,dest);
}
*/
