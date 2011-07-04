/* ********************************************************************** */
/* box_representation.c: general management */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box_representation.h"

/* ********************************************************************** */
/* Internal functions */
/* ********************************************************************** */
box_t* box_alloc(size_t intdim, size_t realdim)
{
  box_t* itv = malloc(sizeof(box_t));
  itv->p = NULL;
  itv->intdim = intdim;
  itv->realdim = realdim;
  return itv;
}

void box_init(box_t* a)
{
  size_t i;
  size_t nbdims = a->intdim + a->realdim;
  assert(a->p==NULL);
  a->p = itv_array_alloc(nbdims+1); 
  /* Add an unused dimension to differentiate
     empty and top values in dimension 0+0 */
}

void box_set_bottom(box_t* a)
{
  if (a->p){
    itv_array_free(a->p,a->intdim+a->realdim+1);
    a->p = NULL;
  }
}

void box_set_top(box_t* a)
{
  size_t i;
  size_t nbdims;
  
  nbdims = a->intdim + a->realdim;
  if (a->p==NULL){
    box_init(a);
  };
  for (i=0; i<nbdims; i++){
    itv_set_top(a->p[i]);
  }
}

void box_set(box_t* a, box_t* b)
{
  size_t i;
  size_t nbdims;
  
  if (b->p==NULL)
    return;

  nbdims = b->intdim + b->realdim;
  if (a->p==NULL){
    box_init(a);
  };
  for (i=0; i<nbdims; i++){
    itv_set(a->p[i],b->p[i]);
  }
}

/* ********************************************************************** */
/* 1. Memory */
/* ********************************************************************** */

/* Return a copy of an abstract value, on
   which destructive update does not affect the initial value. */
box_t* box_copy(ap_manager_t* man, box_t* a)
{
  size_t i;
  size_t nbdims = a->intdim+a->realdim;

  box_t* b = box_alloc(a->intdim,a->realdim);
  if (a->p){
    b->p = malloc((nbdims+1)*sizeof(itv_t));
    for (i=0; i<nbdims; i++){
      itv_init_set(b->p[i],a->p[i]);
    }
    itv_init(b->p[nbdims]);  
    /* Add an unused dimension to differentiate
       empty and top values in dimension 0+0 */ 
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return b;
}

/* Free all the memory used by the abstract value */
void box_free(ap_manager_t* man, box_t* a)
{
  if (a->p){
    itv_array_free(a->p,a->intdim+a->realdim+1);
    a->p = NULL;
  }
  free(a);
}

/* Return the abstract size of an abstract value (see ap_manager_t) */
size_t box_size(ap_manager_t* man, box_t* a)
{
  return 2*(a->intdim+a->realdim);
}

/* ********************************************************************** */
/* 2. Control of internal representation */
/* ********************************************************************** */

/* Minimize the size of the representation of a.
   This may result in a later recomputation of internal information.
*/
void box_minimize(ap_manager_t* man, box_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return;
}

/* Put the abstract value in canonical form. (not yet clear definition) */
void box_canonicalize(ap_manager_t* man, box_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return;
}

/* Return an hash code */
int box_hash(ap_manager_t* man, box_t* a)
{
  int i,dec,size,res;
  size = a->intdim +a->realdim;
  res = size * 2999;
  
  if (a->p!=NULL){
    for (i=0; i<size; i += (size+4)/5){
      res = 3*res + itv_hash(a->p[i]);
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
   associated to box_approximate (commodity feature). */
void box_approximate(ap_manager_t* man, box_t* a, int algorithm)
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
void box_fprint(FILE* stream,
	       ap_manager_t* man,
	       box_t* a,
	       char** name_of_dim)
{
  size_t i;
  size_t nbdims = a->intdim + a->realdim;

  fprintf(stream,"interval of dim (%ld,%ld):",
	  (long)a->intdim,(long)a->realdim);
  if (a->p){
    fprintf(stream,"\n");
    for(i=0; i<nbdims; i++){
      if (name_of_dim){
	fprintf(stream,"%8s in ", name_of_dim[i]);
      } else {
	fprintf(stream,"x%ld in ", (long)i);
      }
      itv_fprint(stream,a->p[i]);
      fprintf(stream,"\n");
    }
  }
  else {
    fprintf(stream,nbdims>0 ? " bottom\n" : "top\n");
  }
}

/* Dump the internal representation of an abstract value,
   for debugging purposes */
void box_fdump(FILE* stream,
	      ap_manager_t* man,
	      box_t* a)
{
  size_t i;
  size_t nbdims = a->intdim + a->realdim;

  fprintf(stream,"interval of dim (%ld,%ld):",
	  (long)a->intdim,(long)a->realdim);
  if (a->p){
    fprintf(stream,"\n");
    for(i=0; i<nbdims; i++){
      fprintf(stream,"dim %3ld in ",(long)i);
      itv_fprint(stream,a->p[i]);
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
void box_fprintdiff(FILE* stream,
		   ap_manager_t* man,
		   box_t* a, box_t* b,
		   char** name_of_dim)
{
  size_t i;
  size_t nbdims;
  char* str;

  nbdims = a->intdim + a->realdim;

  fprintf(stream,"diff of 2 intervals of dim (%ld,%ld)",
	  (long)a->intdim,(long)b->intdim);
  if (box_is_eq(man,a,b)){
    fprintf(stream," : none\n");
  }
  else {
    /* we are sure that nbdims>0 */
    if (a->p==0){
      fprintf(stream,"\nbottom =>\n");
      box_fprint(stream,man,b,name_of_dim);
    }
    else if (b->p==0){
      fprintf(stream,"\n");
      box_fprint(stream,man,a,name_of_dim);
      fprintf(stream,"=> bottom\n");
    }
    else {
      bound_t bound;
      bound_init(bound);
      for(i=0; i<nbdims; i++){
	int sgn1 = bound_cmp(a->p[i]->inf, b->p[i]->inf);
	int sgn2 = bound_cmp(a->p[i]->sup, b->p[i]->sup);
	
	if (sgn1!=0 || sgn2!=0){
	  if (name_of_dim) 
	    fprintf(stream,"%8s in ",name_of_dim[i]);
	  else
	    fprintf(stream,"x%ld in ", (long)i);
	  itv_fprint(stream,a->p[i]);
	  fprintf(stream," => ");
	  str =
	    sgn1>0 ?
	    "-[" :
	    ( sgn1<0 ?
	      "+[" :
	      "[=, "  );
	  fprintf(stream,"%s",str);
	  if (sgn1!=0){
	    bound_neg(bound,b->p[i]->inf);
	    bound_fprint(stream,bound);
	    fprintf(stream,", ");
	  }
	  if (sgn2!=0){
	    bound_fprint(stream,b->p[i]->sup);
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
      bound_clear(bound);
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
ap_membuf_t box_serialize_raw(ap_manager_t* man, box_t* a)
{
  ap_membuf_t buf;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"");
  buf.ptr = NULL;
  buf.size = 0;
  return buf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
box_t* box_deserialize_raw(ap_manager_t* man, void* ptr)
{
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,"");
  return NULL;
}

