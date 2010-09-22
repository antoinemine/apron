#ifndef _T1P_ITV_UTILS_H_
#define _T1P_ITV_UTILS_H_

#include <fenv.h>
#include <errno.h>
#include "itv.h"

/* TODO: To be add to itv lib (?) */
static inline bool itv_has_infty_bound(itv_t itv);
static inline bool itv_has_finite_bound(itv_t itv);
static inline bool itv_cmp(itv_t a, itv_t b);

/* mid = RND_NEAREST((sup(a) + inf(a))/2). dev = max(sup(a)-mid, mid-inf(a)) */
static inline void itv_middev(itv_internal_t *itv, itv_t mid, itv_t dev, itv_t a);

static inline bool itv_has_infty_bound(itv_t itv)
{
        return (bound_infty(itv->sup) || bound_infty(itv->inf)) ;
}

static inline bool itv_has_finite_bound(itv_t itv)
{
        return !(bound_infty(itv->sup) || bound_infty(itv->inf)) ;
}

/* be aware: itv->inf is encoded with minus sign */
static inline bool itv_has_not_zero(itv_t itv)
{
	return ((bound_sgn(bound_numref(itv->inf))==-1) || (bound_sgn(bound_numref(itv->sup))==-1));
}

/* if a is bottom or has one infity bound, mid and dev are set to top. */
static inline void itv_middev(itv_internal_t *itv, itv_t mid, itv_t dev, itv_t a)
/* L'intervalle [mid-dev,mid+dev] colle plus � l'intervalle d'origine [x,y] si mid est calcule en arrondissant au plus pres.
 * En effet, [mid-dev,mid+dev] est decalle de la distance qui separe le vrai milieu de celui arrondi, l'arrondi au plus pres minimise cette distance.
 */
{
    bound_t tmp[4];
    bound_init(tmp[0]);
    bound_init(tmp[1]);
    bound_init(tmp[2]);
    bound_init(tmp[3]);

    if (itv_is_point(itv, a)) {
	itv_set(mid,a);
	itv_set_int(dev,0);
    } else if (itv_has_infty_bound(a) || itv_is_bottom(itv, a)) {
	itv_set_top(mid);
	itv_set_top(dev);
    } else {
	/* a = [x,y], x < y,
	 * tmp[0] = x+y */
	/* Rounding to Nearest, only x86 Unix like */
	bound_sub(tmp[0], a->sup, a->inf);
	if (!fesetround(FE_TONEAREST)) {
	    /* tmp[1] = (x+y)/2 -- using ldexp if double */
	    bound_div_2(tmp[1], tmp[0]);
	    /* mid = [tmp[1], tmp[1]] */
	    bound_set(mid->sup, tmp[1]);
	    bound_neg(mid->inf, tmp[1]);
	}
	else {
	    fprintf(stderr,"fesetround: %s\n", strerror (errno));
	    abort();
	}

	/* Come back to default rounding mode */
	if (fesetround(FE_UPWARD)) {
	    fprintf(stderr,"fesetround: %s\n", strerror (errno));
	    abort();
	} else {
	    /* tmp[0] = (x+y)/2 - x */
	    bound_add(tmp[0], tmp[1], a->inf);
	    /* tmp[2] = y - (x+y)/2 */
	    bound_sub(tmp[2], a->sup, tmp[1]);
	    /* tmp[3] = max(tmp[0],tmp[2]) -- fmax if double */
	    bound_max(tmp[3], tmp[0], tmp[2]);
	    /* dev = [tmp[3], tmp[3]] */
	    bound_set(dev->sup, tmp[3]);
	    bound_neg(dev->inf, tmp[3]);
	}
    }
    bound_clear(tmp[0]);
    bound_clear(tmp[1]);
    bound_clear(tmp[2]);
    bound_clear(tmp[3]);
}

/* return true if for all x in a, for all y in b: x <= y */
static inline bool itv_cmp(itv_t a, itv_t b){
    bool res = false;
    bound_t tmp; bound_init(tmp);
    bound_neg(tmp,b->inf);
    res = bound_cmp(a->sup,tmp)<=0? true: false;
    bound_clear(tmp);
    return res;
}
static inline bool itv_is_generic(itv_t a, itv_t b) 
{
    if (itv_is_leq(a,b) || itv_is_leq(a,b)) {
	if ((bound_cmp(a->inf, b->inf) == 0) || (bound_cmp(a->sup, b->sup) == 0)) return true; else return false;
    } else return true;
}
#endif
