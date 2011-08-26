/* ********************************************************************** */
/* pk_option.c: options */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkMPQ.h"
#include "pkMPQ_internal.h"

pk_option_t* pk_manager_option_ref(ap_manager_t* man)
{
  pkMPQ_internal_t* intern = (pkMPQ_internal_t*)man->internal;
  pk_option_t* opt = &(intern->option);
  return opt;
}
