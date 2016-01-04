/* ************************************************************************* */
/* environment.c: binding of addresses (strings) to dimensions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <string.h>
#include <limits.h>
#include "ap_manager.h"
#include "ap_environment.h"

static int var_cmp(const void* a, const void* b)
{
  void* aa = *((void*const*)a);
  void* bb = *((void*const*)b);
  return ap_var_operations->compare(aa,bb);
}

typedef struct vardim_t {
  ap_var_t var;
  ap_dim_t dim;
} vardim_t;

static int vardim_cmp(const void* a, const void* b)
{
  const vardim_t* aa = (const vardim_t*)a;
  const vardim_t* bb = (const vardim_t*)b;
  return ap_var_operations->compare(aa->var,bb->var);
}

/* ************************************************************************* */
/* I. env_t */
/* ************************************************************************* */

/* The env_t internal type store the binding name to dimension for one type.
   We use this type to factorize most operations on environments.
*/

/* ========================================================================= */
/* Datatype */
/* ========================================================================= */

typedef struct env_t {
  ap_var_t* var_of_dim;
  /*
    Array of size size, indexed by dimensions,
    representing the map dim -> name.
    - It should not contain identical strings.
    - It is lexicographically sorted.
    - The memory allocated for the strings are attached to the structure
    (they are freed when the structure is no longer in use)
  */
  size_t size; /* Size of the array */
} env_t;

/* Free all the memory pointed from env and reinitialize fields with 0. */
static void env_clear(env_t* env)
{
  size_t i;
  for (i=0;i<env->size; i++){
    ap_var_operations->free(env->var_of_dim[i]);
    env->var_of_dim[i] = NULL;
  }
  if (env->var_of_dim){
    free(env->var_of_dim);
    env->var_of_dim = NULL;
  }
  env->size = 0;
}

/* Normalize env by sorting the array of names.
   If perm!=NULL, fill already allocated and initialized perm
   with the applied permutation*/
static void env_normalize(env_t* env, ap_dim_t* perm)
{
  if (perm==NULL){
    qsort(env->var_of_dim,env->size,sizeof(ap_var_t),var_cmp);
  }
  else {
    size_t i;
    vardim_t* vardim = malloc(env->size*sizeof(vardim_t));
    for (i=0; i<env->size; i++){
      vardim[i].var = env->var_of_dim[i];
      vardim[i].dim = i;
    }
    qsort(vardim,env->size,sizeof(vardim_t),vardim_cmp);
    for(i=0; i<env->size; i++){
      env->var_of_dim[i] = vardim[i].var;
      perm[vardim[i].dim] = i; /* invert the permutation vardim[].dim */
    }
    free(vardim);
  }
}

/* Build an object env_t from its component */
static env_t env_cons(ap_var_t* var_of_dim, size_t size)
{
  env_t res;
  res.var_of_dim = var_of_dim;
  res.size = size;
  return res;
}

/* Return true iff invariant not satisfied */
static bool env_check(env_t* env)
{
  int i;
  /* Check that there is no doublons */
  for (i=0; i<(int)env->size - 1; i++){
    if (ap_var_operations->compare(env->var_of_dim[i],env->var_of_dim[i+1])>=0){
      return true;
    }
  }
  return false;
}

/* ========================================================================= */
/* Tests */
/* ========================================================================= */

/* Return true iff the 2 environments share some variables */
static bool env_is_inter_empty(env_t* env1 ,env_t* env2)
{
  size_t i1,i2;
  i1 = i2 = 0;
  while (i1<env1->size && i2<env2->size){
    int sgn = ap_var_operations->compare(env1->var_of_dim[i1],env2->var_of_dim[i2]);
    if (sgn==0)
      return false;
    if (sgn>0) i2++; else i1++;
  }
  return true;
}

/* Return true iff env1 is a subset of env2 */
static bool env_is_leq(env_t* env1,
		       env_t* env2)
{
  size_t i,j;
  int sgn = 0;
  if (env1->size<=env2->size){
    for (i=0;i<env1->size; i++){
      j = 0;
      while (j<env2->size){
	sgn = ap_var_operations->compare(env1->var_of_dim[i],env2->var_of_dim[j]);
	j++;
	if (sgn<0) return false;
	if (sgn==0) break;
      }
      if(sgn!=0) return false;
    }
    return true;
  }
  else
    return false;
}


/* ========================================================================= */
/* Operations */
/* ========================================================================= */

/* Add an array of new names to the environment and return the resulting
   environment.

   - The new names are duplicated, so it is the responsability of the user to
   free the new names he provides.

   - If a new name is already present in the initial environment, the result
   does not satisfy the datatype invariant.

   - If perm!=NULL, fill already allocated and initialized perm with the
   applied permutation to move the new names from the end to their sorted
   location.
*/
static env_t env_add(env_t* env, ap_var_t* var_of_dim, size_t size, ap_dim_t* perm)
{
  env_t nenv;
  size_t i;
  assert( var_of_dim!=NULL || size==0);
  nenv.var_of_dim = malloc((env->size+size)*sizeof(ap_var_t));
  nenv.size = env->size+size;
  for (i=0;i<env->size; i++){
    nenv.var_of_dim[i] = ap_var_operations->copy(env->var_of_dim[i]);
  }
  for (i=0;i<size; i++){
    nenv.var_of_dim[env->size+i] = ap_var_operations->copy(var_of_dim[i]);
  }
  env_normalize(&nenv,perm);
  return nenv;
}

/* Remove from an environment an array of names.

- May reorder the input array
- If a name to be removed was not present, return { NULL, UINT_MAX }.
*/
static env_t env_remove(env_t* env, ap_var_t* var_of_dim, size_t size)
{
  env_t nenv;
  size_t i,j;

  assert( var_of_dim!=NULL || size==0);
  if (size > env->size){
    return env_cons(NULL,UINT_MAX);
  }
  nenv.var_of_dim = malloc((env->size-size)*sizeof(ap_var_t));
  nenv.size = env->size-size;
  if (var_of_dim && size){
    qsort(var_of_dim,size,sizeof(ap_var_t),var_cmp);
    j=0;
    for (i=0;i<nenv.size; i++){
      while (j<size){
	int sgn = ap_var_operations->compare(env->var_of_dim[i+j],var_of_dim[j]);
	assert(sgn<=0);
	if (sgn==0)
	  j++;
	else
	  break;
      }
      nenv.var_of_dim[i] = ap_var_operations->copy(env->var_of_dim[i+j]);
    }
    while (j<size){
      int sgn = ap_var_operations->compare(env->var_of_dim[nenv.size+j],var_of_dim[j]);
      if (sgn==0)
	j++;
      else {
	env_clear(&nenv);
	nenv.size = UINT_MAX;
	break;
      }
    }
  }
  else {
    /* Perform just a copy */
    for (i=0;i<env->size; i++){
      nenv.var_of_dim[i] = ap_var_operations->copy(env->var_of_dim[i]);
    }
  }
  return nenv;
}

/* Least common environment of the two environments */
static env_t env_lce(env_t* e1, env_t* e2)
{
  env_t e;
  size_t i,i1,i2;
  int sgn;

  e.var_of_dim = malloc((e1->size+e2->size)*sizeof(ap_var_t));
  i = 0;
  i1 = 0;
  i2 = 0;
  while (i1<e1->size && i2<e2->size){
    sgn = ap_var_operations->compare(e1->var_of_dim[i1],e2->var_of_dim[i2]);
    e.var_of_dim[i] = ap_var_operations->copy(sgn>0 ? e2->var_of_dim[i2] : e1->var_of_dim[i1]);
    i++;
    if (sgn>0)
      i2++;
    else if (sgn<0)
      i1++;
    else {
      i1++; i2++;
    }
  }
  while (i1<e1->size){
    e.var_of_dim[i] = ap_var_operations->copy(e1->var_of_dim[i1]);
    i++; i1++;
  }
  while (i2<e2->size){
    e.var_of_dim[i] = ap_var_operations->copy(e2->var_of_dim[i2]);
    i++; i2++;
  }

  e.size = i;
  e.var_of_dim = realloc(e.var_of_dim, e.size*sizeof(ap_var_t));
  return e;
}

/*
  - env1 is supposed to be subenvironment of env

  - The function computes in the already allocated dimchange1 the conversion
  permutation from env1 to env, using offsetwriter and offsetread to shift the
  dimensions.

  - return true iff wrong arguments
*/

static bool env_lce_dimchange(env_t* e1, env_t* e,
			      ap_dimchange_t* dimchange1,
			      size_t offsetwrite, size_t offsetread)
{
  size_t i,i1,k1;
  int sgn=0;

  if (e1->size>e->size) return true;
  i = 0;
  k1 = 0;
  for (i1=0; i1<e1->size; i1++){
    while (i<e->size){
      sgn = ap_var_operations->compare(e1->var_of_dim[i1],e->var_of_dim[i]);
      i++;
      if (sgn<0) return true;
      if (sgn==0) break;
      /* here, a new dimension to insert */
      dimchange1->dim[offsetwrite+k1] = offsetread+i1;
      k1++;
    }
    if(sgn!=0) return true;
  }
  while (i<e->size){
    dimchange1->dim[offsetwrite+k1] = offsetread+i1;
    i++;
    k1++;
  }
  return false;
}

/* Least common environment of an array of environments */
static env_t env_lce_array(env_t** tenv,
			   size_t* tindex,
			   size_t size)
{
  int sgn;
  size_t i,index,maxvars;
  ap_var_t name;
  env_t env;

  maxvars=0;
  for (i=0; i<size; i++){
    maxvars += tenv[i]->size;
    tindex[i] = 0;
  }
  env.var_of_dim = malloc( maxvars*sizeof(ap_var_t) );
  index = 0;
  while (true){
    /* Find one possible next variable, and exit if none */
    name = NULL;
    for (i=0;i<size;i++){
      if (tindex[i]<tenv[i]->size){
	name = tenv[i]->var_of_dim[tindex[i]];
	break;
      }
    }
    if (name==NULL) break;
    /* Find the minimal variable */
    for (i=1; i<size; i++){
      if (tindex[i]<tenv[i]->size){
	sgn = ap_var_operations->compare(name,tenv[i]->var_of_dim[tindex[i]]);
	if (sgn>0)
	  name = tenv[i]->var_of_dim[tindex[i]];
      }
    }
    env.var_of_dim[index] = ap_var_operations->copy(name);
    index++;
    /* Increment the indexes */
    for (i=0; i<size; i++){
      if (tindex[i]<tenv[i]->size){
	sgn = ap_var_operations->compare(name,tenv[i]->var_of_dim[tindex[i]]);
	if (sgn==0)
	  tindex[i]++;
      }
    }
  }
  env.size = index;
  env.var_of_dim = realloc(env.var_of_dim, env.size*sizeof(ap_var_t));
  return env;
}

/* ************************************************************************* */
/* II. ap_environment_t */
/* ************************************************************************* */

/* ========================================================================= */
/* Datatype */
/* ========================================================================= */

/* The denv_t internal type store the binding name to dimension for all types */

typedef struct denv_t {
  env_t envint;
  env_t envreal;
} denv_t;


static denv_t denv_of_environment(ap_environment_t* env)
{
  denv_t res;
  res.envint = env_cons(env->var_of_dim,env->intdim);
  res.envreal = env_cons(env->var_of_dim ?
			 &env->var_of_dim[env->intdim] :
			 NULL,
			 env->realdim);
  return res;
}

static ap_environment_t* environment_of_denv(denv_t* denv)
{
  ap_environment_t* res;
  res = malloc(sizeof(ap_environment_t));
  res->var_of_dim = malloc((denv->envint.size + denv->envreal.size)*
			   sizeof(ap_var_t));
  memcpy(&res->var_of_dim[0],
	 denv->envint.var_of_dim,denv->envint.size*sizeof(ap_var_t));
  memcpy(&res->var_of_dim[denv->envint.size],
	 denv->envreal.var_of_dim,denv->envreal.size*sizeof(ap_var_t));
  free(denv->envint.var_of_dim);
  free(denv->envreal.var_of_dim);
  res->intdim = denv->envint.size;
  res->realdim = denv->envreal.size;
  res->count = 1;
  return res;
}

/* ========================================================================= */
/* Access */
/* ========================================================================= */
ap_dim_t ap_environment_dim_of_var(ap_environment_t* env, ap_var_t name){
  ap_var_t* res;
  res = bsearch(&name,env->var_of_dim,env->intdim,sizeof(ap_var_t),var_cmp);
  if (res!=NULL){
    return ((long int)res - (long int)env->var_of_dim)/sizeof(ap_var_t);
  }
  else {
    res = bsearch(&name,&env->var_of_dim[env->intdim],env->realdim,sizeof(ap_var_t),var_cmp);
    if (res!=NULL){
      return ((long int)res - (long int)env->var_of_dim)/sizeof(ap_var_t);
    }
    else {
      return AP_DIM_MAX;
    }
  }
}

/* ========================================================================= */
/* Checks */
/* ========================================================================= */


bool ap_environment_check(ap_environment_t* env)
{
  denv_t denv = denv_of_environment(env);

  /* Check that there is no doublons */
  if (env_check(&denv.envint)) return true;
  if (env_check(&denv.envreal)) return true;
  /* Check that integer and real variables are disjoint */
  if (! env_is_inter_empty(&denv.envint,&denv.envreal)) return true;
  return false;
}

bool ap_environment_check_compatibility(ap_environment_t* env1,
					ap_environment_t* env2)
{
  denv_t denv1 = denv_of_environment(env1);
  denv_t denv2 = denv_of_environment(env2);
  /* Check that integer of one and real variables of other are disjoint */
  return !(env_is_inter_empty(&denv1.envint,&denv2.envreal) &&
	   env_is_inter_empty(&denv2.envint,&denv1.envreal) );
}

/* ========================================================================= */
/* Memory management */
/* ========================================================================= */

ap_environment_t* ap_environment_add(ap_environment_t* env,
				     ap_var_t* name_of_intdim, size_t intdim,
				     ap_var_t* name_of_realdim, size_t realdim)
{
  ap_environment_t* res;
  denv_t denv2;

  denv_t denv1 = denv_of_environment(env);

  denv2.envint = env_add(&denv1.envint, name_of_intdim, intdim, NULL);
  denv2.envreal = env_add(&denv1.envreal, name_of_realdim, realdim, NULL);
  res = environment_of_denv(&denv2);
  /* Check now that there is no problem */
  if (ap_environment_check(res)){
    ap_environment_free(res);
    return NULL;
  }
  return res;
}

ap_environment_t* ap_environment_add_perm(ap_environment_t* env,
					  ap_var_t* name_of_intdim, size_t intdim,
					  ap_var_t* name_of_realdim, size_t realdim,
					  ap_dimperm_t* perm)
{
  ap_environment_t* res;
  size_t i,nsize;
  denv_t denv2;
  denv_t denv1 = denv_of_environment(env);

  nsize = env->intdim+intdim+env->realdim+realdim;
  ap_dimperm_init(perm,nsize);
  denv2.envint = env_add(&denv1.envint, name_of_intdim, intdim,
			 &(perm->dim[0]));
  denv2.envreal = env_add(&denv1.envreal, name_of_realdim, realdim,
			  &(perm->dim[env->intdim+intdim]));
  for (i=0;i<env->realdim+realdim;i++){
    perm->dim[env->intdim + intdim + i] += env->intdim + intdim;
  }
  res = environment_of_denv(&denv2);
  /* Check now that there is no problem */
  if (ap_environment_check(res)){
    ap_environment_free(res);
    ap_dimperm_clear(perm);
    res = NULL;
  }
  return res;
}

ap_environment_t* ap_environment_remove(ap_environment_t* env,
					ap_var_t* tvar, size_t size)
{
  ap_var_t* tvar2;
  size_t i;
  size_t intdim,realdim;
  ap_var_t* tvarint;
  ap_var_t* tvarreal;
  denv_t denv1,denv2;
  ap_environment_t* res;

  if (size==0)
    return ap_environment_copy(env);
  ;
  /* Copy the input array and split it into integer and real arrays */
  tvar2 = malloc(size*sizeof(ap_var_t));
  memcpy(tvar2,tvar,size*sizeof(ap_var_t));
  intdim = size;
  i = 0;
  while (i<size && i<intdim){
    ap_dim_t dim = ap_environment_dim_of_var(env,tvar2[i]);
    if (dim==AP_DIM_MAX){
      free(tvar2);
      return NULL;
    }
    if (dim>=env->intdim){
      intdim--;
      ap_var_t t = tvar2[i]; tvar2[i] = tvar2[intdim]; tvar2[intdim] = t;
    }
    else {
      i++;
    }
  }
  tvarint = intdim ? &tvar2[0] : NULL;
  realdim = size-intdim;
  tvarreal = realdim ? &tvar2[intdim] : NULL;

  denv1 = denv_of_environment(env);
  denv2.envint = env_remove(&denv1.envint, tvarint, intdim);
  denv2.envreal = env_remove(&denv1.envreal, tvarreal, realdim);
  if (denv2.envint.size==UINT_MAX ||
      denv2.envreal.size==UINT_MAX){
    res = NULL;
  } else {
    res = environment_of_denv(&denv2);
  }
  free(tvar2);
  return res;
}

ap_environment_t* ap_environment_alloc(ap_var_t* name_of_intdim, size_t intdim,
				       ap_var_t* name_of_realdim, size_t realdim)
{
  ap_environment_t env = { NULL, 0,0,0 };
  return ap_environment_add(&env,
			    name_of_intdim, intdim,
			    name_of_realdim, realdim);
}
ap_environment_t* ap_environment_alloc_empty()
{
  return ap_environment_alloc(NULL,0,NULL,0);
}

void ap_environment_free2(ap_environment_t* env)
{
  size_t i;
  if (env->var_of_dim){
    for(i=0;i<env->intdim+env->realdim;i++){
      if(env->var_of_dim[i]){
	ap_var_operations->free(env->var_of_dim[i]);env->var_of_dim[i]=NULL;
      }
    }
    free(env->var_of_dim); env->var_of_dim = NULL;
  }
  env->count = 0;
  free(env);
}
void ap_environment_fdump(FILE* stream, ap_environment_t* env)
{
  size_t i;
  char* name;

  fprintf(stream,"environment: dim = (%lu,%lu), count = %lu\n",
	  (unsigned long)env->intdim,(unsigned long)env->realdim,
	  (unsigned long)env->count);
  for (i=0; i<env->intdim+env->realdim; i++){
    name = ap_var_operations->to_string(env->var_of_dim[i]);
    fprintf(stream,"%2lu: %s\n",(unsigned long)i,name);
    free(name);
  }
}

ap_environment_name_of_dim_t* ap_environment_name_of_dim_alloc(ap_environment_t* env)
{
  size_t i,size;
  size = env->intdim+env->realdim;
  ap_environment_name_of_dim_t* res = malloc(sizeof(ap_environment_name_of_dim_t)+size*sizeof(char*));
  res->size = size;
  for (i=0; i<size; i++){
    res->p[i] = ap_var_operations->to_string(env->var_of_dim[i]);
  }
  return res;
}
void ap_environment_name_of_dim_free(ap_environment_name_of_dim_t* p)
{
  size_t i;
  for (i=0; i<p->size; i++){
    free(p->p[i]);
  }
  free(p);
}

/* ========================================================================= */
/* Tests */
/* ========================================================================= */

bool ap_environment_is_eq(ap_environment_t* env1,
			  ap_environment_t* env2)
{
  bool res = (env1==env2);
  if (!res){
    res =
      (env1->intdim==env2->intdim) &&
      (env1->realdim==env2->realdim);
    if (res){
      size_t i;
      for (i=0; i<env1->intdim+env1->realdim; i++){
	if (ap_var_operations->compare(env1->var_of_dim[i],env2->var_of_dim[i])){
	  res = false;
	  break;
	}
      }
    }
  }
  return res;
}

bool ap_environment_is_leq(ap_environment_t* env1,
			   ap_environment_t* env2)
{
  bool res = (env1==env2);
  if (!res){
    res =
      (env1->intdim<=env2->intdim) &&
      (env1->realdim<=env2->realdim);
    if (res){
      denv_t denv1 = denv_of_environment(env1);
      denv_t denv2 = denv_of_environment(env2);
      res =
	env_is_leq(&denv1.envint,&denv2.envint) &&
	env_is_leq(&denv1.envreal,&denv2.envreal);
    }
  }
  return res;
}

int ap_environment_compare(ap_environment_t* env1,
			   ap_environment_t* env2)
{
  ap_dimchange_t* dimchange1=NULL;
  ap_dimchange_t* dimchange2=NULL;
  ap_environment_t* e=NULL;
  int res;

  e = ap_environment_lce(env1,env2,&dimchange1,&dimchange2);
  if (e==NULL) res = -2;
  else if (dimchange1==NULL && dimchange2==NULL) res = 0;
  else if (dimchange1==NULL) res = 1;
  else if (dimchange2==NULL) res = -1;
  else res = 2;
  if (e) ap_environment_free(e);
  if (dimchange1) ap_dimchange_free(dimchange1);
  if (dimchange2) ap_dimchange_free(dimchange2);
  return res;
}

int ap_environment_hash(ap_environment_t* env)
{
  int res;
  size_t size,i,dec;

  res = 997*(7*env->intdim+11*env->realdim);
  size = env->intdim+env->realdim;
  dec = 0;
  for (i=0; i<size; i += (size+3)/4){
    res += ap_var_operations->hash(env->var_of_dim[i]) << dec;
    dec++;
  }
  return res;
}


/* ========================================================================= */
/* Compute least common environment of 2 environments */
/* ========================================================================= */

ap_dimchange_t* ap_environment_dimchange(ap_environment_t* env1,
					 ap_environment_t* env)
{
  bool b;
  ap_dimchange_t* dimchange;

  if (env->intdim < env1->intdim || env->realdim < env1->realdim) return NULL;

  denv_t denv1 = denv_of_environment(env1);
  denv_t denv = denv_of_environment(env);

  dimchange = ap_dimchange_alloc(env->intdim - env1->intdim,
				 env->realdim - env1->realdim);
  b = env_lce_dimchange(&denv1.envint,&denv.envint,
			dimchange, 0, 0);
  if (!b)
    b = env_lce_dimchange(&denv1.envreal,&denv.envreal,
			  dimchange, dimchange->intdim, env1->intdim);
  if (b){
    ap_dimchange_free(dimchange);
    dimchange = NULL;
  }
  return dimchange;
}

/*
  Compute the transformation for switching from one environment to another one (adding and then removal of dimensions.

  - If environments are not compatible, return NULL
*/
ap_dimchange2_t* ap_environment_dimchange2(ap_environment_t* env1,
					   ap_environment_t* env2)
{
  size_t size;
  denv_t denv;
  bool eq1,eq2;
  ap_dimchange2_t* res;

  if (ap_environment_check_compatibility(env1,env2))
    return NULL;

  denv_t denv1 = denv_of_environment(env1);
  denv_t denv2 = denv_of_environment(env2);

  denv.envint = env_lce(&denv1.envint, &denv2.envint);
  denv.envreal = env_lce(&denv1.envreal, &denv2.envreal);
  size = denv.envint.size+denv.envreal.size;

  res = ap_dimchange2_alloc(NULL,NULL);
  eq1 =
    denv.envint.size==denv1.envint.size &&
    denv.envreal.size==denv1.envreal.size;
  eq2 =
    denv.envint.size==denv2.envint.size &&
    denv.envreal.size==denv2.envreal.size;
  if (!eq1){
    res->add = ap_dimchange_alloc(denv.envint.size-denv1.envint.size,
				  denv.envreal.size-denv1.envreal.size);
    env_lce_dimchange(&denv1.envint,&denv.envint,
		      res->add, 0, 0);
    env_lce_dimchange(&denv1.envreal,&denv.envreal,
		      res->add, res->add->intdim, denv1.envint.size);

  }
  if (!eq2){
    /* First build (as for lce) the dimchange for adding dimensions from env2
       to env(lce), then invert it */
    res->remove = ap_dimchange_alloc(denv.envint.size-denv2.envint.size,
				     denv.envreal.size-denv2.envreal.size);
    env_lce_dimchange(&denv2.envint,&denv.envint,
		      res->remove, 0,0);
    env_lce_dimchange(&denv2.envreal,&denv.envreal,
		      res->remove, res->remove->intdim, denv2.envint.size);
    ap_dimchange_add_invert(res->remove);
  }
  env_clear(&denv.envint);
  env_clear(&denv.envreal);
  return res;
}

/*
  Least common environment to two environment.

  - If environments are not compatible, return NULL

  - Compute also in dimchange1 and dimchange2 the conversion transformations.

  - If no dimensions to add to env1, this implies that env is
    actually env1. In this case, *dimchange1==NULL.
*/
ap_environment_t* ap_environment_lce(ap_environment_t* env1,
				     ap_environment_t* env2,
				     ap_dimchange_t** dimchange1,
				     ap_dimchange_t** dimchange2)
{
  size_t size;
  denv_t denv;
  bool eq1,eq2;

  if (ap_environment_check_compatibility(env1,env2))
    return NULL;

  denv_t denv1 = denv_of_environment(env1);
  denv_t denv2 = denv_of_environment(env2);

  denv.envint = env_lce(&denv1.envint, &denv2.envint);
  denv.envreal = env_lce(&denv1.envreal, &denv2.envreal);
  size = denv.envint.size+denv.envreal.size;

  eq1 =
    denv.envint.size==denv1.envint.size &&
    denv.envreal.size==denv1.envreal.size;
  eq2 =
    denv.envint.size==denv2.envint.size &&
    denv.envreal.size==denv2.envreal.size;
  if (eq1){
    *dimchange1 = NULL;
  }
  else {
    *dimchange1 = ap_dimchange_alloc(denv.envint.size-denv1.envint.size,
				     denv.envreal.size-denv1.envreal.size);
    env_lce_dimchange(&denv1.envint,&denv.envint,
		      *dimchange1, 0, 0);
    env_lce_dimchange(&denv1.envreal,&denv.envreal,
		      *dimchange1, (*dimchange1)->intdim, denv1.envint.size);
  }
  if (eq2){
    *dimchange2 = NULL;
  }
  else {
    *dimchange2 = ap_dimchange_alloc(denv.envint.size-denv2.envint.size,
				     denv.envreal.size-denv2.envreal.size);
    env_lce_dimchange(&denv2.envint,&denv.envint,
		      *dimchange2, 0,0);
    env_lce_dimchange(&denv2.envreal,&denv.envreal,
		      *dimchange2, (*dimchange2)->intdim, denv2.envint.size);
  }
  if (eq1){
    /* env=env1 */
    env_clear(&denv.envint);
    env_clear(&denv.envreal);
    return ap_environment_copy(env1);
  }
  else if (eq2){
    /* env=env1 */
    env_clear(&denv.envint);
    env_clear(&denv.envreal);
    return ap_environment_copy(env2);
  }
  else {
    return environment_of_denv(&denv);
  }
}

/* ========================================================================= */
/* Compute least common environment of an array of environments */
/* ========================================================================= */

/*
  Least common environment to an array environments.

  - Assume the size of the array is at least one.

  - If all input environments are the same, *ptdimchange==NULL.
  Otherwise, compute in *ptdimchange the conversion permutations.

  - Dimensions to add before applying the permutation (according to the
  conventions of the function ap_abstract0_add_permute_dimensions) are implictly
  given by env->intdim-env[i]->intdim and env->realdim - env[i]->realdim.

  - If no dimensions to add to env[i], this implies that env is actually
  env[i]. In this case, (*ptdimchange)[i]==NULL. Otherwise, the function allocates
  the array (*ptdimchange)[0] with malloc.

*/
static int env_cmp(const void* a, const void* b)
{
  ap_environment_t* pa = *((ap_environment_t*const*)a);
  ap_environment_t* pb = *((ap_environment_t*const*)b);
  return (pa>pb ? 1 : (pa==pb ? 0 : -1));
}

ap_environment_t* ap_environment_lce_array(ap_environment_t** tenv,
					   size_t size,
					   ap_dimchange_t*** ptdimchange)
{
  size_t i, intrealdim;
  denv_t* tdenv;
  env_t** te;
  size_t* tindex;
  denv_t denv;
  int j;
  bool change;

  assert(size>=1);

  qsort(tenv,size,sizeof(ap_environment_t*),env_cmp);
  /* remove doublons */
  for(i=0;i<size-1;i++){
    if (tenv[i]==tenv[i+1]){
      if (i<size-2){
	memmove(&tenv[i+1],&tenv[i+2],(size-i-2)*sizeof(ap_environment_t*));
      }
      size--;
    }
  }
  if (size==1){
    *ptdimchange = malloc(sizeof(ap_dim_t*));
    (*ptdimchange)[0] = NULL;
    return tenv[0];
  }
  if (size==2){
    if (ap_environment_is_eq(tenv[0],tenv[1])){
      *ptdimchange = NULL;
      return tenv[0];
    }
    else {
      *ptdimchange = malloc(2*sizeof(ap_dimchange_t*));
      return ap_environment_lce(tenv[0],tenv[1],&(*ptdimchange)[0],&(*ptdimchange)[1]);
    }
  }

  /* Compute env */
  tdenv = malloc(size*sizeof(denv_t));
  for (i=0;i<size;i++){
    tdenv[i] = denv_of_environment(tenv[i]);
  }

  tindex = malloc(size*sizeof(size_t));
  te = malloc(size*sizeof(env_t*));

  for (i=0;i<size;i++){
    te[i] = &tdenv[i].envint;
  }
  denv.envint = env_lce_array(te,tindex,size);
  for (i=0;i<size;i++){
    te[i] = &tdenv[i].envreal;
  }
  denv.envreal = env_lce_array(te,tindex,size);
  free(te);
  free(tindex);

  intrealdim = denv.envint.size + denv.envreal.size;

  /* Compute transformations */
  *ptdimchange = malloc(size*sizeof(ap_dimchange_t*));
  change = false;
  j = -1;
  for (i=0;i<size;i++){
    if (tdenv[i].envint.size==denv.envint.size &&
	tdenv[i].envreal.size==denv.envreal.size){
      (*ptdimchange)[i] = NULL;
      if (j<0) j = (int)i;
    }
    else {
      change = true;
      (*ptdimchange)[i] = ap_dimchange_alloc(denv.envint.size-tdenv[i].envint.size,
					     denv.envreal.size-tdenv[i].envreal.size);
      env_lce_dimchange(&tdenv[i].envint,&denv.envint,
			(*ptdimchange)[i],0,0);
      env_lce_dimchange(&tdenv[i].envreal,&denv.envreal,
			(*ptdimchange)[i],(*ptdimchange)[i]->intdim, tdenv[i].envint.size);
    }
  }
  free(tdenv);
  if (j>0){
    env_clear(&denv.envint);
    env_clear(&denv.envreal);
    if (!change){
      free(*ptdimchange);
      *ptdimchange=NULL;
    }
    return ap_environment_copy(tenv[j]);
  }
  else {
    ap_environment_t* env = environment_of_denv(&denv);
    if (ap_environment_check(env)){
      for (i=0;i<size;i++){
	if ((*ptdimchange)[i]) free((*ptdimchange)[i]);
      }
      free(*ptdimchange);
      *ptdimchange=NULL;
      ap_environment_free(env);
      env = NULL;
    }
    return env;
  }
}

/* ========================================================================= */
/* Renaming the variables of an environment */
/* ========================================================================= */

ap_environment_t* ap_environment_rename(ap_environment_t* env,
					ap_var_t* tvar1, ap_var_t* tvar2, size_t size,
					ap_dimperm_t* perm)
{
  size_t i,j,nbdims;
  ap_environment_t* res;
  denv_t denv;

  nbdims = env->intdim+env->realdim;
  res = malloc(sizeof(ap_environment_t));
  res->intdim = env->intdim;
  res->realdim = env->realdim;
  res->count = 1;
  res->var_of_dim = malloc(nbdims*sizeof(ap_var_t));

  /* Build the new environment */
  for (i=0; i<nbdims; i++){
    res->var_of_dim[i] = NULL;
  }
  for (j=0; j<size; j++){
    ap_dim_t dim = ap_environment_dim_of_var(env,tvar1[j]);
    if (dim==AP_DIM_MAX){
      ap_environment_free(res);
      return NULL;
    }
    res->var_of_dim[dim] = ap_var_operations->copy(tvar2[j]);
  }
  for (i=0; i<nbdims; i++){
    if (res->var_of_dim[i] == NULL){
      res->var_of_dim[i] = ap_var_operations->copy(env->var_of_dim[i]);
    }
  }

  /* Now we have to sort it and compute the permutation transformation */
  ap_dimperm_init(perm,nbdims);
  denv = denv_of_environment(res);
  env_normalize(&denv.envint,perm->dim);
  env_normalize(&denv.envreal,perm->dim+res->intdim);
  for (i=0; i<res->realdim; i++){
    perm->dim[res->intdim + i] += res->intdim;
  }
  if (ap_environment_check(res)){
    ap_environment_free(res);
    ap_dimperm_clear(perm);
    res = NULL;
  }
  return res;
}
