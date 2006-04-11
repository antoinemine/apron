/* ************************************************************************* */
/* ap_dimension.c: dimensions and related operations */
/* ************************************************************************* */

#include <string.h>
#include <limits.h>
#include <assert.h>
#include "ap_dimension.h"

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

  fprintf(stream,"dimchange: intdim=%d, realdim=%d\n           ",
	  dimchange->intdim,dimchange->realdim);
  for (i=0;i<dimchange->intdim+dimchange->realdim;i++){
    fprintf(stream,"%2d ",dimchange->dim[i]);
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

  fprintf(stream,"dimperm: size=%d\n",perm->size);
  for (i=0;i<perm->size;i++){
    fprintf(stream,"%2d -> %2d\n",i,perm->dim[i]);
  }
}

/* Generates the identity permutation */
void ap_dimperm_set_id(ap_dimperm_t* perm)
{
  size_t i;
  for (i=0; i<perm->size; i++) perm->dim[i] = i;
}

/* Compose 2 permutations */
void ap_dimperm_compose(ap_dimperm_t* perm, const ap_dimperm_t* perm1, const ap_dimperm_t* perm2)
{
  size_t i;
  
  assert(perm->size==perm1->size && perm->size==perm2->size);
  for (i=0; i<perm->size; i++){
    perm->dim[i] = perm2->dim[perm1->dim[i]];
  }
}
/* Invert a permutation */
void ap_dimperm_invert(ap_dimperm_t* nperm, const ap_dimperm_t* perm)
{
  size_t i;

  assert(nperm->size==perm->size);
  for (i=0; i<perm->size; i++){
    nperm->dim[perm->dim[i]] = i;
  }
}
