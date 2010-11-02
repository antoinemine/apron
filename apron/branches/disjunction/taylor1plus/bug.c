
#include "itv.h"
#include "itv_linexpr.h"

int main(void) {
    itv_linexpr_t* lres;
    itv_lincons_array_t *array;
    itv_lincons_array_t res;
    itv_lincons_array_init(&res,1);
    array = &res;
    itv_lincons_array_reinit(array,1);
    lres = &array->p[0].linexpr;
    itv_linexpr_reinit(lres,1);
    return 0;
}
