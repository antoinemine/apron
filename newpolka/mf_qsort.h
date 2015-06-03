/* ********************************************************************** */
/* mf_qsort.h: quicksort */
/* ********************************************************************** */

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QSORT_EXTRA_CMP_ARGUMENT

typedef int (*qsort2_cmp)(void*, void *, void *);

void qsort2(void *base_ptr, size_t count, size_t size,
	    qsort2_cmp cmp,
	    void *cmp_argument);

#ifdef __cplusplus
}
#endif

