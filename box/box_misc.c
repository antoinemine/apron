/* ********************************************************************** */
/* box_misc.c */
/* ********************************************************************** */

#include "boxMPQ_internal.h"
#include "boxMPQ.h"

box_option_t* box_manager_option_ref(ap_manager_t* man)
{
  boxMPQ_internal_t* intern = (boxMPQ_internal_t*)man->internal;
  return &(intern->option);
}
