/* ********************************************************************** */
/* box.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_H_
#define _BOX_H_

#include "num_types.h"
#include "ap_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t* box_manager_alloc(num_discr_t discr);

#ifdef __cplusplus
}
#endif

#endif
