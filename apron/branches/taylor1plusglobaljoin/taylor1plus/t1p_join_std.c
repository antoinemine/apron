#include <stdlib.h>

#include "num.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

#include "ap_manager.h"
#include "ap_tcons0.h"
#include "ap_lincons0.h"
#include "ap_generator0.h"
#include "ap_generic.h"

#include "t1p_internal.h"
#include "t1p_representation.h"
#include "t1p_constructor.h"

#include "t1p_itv_utils.h"
#include "t1p_fun.h"

#include "t1p_meet.h"
#include "t1p_join_std.h"

/* Standard Join */
/*****************/


/* local join */
t1p_t* t1p_join_std(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2)
    /* TODO destructive not used  */
{
    CALL();
    size_t i = 0;
    t1p_internal_t* pr = (t1p_internal_t*)t1p_init_from_manager(man, AP_FUNID_JOIN);
    arg_assert(a1->dims==a2->dims && a1->intdim==a2->intdim,abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### JOIN OPERANDS (des %d) (%tx U %tx) ###\n",destructive, (intptr_t)a1, (intptr_t)a2);
    t1p_fprint(stdout, man, a1, NULL);
    t1p_fprint(stdout, man, a2, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    /* FILE* stream = fopen("log", "a+"); */
    /* fprintf(stream,"** %zu **\n", pr->dim); */
    t1p_t* res;
    size_t intdim = a1->intdim;
    size_t realdim = a1->dims - a1->intdim;
    if (t1p_is_eq(man, a1, a2)) {
	if (destructive) res = a1;
	else res = t1p_copy(man, a1);
    }
    else if (tbool_or(t1p_is_top(man, a1), t1p_is_top(man, a2)) == tbool_true) {
	if (destructive) {
	    t1p_free(man, a1);
	    res = a1 = t1p_top(man,intdim,realdim);
	} else {
	    res = t1p_top(man,intdim,realdim);
	}
    } else if (t1p_is_bottom(man, a1)) {
	if (destructive) {
	    t1p_free(man, a1);
	    res = a1 = t1p_copy(man,a2);
	} else {
	    res = t1p_copy(man,a2);
	}
    } else if (t1p_is_bottom(man, a2)) {
	if (destructive) res = a1;
	else res = t1p_copy(man,a1);
    } else {
	/* TODO: destructive not yet supported */
	itv_t tmp; itv_init(tmp);
	res = t1p_alloc(man, intdim, realdim);
	/* update res->box */
	for (i=0; i<(intdim+realdim); i++) itv_join(res->box[i], a1->box[i], a2->box[i]);

	if (a1->hypercube && a2->hypercube) {
	    for (i=0; i<(intdim+realdim); i++) {
		//printf("%d: ",i);
		if (t1p_aff_is_bottom(pr, a1->paf[i])) res->paf[i] = a2->paf[i];
		else if (t1p_aff_is_bottom(pr, a2->paf[i])) res->paf[i] = a1->paf[i];
		else if (t1p_aff_is_top(pr, a1->paf[i]) || t1p_aff_is_top(pr, a2->paf[i])) res->paf[i] = pr->top;
		/*
		else if (t1p_aff_is_eq(pr, a1->paf[i], a2->paf[i])) {
		    fprintf(stream,"** res = x **\n");
		    res->paf[i] = a1->paf[i];
		} else if (t1p_aff_is_leq_constrained(pr, a2->paf[i], a1->paf[i], a2,a1)) {
		    fprintf(stream,"** res = x **\n");
		    res->paf[i] = a1->paf[i];
		} else if (t1p_aff_is_leq_constrained(pr, a1->paf[i], a2->paf[i], a1, a2)) {
		    fprintf(stream,"** res = y **\n");
		    res->paf[i] = a2->paf[i];
		    *
		    res->paf[i] = t1p_aff_alloc_init(pr);
		    itv_set(res->paf[i]->c, a2->paf[i]->c);
		    itv_set(res->paf[i]->itv, a2->paf[i]->itv);
		    t1p_aaterm_t* p = NULL;
		    for(p = a2->paf[i]->q; p; p=p->n) {
			t1p_aff_nsym_add(pr, res->paf[i], p->coeff, p->pnsym);
			if (p->pnsym->type == UN) {
			    res->paf[i]->end->pnsym = pr->mubGlobal.p[p->pnsym->index].y;
			}
		    }
		    *
		} */ else {
		    if (itv_has_infty_bound(a1->box[i]) || itv_has_infty_bound(a2->box[i])) {
			/* Do nothing, the join of concretisations is already done and stored in res->box */
			res->paf[i] = t1p_aff_alloc_init(pr);
			itv_set(res->paf[i]->c, res->box[i]);
		    } else {
			/* join two affine form expressions */
			itv_set(a1->paf[i]->itv, a1->box[i]);
			itv_set(a2->paf[i]->itv, a2->box[i]);
			res->paf[i] = t1p_aff_join_constrained6(pr, a1->paf[i], a2->paf[i], a1, a2, res);
		    }
		}
		//printf("%d",i);itv_print(a1->box[i]); printf("\t");itv_print(a2->box[i]);printf("\n");
		res->paf[i]->pby++;
	    }

	} else {
	    size_t k = 0;
	    ap_dim_t j = 0;
	    size_t dims1 = t1p_nsymcons_get_dimension(pr, a1);
	    size_t dims2 = t1p_nsymcons_get_dimension(pr, a2);
	    if (dims1 && dims2) {
		size_t dim2 = 0;
		/* au pire il y a toute la liste de l'un a rajouter dans l'autre */
		ap_dimchange_t* dimchange2 = ap_dimchange_alloc(0, dims2);
		/* par defaut, res->size = 128 lors de sa creation. verifier que la taille est ok avant de faire la copie */
		if (dims1 > res->size) {
		    res->nsymcons = (ap_dim_t*)realloc(res->nsymcons, (dims1)*sizeof(ap_dim_t));
		    res->gamma = (ap_interval_t**)realloc(res->gamma, (dims1)*sizeof(ap_interval_t*));
		    for (k=res->size;k<dims1;k++) res->gamma[k] = NULL;
		    res->size = dims1;
		}
		res->nsymcons = memcpy((void *)res->nsymcons, (const void *)a1->nsymcons, dims1*sizeof(ap_dim_t));
		//for (i=0; i<dims1; i++) res->gamma[i] = ap_interval_alloc_set(a1->gamma[i]);
		//for (i=0; i<dims1; i++) res->gamma[i] = NULL;
		ap_abstract0_free(pr->manNS, res->abs);

		res->abs = ap_abstract0_copy(pr->manNS, a1->abs);
		for (k=0; k<dims1; k++) {
		    if (!t1p_nsymcons_get_dimpos(pr, &j, a1->nsymcons[k], a2)) {
			dimchange2->dim[dim2] = j;
			dim2++;
		    }
		}
		dimchange2->realdim = dim2;
		size_t dim1 = 0;
		for (k=0; k<dims2; k++) t1p_insert_constrained_nsym(pr, &j, a2->nsymcons[k], res);

		/* destructive, without projection (new dimension set to top) */
		ap_abstract0_add_dimensions(pr->manNS, true, a2->abs, dimchange2, false);

		ap_abstract0_join(pr->manNS, true, res->abs, a2->abs);

		/* update res->gamma */
		t1p_update_nsymcons_gamma(pr, res);

		ap_dimchange_add_invert(dimchange2);
		ap_abstract0_remove_dimensions(pr->manNS, true, a2->abs, dimchange2);

		dimchange2->realdim = dims2; ap_dimchange_free(dimchange2);

		size_t nsymcons_size = t1p_nsymcons_get_dimension(pr, res);
		pr->dimtoremove = (ap_dim_t*)realloc(pr->dimtoremove, (nsymcons_size)*sizeof(ap_dim_t));
		memset((void *)pr->dimtoremove, (int)0, nsymcons_size*sizeof(int));
	    } else {
		/* res->abs is a hypercube */
	    }
	    for (i=0; i<(intdim+realdim); i++) {
		if (t1p_aff_is_bottom(pr, a1->paf[i])) res->paf[i] = a2->paf[i];
		else if (t1p_aff_is_bottom(pr, a2->paf[i])) res->paf[i] = a1->paf[i];
		else if (t1p_aff_is_top(pr, a1->paf[i]) || t1p_aff_is_top(pr, a2->paf[i])) res->paf[i] = pr->top;
		else if (t1p_aff_is_eq(pr, a1->paf[i], a2->paf[i])) res->paf[i] = a1->paf[i];
		else {
		    if (itv_has_infty_bound(a1->box[i]) || itv_has_infty_bound(a2->box[i])) {
			/* Do nothing, the join of concretisations is already done and stored in res->box */
			res->paf[i] = t1p_aff_alloc_init(pr);
			itv_set(res->paf[i]->c, res->box[i]);
		    } else {
			/* join two affine form expressions */
			itv_set(a1->paf[i]->itv, a1->box[i]);
			itv_set(a2->paf[i]->itv, a2->box[i]);
			res->paf[i] = t1p_aff_join_constrained6(pr, a1->paf[i], a2->paf[i], a1, a2, res);
		    }
		}
		res->paf[i]->pby++;
	    }

	    man->result.flag_best = tbool_top;
	    man->result.flag_exact = tbool_top;
	}
	pr->mubGlobal.cx = NULL;
	pr->mubGlobal.cy = NULL;
	free(pr->mubGlobal.p);
	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_top;
	itv_clear(tmp);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;


#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of JOIN (des %d) [%tx] ###\n",destructive, (intptr_t)res);
    t1p_fprint(stdout, man, res, NULL);
    /* for (i=0; i<intdim+realdim;i++) */
    /*   {fprintf(stream,"%zu :",i);itv_fprint(stream,res->box[i]); fprintf(stream,"\n");} */
    /* fprintf(stream,"** %zu **\n", pr->dim); */
    /* fprintf(stream, "### ### ###\n"); */
    /* fclose(stream); */
#endif

    return res;
}

