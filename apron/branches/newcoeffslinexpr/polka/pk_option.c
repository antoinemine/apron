/* ************************************************************************* */
/* internal.c: internal manager */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkMPQ_internal.h"

void pk_set_max_coeff_size(ap_manager_t* man, unsigned int size)
{
  pkMPQ_internal_t* pk = man->internal;
  pk->max_coeff_size = size;
}
void pk_set_approximate_max_coeff_size(ap_manager_t* man, unsigned int size)
{
  pkMPQ_internal_t* pk = man->internal;
  pk->approximate_max_coeff_size = size;
}
unsigned int pk_get_max_coeff_size(ap_manager_t* man)
{
  pkMPQ_internal_t* pk = man->internal;
  return pk->max_coeff_size;
}
unsigned int pk_get_approximate_max_coeff_size(ap_manager_t* man)
{
  pkMPQ_internal_t* pk = man->internal;
  return pk->approximate_max_coeff_size;
}
