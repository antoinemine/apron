/* ************************************************************************* */
/* ap_dimension.c: dimensions and related operations */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <string.h>
#include <limits.h>
#include <assert.h>
#include "ap_dimension.h"

/* ====================================================================== */
/* ap_dimchange_t */
/* ====================================================================== */

/* Allocating a transformation */
void ap_dimchange_init(ap_dimchange_t* dimchange, size_t intdim, size_t realdim)
{
  dimchange->dim = intdim+realdim==0 ? NULL : malloc((intdim+realdim)*sizeof(ap_dim_t));
  dimchange->intdim = intdim;
  dimchange->realdim = realdim;
}
ap_dimchange_t* ap_dimchange_alloc(size_t intdim, size_t realdim)
{
  ap_dimchange_t* res = malloc(sizeof(ap_dimchange_t));
  ap_dimchange_init(res,intdim,realdim);
  return res;
}

/* Printing a transformation */
void ap_dimchange_fprint(FILE* stream, ap_dimchange_t* dimchange)
{
  size_t i;

  fprintf(stream,"dimchange: intdim=%lu, realdim=%lu\n           ",
	  (unsigned long)dimchange->intdim,
	  (unsigned long)dimchange->realdim);
  for (i=0;i<dimchange->intdim+dimchange->realdim;i++){
    fprintf(stream,"%2lu ",(unsigned long)dimchange->dim[i]);
  }
  fprintf(stream,"\n");
}

/* Inverting in-place an adding transformation from the given dimensionality */
void ap_dimchange_add_invert(ap_dimchange_t* dimchange)
{
  size_t i;
  for (i=0;i<dimchange->intdim+dimchange->realdim; i++){
    dimchange->dim[i] += i;
  }
}

/* ====================================================================== */
/* ap_dimchange2_t */
/* ====================================================================== */

/* Clear a dimchange structure (deallocate internal arrays) */
void ap_dimchange2_clear(ap_dimchange2_t* dimchange2)
{
  if (dimchange2->add){
    ap_dimchange_free(dimchange2->add);
    dimchange2->add = NULL;
  }
  if (dimchange2->remove){
    ap_dimchange_free(dimchange2->remove);
    dimchange2->remove = NULL;
  }
}
/* Deallocate and clear a dimchange2 structure */
void ap_dimchange2_free(ap_dimchange2_t* dimchange2)
{
  ap_dimchange2_clear(dimchange2);
  free(dimchange2);
}

/* Printing */
void ap_dimchange2_fprint(FILE* stream, ap_dimchange2_t* dimchange2)
{
  fprintf(stream,"add: ");
  if (dimchange2->add){
    ap_dimchange_fprint(stream,dimchange2->add);
  }
  else {
    fprintf(stream,"NULL\n");
  }
  fprintf(stream,"remove: ");
  if (dimchange2->remove){
    ap_dimchange_fprint(stream,dimchange2->remove);
  }
  else {
    fprintf(stream,"NULL\n");
  }
}

/* ====================================================================== */
/* ap_dimperm_t */
/* ====================================================================== */

/* Allocating a permutation */
void ap_dimperm_init(ap_dimperm_t* dimperm, size_t size)
{
  dimperm->dim = size==0 ? NULL : malloc(size*sizeof(ap_dim_t));
  dimperm->size = size;
}
ap_dimperm_t* ap_dimperm_alloc(size_t size)
{
  ap_dimperm_t* dimperm = malloc(sizeof(ap_dimperm_t));
  ap_dimperm_init(dimperm,size);
  return dimperm;
}

/* Printing a permutation */
void ap_dimperm_fprint(FILE* stream, ap_dimperm_t* perm)
{
  size_t i;

  fprintf(stream,"dimperm: size=%lu\n",(unsigned long)perm->size);
  for (i=0;i<perm->size;i++){
    fprintf(stream,"%2lu -> %2lu\n",(unsigned long)i,
	    (unsigned long)perm->dim[i]);
  }
}

/* Generates the identity permutation */
void ap_dimperm_set_id(ap_dimperm_t* perm)
{
  size_t i;
  for (i=0; i<perm->size; i++) perm->dim[i] = i;
}

/* Compose 2 permutations */
void ap_dimperm_compose(ap_dimperm_t* perm, ap_dimperm_t* perm1, ap_dimperm_t* perm2)
{
  size_t i;

  assert(perm->size==perm1->size && perm->size==perm2->size);
  for (i=0; i<perm->size; i++){
    perm->dim[i] = perm2->dim[perm1->dim[i]];
  }
}
/* Invert a permutation */
void ap_dimperm_invert(ap_dimperm_t* nperm, ap_dimperm_t* perm)
{
  size_t i;

  assert(nperm->size==perm->size);
  for (i=0; i<perm->size; i++){
    nperm->dim[perm->dim[i]] = i;
  }
}

/* ====================================================================== */
/* Supports */
/* ====================================================================== */

/* ttdim[i] should be of size nbdims+1, if nbdims is the number of dimensions to be encountered.
   
   Merge buffers indexed by k0 and k1, and return the new buffer in the index
   *pk (normally, k2) */
void ap_dimsupport_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk)
{
  size_t k0 = *pk;
  size_t k1 = (k0+1)%3;
  size_t k2 = (k0+2)%3;
  if (k0==0)
    *pk = k1;
  else if (k1==0)
    *pk = k0;
  else {
    size_t i0,i1,i2;
    bool end0, end1;

    i0 = i1 = i2 = 0;
    end0 = end1 = false;
    while (true){
      end0 = end0 || i0>=tnb[k0];
      end1 = end1 || i1>=tnb[k1];
      if (end0 && end1)
	break;
      else if (end0 || (!end1 && ttdim[k1][i1] < ttdim[k0][i0])){
	ttdim[k2][i2] = ttdim[k1][i1];
	i2++; i1++;
      }
      else if (end1 || (!end0 && ttdim[k1][i1] > ttdim[k0][i0])){
	ttdim[k2][i2] = ttdim[k0][i0];
	i2++; i0++;
      }
      else {
	ttdim[k2][i2] = ttdim[k0][i0];
	i2++; i0++; i1++;
      }
    }
    tnb[k2] = i2;
    *pk = k2;
  }
}

