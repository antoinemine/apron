/* ********************************************************************** */
/* pk_cherni.c: Conversion from one representation to the dual one.  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_bit.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk_cherni.h"

/* ********************************************************************** */
/* I. Checking function */
/* ********************************************************************** */

/* These functions perform coherence checks about minimized representation of
   polyhedra.

   cherni_checksatmat recomputes the saturation matrix of C and
   F and checks the equality with satC. */

bool cherni_checksatmat(pk_internal_t* pk,
			bool con_to_ray,
			pk_matrix_t* C, pk_matrix_t* F, satmat_t* satC)
{
  int s1,s2;
  size_t i;
  bitindex_t j;

  for (i=0; i<F->nbrows; i++){
    for (j = bitindex_init(0); j.index  < C->nbrows; bitindex_inc(&j)){
      vector_product(pk,
		     pk->cherni_prod,
		     F->p[i],
		     C->p[j.index],F->nbcolumns);
      s1 = numint_sgn(pk->cherni_prod);
      s2 = satmat_get(satC,i,j);
      if (s1<0 || (s1!=0 && s2==0) || (s1==0 && s2!=0)){
	printf("cherni_checksatmat con_to_ray=%d: ray %lu, con %lu\n",
	       con_to_ray,(unsigned long)i,(unsigned long)j.index);
	printf("Constraints\n"); pk_matrix_print(C);
	printf("Frames\n"); pk_matrix_print(F);
	satmat_print(satC);
	return false;
      }
    }
  }
  return true;
}

/* This function checks the saturation numbers of constraints and rays. */

bool cherni_checksat(pk_internal_t* pk,
		       bool con_to_ray,
		       pk_matrix_t* C, size_t nbequations,
		       pk_matrix_t* F, size_t nblines,
		       satmat_t* satC)
{
  size_t i,k,nb,rank;
  bitindex_t j;
  pk_matrix_t* mat;

  bool res = true;
  const size_t nbcols = C->nbcolumns;

  /* saturation des rayons */
  mat = pk_matrix_alloc(C->nbrows,nbcols,false);
  for (i=0; i<F->nbrows; i++){
    nb = 0;
    for (j = bitindex_init(0); j.index < C->nbrows; bitindex_inc(&j)){
      if (satmat_get(satC,i,j)==0){
	for (k=0;k<nbcols;k++){
	  numint_set(mat->p[nb][k],C->p[j.index][k]);
	}
	nb++;
      }
    }
    rank = cherni_gauss(pk,mat,nb);
    if (!( (numint_sgn(F->p[i][0])==0 && nb == C->nbrows)
	   || (rank==nbcols-2-nblines && nb >= rank))){
      printf("cherni_checksat con_to_ray=%d: wrong ray %lu; expected = %lu, effective = (%lu<=%lu)\n",
	     con_to_ray,(unsigned long)i,(unsigned long)(nbcols-2-nblines),
	     (unsigned long)rank,(unsigned long)nb);
      res = false;
    }
  }
  pk_matrix_free(mat);

  /* saturation des contraintes */
  mat = pk_matrix_alloc(F->nbrows,nbcols,false);
  for (j = bitindex_init(0); j.index < C->nbrows; bitindex_inc(&j)){
    nb = 0;
    for (i=0; i<F->nbrows; i++){
      if (satmat_get(satC,i,j)==0){
	for (k=0;k<nbcols;k++){
	  numint_set(mat->p[nb][k],F->p[i][k]);
	}
	nb++;
      }
    }
    rank = cherni_gauss(pk,mat,nb);
    if (!( (numint_sgn(C->p[j.index][0])==0 && nb == F->nbrows)
	   || (rank==nbcols-2-nbequations && nb >= rank))){
      printf("cherni_checksat con_to_ray=%d: wrong con %lu; expected = %lu, effective = (%lu<=%lu)\n",
	     con_to_ray, (unsigned long)j.index, (unsigned long)(nbcols-2-nbequations), (unsigned long)rank,(unsigned long)nb);
      res = false;
    }
  }
  pk_matrix_free(mat);

  if (res==false){
    printf("Constraints\n"); pk_matrix_print(C);
    printf("Frames\n"); pk_matrix_print(F);
    satmat_print(satC);
  }
  return res;
}

/* ********************************************************************** */
/* II. Conversion algorithm */
/* ********************************************************************** */

void cherni_resize(pk_matrix_t* mat, satmat_t* sat)
{
  assert(mat->nbrows==sat->nbrows);
  size_t nbrows = mat->nbrows;
  size_t currentsize = mat->_maxrows >= sat->_maxrows ? mat->_maxrows : sat->_maxrows;
  size_t addsize = currentsize < 20 ? 10 : currentsize / 2;
  pk_matrix_resize_rows(mat, currentsize+addsize);
  satmat_resize_rows(sat, currentsize+addsize);
  mat->nbrows = sat->nbrows = nbrows;
  return;
}

/*
- con is the constraints matrix,
- start indicates the number of constraints supposed to be already taken in
  account,
- ray is the frames matrix, with the start first constraints taken in account,
- satc is the saturation matrix initialized according to con and ray,
  row i of satc refer to ray i, column k refer to constraint k.
- nbline indicates the number of lines in ray.

One suppose that the saturation matrix has a
maximum size of ray->polka_maxrows x bitindex_size(con->nbrows).
For the saturation matrix, we suppose that Unused bits of existing rows
(from 0 to start) are set to zero. Each time a row is
added, this hypothese must remain true.

The saturation matrix is organised as follows:
- the rows are indexed by generators,
- the columns by constraints.

The result is given by ray, sat and pnbline. The
status coefficient of rows of ray are set properly at the end of
the function.

Throw exception.
*/

size_t cherni_conversion(pk_internal_t* pk,
			 pk_matrix_t* con, size_t start,
			 pk_matrix_t* ray, satmat_t* satc, size_t nbline)
{
  size_t i,j,l,w;
  int is_inequality;
  size_t index_non_zero;
  size_t equal_bound,sup_bound,inf_bound,bound;
  int nbcommonconstraints;
  bitindex_t k;
  bitstring_t m,aux;
  bool redundant;
  bitstring_t* bitstringp;

  const size_t nbcols = con->nbcolumns;
  const size_t satnbcols = bitindex_size(con->nbrows);
  size_t nbrows = ray->nbrows;

  bitstringp = bitstring_alloc(satnbcols);

  /* ================= Code ================== */
  k = bitindex_init(start);
  while (k.index < con->nbrows){
    /* Iteration sur les contraintes */
    is_inequality = numint_sgn(con->p[k.index][0]);

    /* Scalar product and index: */
    /*
      We compute for the new considered constraint its scalar products
      with each frame and put them into the first coefficient of the
      frames. Moreover we set index_non_zero to the index of the
      first frame that does not saturate the constraint.
    */

    index_non_zero = nbrows;
    for (i=0; i<nbrows; i++){
      vector_product(pk,ray->p[i][0],
		     ray->p[i],
		     con->p[k.index],nbcols);
      if (index_non_zero == nbrows && numint_sgn(ray->p[i][0])!=0){
	index_non_zero = i;
      }
    }

    if (index_non_zero < nbline){
      /* If a line doesn't satisfy the constraint */
      /* Line becomes ray:
	 We transform the first line that does not saturate the constraint
	 into a ray and computes a new pointed cone. */

      /* remove it of lines and put it at index nbline */
      nbline--;
      if (index_non_zero != nbline)
	pk_matrix_exch_rows(ray,index_non_zero,nbline);
      /* compute new lineality space */
      for (i=index_non_zero; i<nbline; i++)
	if (numint_sgn(ray->p[i][0]) != 0){
	  pk_matrix_combine_rows(pk,ray,i,nbline,i,0);
	  if (pk->exn) goto cherni_conversion_exit0;
	}

      /* orient the new ray */
      if (numint_sgn(ray->p[nbline][0]) < 0)
	for (j=0; j<nbcols; j++)
	  numint_neg(ray->p[nbline][j],ray->p[nbline][j]);

      /* compute the new pointed cone */
      for (i=nbline+1; i<nbrows; i++)
	if (numint_sgn(ray->p[i][0])){
	  pk_matrix_combine_rows(pk,ray,i,nbline,i,0);
	  if (pk->exn) goto cherni_conversion_exit0;
	}
      
      /* For the saturation matrix, we only add a column, */
      /* so new bits are initialized to zero (see above) */
      if (is_inequality){
	/* rays saturates all apart the last inequality */
	satmat_set(satc,nbline,k);
      } else {
	/* one remove the ray */
	nbrows --; ray->nbrows --; satc->nbrows--;
	pk_matrix_exch_rows(ray, nbline, nbrows);
	satmat_exch_rows(satc, nbline, nbrows);
      }

      bitindex_inc(&k);
    }

    else {
      /* if all lines saturates the constraint */
      /* Sort the rays: */
      /*
	Rays are sorted as follows:
	- nbline<= i < equal_bound:
	saturate the constraint;
	- equal_bound <= i < sup_bound:
	verify it;
	- sup_bound <= i < nbrows:
	do not verify it.
      */

      equal_bound=nbline;
      sup_bound=nbline;
      inf_bound=nbrows;
      while (inf_bound>sup_bound) {
	int s = numint_sgn(ray->p[sup_bound][0]);
	if (s==0){
	  pk_matrix_exch_rows(ray, sup_bound, equal_bound);
	  satmat_exch_rows(satc, sup_bound, equal_bound);
	  equal_bound++;
	  sup_bound++;
	} else if (s<0) {
	  inf_bound--;
	  pk_matrix_exch_rows(ray, sup_bound, inf_bound);
	  satmat_exch_rows(satc, sup_bound, inf_bound);
	} else {
	  sup_bound++;
	}
      }
      if (is_inequality && sup_bound == nbrows){
	/* all rays satisfy the constraint:redundancy */
	con->nbrows--;
	pk_matrix_exch_rows(con, k.index, con->nbrows);
      }
      else {
	if (sup_bound==nbline){ /* no ray satisfies the constraint */
	  nbrows = ray->nbrows = satc->nbrows = nbline;
	}
	else { /* some rays do not satisfy the constraint */
	  /* Compute the new cones by combining adjacent constraints: */
	  bound = nbrows;
	  for (i=equal_bound; i<sup_bound; i++){
	    for(j=sup_bound; j<bound; j++){
	      /* For each pair R+,R-, */
	      /* compute the set of constraints saturated by both of them,
		 including equalities */
	      nbcommonconstraints=0;
	      for (w=0; w<k.word; w++) {
		aux = satc->p[i][w] | satc->p[j][w];
		bitstringp[w] = aux;
		for (m=bitstring_msb; m!=0; m>>=1)
		  if ((aux & m)==0) nbcommonconstraints++;
	      }
	      aux = satc->p[i][k.word] | satc->p[j][k.word];
	      bitstringp[k.word] = aux;
	      for (m=bitstring_msb; m!=k.bit; m>>=1){
		if ((aux & m)==0) nbcommonconstraints++;
	      }
	      if (nbcommonconstraints+nbline>=nbcols-3){ /* possibly adjacent */
		/* Does exist another ray saturating the same constraints ? */
		redundant=false;
		for (l=nbline; l<bound; l++){
		  if ((l!=i)&&(l!=j)){
		    for (w=0; w<=k.word; w++){
		      if (satc->p[l][w] & ~(bitstringp[w]))
			break;
		    }
		    if (w>k.word){
		      redundant=true; break;
		    }
		  }
		}
		if (!redundant){ /* if not */
		  if (pk->funopt->max_object_size && nbrows * (nbcols - pk->dec) >  pk->funopt->max_object_size){
		    /* out of space overflow */
		    pk->exn = AP_EXC_OUT_OF_SPACE;
		    goto cherni_conversion_exit0;
		  }
		  if (nbrows>=pk_matrix_get_maxrows(ray) || nbrows>=satc->_maxrows){
		    /* resize output matrices */
		    cherni_resize(ray,satc);
		  }
		  /* Compute the new ray and put it at end */
		  pk_matrix_combine_rows(pk,ray,j,i,nbrows,0);
		  if (pk->exn) goto cherni_conversion_exit0;
		  /* New row in saturation matrix */
		  for (w=0; w<=k.word; w++){
		    satc->p[nbrows][w] = bitstringp[w];
		  }
		  for (w=k.word+1; w<satnbcols; w++){
		    satc->p[nbrows][w] = 0;
		  }
		  nbrows ++; ray->nbrows ++; satc->nbrows ++;
		}
	      }
	    }
	  }
	  /* Remove non extremal rays by exchanging added rays */
	  /* with those that don't verify the constraint */
	  {
	    size_t l;
	    if (is_inequality){
	      j = sup_bound;
	      for (l=equal_bound; l<sup_bound; l++) satmat_set(satc,l,k);
	    }
	    else {
	      j = equal_bound;
	    }
	    i = nbrows;
	    while ((j<bound)&&(i>bound)) {
	      i--;
	      pk_matrix_exch_rows(ray,i,j);
	      satmat_exch_rows(satc,i,j);
	      j++;
	    }
	    nbrows = (j==bound) ? i : j;
	    ray->nbrows = satc->nbrows = nbrows;
	  }
	}
	bitindex_inc(&k);
      }
    }
  }

  /* status coefficient */
  for (i=0; i<nbline; i++){
    numint_set_int(ray->p[i][0],0);
  }
  for (i = nbline; i<nbrows; i++){
    numint_set_int(ray->p[i][0],1);
  }
  ray->nbrows = satc->nbrows = nbrows;
  bitstring_free(bitstringp);
  return nbline;

 cherni_conversion_exit0:
  bitstring_free(bitstringp);
  return 0;
}

/* ********************************************************************** */
/* III. Simplification of origin representation */
/* ********************************************************************** */

/* ====================================================================== */
/* III.1 Gauss pivot */
/* ====================================================================== */

/*
The function finds a minimal system for equalities, and returns the rank $r$ of
this system, equations of which are numbered from $0$ to $r-1$. Redundant (now
null) equations are put between row $r$ and row $nbeq$.  The function also
fills the array pk->cherni_intp which indicates the ``main'' coefficent
of an equation, in this case the left-most non-zero one.
*/

size_t cherni_gauss(pk_internal_t* pk,
		    pk_matrix_t* con, size_t nbeq)
{
  size_t i,j,k;
  numint_t** p = con->p;
  size_t rank = 0;
  int s = 0;
  for (j = con->nbcolumns-1; j>=1; j--){
    /* find the first i such that p[i][j] is non zero */
    for (i=rank; i<nbeq; i++) {
      s = numint_sgn(p[i][j]);
      if (s) break;
    }
    if (i<nbeq) { /* was one found ? */
      if (i>rank) { /* put it in rank */
	pk_matrix_exch_rows(con,i,rank);
      }
      if (s<0) {  /* normalize with positive coefficient */
	for (k=1; k<con->nbcolumns; k++)
	  numint_neg(p[rank][k],p[rank][k]);
      }
      numint_set_int(p[rank][0],0);
      for (k=i+1; k<nbeq; k++) {
	if (numint_sgn(p[k][j]))
	  pk_matrix_combine_rows(pk,con,k,rank,k,j);
      }
      pk->cherni_intp[rank] = j;
      rank++;
    }
  }
  return rank;
}

/* ====================================================================== */
/* III.2 Backsubstitution */
/* ====================================================================== */

/* This function backsubstitute the coefficients according to the system of
   equations and the array pk->cherni_intp properly set by
   gauss. */


void cherni_backsubstitute(pk_internal_t* pk, pk_matrix_t* con, size_t rank)
{
  size_t i,j;
  int k;

  for (k=(int)rank-1; k>=0; k--) {
    j = pk->cherni_intp[k];
    for (i=0; i<con->nbrows; i++) {
      if (i != (size_t)k && numint_sgn(con->p[i][j]))
	pk_matrix_combine_rows(pk,con,i,(size_t)k,i,j);
    }
  }
}

/* ====================================================================== */
/* III.3 Simplifying constraints (main function) */
/* ====================================================================== */

/*
We suppose that we just obtained ray and satc from con
with the chernikova algorithms. As a consequence the system of rays is
minimal. satf is the transposed matrix of satc, i.e. rows are
indexed by constraints. con is supposed to be non empty.

We have still to simplify con by detecting new equality constraints,
removing redundant inequalities, and obtaining a minimal system of
equalities. This is performed by gauss elimination.

Throw exception.
*/

int cherni_simplify(pk_internal_t* pk,
		    pk_matrix_t* con, pk_matrix_t* ray, satmat_t* satf, size_t nbline)
{
  size_t i,j;
  long int nb,nbj;
  size_t nbeq,rank;
  size_t w;
  bitstring_t m;

  bool redundant, is_equality;

  const size_t nbcols = con->nbcolumns;
  bitindex_t nbrays = bitindex_init(ray->nbrows);
  size_t nbcons = con->nbrows;

  con->_sorted = false;

  /* find the first inequality */
  for (nbeq=0; nbeq < nbcons; nbeq ++){
    if (numint_sgn(con->p[nbeq][0])) break;
  }

  /* For each constraint,
     - put it with equalities if it detected as one
     - or set the status word to the number of rays that saturates the constraint,
  */
  for (i=nbeq; i < nbcons; i++){
    is_equality = (numint_sgn(con->p[i][0])==0);
    if (!is_equality){
      is_equality = true;
      for (w=0; w<satf->nbcolumns; w++){
	if (satf->p[i][w]){
	  is_equality = false; break;
	}
      }
    }
    if (is_equality){
      /* we have an equality */
      numint_set_int(con->p[i][0],0);
      pk_matrix_exch_rows(con, i,nbeq);
      satmat_exch_rows(satf,i,nbeq);
      nbeq++;
    }
    else {
      /* we count the number of zero bits */
      nb = 0;
      for (w=0; w<nbrays.word; w++){
	for (m=bitstring_msb; m!=0; m>>=1)
	  if ((satf->p[i][w] & m)==0) nb++;
      }
      for (m=bitstring_msb; m!=nbrays.bit; m>>=1)
	if ((satf->p[i][nbrays.word] & m)==0) nb++;
      numint_set_int(con->p[i][0],(int)nb);
    }
  }
  /* remove redundant equalities and update nbeq */
  rank = cherni_gauss(pk,con, nbeq); /* gauss pivot to simplify equalities */
  pk->exn = AP_EXC_NONE;

  /* remove redundants equations, located between rank and nbeq */
  if (rank<nbeq) {
    i = nbcons;
    j = rank;
    while( j < nbeq && i > nbeq ) {
      i--;
      pk_matrix_exch_rows(con, j,i);
      satmat_exch_rows(satf,j,i);
      j++;
    }
    nbcons += rank - nbeq;
    nbeq = rank;
  }
  /* remove trivially redundants inequalities (nb < nbcols-nbeq-2) */
  i = nbeq;
  while (i < nbcons){
    int_set_numint(&nb, con->p[i][0]);
    if (nb < (long int)(nbcols-nbeq-2)){ /* redundant constraint */
      nbcons--;
      pk_matrix_exch_rows(con, i,nbcons);
      satmat_exch_rows(satf,i,nbcons);
    }
    else
      i++;
  }
  /* remove others redundants inequalities */
  i=nbeq;
  while (i < nbcons){
    int_set_numint(&nb,con->p[i][0]);
    redundant = false;
    j = nbeq;
    while (j < nbcons){
      int_set_numint(&nbj,con->p[j][0]);
      if (nbj > nb){
	/* does j saturates a strictly overset ? */
	redundant = true;
	for (w=0; w<satf->nbcolumns; w++){
	  if( ~(satf->p[i][w]) & satf->p[j][w] ){
	    redundant = false; break;
	  }
	}
	if (redundant)
	  break;
	else
	  j++;
      }
      else if (nbj == nb && j != i){
	  /* is j mutually redundant with i ? */
	  is_equality = true;
	  for (w=0; w<satf->nbcolumns; w++){
	    if( satf->p[i][w] != satf->p[j][w] ){
	      is_equality = false; break;
	    }
	  }
	  if (is_equality){
	    /* yes: we can remove j */
	    nbcons--;
	    pk_matrix_exch_rows(con, j,nbcons);
	    satmat_exch_rows(satf,j,nbcons);
	  }
	  else
	    j++;
      }
      else
	j++;
    }
    if (redundant){
      nbcons--;
      pk_matrix_exch_rows(con, i,nbcons);
      satmat_exch_rows(satf,i,nbcons);
    }
    else
      i++;
  }
  /* setting status coefficient */
  for (i=nbeq; i<nbcons; i++){
    numint_set_int(con->p[i][0],1);
  }
  con->nbrows = satf->nbrows = nbcons;

  /* back substitution of remaining constraints */
  cherni_backsubstitute(pk, con, nbeq);
  pk->exn = AP_EXC_NONE;

  return nbeq;
}

/* ********************************************************************** */
/* IV. Minimization */
/* ********************************************************************** */

/* These functions have mainly a side-effect semantics */
/* They Throw exception. */

/* In all the following functions, for the structure poly_t:

   - If con_to_ray is true, standard meaning of everything
   - If con_to_ray is false,
     - matrices C and F, satC and satF have been exchanged,
     - nbeq and nbline have been exchanged
     - in status, nothing has changed (status is not used at this level)
*/

/* ====================================================================== */
/* IV.1 Standard minimization */
/* ====================================================================== */

/* po is a polyhedron with a non-empty constraint system, and empty frame and
   saturation matrices. */

#define CHERNI_FACTOR 2

static inline size_t uint_max(size_t a, size_t b)
{ return (a>b ? a : b); }

void cherni_minimize(pk_internal_t* pk,
		     bool con_to_ray,
		     pk_t* po)
{
  size_t i;
  bool special;
  pk_matrix_t* C;
  pk_matrix_t* F;
  satmat_t* satC;

  C = po->C;

  assert(pk_matrix_is_sorted(C) && 
	 po->F==NULL && po->satC==NULL && po->satF==NULL);

  /* initialization of F and sat */
  F = pk_matrix_alloc(CHERNI_FACTOR*uint_max(C->nbrows, C->nbcolumns-1),
		   C->nbcolumns,false);
  satC = satmat_alloc(F->nbrows, bitindex_size(C->nbrows));
  F->nbrows = satC->nbrows = C->nbcolumns-1;
  for (i=0; i<C->nbcolumns-1; i++){
    numint_set_int(F->p[i][i+1],1);
  }
  /* conversion */
  pk->exn = AP_EXC_NONE;
  po->nbline = cherni_conversion(pk, C, 0, F, satC, C->nbcolumns-1);

  if (pk->exn){
    /* out of space, overflow */
    pk_matrix_free(F);
    satmat_free(satC);
    po->nbeq = po->nbline = 0;
  }
  else {  
    /* If con_to_ray, special case ? */
    /* We have to test
       - In non-strict mode, that $\xi$ can be strictly positive.
       - In strict mode, that both $\xi$ and $\epsilon$ can be strictly
       positive. Because $\xi\geq\epsilon$, we just need to check that
       $\epslion$ can be strictly positive. */

    if (con_to_ray){
      special = true;
      for (i = po->nbline; i<F->nbrows; i++){
	if (numint_sgn(F->p[i][pk->dec-1])>0){
	  special = false;
	  break;
	}
      }
      if (special){
	/* this means we have an empty polyhedron */
	pk_matrix_free(C);
	pk_matrix_free(F);
	satmat_free(satC);
	po->C = 0;
	po->nbeq = po->nbline = 0;
	return;
      }
    }
    po->F = F;
    po->satF = satmat_transpose(satC,C->nbrows);
    satmat_free(satC);      
    po->nbeq = cherni_simplify(pk,C,F,po->satF,po->nbline);
    if (F->_maxrows > 3*F->nbrows/2){
      pk_matrix_resize_rows(F,F->nbrows);
      satmat_resize_cols(po->satF,bitindex_size(F->nbrows));
    }
  } 
}

/* ====================================================================== */
/* IV.2 Adding of a matrix of constraints to a minimal representation */
/* ====================================================================== */

/* Same as previous function, but this time we assume that po is an already
   minimized polyhedron, with the start first constraints taken into
   account. The function takes into account the constraints numbered from start
   and returns with a minimized polyhedron. */
   
void cherni_add_and_minimize(pk_internal_t* pk, 
			     bool con_to_ray,
			     pk_t* po,
			     size_t start)
{
  size_t i;
  bool special;
  pk_matrix_t* C;
  pk_matrix_t* F;
  satmat_t* satC;
  size_t nbrows,nbcols;

  assert(bitindex_size(po->C->nbrows)==po->satC->nbcolumns);

  C = po->C;
  F = po->F;
  satC = po->satC;
  nbrows = C->nbrows;
  nbcols = C->nbcolumns;

  assert(C!=NULL && F!=NULL && satC!=NULL);
  
  if (po->satF!=NULL){ 
    satmat_free(po->satF); 
    po->satF=NULL; 
  }

  /* saturation matrix */
  satmat_resize_cols(satC, bitindex_size(C->nbrows));
  /* conversion */
  F->_sorted = false;
  pk->exn = AP_EXC_NONE;
  po->nbline = cherni_conversion(pk,
				 C,start,F,satC,po->nbline);
  if (pk->exn){
    /* out of space, overflow */
    pk_matrix_free(F);
    satmat_free(satC);
    po->F = 0;
    po->satC = po->satF = 0;
    po->nbeq = po->nbline = 0;
  }
  else {  
    /* If con_to_ray, special case ? */
    /* We have to test
       - In non-strict mode, that $\xi$ can be strictly positive.
       - In strict mode, that both $\xi$ and $\epsilon$ can be strictly
       positive. Because $\xi\geq\epsilon$, we just need to check that
       $\epsilon$ can be strictly positive. */
    if (con_to_ray){ 
      special = true;
      for (i = po->nbline; i<F->nbrows; i++){
	if (numint_sgn(F->p[i][pk->dec-1])>0){
	  special = false;
	  break;
	}
      }
      if (special){ /* this means we have an empty polyhedron */
	pk_matrix_free(C);
	pk_matrix_free(F);
	satmat_free(satC);
	po->C = po->F = 0;
	po->satC = 0;
	po->nbeq = po->nbline = 0;
	return;
      }
    }
    po->satF = satmat_transpose(satC,C->nbrows);
    satmat_free(satC);
    po->satC = NULL;
    po->nbeq = cherni_simplify(pk,C,F,po->satF,po->nbline);
    if (F->_maxrows > 3*F->nbrows/2){
      pk_matrix_resize_rows(F,F->nbrows);
      satmat_resize_cols(po->satF,bitindex_size(F->nbrows));
    } 
  }
}

/* ********************************************************************** */
/*  V. Computation of saturation matrix */
/* ********************************************************************** */

/* Bits of satline are index by the constraints. satline are
   supoosed to be filled with zeros. */


void cherni_buildsatline(pk_internal_t* pk,
			 pk_matrix_t* con, numint_t* tab,
			 bitstring_t* satline)
{
  bitindex_t jx = bitindex_init(0);
  while (jx.index<con->nbrows){
    vector_product(pk,pk->cherni_prod,
		   con->p[jx.index],
		   tab, con->nbcolumns);
    if (numint_sgn(pk->cherni_prod)) bitstring_set(satline,jx);
    bitindex_inc(&jx);
  }
}

/* ********************************************************************** */
/*  VI. epsilon-Minimisation */
/* ********************************************************************** */

/* Library is supposed to be in strict mode (pk->strict==true), polyhedron is
   supposed to be in double representation, with nbeq and nbline correctly
   initialized, and with saturation matrix satF (row i of satF refers to
   constraint i, column k refers to ray k).
 */

bool cherni_minimizeeps(pk_internal_t* pk, pk_t* po)
{
  bool is_minimaleps,change,removed;
  pk_matrix_t* C;
  pk_matrix_t* F;
  satmat_t* satF;
  size_t i,j,w,Fnbrows,satFnbcols;
  bitindex_t k;
  bitstring_t* mask1;
  bitstring_t* mask2;


  assert(pk->strict && po->C && po->F && po->satF);
  is_minimaleps = true;
 
  C = po->C;
  F = po->F;
  Fnbrows = po->F->nbrows;
  satF = po->satF;
  satFnbcols = satF->nbcolumns;

  /* Builds the mask for the 2 redundancy tests */
  mask1 = bitstring_alloc(satFnbcols);
  mask2 = bitstring_alloc(satFnbcols);
  bitstring_clear(mask1,satFnbcols);
  bitstring_clear(mask2,satFnbcols);

  /* mask1: for testing if a constraint satisfies no closure generator points */
  /* mask2: for testing if a constraint is subsumed by another by considering
     only closure points and rays */
  /* Iterates on generators */
  k =  bitindex_init(po->nbline);
  while(k.index < Fnbrows){
    if (numint_sgn(F->p[k.index][polka_eps])==0){
      mask2[k.word] |= k.bit;
      if (numint_sgn(F->p[k.index][polka_cst])>0){
	mask1[k.word] |= k.bit;
      }
    }
    bitindex_inc(&k);
  }

  /* Iterates on strict constraints that are no positivity constraint */
  i = po->nbeq;
  while (i<C->nbrows){
    removed = false;
    if (numint_sgn(C->p[i][polka_eps])<0 &&
	!vector_is_positivity_constraint(pk,C->p[i],C->nbcolumns)){
      /* 1. First tests if it does saturate no closure generator points */
      
      /* Iterates on saturation line with mask1 */
      /* We test if one always has: mask=1 ==> not (sat=0) */
      bool sat_no_closuregen = true;
      for (w=0; w<satFnbcols; w++){
	bitstring_t sat = satF->p[i][w] | ~mask1[w];
	if (~sat != 0){
	  sat_no_closuregen = false;
	  break;
	}
      }
      if (sat_no_closuregen){
	removed = true;
      }
      else {
	/* 2. Check if there is no another constraint that subsumes it */
	for (j=po->nbeq; j<C->nbrows; j++){
	  if (j != i && numint_sgn(C->p[j][polka_eps])<0){
	    /* Iterates on closure points and rays */
	    bool included = true;
	    for (w=0; w<satFnbcols; w++){
	      /* We test: (mask=1 && i==0) ==> j==0 */
	      bitstring_t isincluded = ~satF->p[j][w] | satF->p[i][w] | ~mask2[w];
	      if (~isincluded != 0){
		included = false;;
		break;
	      }
	    }
	    if (included){
	      removed = true;
	      break;
	    }
	  }
	}
      }
    }
    if (removed){
      is_minimaleps = false;
      /* one remove the constraint */
      C->nbrows--; satF->nbrows--;
      pk_matrix_exch_rows(C, i, C->nbrows);
      satmat_exch_rows(satF, i, C->nbrows);
    }
    else {
      i++;
    }
  }
  bitstring_free(mask1); mask1=NULL;
  bitstring_free(mask2); mask2=NULL;

  if (is_minimaleps){
    po->status |= pk_status_minimaleps;
    change = false;
  }
  else {
    po->status &= pk_status_conseps | pk_status_consgauss;
    po->status |= pk_status_minimaleps;
    pk_matrix_free(po->F); po->F = NULL;
    satmat_free(po->satF); po->satF = NULL;
    if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
    /* Re-add positivity constraint (note: we are sure there is one free row */
    vector_clear(C->p[C->nbrows],C->nbcolumns);
    numint_set_int(C->p[C->nbrows][0],1);
    numint_set_int(C->p[C->nbrows][polka_cst],1);
    numint_set_int(C->p[C->nbrows][polka_eps],-1);
    C->_sorted = false;
    C->nbrows++;
    change = true;
  }
  if (!(po->status & pk_status_conseps)){
    bool change2 = pk_matrix_normalize_constraint(pk,po->C,po->intdim,po->realdim);
    if (change2){
      if (po->F){ pk_matrix_free(po->F); po->F = NULL; }
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    }
    po->status |= pk_status_conseps;
    change = change || change2;
  }
  return change;
}
