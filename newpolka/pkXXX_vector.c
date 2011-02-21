/* ********************************************************************** */
/* pkXXX_vector.c: operations on vectors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"

/* ********************************************************************** */
/* I. Basic operations: creation, destruction, copying and printing */
/* ********************************************************************** */

/* Internal allocation function: the elements are not initialized. */
numintXXX_t* _vectorXXX_alloc_int(size_t size){
  assert(size>0);
  return (numintXXX_t*)malloc(size*sizeof(numintXXX_t));
}

/* Standard allocation function, with initialization of the elements. */
numintXXX_t* vectorXXX_alloc(size_t size)
{
  size_t i;
  numintXXX_t* q;
  q = _vectorXXX_alloc_int(size);
  for(i=0; i<size; i++){
    numintXXX_init(q[i]);
  }
  return q;
}

/* Reallocation function, to change the dimension */
void vectorXXX_realloc(numintXXX_t** pq, size_t size, size_t nsize)
{
  size_t i;
  numintXXX_t* q;
  numintXXX_t* nq;

  q = *pq;
  for (i=nsize; i<size; i++){
    numintXXX_clear(q[i]);
  }
  nq = realloc(q, nsize*sizeof(numintXXX_t));
  for (i=size; i<nsize; i++){
    numintXXX_init(nq[i]);
  }
  *pq = nq;
}

/* Copy/Assign function */
void vectorXXX_copy(numintXXX_t* q2, numintXXX_t* q1, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    numintXXX_set(q2[i],q1[i]);
  }
}
/* Deallocation function. */
void vectorXXX_free(numintXXX_t* q, size_t size)
{
  size_t i;
  for(i=0; i<size; i++) numintXXX_clear(q[i]);
  free(q);
}

/* Set all elements to zero. */
void vectorXXX_clear(numintXXX_t* q, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numintXXX_set_int(q[i],0);
}

/* Raw printing function. */
void vectorXXX_print(numintXXX_t* q, size_t size)
{
  size_t i;
  printf("vector %ld: ", (long)size);
  for (i=0; i<size; i++){
    numintXXX_print(q[i]); printf(" ");
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
vectorXXX_min_notzero(pkXXX_internal_t* pk,
		      size_t size,
		      int* index, numintXXX_t min)
{
  size_t i;

  numintXXX_t* v = pk->vector_numintp;

  numintXXX_set_int(min,0);

  /* search the first non-zero coefficient
     and stores the index and the coeff in *index and *min */
  i = 0;
  while (i<size){
    if (numintXXX_sgn(v[i])){
      *index = i;
      numintXXX_set(min,v[i]);
      break;
    }
    i++;
  }
  i++;
  /* search now the minimum */
  while (i<size) {
    if (numintXXX_sgn(v[i])){
      if (numintXXX_cmp(min,v[i]) > 0){
	*index = i;
	numintXXX_set(min,v[i]);
      }
    }
    i++;
  }
}
/* This function computes the pgcd of a vector.

   This function uses pk->vector_numintp and pk->vector_tmp[0]. */

void vectorXXX_gcd(pkXXX_internal_t* pk,
		   numintXXX_t* q, size_t size, numintXXX_t gcd)
{
  size_t i;
  bool not_all_zero;
  numintXXX_t* v = pk->vector_numintp;

  for (i=0;i<size;i++)
    numintXXX_abs(v[i],q[i]);

  do {
    int index=0;
    vectorXXX_min_notzero(pk,size,&index,gcd);
    if (numintXXX_sgn(gcd)==0) break;
    not_all_zero = false;
    for (i=0; i<size; i++)
      if ((int)i!=index){
	numintXXX_mod(v[i],v[i],gcd);
	not_all_zero = not_all_zero || numintXXX_sgn(v[i]);
      }
  } while (not_all_zero);
}


/* ====================================================================== */
/* II.3 Main functions */
/* ====================================================================== */

/* The function vector_normalize normalizes the vector considered as
   a contraint or a generator. It does not modify q[0].

   This function use pk->vector_tmp[0..2] and pk->numintp. */

bool vectorXXX_normalize(pkXXX_internal_t* pk,
			 numintXXX_t* q, size_t size)
{
  size_t i;

  assert(size<=pk->maxcols);

  /*  computation of the pgcd */
  vectorXXX_gcd(pk,&q[1],size-1, pk->vector_tmp[1]);
  /* possible division */
  if (numintXXX_cmp_int(pk->vector_tmp[1],1)>0){
    for (i=1; i<size; i++)
      numintXXX_divexact(q[i],q[i],pk->vector_tmp[1]);
    return true;
  }
  else
    return false;
}

/* The function vector_normalize normalizes the vector considered as
   an expression. It modifies q[0].

   This function use pk->vector_tmp[0..2] and pk->numintp. */

bool vectorXXX_normalize_expr(pkXXX_internal_t* pk,
			      numintXXX_t* q, size_t size)
{
  size_t i;

  assert(size<=pk->maxcols);

  /*  computation of the pgcd */
  vectorXXX_gcd(pk,&q[0],size, pk->vector_tmp[1]);
  /* possible division */
  if (numintXXX_cmp_int(pk->vector_tmp[1],1)>0){
    for (i=0; i<size; i++)
      numintXXX_divexact(q[i],q[i],pk->vector_tmp[1]);
    return true;
  }
  else
    return false;
}

/* The function vector_normalize_constraint normalizes the vector considered as
   a constraint.

   - if strict mode, the epsilon coefficient is put to 0 or 1

   This function use pk->vector_tmp[0..1] and pk->numintp. */

bool vectorXXX_normalize_constraint(pkXXX_internal_t* pk,
				    numintXXX_t* q,
				    ap_dimension_t dim)
{
  size_t i;
  bool change = false;
  size_t size = pk->dec+dim.intd+dim.reald;

  assert(pk->dec+dim.intd+dim.reald <= pk->maxcols);

  if (pk->strict && numintXXX_sgn(q[0])!=0 && numintXXX_sgn(q[polka_eps])<0){
    /*  computation of the pgcd without epsilon */
    change = (numintXXX_cmp_int(q[polka_eps],-1) != 0);
    numintXXX_set_int(q[polka_eps],0);
    vectorXXX_gcd(pk,&q[1], size-1, pk->vector_tmp[1]);
    numintXXX_set_int(q[polka_eps],-1);
    /* possible division */
    if (numintXXX_cmp_int(pk->vector_tmp[1],1)>0){
      change = true;
      numintXXX_divexact(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
      for (i=pk->dec; i<size; i++)
	numintXXX_divexact(q[i],q[i],pk->vector_tmp[1]);
    }
  }
  else {
    vectorXXX_normalize(pk,q,size);
  }
  return change;
}

/* The function vector_normalize_constraint_int normalizes the vector
   considered as a constraint.

   - if it involves only integer dimensions, the constraint is tightened and
   renormalized.

   - it implies standard constraint normalization

   This function use pk->vector_tmp[0..1] and pk->numintp. */

bool vectorXXX_normalize_constraint_int(pkXXX_internal_t* pk,
					numintXXX_t* q,
					ap_dimension_t dim)
{
  size_t i;
  bool change = false;
  size_t size = pk->dec+dim.intd+dim.reald;

  assert(pk->dec+dim.intd+dim.reald <= pk->maxcols);

  if (dim.intd>0 &&
      vectorXXX_is_integer(pk,q,dim) &&
      !vectorXXX_is_positivity_constraint(pk,q,size)){
    if (pk->strict && numintXXX_sgn(q[polka_eps])<0){
      change = true;
      numintXXX_set_int(q[polka_eps],0);
      numintXXX_sub_uint(q[polka_cst], q[polka_cst], 1);
    }
    /*  computation of the pgcd without constant (and epsilon, of course) */
    vectorXXX_gcd(pk, &q[pk->dec], size-pk->dec, pk->vector_tmp[1]);
    /* possible division */
    if (numintXXX_cmp_int(pk->vector_tmp[1],1)>0){
      change = true;
      for (i=pk->dec; i<size; i++)
	numintXXX_divexact(q[i],q[i],pk->vector_tmp[1]);
      /* round the constant coefficient */
      if (numintXXX_sgn(q[0])==0){
	numintXXX_mod(q[0],q[polka_cst],pk->vector_tmp[1]);
	if (numintXXX_sgn(q[0])){
	  vectorXXX_clear(q,size);
	  numintXXX_set_int(q[polka_cst],1);
	} else {
	  numintXXX_divexact(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
	}
      }
      else {
	numintXXX_fdiv_q(q[polka_cst],q[polka_cst],pk->vector_tmp[1]);
      }
    }
  }
  else {
    change = vectorXXX_normalize_constraint(pk,q,dim);
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

int vectorXXX_compare(pkXXX_internal_t* pk,
		      numintXXX_t* q1, numintXXX_t* q2,
		      size_t size)
{
  size_t i;
  int res=1;

  assert(size<=pk->maxcols);

  /* bidirectional/unidirectional ? */
  res = numintXXX_cmp(q1[0],q2[0]);
  if (res) return res;
  /* comparison */
  for(i=pk->dec; i<size; i++){
    res = numintXXX_cmp(q1[i],q2[i]);
    if (res) return res;
  }
  if (polka_cst<size){
    res = numintXXX_cmp(q1[polka_cst],q2[polka_cst]);
    if (res) return res;
    if (pk->strict && polka_eps < size){
      res = numintXXX_cmp(q1[polka_eps],q2[polka_eps]);
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

void vectorXXX_combine(pkXXX_internal_t* pk,
		       numintXXX_t* q1, numintXXX_t* q2,
		       numintXXX_t* q3, size_t k, size_t size)
{
  size_t j;
  numintXXX_gcd(pk->vector_tmp[0],q1[k],q2[k]);
  numintXXX_divexact(pk->vector_tmp[1],q1[k],pk->vector_tmp[0]);
  numintXXX_divexact(pk->vector_tmp[2],q2[k],pk->vector_tmp[0]);
  for (j=1;j<size;j++){
    if (j!=k){
      numintXXX_mul(pk->vector_tmp[3],pk->vector_tmp[2],q1[j]);
      numintXXX_mul(pk->vector_tmp[4],pk->vector_tmp[1],q2[j]);
      numintXXX_sub(q3[j],pk->vector_tmp[3],pk->vector_tmp[4]);
    }
  }
  numintXXX_set_int(q3[k],0);
  vectorXXX_normalize(pk,q3,size);

  if (pk->max_coeff_size){
    for (j=0; j<size; j++){
      if (numintXXX_size(q3[j]) > pk->max_coeff_size){
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

void vectorXXX_product(pkXXX_internal_t* pk,
		       numintXXX_t prod,
		       numintXXX_t* q1, numintXXX_t* q2, size_t size)
{
  size_t j;
  numintXXX_set_int(prod,0);
  for (j=1; j<size; j++){
    numintXXX_mul(pk->vector_tmp[0],q1[j],q2[j]);
    numintXXX_add(prod,prod,pk->vector_tmp[0]);
  }
}

/* Same as previous function, but in case where pk->strict is
   true, the $\epsilon$ coefficients are not taken into account. */

void vectorXXX_product_strict(pkXXX_internal_t* pk,
			      numintXXX_t prod,
			      numintXXX_t* q1, numintXXX_t* q2, size_t size)
{
  size_t j;
  if (polka_cst<size){
    numintXXX_mul(prod,q1[polka_cst],q2[polka_cst]);
  }
  else {
    numintXXX_set_int(prod,0);
    return;
  }
  for (j=pk->dec; j<size; j++){
    numintXXX_mul(pk->vector_tmp[0],q1[j],q2[j]);
    numintXXX_add(prod,prod,pk->vector_tmp[0]);
  }
}

/* ********************************************************************** */
/* VI. Predicates */
/* ********************************************************************** */

/* The function tests if the given vector is null. */

bool vectorXXX_is_null(pkXXX_internal_t* pk,
		       numintXXX_t* q, size_t size)
{
  size_t i;
  bool res = true;

  for (i=1; i<size; i++){
    if (numintXXX_sgn(q[i])!=0){
      res = false;
      break;
    }
  }
  return res;
}

/* The function tests if the given vector projected on the
   non-$\epsilon$ coefficients is null. */

bool vectorXXX_is_null_strict(pkXXX_internal_t* pk,
			      numintXXX_t* q, size_t size)
{
  size_t i;
  bool res = true;

  if (size>polka_cst){
    res = numintXXX_sgn(q[polka_cst])==0;
    if (res){
      for (i=pk->dec; i<size; i++){
	if (numintXXX_sgn(q[i])!=0){
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

bool vectorXXX_is_positivity_constraint(pkXXX_internal_t* pk,
					numintXXX_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else {
    size_t i;
    bool res;

    res = numintXXX_sgn(q[0])>0;
    if (res){
      int s = numintXXX_sgn(q[polka_cst]);
      if (s>0){
	/* Tests if it could be the positivity constraint */
	res = pk->strict ? numintXXX_sgn(q[polka_eps])<0 : true;
      }
      if (res){
	for (i=pk->dec; i<size; i++){
	  if (numintXXX_sgn(q[i]) != 0){
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

bool vectorXXX_is_dummy_constraint(pkXXX_internal_t* pk,
				   numintXXX_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else {
    size_t i;
    bool res;

    res = numintXXX_sgn(q[0])>0;
    if (res){
      int s = numintXXX_sgn(q[polka_cst]);
      if (s>0){
	/* Tests if it could be the positivity constraint */
	res = pk->strict ? numintXXX_sgn(q[polka_eps])<0 : true;
      }
      else if (s==0){
	/* Tests if it could be the strictness constraint */
	res = pk->strict && numintXXX_sgn(q[polka_eps])>0;
      }
      if (res){
	for (i=pk->dec; i<size; i++){
	  if (numintXXX_sgn(q[i]) != 0){
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
bool vectorXXX_is_dummy_or_strict_generator(pkXXX_internal_t* pk,
					    numintXXX_t* q, size_t size)
{
  if (size < pk->dec){
    return false;
  }
  else if (!numintXXX_sgn(q[0])) /* line */
    return false;
  else if (pk->strict && numintXXX_sgn(q[polka_eps]))
    return true;
  else
    return false;
}

/* Return true if all dimensions involved in the expression are integer
   dimensions */

bool vectorXXX_is_integer(pkXXX_internal_t* pk,
			  numintXXX_t* vec,
			  ap_dimension_t dim)
{
  size_t i;

  for (i=dim.intd; i<dim.intd+dim.reald; i++){
    if (numintXXX_sgn(vec[pk->dec+i]) != 0){
      return false;
    }
  }
  return true;
}

long vectorXXX_hash(pkXXX_internal_t* pk,numintXXX_t* vec, size_t size)
{
  long res,t;
  size_t i;

  lint_set_numintXXX(&t,vec[polka_cst],pk->num);
  res = t;
  for (i=pk->dec; i<size; i += ((size-pk->dec)+2)/3){
    lint_set_numintXXX(&t,vec[i],pk->num);
    res = res*3 + t;
  }
  return res;
}
