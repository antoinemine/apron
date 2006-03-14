/* ********************************************************************** */
/* permutation.c: permutations */
/* ********************************************************************** */

#include "permutation.h"

/* Printing */
void permutation_fprint(FILE* stream, dim_t* perm, size_t size)
{
  size_t i;

  fprintf(stream,"permutation: size=%d\n",size);
  for (i=0;i<size;i++){
    fprintf(stream,"%2d -> %2d\n",i,perm[i]);
  }
}
/* Generates the identity permutation */
void permutation_set_id(dim_t* perm, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) perm[i] = i;
}

/* Generates the permutation

   [0..intdim-1,0..realdim-1, 0..intdimsup-1,0..realdimsup-1]
   to
   [0..intdim-1,0..intdimsup-1, 0..realdim-1,0..realdimsup-1]
*/

void permutation_set_intreal_plus(dim_t* perm,
				  size_t intdim, size_t realdim,
				  size_t intdimsup, size_t realdimsup)
{
  size_t i;
  permutation_set_id(perm,intdim+realdim+intdimsup+realdimsup);
  for (i=intdim; i<intdim+realdim; i++){
    perm[i] = i+intdimsup;
  }
  for (i=intdim+realdim; i<intdim+realdim+intdimsup; i++){
    perm[i] = i-realdim;
  }
}
/* Generates the permutation

   [0..intdim-1,0..intdimsup-1, 0..realdim-1,0..realdimsup-1]
   to
   [0..intdim-1,0..realdim-1, 0..intdimsup-1,0..realdimsup-1]
*/

void permutation_set_intreal_minus(dim_t* perm,
				   size_t intdim, size_t realdim,
				   size_t intdimsup, size_t realdimsup)
{
  size_t i;
  permutation_set_id(perm,intdim+realdim+intdimsup+realdimsup);
  for (i=intdim; i<intdimsup; i++){
    perm[i] = i+realdim;
  }
  for (i=intdim+intdimsup; i<intdim+intdimsup+realdim; i++){
    perm[i] = i-intdimsup;
  }
}

/* Compose 2 permutations */
void permutation_compose(dim_t* perm, const dim_t* perm1, const dim_t* perm2, size_t size)
{
  size_t i;

  for (i=0; i<size; i++){
    perm[i] = perm2[perm1[i]];
  }
}
/* Invert a permutation */
void permutation_invert(dim_t* nperm, const dim_t* perm, size_t size)
{
  size_t i;

  for (i=0; i<size; i++){
    nperm[perm[i]] = i;
  }
}
