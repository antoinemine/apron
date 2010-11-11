/* ********************************************************************** */
/* boxXXX_representation.c: general management */
/* ********************************************************************** */

#ifndef _BOX_XXX_REPRESENTATION_H_
#define _BOX_XXX_REPRESENTATION_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Internal functions */
boxXXX_t* boxXXX_alloc(size_t intdim, size_t realdim);
void boxXXX_init(boxXXX_t* a);
void boxXXX_set_bottom(boxXXX_t* a);
void boxXXX_set_top(boxXXX_t* a);
void boxXXX_set(boxXXX_t* a, boxXXX_t* b);

#ifdef __cplusplus
}
#endif

#endif
