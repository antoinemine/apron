/* ************************************************************************* */
/* ap_disjpk.c: disjunctions of Newpolka polyhedra */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_global0.h"
#include "ap_disjunction.h"

#include "pk.h"

void ap_disjpk_merge(ap_manager_t* man, ap_disjunction_t* a)
{
  return;
}

ap_manager_t* ap_disjpk_manager_alloc(ap_manager_t* manpk)
{
  return ap_disjunction_manager_alloc(manpk,
				 &ap_disjpk_merge);
}

ap_lincons0_array_t ap_disjpk_to_lincons_set(ap_manager_t* manager,
					     ap_abstract0_t* abs)
{
  return ap_disjunction_to_lincons_set(manager,abs->value);
}
