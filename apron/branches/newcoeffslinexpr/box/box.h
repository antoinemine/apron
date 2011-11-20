/* ********************************************************************** */
/* box.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_H_
#define _BOX_H_

#include "ap_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct box_option_t {
  unsigned int kmax;
  /* number of local iterations (at least 1) in
     "boxization", that is, intersection of a box with a linear
     constraint.

     Important for meet_(lin|t)cons_array and substitute_(lin|t)expr
  */
} box_option_t;

ap_manager_t* box_manager_alloc(num_discr_t discr);
box_option_t* box_manager_option_ref(ap_manager_t* man);

#ifdef __cplusplus
}
#endif

#endif
