/* ********************************************************************** */
/* box.c: abstract lattice of intervals */
/* ********************************************************************** */

#include <stdio.h>
#include "box.h"

#include "boxIll.h"
#include "boxD.h"
#include "boxMPQ.h"
#include "boxMPFR.h"

ap_manager_t* box_manager_alloc(num_discr_t discr)
{
  switch(discr){
  case NUM_Ill:
    return boxIll_manager_alloc();
  case NUM_D:
    return boxD_manager_alloc();
  case NUM_MPQ:
    return boxMPQ_manager_alloc();
  case NUM_MPFR:
    return boxMPFR_manager_alloc();
  default:
    fprintf(stderr," box_manager_alloc: given numerical type not supported\n");
    return NULL;
  }
}
