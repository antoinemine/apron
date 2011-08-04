/* ********************************************************************** */
/* boxXXX_representation.c: general management */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boxXXX_internal.h"

/* ********************************************************************** */
/* Internal functions */
/* ********************************************************************** */
boxXXX_t* boxXXX_alloc(ap_dimension_t dim)
{
  boxXXX_t* itv = malloc(sizeof(boxXXX_t));
  itv->e->linterm = NULL;
  itv->dim = dim;
  return itv;
}

void boxXXX_init(boxXXX_t* a)
{
  size_t i;
  size_t nbdims = ap_dimension_size(a->dim);
  assert(a->e->linterm==NULL);
  ap_linexprXXX_init(a->e,nbdims+1);
  a->e->effsize = nbdims;
  /* Add an unused dimension to differentiate
     empty and top values in dimension 0+0 */
  for (i=0;i<nbdims;i++){
    ap_lintermXXX_ptr linterm = a->e->linterm[i];
    linterm->dim = i;
  }
}

void boxXXX_set_bottom(boxXXX_t* a)
{
  if (a->e->linterm){
    ap_linexprXXX_clear(a->e);
  }
}

void boxXXX_set_top(boxXXX_t* a)
{
  size_t i;
  size_t nbdims;

  nbdims = a->dim.intd + a->dim.reald;
  if (a->e->linterm==NULL){
    boxXXX_init(a);
  };
  for (i=0; i<nbdims; i++){
    eitvXXX_set_top(a->e->linterm[i]->eitv);
  }
}

void boxXXX_set(boxXXX_t* a, boxXXX_t* b)
{
  size_t i;
  size_t nbdims;

  assert(ap_dimension_size(a->dim) == ap_dimension_size(b->dim));
  if (b->e->linterm==NULL){
    if (a->e->linterm) ap_linexprXXX_clear(a->e);
    return;
  }
  nbdims = b->dim.intd + b->dim.reald;
  if (a->e->linterm==NULL){
    boxXXX_init(a);
  };
  for (i=0; i<nbdims; i++){
    eitvXXX_set(a->e->linterm[i]->eitv,b->e->linterm[i]->eitv);
  }
}

/* ********************************************************************** */
/* 1. Memory */
/* ********************************************************************** */

/* Return a copy of an abstract value, on
   which destructive update does not affect the initial value. */
boxXXX_t* boxXXX_copy(ap_manager_t* man, boxXXX_t* a)
{
  size_t i;
  size_t nbdims = a->dim.intd+a->dim.reald;

  boxXXX_t* b = boxXXX_alloc(a->dim);
  boxXXX_set(b,a);
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return b;
}

/* Free all the memory used by the abstract value */
void boxXXX_free(ap_manager_t* man, boxXXX_t* a)
{
  if (a->e->linterm){
    ap_linexprXXX_clear(a->e);
  }
  free(a);
}

/* Return the abstract size of an abstract value (see ap_manager_t) */
size_t boxXXX_size(ap_manager_t* man, boxXXX_t* a)
{
  return 2*ap_dimension_size(a->dim);
}

/* ********************************************************************** */
/* 2. Control of internal representation */
/* ********************************************************************** */

/* Minimize the size of the representation of a.
   This may result in a later recomputation of internal information.
*/
void boxXXX_minimize(ap_manager_t* man, boxXXX_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return;
}

/* Put the abstract value in canonical form. (not yet clear definition) */
void boxXXX_canonicalize(ap_manager_t* man, boxXXX_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return;
}

/* Return an hash code */
int boxXXX_hash(ap_manager_t* man, boxXXX_t* a)
{
  int i,dec,size,res;
  size = a->dim.intd +a->dim.reald;
  res = size * 2999;

  if (a->e->linterm!=NULL){
    for (i=0; i<size; i += (size+4)/5){
      res = 3*res + eitvXXX_hash(a->e->linterm[i]->eitv);
    }
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return res;
}

/* Perform some transformation on the abstract value, guided by the
   field algorithm.

   The transformation may lose information.  The argument "algorithm"
   overrides the field algorithm of the structure of type foption_t
   associated to boxXXX_approximate (commodity feature). */
void boxXXX_approximate(ap_manager_t* man, boxXXX_t* a, int algorithm)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
 return;
}

/* ********************************************************************** */
/* 3. Printing */
/* ********************************************************************** */

/* Print the abstract value in a pretty way, using function
   name_of_dim to name dimensions */
void boxXXX_fprint(FILE* stream,
	       ap_manager_t* man,
	       boxXXX_t* a,
	       char** name_of_dim)
{
  size_t i;
  size_t nbdims = a->dim.intd + a->dim.reald;

  fprintf(stream,"interval of dim (%ld,%ld):",
	  (long)a->dim.intd,(long)a->dim.reald);
  if (a->e->linterm){
    fprintf(stream,"\n");
    for(i=0; i<nbdims; i++){
      if (name_of_dim){
	fprintf(stream,"%8s in ", name_of_dim[i]);
      } else {
	fprintf(stream,"x%ld in ", (long)i);
      }
      eitvXXX_fprint(stream,a->e->linterm[i]->eitv);
      fprintf(stream,"\n");
    }
  }
  else {
    fprintf(stream,nbdims>0 ? " bottom\n" : "top\n");
  }
}

/* Dump the internal representation of an abstract value,
   for debugging purposes */
void boxXXX_fdump(FILE* stream,
	      ap_manager_t* man,
	      boxXXX_t* a)
{
  size_t i;
  size_t nbdims = a->dim.intd + a->dim.reald;

  fprintf(stream,"interval of dim (%ld,%ld):",
	  (long)a->dim.intd,(long)a->dim.reald);
  if (a->e->linterm){
    fprintf(stream,"\n");
    for(i=0; i<nbdims; i++){
      fprintf(stream,"dim %3ld in ",(long)i);
      eitvXXX_fprint(stream,a->e->linterm[i]->eitv);
      fprintf(stream,"\n");
    }
  }
  else {
    fprintf(stream, nbdims>0 ? " bottom\n" : "top\n");
  }
}

/* Print the difference between a1 (old value) and a2 (new value),
   using function name_of_dim to name dimensions.
   The meaning of difference is library dependent. */
void boxXXX_fprintdiff(FILE* stream,
		   ap_manager_t* man,
		   boxXXX_t* a, boxXXX_t* b,
		   char** name_of_dim)
{
  size_t i;
  size_t nbdims;
  char* str;

  nbdims = a->dim.intd + a->dim.reald;

  fprintf(stream,"diff of 2 intervals of dim (%ld,%ld)",
	  (long)a->dim.intd,(long)b->dim.intd);
  if (boxXXX_is_eq(man,a,b)){
    fprintf(stream," : none\n");
  }
  else {
    /* we are sure that nbdims>0 */
    if (a->e->linterm==0){
      fprintf(stream,"\nbottom =>\n");
      boxXXX_fprint(stream,man,b,name_of_dim);
    }
    else if (b->e->linterm==0){
      fprintf(stream,"\n");
      boxXXX_fprint(stream,man,a,name_of_dim);
      fprintf(stream,"=> bottom\n");
    }
    else {
      boundXXX_t bound;
      boundXXX_init(bound);
      for(i=0; i<nbdims; i++){
	int sgn1 = boundXXX_cmp(a->e->linterm[i]->eitv->itv->neginf, b->e->linterm[i]->eitv->itv->neginf);
	int sgn2 = boundXXX_cmp(a->e->linterm[i]->eitv->itv->sup, b->e->linterm[i]->eitv->itv->sup);

	if (sgn1!=0 || sgn2!=0){
	  if (name_of_dim)
	    fprintf(stream,"%8s in ",name_of_dim[i]);
	  else
	    fprintf(stream,"x%ld in ", (long)i);
	  eitvXXX_fprint(stream,a->e->linterm[i]->eitv);
	  fprintf(stream," => ");
	  str =
	    sgn1>0 ?
	    "-[" :
	    ( sgn1<0 ?
	      "+[" :
	      "[=, "  );
	  fprintf(stream,"%s",str);
	  if (sgn1!=0){
	    boundXXX_neg(bound,b->e->linterm[i]->eitv->itv->neginf);
	    boundXXX_fprint(stream,bound);
	    fprintf(stream,", ");
	  }
	  if (sgn2!=0){
	    boundXXX_fprint(stream,b->e->linterm[i]->eitv->itv->sup);
	  }
	  str =
	    sgn2>0 ?
	    "]+" :
	    ( sgn2<0 ?
	      "]-" :
	      "=]"  );
	  fprintf(stream,"%s\n",str);
	}
	else {
	  if (name_of_dim)
	    fprintf(stream,"%8s in [=,=]\n",name_of_dim[i]);
	  else
	    fprintf(stream,"x%ld in [=,=]\n",(long)i);
	}
      }
      boundXXX_clear(bound);
    }
  }
}


/* ********************************************************************** */
/* 4. Serialization */
/* ********************************************************************** */

/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */
ap_membuf_t boxXXX_serialize_raw(ap_manager_t* man, boxXXX_t* a)
{
  ap_membuf_t buf;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"");
  buf.ptr = NULL;
  buf.size = 0;
  return buf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
boxXXX_t* boxXXX_deserialize_raw(ap_manager_t* man, void* ptr)
{
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,"");
  return NULL;
}
