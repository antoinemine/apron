/* ********************************************************************** */
/* permutation.h: permutations */
/* ********************************************************************** */

#ifndef _PERMUTATION_H_
#define _PERMUTATION_H_

#include <stdio.h>
#include "expr0.h"

void permutation_fprint(FILE* stream, dim_t* perm, size_t size);
  /* Print a permutation under the form:
     permutation: size=...
     0 -> perm[0]
     1 -> perm[1]
     ...
 */

void permutation_set_id(dim_t* perm, size_t size);
  /* Generate the identity permutation */

void permutation_set_intreal_plus(dim_t* perm,
                                  size_t intdim, size_t realdim,
                                  size_t intdimsup, size_t realdimsup);
  /* Generates the permutation

     [0..intdim-1,0..realdim-1, 0..intdimsup-1,0..realdimsup-1]
     to
     [0..intdim-1,0..intdimsup-1, 0..realdim-1,0..realdimsup-1]
  */

void permutation_set_intreal_minus(dim_t* perm,
                                   size_t intdim, size_t realdim,
                                   size_t intdimsup, size_t realdimsup);
  /* Generates the permutation

     [0..intdim-1,0..intdimsup-1, 0..realdim-1,0..realdimsup-1]
     to
     [0..intdim-1,0..realdim-1, 0..intdimsup-1,0..realdimsup-1]
  */

void permutation_compose(dim_t* perm, const dim_t* perm1, const dim_t* perm2, 
                         size_t size);
  /* Compose the 2 permutations perm1 and perm2 (in this order) 
     and store the result the already allocated perm.
     size is the commonsize of all permutations.
     At exit, we have perm[i] = perm2[perm1[i]]
*/
void permutation_invert(dim_t* nperm, const dim_t* perm, size_t size);
  /* Invert the permutation perm and store it in the already allocated nperm. 
     size is the commonsize of the 2 permutations.
  */

#endif
