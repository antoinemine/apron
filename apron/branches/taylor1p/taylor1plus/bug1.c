
#include "itv.h"

int main(void) {
    itv_t toto;
    itv_t titi;
    itv_ptr pitv;
    __itv_struct * coeff;
    itv_internal_t* itv = itv_internal_alloc();

    pitv = &toto;
    coeff = &titi;
    itv_init(pitv);
    itv_init(coeff);

    itv_set_int(pitv, 1);
    itv_mul_2exp(coeff, pitv, -1);
    itv_neg(coeff, coeff);
    itv_mul(itv, pitv, coeff, pitv);
    itv_fprint(stdout, pitv);
    return 0;
}
