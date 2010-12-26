/* ********************************************************************** */
/* pk_vector.c: operations on vectors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pk_internal.h"
#include "pk_num.h"

/* ********************************************************************** */
/* I. Basic operations: creation, destruction, copying and printing */
/* ********************************************************************** */

/* Internal allocation function: the elements are not initialized. */
numintMPQ_t* _vector_alloc_int(size_t size){
  assert(size>0);
  return (numintMPQ_t*)malloc(size*sizeof(numintMPQ_t));
}

/* Standard allocation function, with initialization of the elements. */
numintMPQ_t* vector_alloc(size_t size)
{
  size_t i;
  numintMPQ_t* q;
  q = _vector_alloc_int(size);
  for(i=0; i<size; i++){
    numintMPQ_init(q[i]);
  }
  return q;
}

/* Reallocation function, to change the dimension */
void vector_realloc(numintMPQ_t** pq, size_t size, size_t nsize)
{
  size_t i;
  numintMPQ_t* q;
  numintMPQ_t* nq;

  q = *pq;
  for (i=nsize; i<size; i++){
    numintMPQ_clear(q[i]);
  }
  nq = realloc(q, nsize*sizeof(numintMPQ_t));
  for (i=size; i<nsize; i++){
    numintMPQ_init(nq[i]);
  }
  *pq = nq;
}

/* Copy/Assign function */
void vector_copy(numintMPQ_t* q2, numintMPQ_t* q1, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    numintMPQ_set(q2[i],q1[i]);
  }
}
/* Deallocation function. */
void vector_free(numintMPQ_t* q, size_t size)
{
  size_t i;
  for(i=0; i<size; i++) numintMPQ_clear(q[i]);
  free(q);
}

/* Set all elements to zero. */
void vector_clear(numintMPQ_t* q, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numintMPQ_set_int(q[i],0);
}

/* Raw printing function. */
void vector_print(numintMPQ_t* q, size_t size)
{
  size_t i;
  printf("vector %ld: ", (long)size);
  for (i=0; i<size; i++){
    numintMPQ_print(q[i]); printf(" ");
  }
  printf("\n");
}

/* ********************************************************************** */
/* II. Normalization */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Pgcd computation */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Searching the minimum non-zero coefficient */
/* ---------------------------------------------------------------------- */

/* The following functions search the index and the absolute value of the
   minimal non-zero coefficient of the vector pk->vector_numintp, of size size,
   supposed to contain positive values only.
   
   It returns its results with
   pointers index and min. If all coefficients are zero, then
   index is set to size and *min to 0.

   This function uses pk->vector_numintp and pk->vector_tmp[0]. */

static void
vector_min_notzero(pk_internal_t* pk,
		   size_t size,
		   int* index, numintMPQ_t min)
{
  size_t i;

  numintMPQ_t* v = pk->vector_numintp; 

  numintMPQ_set_int(min,0);

  /* search the first non-zero coefficient
     and stores the index and the coeff in *index and *min */
  i = 0;
  while (i<size){
    if (numintMPQ_sgn(v[i])){
      *index = i;
      numintMPQ_set(min,v[i]);
      break;
    }
    i++;
  }
  i++;
  /* search now the minimum */
  while (i<size) {
    if (numintMPQ_sgn(v[i])){
      if (numintMPQ_cmp(min,v[i]) > 0){
	*index = i;
	numintMPQ_set(min,v[i]);
      }
    }
    i++;
  }
}
/* This function computes the pgcd of a vector.

   This function uses pk->vector_numintp and pk->vector_tmp[0]. */

void vector_gcd(pk_internal_t* pk,
		numintMPQ_t* q, size_t size, numintMPQ_t gcd)
{
  size_t i;
  bool not_all_zero;
  numintMPQ_t* v = pk->vector_numintp; 

  for (i=0;i<size;i++)
    numintMPQ_abs(v[i],q[i]);

  do {
    int index=0;
    vector_min_notzero(pk,size,&index,gcd);
    if (numintMPQ_sgn(gcd)==0) break;
    not_all_zero = false;
    for (i=0; i<size; i++)
      if ((int)i!=index){
	numintMPQ_mod(v[i],v[i],gcd);
	not_all_zero = not_all_zero || numintMPQ_sgn(v[i]);
      }
  } while (not_all_zero);
}


/* ====================================================================== */
/* II.3 Main functions */
/* ====================================================================== */

/* The function vector_normalize normalizes the vector considered as
   a contraint or a generator. It does not modify q[0].

   This function use pk->vector_tmp[0..2] and pk->numintp. */

bool vector_normalize(pk_internal_t* pk,
		      numintMPQ_t* q, size_t size)
{
  size_t i;

  assert(size<=pk->maxcols);

  /*  computation of the pgcd */
  vector_gcd(pk,&q[1],size-1, pk->vector_tmp[1]);
  /* possible division */
  if (numintMPQ_cmp_int(pk->vector_tmp[1],1)>0){
    for (i=1; i<size; i++)
      numintMPQ_divexact(q[i],q[i],pk->vector_tmp[1]);
    return true;
  }
  else
    return false;
}

/* The function vector_normalize normalizes the vector considered as
   an expression. It modifies q[0].

   This function use pk->vector_tmp[0..2] and pk->numintp. */

bool vector_normalize_expr(pk_internal_t* pk,
			   numintMPQ_t* q, size_t size)
{
  size_t i;

  assert(size<=pk->maxcols);

  /*  computation of the pgcd */
  vector_gcd(pk,&q[0],size, pk->vector_tmp[1]);
  /* possible division */
  if (numintMPQ_cmp_int(pk->vector_tmp[1],1)>0){
    for (i=0; i<size; i++)
      numintMPQ_divexact(q[i],q[i],pk->vector_tmp[1]);
    return true;
  }
  else
    return false;
}

/* The function vector_normalize_constraint normalizes the vector considered as
   a constraint.

   - if strict mode, the epsilon coefficient is put to 0 or 1
 
   This function use pk->vector_tmp[0..1] and pk->numintp. */

bool vector_normalize_constraint(pk_internal_t* pk,
				 numintMPQ_t* q,
				 ap_dimension_t dim)
{
  size_t i;
  bool change = false;
  size_t size = pk->dec+dim.intd+dim.reald;
  
  assert(pk->dec+dim.intd+dim.reald <= pk->maxcols);

  if (pk->strict && numintMPQ_sgn(q[0])!=0 && numintMPQ_sgn(q[polka_eps])<0){
    /*  computation of the pgcd without epsilon */
    change = (numintMPQ_cmp_int(q[polka_eps],-1) != 0);
    numintMPQ_set_int(q[polka_eps],0);
    vector_gcd(pk,&q[1], size-1, pk->vector_tmp[1]);
    numintMPQ_set_int(q[polka_eps],-1);
    /* possible division */
    if (numintMPQ_cmp_int(pk->vector_tmp[1],1)>0){
      change = true;
      numintMPQ_divexact(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
      for (i=pk->dec; i<size; i++)
	numintMPQ_divexact(q[i],q[i],pk->vector_tmp[1]);
    }
  }
  else {
    vector_normalize(pk,q,size);
  }
  return change;
}

/* The function vector_normalize_constraint_int normalizes the vector 
   considered as a constraint.

   - if it involves only integer dimensions, the constraint is tightened and
     renormalized.

   - it implies standard constraint normalization
    
   This function use pk->vector_tmp[0..1] and pk->numintp. */

bool vector_normalize_constraint_int(pk_internal_t* pk,
				       numintMPQ_t* q,
				       ap_dimension_t dim)
{
  size_t i;
  bool change = false;
  size_t size = pk->dec+dim.intd+dim.reald;
  
  assert(pk->dec+dim.intd+dim.reald <= pk->maxcols);

  if (dim.intd>0 && 
      vector_is_integer(pk,q,dim) &&
      !vector_is_positivity_constraint(pk,q,size)){
    if (pk->strict && numintMPQ_sgn(q[polka_eps])<0){
      change = true;
      numintMPQ_set_int(q[polka_eps],0);
      numintMPQ_sub_uint(q[polka_cst], q[polka_cst], 1);
    }
    /*  computation of the pgcd without constant (and epsilon, of course) */
    vector_gcd(pk, &q[pk->dec], size-pk->dec, pk->vector_tmp[1]);
    /* possible division */
    if (numintMPQ_cmp_int(pk->vector_tmp[1],1)>0){
      change = true;
      for (i=pk->dec; i<size; i++)
	numintMPQ_divexact(q[i],q[i],pk->vector_tmp[1]);
      /* round the constant coefficient */
      if (numintMPQ_sgn(q[0])==0){
	numintMPQ_mod(q[0],q[polka_cst],pk->vector_tmp[1]);
	if (numintMPQ_sgn(q[0])){
	  vector_clear(q,size);
	  numintMPQ_set_int(q[polka_cst],1);
	} else {
	  numintMPQ_divexact(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
	}
      }
      else {
	numintMPQ_fdiv_q(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
      }
    }
  }
  else {
    change = vector_normalize_constraint(pk,q,dim);
  }
  return change;
}

/* ********************************************************************** */
/* III. Comparison function */
/* ********************************************************************** */

/* Comparison function for vectors

The used order is the lexicographic order, with the exception that the
constant (and possibly epsilon) coefficient is considered last. As a
consequence, the equations or lines are classified before the
inequalities or rays when vectors are rows of a sorted matrix.

The meaning of the returned result res is:
- <0 : q1 is smaller than q2
- =0 : they are equal
- >0: q1 is greater than q2

This function uses pk->vector_tmp[0..3] and pk->vector_numintp.
*/

int vector_compare(pk_internal_t* pk,
		   numintMPQ_t* q1, numintMPQ_t* q2,
		   size_t size)
{
  size_t i;
  int res=1;

  assert(size<=pk->maxcols);

  /* bidirectional/unidirectional ? */
  res = numintMPQ_cmp(q1[0],q2[0]);
  if (res) return res;
  /* comparison */
  for(i=pk->dec; i<size; i++){
    res = numintMPQ_cmp(q1[i],q2[i]);
    if (res) return res;
  }
  if (polka_cst<size){
    res = numintMPQ_cmp(q1[polka_cst],q2[polka_cst]);
    if (res) return res;
    if (pk->strict && polka_eps < size){
      res = numintMPQ_cmp(q1[polka_eps],q2[polka_eps]);
    }
  }
  return res;
}

/* ********************************************************************** */
/* IV. Combine function */
/* ********************************************************************** */

/* vector_combine computes a combination q3 of q1 and
   q2 such that q3[k]=0.  The first coefficient is never
   considered for computations, except when k==0.

   This function uses pk->vector_tmp[0..4] and pk->vector_numintp. */

void vector_combine(pk_internal_t* pk,
		    numintMPQ_t* q1, numintMPQ_t* q2,
		    numintMPQ_t* q3, size_t k, size_t size)
{
  size_t j;
  numintMPQ_gcd(pk->vector_tmp[0],q1[k],q2[k]);
  numintMPQ_divexact(pk->vector_tmp[1],q1[k],pk->vector_tmp[0]);
  numintMPQ_divexact(pk->vector_tmp[2],q2[k],pk->vector_tmp[0]);
  for (j=1;j<size;j++){
    if (j!=k){
      numintMPQ_mul(pk->vector_tmp[3],pk->vector_tmp[2],q1[j]);
      numintMPQ_mul(pk->vector_tmp[4],pk->vector_tmp[1],q2[j]);
      numintMPQ_sub(q3[j],pk->vector_tmp[3],pk->vector_tmp[4]);
    }
  }
  numintMPQ_set_int(q3[k],0);
  vector_normalize(pk,q3,size);

  if (pk->max_coeff_size){
    for (j=0; j<size; j++){
      if (numintMPQ_size(q3[j]) > pk->max_coeff_size){
	pk->exn = AP_EXC_OVERFLOW;
      }
    }
  }
}

//* ********************************************************************** */
/* V. Algebraic operations */
/* ********************************************************************** */

/* Scalar product.

Compute the scalar product of q1 and q2 considered as vectors
of length size. The first coefficients are never considered.

This function uses pk->vector_tmp[0]. */

void vector_product(pk_internal_t* pk,
		    numintMPQ_t prod,
		    numintMPQ_t* q1, numintMPQ_t* q2, size_t size)
{
  size_t j;
  numintMPQ_set_int(prod,0);
  for (j=1; j<size; j++){
    numintMPQ_mul(pk->vector_tmp[0],q1[j],q2[j]);
    numintMPQ_add(prod,prod,pk->vector_tmp[0]);
  }
}

/* Same as previous function, but in case where pk->strict is
   true, the $\epsilon$ coefficients are not taken into account. */

void vector_product_strict(pk_internal_t* pk,
			   numintMPQ_t prod,
			   numintMPQ_t* q1, numintMPQ_t* q2, size_t size)
{
  size_t j;
  if (polka_cst<size){
    numintMPQ_mul(prod,q1[polka_cst],q2[polka_cst]);
  }
  else {
    numintMPQ_set_int(prod,0);
    return;
  }
  for (j=pk->dec; j<size; j++){
    numintMPQ_mul(pk->vector_tmp[0],q1[j],q2[j]);
    numintMPQ_add(prod,prod,pk->vector_tmp[0]);
  }
}

/* ********************************************************************** */
/* VI. Predicates */
/* ********************************************************************** */

/* The function tests if the given vector is null. */

bool vector_is_null(pk_internal_t* pk,
		    numintMPQ_t* q, size_t size)
{
  size_t i;
  bool res = true;
 
  for (i=1; i<size; i++){
    if (numintMPQ_sgn(q[i])!=0){
      res = false;
      break;
    }
  }
  return res;
}

/* The function tests if the given vector projected on the
   non-$\epsilon$ coefficients is null. */

bool vector_is_null_strict(pk_internal_t* pk,
			   numintMPQ_t* q, size_t size)
{
  size_t i;
  bool res = true;

  if (size>polka_cst){
    res = numintMPQ_sgn(q[polka_cst])==0;
    if (res){
      for (i=pk->dec; i<size; i++){
	if (numintMPQ_sgn(q[i])!=0){
	  res = false;
	  break;
	}
      }
    }
  }
  return res;
}

/* The function tests if the given vector represents a positivity
   constraint. */

bool vector_is_positivity_constraint(pk_internal_t* pk,
				     numintMPQ_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else {
    size_t i;
    bool res;

    res = numintMPQ_sgn(q[0])>0;
    if (res){
      int s = numintMPQ_sgn(q[polka_cst]);
      if (s>0){
	/* Tests if it could be the positivity constraint */
	res = pk->strict ? numintMPQ_sgn(q[polka_eps])<0 : true;
      }
      if (res){
	for (i=pk->dec; i<size; i++){
	  if (numintMPQ_sgn(q[i]) != 0){
	    res = false;
	    break;
	  }
	}
      }
    }
    return res;
  }
}

/* The function tests if the given vector represents a positivity
   or a strictness constraint. */

bool vector_is_dummy_constraint(pk_internal_t* pk,
				numintMPQ_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else {
    size_t i;
    bool res;

    res = numintMPQ_sgn(q[0])>0;
    if (res){
      int s = numintMPQ_sgn(q[polka_cst]);
      if (s>0){
	/* Tests if it could be the positivity constraint */
	res = pk->strict ? numintMPQ_sgn(q[polka_eps])<0 : true;
      }
      else if (s==0){
	/* Tests if it could be the strictness constraint */
	res = pk->strict && numintMPQ_sgn(q[polka_eps])>0;
      }
      if (res){
	for (i=pk->dec; i<size; i++){
	  if (numintMPQ_sgn(q[i]) != 0){
	    res = false;
	    break;
	  }
	}
      }
    }
    return res;
  }
}

/* The function tests if the given vector represents the strictness ray, or has a non-null epsilon component. */ 
bool vector_is_dummy_or_strict_generator(pk_internal_t* pk,
					 numintMPQ_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else if (!numintMPQ_sgn(q[0])) /* line */
    return false;
  else if (pk->strict && numintMPQ_sgn(q[polka_eps]))
    return true;
  else 
    return false;
}

/* Return true if all dimensions involved in the expression are integer
   dimensions */

bool vector_is_integer(pk_internal_t* pk, 
		       numintMPQ_t* vec,
		       ap_dimension_t dim)
{
  size_t i;
  
  for (i=dim.intd; i<dim.intd+dim.reald; i++){
    if (numintMPQ_sgn(vec[pk->dec+i]) != 0){
      return false;
    }
  }
  return true;
}

long vector_hash(pk_internal_t* pk,numintMPQ_t* vec, size_t size)
{
  long res,t;
  size_t i;

  lint_set_numintMPQ(&t,vec[polka_cst],pk->num);
  res = t;
  for (i=pk->dec; i<size; i += ((size-pk->dec)+2)/3){
    lint_set_numintMPQ(&t,vec[i],pk->num);
    res = res*3 + t;
  }
  return res;
}
