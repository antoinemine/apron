/* ********************************************************************** */
/* box_internal.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"
#include "box_internal.h"
#include "box_representation.h"
#include "box_policy.h"

/* ============================================================ */
/* Policy */
/* ============================================================ */

ap_policy_manager_t* box_policy_manager_alloc()
{
  ap_policy_manager_t* man;
  funptr_t* funptr_policy;
  man = (ap_policy_manager_t*)malloc(sizeof(ap_policy_manager_t));
  memset(man, 0, sizeof(ap_policy_manager_t));
#if 0
  size_t i;
  box_internal_t* itv;
  void** funptr;

  if (!ap_fpu_init()) {
    fprintf(stderr,"box_manager_alloc cannot change the FPU rounding mode\n");
  }

  itv = box_internal_alloc();

  ap_policy_manager_t* man;
  assert(sizeof(bool)==1);
  man = (ap_policy_manager_t*)malloc(sizeof(ap_policy_manager_t));
  memset(man, 0, sizeof(ap_policy_manager_t));
  man->ident = 0x5a;
  man->standard_manager.library = "box_policy";
  man->standard_manager.version = "1.0 with " NUM_NAME;
  man->standard_manager.internal = itv;
  man->standard_manager.internal_free = (void (*)(void*))box_internal_free;

  man->standard_manager.count = 1;
  ap_option_init(&man->standard_manager.option);
  ap_result_init(&man->standard_manager.result);

  funptr = man->standard_manager.funptr;

  funptr[AP_FUNID_COPY] = &box_copy;
  funptr[AP_FUNID_FREE] = &box_free;
  funptr[AP_FUNID_ASIZE] = &box_size;
  funptr[AP_FUNID_MINIMIZE] = &box_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &box_canonicalize;
  funptr[AP_FUNID_HASH] = &box_hash;
  funptr[AP_FUNID_APPROXIMATE] = &box_approximate;
  funptr[AP_FUNID_FPRINT] = &box_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &box_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &box_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &box_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &box_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &box_bottom;
  funptr[AP_FUNID_TOP] = &box_top;
  funptr[AP_FUNID_OF_BOX] = &box_of_box;
  funptr[AP_FUNID_DIMENSION] = &box_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &box_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &box_is_top;
  funptr[AP_FUNID_IS_LEQ] = &box_is_leq;
  funptr[AP_FUNID_IS_EQ] = &box_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &box_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &box_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &box_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &box_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &box_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &box_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &box_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &box_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &box_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &box_to_tcons_array;
  funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &box_to_generator_array;
  funptr[AP_FUNID_MEET] = &transfer_policy_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &transfer_policy_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &transfer_policy_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &transfer_policy_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &box_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &box_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &box_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &box_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &box_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &box_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &box_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &box_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &box_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &box_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &box_forget_array;
  funptr[AP_FUNID_EXPAND] = &box_expand;
  funptr[AP_FUNID_FOLD] = &box_fold;
  funptr[AP_FUNID_WIDENING] = &box_widening;
  funptr[AP_FUNID_CLOSURE] = &box_closure;

  for (i=0; i<AP_EXC_SIZE; i++){
    ap_manager_set_abort_if_exception(&man->standard_manager, i, false);
  }
#endif

  man->count = 1;
  funptr_policy = (funptr_t*) man->funpolicy_array;
  funptr_policy[AP_FUNPOLICYID_CREATE] = (void (*)(void*)) &box_policy_create;
  funptr_policy[AP_FUNPOLICYID_FREE_ALL] = (void (*)(void*)) &box_policy_free_all;

  funptr_policy[AP_FUNPOLICYID_COPY] = (void (*)(void*)) &box_policy_copy;
  funptr_policy[AP_FUNPOLICYID_FREE] = (void (*)(void*)) &box_policy_free;
  funptr_policy[AP_FUNPOLICYID_PRINTF] = (void (*)(void*)) &box_policy_printf;
  funptr_policy[AP_FUNPOLICYID_MEET] = (void (*)(void*)) box_meet_policy;
  funptr_policy[AP_FUNPOLICYID_MEET_ARRAY] = (void (*)(void*)) box_meet_array_policy;
  funptr_policy[AP_FUNPOLICYID_MEET_LINCONS_ARRAY] = (void (*)(void*)) box_meet_lincons_array_policy;
  funptr_policy[AP_FUNPOLICYID_MEET_TCONS_ARRAY] = (void (*)(void*)) box_meet_tcons_array_policy;
  funptr_policy[AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY] = (void (*)(void*)) box_assign_linexpr_array_policy;
  funptr_policy[AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY] = (void (*)(void*)) box_assign_texpr_array_policy;
  return man;
}

void box_policy_init(box_policy_t* policy, size_t dimension) {
   assert(policy && dimension > 0 && ((policy->size == 0 || policy->size == dimension)));

   if (policy->size == 0) {
      policy->p = (box_policy_dim_t*) malloc(dimension*sizeof(box_policy_dim_t));
      policy->size = dimension;
   };
   for (size_t i = 0; i < dimension; ++i) {
      policy->p[i].inf = (box_policy_choice_t) (rand() % 2);
      policy->p[i].sup = (box_policy_choice_t) (rand() % 2);
   };
}

void box_policy_create(ap_policy_manager_t* man, ap_current_policy_t* current_policy,
     size_t dimension, size_t nb_policies) {
   assert(nb_policies > 0);
   box_policy_t** result = (box_policy_t**)  malloc(sizeof(box_policy_t*)*nb_policies);
   for (int index = 0; index < (int) nb_policies; ++index) {
      box_policy_t* current_result = (box_policy_t*) malloc(sizeof(box_policy_t)*dimension);
      memset(current_result, 0, sizeof(box_policy_t)*dimension);
      box_policy_init(current_result, dimension);
      result[index] = current_result;
   };
   current_policy->policies = (void*) result;
   current_policy->mode = AP_POLICY_APPLY;
   current_policy->index = 0;
   current_policy->size = nb_policies;
}

void box_policy_free_all(ap_policy_manager_t* man, ap_current_policy_t* current_policy) {
   box_policy_t** result = (box_policy_t**) current_policy->policies;
   if (result) {
      for (int index = 0; index < (int) current_policy->size; ++index)
         box_policy_free(result[index]);
      free(result);
      current_policy->policies = 0;
      current_policy->mode = AP_POLICY_APPLY;
      current_policy->index = 0;
      current_policy->size = 0;
   };
}

/*
void box_policy_clear_one(void* a_policy_one);
void box_policy_clear(void* a_policy) {
   box_policy_t* policy = (box_policy_t*) a_policy;
   if (policy) {
     size_t i;
     for (i = 0; i < policy->size; ++i)
        box_policy_clear_one(&policy->p[i]);
     free(policy->p);
     policy->p = NULL;
     policy->size = 0;
   };
}
*/

void box_policy_free(void* a_policy) {
  box_policy_clear(a_policy);
  if (a_policy)
    free(a_policy);
}

/*
void box_policy_free_one(void* a_policy_one) {
  box_policy_clear_one(a_policy_one);
  if (a_policy_one)
    free(a_policy_one);
}
*/

/* void box_policy_clear_one(void* a_policy_one) */
void box_policy_clear(void* a_policy) {
   box_policy_t* policy = (box_policy_t*) a_policy;
   if (policy) {
     free(policy->p);
     policy->p = NULL;
     policy->size = 0;
   };
}

void* box_policy_copy(void* a_policy) {
  box_policy_t* result = NULL;
  if (a_policy) {
    box_policy_t* policy = (box_policy_t*) a_policy;
    result = (box_policy_t*) malloc(sizeof(box_policy_t));
    if (result) {
       result->size = policy->size;
       result->p = (box_policy_dim_t*) malloc(sizeof(box_policy_dim_t)*policy->size);
       memcpy(result->p, policy->p, sizeof(box_policy_dim_t)*policy->size);
    };
  };
  return result;
}

/*
void* box_policy_copy(void* a_policy) {
  if (a_policy) {
     box_policy_t* policy = (box_policy_t*) a_policy;
     box_policy_t* result = (box_policy_t*) malloc(sizeof(box_policy_t));
     if (result) {
       size_t i;
       result->size = policy->size;
       result->p = NULL;
       if (policy->size) {
          result->p = (box_policy_one_t*) malloc(sizeof(box_policy_one_t)*policy->size);
          for (i = 0; i < policy->size; ++i) {
            result->p[i].size = policy->p[i].size;
            result->p[i].p = (box_policy_dim_t*) malloc(sizeof(box_policy_dim_t)*policy->p[i].size);
            memcpy(result->p[i].p, policy->p[i].p, sizeof(box_policy_dim_t)*policy->p[i].size);
          };
       };
     };
     return result;
   };
   return NULL;
}
*/

void box_policy_printf(FILE* stdout, void* a_policy) {
  box_policy_t* policy = (box_policy_t*) a_policy;
  if (policy) {
    for (size_t var = 0; var < policy->size; ++var) {
/*    int var;
      int local_size = policy->p[i].size;
      for (var = 0; var < local_size; ++var) { */
         box_policy_dim_t* var_policy = &policy->p[var];
         fprintf(stdout, "%d=", var);
         if (var_policy->inf == BOX_POLICY_1)
           fputc('l', stdout);
         else if (var_policy->inf == BOX_POLICY_2)
           fputc('r', stdout);
         else
           fputc('x', stdout);
         if (var_policy->sup == BOX_POLICY_1)
           fputc('l', stdout);
         else if (var_policy->sup == BOX_POLICY_2)
           fputc('r', stdout);
         else
           fputc('x', stdout);
         if (var < policy->size)
           fputc(' ', stdout);
         else
           fputc('\n', stdout);
/*    }; */
    };
  };
}

/*
box_policy_t* box_policy_alloc(int size, int dimension ) {
  assert (size > 0 && dimension > 0);
  box_policy_t* result = (box_policy_t*) malloc(sizeof(box_policy_t));
  if (result) {
    int i;
    result->size = size;
    result->p = (box_policy_one_t*) malloc(sizeof(box_policy_one_t)*size);
    for (i = 0; i < size; ++i) {
      result->p[i].size = dimension;
      result->p[i].p = (box_policy_dim_t*) malloc(sizeof(box_policy_dim_t)*dimension);
    };
  };
  return result;
}
*/

box_policy_t* box_policy_alloc(int dimension) {
  box_policy_t* result = 0;
  if (dimension > 0) {
     result = (box_policy_t*) malloc(sizeof(box_policy_t));
     if (result) {
       result->size = dimension;
       result->p = (box_policy_dim_t*) malloc(sizeof(box_policy_dim_t)*dimension);
     };
  };
  return result;
}

/* ============================================================ */
/* Meet */
/* ============================================================ */

static 
bool itv_meet_policy(itv_internal_t* intern, box_policy_dim_t* policy,
		     itv_t a, itv_t b, itv_t c) 
{
  if (policy->sup == BOX_POLICY_1)
    bound_set(a->sup, b->sup);
  else if (policy->sup == BOX_POLICY_2)
    bound_set(a->sup, c->sup);
  else
    assert(false);

  if (policy->inf == BOX_POLICY_1)
    bound_set(a->inf, b->inf);
  else if (policy->inf == BOX_POLICY_2)
    bound_set(a->inf, c->inf);
  else
    assert(false);
  return itv_canonicalize(intern,a,false);
}

static
bool itv_meet_change_policy(itv_internal_t* intern, box_policy_dim_t* policy,
			    itv_t a, itv_t b, itv_t c) 
{
  box_policy_dim_t newpolicy;
  int cmp;
  bool exc;

  cmp = bound_cmp(b->inf, c->inf);
  newpolicy.inf = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;

  cmp = bound_cmp(b->sup, c->sup);
  newpolicy.sup = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
  
  exc = itv_meet_policy(intern,policy,a,b,c);
  *policy = newpolicy;
  return exc;
}

box_t* box_meet_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* current_policy,
		       bool destructive, box_t* a1, box_t* a2) {
  size_t i;
  bool exc = false;
  size_t nbdims;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;
  box_policy_t* policy = ((box_policy_t**) current_policy->policies)[current_policy->index];
  assert(current_policy->index < current_policy->size);

  /* if (policy->size!=1) abort(); */

  ++current_policy->index;
  man->result.flag_best = false;
  man->result.flag_exact = false;

  res = destructive ? a1 : (box_t*) box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL || a2->p==NULL){
    box_set_bottom(res);
    return res;
  }
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a1->realdim;
  if (policy->size != nbdims) abort();
  for (i=0; i<nbdims; i++) {
    if (current_policy->mode == AP_POLICY_APPLY)
      exc = itv_meet_policy(intern->itv, &policy->p[i], 
			    res->p[i], a1->p[i], a2->p[i]);
    else if (current_policy->mode == AP_POLICY_CHANGE)
      exc = itv_meet_change_policy(intern->itv, &policy->p[i], 
				   res->p[i], a1->p[i],a2->p[i]);
    else
      assert(false);
    if (exc){
      box_set_bottom(res);
      break;
    }
  }
  return res;
}

box_t* box_meet_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* current_policy, 
			     box_t** tab, size_t size)
{
  /* box_policy_t policy1; */
  size_t i;
  box_t* res;
  box_policy_t* policy = ((box_policy_t**) current_policy->policies)[current_policy->index];
  res = box_copy(man,tab[0]);

  if(size-1 != policy->size) abort();

  int index = current_policy->index;
  /* policy1.size = 1; */
  for (i=1;i<size;i++){
    /* policy1.p = &policy->p[i];*/
    box_meet_policy(man,policy_man,current_policy /* mode, &policy1*/,true,res,tab[i]);
    if (res->p==NULL) break;
    if (i < size)
       current_policy->index = index;
  }
  ++current_policy->index;
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return res;
}

/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

/* Meet of an abstract value with a constraint */
void box_meet_lincons_policy_internal(box_internal_t* intern,
				      ap_policy_mode_t mode,
				      box_policy_t* policy,
				      box_t* a,
				      itv_lincons_t* cons)
{
  size_t nbcoeffs,nbdims;
  ap_dim_t dim = AP_DIM_MAX;
  int previous_dim;
  size_t i;
  itv_ptr pitv;
  itv_linexpr_t* expr;
  bool *peq;
  bool equality;
  bool exc;
  int cmp;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (a->p==NULL){
    box_set_bottom(a);
  }
  nbdims = a->intdim + a->realdim;
  expr = &cons->linexpr;

  /* Iterates on coefficients */
  nbcoeffs = 0;
  itv_set_int(intern->meet_lincons_internal_itv2,0);
  previous_dim = -1;
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    {
      int j;
      for(j=previous_dim+1; j<(int)dim; j++){
	if (policy->p[j].inf == BOX_POLICY_2){
	  bound_set_infty(a->p[dim]->inf,1);
	  if (mode == AP_POLICY_CHANGE)
	    policy->p[j].inf = BOX_POLICY_1;
	}
	if (policy->p[j].sup == BOX_POLICY_2){
	  bound_set_infty(a->p[dim]->sup,1);
	  if (mode == AP_POLICY_CHANGE)
	    policy->p[j].sup = BOX_POLICY_1;
	}
      }
      previous_dim = (int)dim;
    }
    nbcoeffs++;
    box_policy_dim_t newpolicy_dim = policy->p[dim];

    /* 1. sign of a in the expression e = ax+e' */
    int sgncoeff = 
      bound_sgn(pitv->inf)<0 ?
      1 :
      ( bound_sgn(pitv->sup)<0 ?
	-1 :
	0 );
    if (sgncoeff==0){
      if (policy->p[dim].inf == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->inf,1);
	if (mode == AP_POLICY_CHANGE)
	  newpolicy_dim.inf = BOX_POLICY_1;
      }
      if (policy->p[dim].sup == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->sup,1);
	if (mode == AP_POLICY_CHANGE)
	  newpolicy_dim.sup = BOX_POLICY_1;
      }
    } else { /* 2. We decompose the expression e = ax+e' */
      /* We save the linterm */
      itv_swap(intern->meet_lincons_internal_itv2,pitv);
      equality = *peq;
      *peq = true;
      /* 2. We evaluate e' */
      itv_eval_linexpr(intern->itv,
		       intern->meet_lincons_internal_itv3,expr,a->p);
      if (equality){
	/*
	  If we have ax+e' >= 0 with a>0
	  we can deduce that x>=-e'/a, or x>= inf(-e'/a)
	  If we have ax+e' >= 0 with a<0
	  we can deduce that -ax<=e', or x<= sup(e'/-a)
	  If we have ax+e'=0
	  we can deduce x=-e'/a, or inf(-e'/a)<= x <= sup(-e'/a)
	*/
	if ((mode == AP_POLICY_CHANGE || policy->p[dim].inf == BOX_POLICY_2)
	    &&
	    (sgncoeff>0 || cons->constyp == AP_CONS_EQ)){
	  /*
	    If we have a>0, we compute sup(e')/a=sup(e'/a)=-inf(-e'/a)
	    If we have a<0, we compute -inf(e')/(-a)=-inf(-e'/a)
	  */
	  if (sgncoeff>0){
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->sup,
		      intern->meet_lincons_internal_itv2->sup);
	  }
	  else {
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->inf,
		      intern->meet_lincons_internal_itv2->inf);
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    newpolicy_dim.inf = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (policy->p[dim].inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if ((mode == AP_POLICY_CHANGE || policy->p[dim].sup == BOX_POLICY_2) && 
	    (sgncoeff<0 || cons->constyp == AP_CONS_EQ)){
	  /*
	    If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	    If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	  */
	  if (sgncoeff<0){
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->sup,
		      intern->meet_lincons_internal_itv2->inf);
	  }
	  else {
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->inf,
		      intern->meet_lincons_internal_itv2->sup);
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    newpolicy_dim.sup = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (policy->p[dim].sup == BOX_POLICY_2){
	    bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	  }
	}
      }
      else { /* We have an interval */
	/*
	  - If we have [m;M]x+e' >= 0 with m>0, 
	  then [m,M]x>=inf(-e'), or [m,M]x>=-sup(e')
	  so we need at least
	  * if -sup(e')>=0: x>=-sup(e')/M
	  * if -sup(e')<=0: x>=-sup(e')/m
	  - If we have [m,M]x+e'<=0 with M<0, then [-M,-m]x>=inf(e')
	  * inf(e')>=0: x>=inf(e')/-m
	  * inf(e')<=0: x>=inf(e')/-M
	  
	  - If we have [m;M]x+e' >= 0 with M<0, 
	  then [-M,-m]x<=sup(e'), so we need at least
	  * if sup(e')>=0: x<=sup(e')/-M
	  * if sup(e')<=0: x<=sup(e')/-m
	  - If we have [m,M]x+e'<=0 with m>0, then [m,M]x<=sup(-e')
	  or [m,M]x<=-inf(e')
	  * -inf(e')>=0: x<=inf(e')/-M
	  * -inf(e')<=0: x<=inf(e')/-m
	  */
	int sgninf = bound_sgn(intern->meet_lincons_internal_itv3->inf);
	int sgnsup = bound_sgn(intern->meet_lincons_internal_itv3->sup);
	if (policy->p[dim].inf == BOX_POLICY_2 && 
	    (sgncoeff>0 || (cons->constyp==AP_CONS_EQ && sgncoeff<0))){
	  if (sgncoeff>0){
	    if (sgnsup<=0){
	      /* We compute sup(e')/M */
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup,
			intern->meet_lincons_internal_itv2->sup);
	    } else {
	      /* We compute sup(e')/m = (-sup(e'))/(-m) */
	      bound_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup);
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv2->inf);
	    }
	  }
	  else {
	    bound_neg(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->inf);
	    if (sgninf<=0){
	      /* We compute inf(e')/m = (-inf(e'))/(-m) */
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv2->inf);
	    } else {
	      /* We compute inf(e')/M) = - (-inf(e'))/M */
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv2->sup);
	      bound_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_bound);
	    }
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    newpolicy_dim.inf = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (policy->p[dim].inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if (policy->p[dim].sup == BOX_POLICY_2 && 
	    (sgncoeff<0 || (cons->constyp==AP_CONS_EQ && sgncoeff>0))){
	  if (sgncoeff<0){
	    if (sgnsup>=0){
	      /* We compute sup(e')/-M */
	      bound_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv2->sup);
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup,
			intern->meet_lincons_internal_bound);
	    } else {
	      /* We compute sup(e')/-m */
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup,
			intern->meet_lincons_internal_itv2->inf);
	    }
	  }
	  else {
	    if (sgninf>=0){		
	      /* We compute -inf(e')/M */
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->inf,
			intern->meet_lincons_internal_itv2->sup);
	    }
	    else {
	      /* We compute -inf(e')/m = inf(e')/(-m) */
	      bound_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->inf);
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv2->inf);
	    }
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    newpolicy_dim.sup = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (policy->p[dim].sup == BOX_POLICY_2){
	    bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	  }
	}
      }
      if (mode == AP_POLICY_CHANGE){
	policy->p[dim] = newpolicy_dim;
      }
      itv_swap(intern->meet_lincons_internal_itv2,pitv);
      *peq = equality;
    }
    exc = itv_canonicalize(intern->itv,a->p[dim],dim<a->intdim);
    if (exc){
      box_set_bottom(a);
      return;
    }
  }
  if (nbcoeffs==0){ /* Maybe an unsatisfiable constraint */
    bool unsat;
    int sgnsup = bound_sgn(expr->cst->sup);
    int sgninf = expr->equality ? sgnsup : -bound_sgn(expr->cst->inf);
    switch(cons->constyp){
    case AP_CONS_EQ:
      unsat = sgnsup < 0 || sgninf > 0;
      break;
    case AP_CONS_SUPEQ:
      unsat = sgnsup<0;
      break;
    case AP_CONS_SUP:
      unsat = (sgnsup<=0);
      break;
    default:
      abort();
    }
    if (unsat){
      box_set_bottom(a);
    }
    else {
      if (policy->p[dim].inf == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->inf,1);
	if (mode == AP_POLICY_CHANGE)
	  policy->p[dim].inf = BOX_POLICY_1;
      }
      if (policy->p[dim].sup == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->sup,1);
	if (mode == AP_POLICY_CHANGE)
	  policy->p[dim].sup = BOX_POLICY_1;
      }
    }
  }
}

box_t* box_meet_lincons_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* current_policy,
				     bool destructive,
				     box_t* a,
				     ap_lincons0_array_t* array)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  box_internal_t* intern = (box_internal_t*)man->internal;
  box_policy_t** policies = (box_policy_t**) current_policy->policies;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = false;
    man->result.flag_exact = false;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern->itv,&tlincons,array);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      box_set_bottom(res);
    }
    else {
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
          assert(current_policy->index + i < current_policy->size);
	  box_meet_lincons_policy_internal(intern,
						  current_policy->mode,
						  policies[current_policy->index + i],
						  res,
						  &tlincons.p[i]);
	  if (box_is_bottom(man, res)) {
	    k = kmax;
	    i = array->size;
	  };
	}
        current_policy->index += array->size;
      }
    }
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}

box_t* box_meet_tcons_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* current_policy,
				     bool destructive,
				     box_t* a,
				     ap_tcons0_array_t* array)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  box_internal_t* intern = (box_internal_t*)man->internal;
  box_policy_t** policies = (box_policy_t**) current_policy->policies;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = false;
    man->result.flag_exact = false;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern->itv,&tlincons,
				     array,res->p,res->intdim);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      box_set_bottom(res);
    }
    else {
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
          assert(current_policy->index + i < current_policy->size);
	  box_meet_lincons_policy_internal(intern,
						  current_policy->mode,
						  policies[current_policy->index + i],
						  res,
						  &tlincons.p[i]);
	  if (box_is_bottom(man, res)) {
	    k = kmax;
	    i = array->size;
	  };
	}
        current_policy->index += array->size;
      }
    }
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}

box_t*
box_assign_linexpr_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
    bool destructive, box_t* a, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size, box_t* dest) {
  return box_assign_linexpr_array(man, destructive, a, tdim, texpr, size, dest);

}
box_t*
box_assign_texpr_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
    bool destructive, box_t* a, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size, box_t* dest) {
  return box_assign_texpr_array(man, destructive, a, tdim, texpr, size, dest);

}
