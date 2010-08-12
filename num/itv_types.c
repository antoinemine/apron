/* ********************************************************************** */
/* itv_types.c: all kind of intervals and general workspace */
/* ********************************************************************** */

#include "itv_types.h"
#include "itvIl.h"
#include "itvIll.h"
#include "itvMPZ.h"
#include "itvRl.h"
#include "itvRll.h"
#include "itvMPQ.h"
#include "itvD.h"
#include "itvDl.h"
#include "itvMPFR.h"

void itv_internal_init(itv_internal_t intern)
{
  num_internal_init	(intern->num);
  itvIl_internal_init	(intern->Il);
  itvIll_internal_init	(intern->Ill);
  itvMPZ_internal_init	(intern->MPZ);
  itvRl_internal_init	(intern->Rl);
  itvRll_internal_init	(intern->Rll);
  itvMPQ_internal_init	(intern->MPQ);
  itvD_internal_init	(intern->D);
  itvDl_internal_init	(intern->Dl);
  itvMPFR_internal_init (intern->MPFR);
}
void itv_internal_clear(itv_internal_t intern)
{
  num_internal_clear	(intern->num);
  itvIl_internal_clear	(intern->Il);
  itvIll_internal_clear	(intern->Ill);
  itvMPZ_internal_clear	(intern->MPZ);
  itvRl_internal_clear	(intern->Rl);
  itvRll_internal_clear	(intern->Rll);
  itvMPQ_internal_clear	(intern->MPQ);
  itvD_internal_clear	(intern->D);
  itvDl_internal_clear	(intern->Dl);
  itvMPFR_internal_clear(intern->MPFR);
}
itv_internal_ptr itv_internal_alloc(void)
{
  itv_internal_ptr intern = malloc(sizeof(itv_internal_t));
  itv_internal_init(intern);
  return intern;
}
void itv_internal_free(itv_internal_ptr intern)
{
  itv_internal_clear(intern);
  free(intern);
}

#ifdef __cplusplus
}
#endif

#endif
