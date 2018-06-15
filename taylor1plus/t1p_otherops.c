/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#include "t1p_internal.h"
#include "t1p_representation.h"
#include "t1p_meetjoin.h"
#include "t1p_constructor.h"

#include "ap_manager.h"

#include "t1p_otherops.h"

/* Other functions */
/*******************/

t1p_t* t1p_forget_array(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_dim_t* tdim, size_t size,
		bool project)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_FORGET_ARRAY);
    t1p_t* res;
    size_t i;

    man->result.flag_best = true;
    man->result.flag_exact = true;

    res = destructive ? a : t1p_copy(man,a);
    if (project){
	for (i=0;i<size;i++){
	    t1p_aff_check_free(pr, res->paf[tdim[i]]);
	    res->paf[tdim[i]] = t1p_aff_alloc_init(pr);
	    res->paf[tdim[i]]->pby++;
	}
    }
    else {
	for (i=0;i<size;i++){
	    t1p_aff_check_free(pr, res->paf[tdim[i]]);
	    res->paf[tdim[i]] = pr->top;
	    res->paf[tdim[i]]->pby++;
        // The field box used for the reduced product was not set. 
        // Detected by Alexandra-Olimpia Bugariu and fixed by Gagandeep Singh on June 7th 2018
        // TODO: Ideally, the box field should be of type abstract0_box to perform the reduced product in a cleaner way 
        itv_set_top(res->box[i]);
	}
    }
    return res;
}

t1p_t t1p_expand(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_var_t var,
		ap_var_t* tvar, size_t size)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_EXPAND);
    not_implemented();
}

t1p_t t1p_fold(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_var_t* tvar, size_t size)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_FOLD);
    not_implemented();
}

t1p_t* t1p_widening(ap_manager_t* man, t1p_t* a1, t1p_t* a2)
{
    CALL();
    size_t i = 0;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_JOIN);
    arg_assert(a1->dims==a2->dims && a1->intdim==a2->intdim,abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### WIDENING OPERANDS ###\n");
    t1p_fprint(stdout, man, a1, NULL);
    t1p_fprint(stdout, man, a2, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_t* res;
    size_t intdim = a1->intdim;
    size_t realdim = a1->dims - a1->intdim;
    if (t1p_is_eq(man, a1, a2)) {
	res = t1p_copy(man, a1);
    }
    else if (tbool_or(t1p_is_top(man, a1), t1p_is_top(man, a2)) == tbool_true) {
	res = t1p_top(man,intdim,realdim);
    } else if (t1p_is_bottom(man, a1)) {
	res = t1p_copy(man,a2);
    } else if (t1p_is_bottom(man, a2)) {
	res = t1p_copy(man,a1);
    } else {
	itv_t tmp; itv_init(tmp);
	res = t1p_alloc(man, intdim, realdim);
	/* update res->box */
	for (i=0; i<(intdim+realdim); i++) itv_widening(res->box[i], a1->box[i], a2->box[i]);
	for (i=0; i<(intdim+realdim); i++) {
	    if (t1p_aff_is_bottom(pr, a1->paf[i])) res->paf[i] = a2->paf[i];
	    else if (t1p_aff_is_bottom(pr, a2->paf[i])) res->paf[i] = a1->paf[i];
	    else if (t1p_aff_is_top(pr, a1->paf[i]) || t1p_aff_is_top(pr, a2->paf[i])) res->paf[i] = pr->top;
	    else if (t1p_aff_is_eq(pr, a1->paf[i], a2->paf[i])) res->paf[i] = a1->paf[i];
	    else {
		if (itv_has_infty_bound(res->box[i])) {
		    /* Do nothing, the join of concretisations is already done and stored in res->box */
		    res->paf[i] = t1p_aff_alloc_init(pr);
		    itv_set(res->paf[i]->c, res->box[i]);
		} else {
		    /* join two affine form expressions */
		    itv_set(a1->paf[i]->itv, a1->box[i]);
		    itv_set(a2->paf[i]->itv, a2->box[i]);
		    res->paf[i] = t1p_aff_widening_constrained6(pr, a1->paf[i], a2->paf[i], a1, a2, res);
		}
	    }
	    res->paf[i]->pby++;
	}
	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_top;
	itv_clear(tmp);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;

#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of WIDENING ###\n");
    t1p_fprint(stdout, man, res, NULL);
    fprintf(stdout, "### ### ###\n");
#endif

    return res;
}

t1p_t* t1p_widening_old(ap_manager_t* man, t1p_t* a1, t1p_t* a2)
{
    CALL();
    size_t i = 0;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_JOIN);
    arg_assert(a1->dims==a2->dims && a1->intdim==a2->intdim,abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### WIDENING OPERANDS ###\n");
    t1p_fprint(stdout, man, a1, NULL);
    t1p_fprint(stdout, man, a2, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_t* res;
    size_t intdim = a1->intdim;
    size_t realdim = a1->dims - a1->intdim;
    if (t1p_is_eq(man, a1, a2)) {
	res = t1p_copy(man, a1);
    }
    else if (tbool_or(t1p_is_top(man, a1), t1p_is_top(man, a2)) == tbool_true) {
	res = t1p_top(man,intdim,realdim);
    } else if (t1p_is_bottom(man, a1)) {
	res = t1p_copy(man,a2);
    } else if (t1p_is_bottom(man, a2)) {
	res = t1p_copy(man,a1);
    } else {
	itv_t tmp; itv_init(tmp);
	res = t1p_alloc(man, intdim, realdim);
	/* update res->box */
	for (i=0; i<(intdim+realdim); i++) itv_join(res->box[i], a1->box[i], a2->box[i]);
	if (a1->hypercube && a2->hypercube) {
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
			res->paf[i] = t1p_aff_widening_constrained6(pr, a1->paf[i], a2->paf[i], a1, a2, res);
		    }
		}
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
		if (dims1 > res->size) {
		    res->nsymcons = (ap_dim_t*)realloc(res->nsymcons, (dims1)*sizeof(ap_dim_t));
		    res->gamma = (ap_interval_t**)realloc(res->gamma, (dims1)*sizeof(ap_interval_t*));
		    for (k=res->size;k<dims1;k++) res->gamma[k] = NULL;
		    res->size = dims1;
		}
		res->nsymcons = memcpy((void *)res->nsymcons, (const void *)a1->nsymcons, dims1*sizeof(ap_dim_t));
		for (i=0; i<dims1; i++) res->gamma[i] = ap_interval_alloc_set(a1->gamma[i]);
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
			res->paf[i] = t1p_aff_widening_constrained6(pr, a1->paf[i], a2->paf[i], a1, a2, res);
		    }
		}
		res->paf[i]->pby++;
	    }

	    man->result.flag_best = tbool_top;
	    man->result.flag_exact = tbool_top;
	}
	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_top;
	itv_clear(tmp);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;

#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of WIDENING ###\n");
    t1p_fprint(stdout, man, res, NULL);
    fprintf(stdout, "### ### ###\n");
#endif

    return res;
}

t1p_t t1p_closure(ap_manager_t* man, bool destructive, t1p_t* a)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_CLOSURE);
    not_implemented();
}

void ap_abstract1_aff_build(ap_manager_t* man, ap_abstract1_t* abstract1, ap_var_t var, unsigned int index, ap_interval_t *interval, bool isunion)
    // TODO TODO TODO : est ce que cette fonction sert tjs ?
{
    CALL();
    itv_t itv;
    itv_init(itv);
    t1p_internal_t* pr = (t1p_internal_t*)man->internal;
    ap_dim_t dim = ap_environment_dim_of_var(abstract1->env,var);
    t1p_t* abs = (t1p_t*)abstract1->abstract0->value;
    itv_set_ap_interval(pr->itv, itv, interval);
    /* on admet que les formes sont affines autrement dit pas réduite à un intervalle avec une borne infinie */
    if (pr->dim <= index) t1p_aff_nsym_create(pr, abs->paf[dim], itv, isunion ? UN : IN);
    else t1p_aff_build(pr, abs->paf[dim], itv, index);
    //if (isunion) abs->paf[dim]->lastu = abs->paf[dim]->end;
    itv_clear(itv);
}

/*
void ap_abstract1_ns_meet_lincons_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_lincons0_array_t* lincons)
{
    t1p_internal_t* pr = (t1p_internal_t*)man->internal;
    t1p_t* abs = (t1p_t*)abstract1->abstract0->value;
    abs->lincons = *lincons;
    if (abs->gamma == NULL) abs->gamma = itv_array_alloc(pr->dim);
    else {
	abs->gamma = (itv_t*)realloc(abs->gamma, (pr->dim)*sizeof(itv_t));
	itv_init_array(abs->gamma, pr->dim);
    }
    ap_abstract0_t* abs1 = ap_abstract0_of_lincons_array(pr->manNS, 0, pr->dim, &(abs->lincons));
    ap_abstract0_t* abs2 = ap_abstract0_meet_lincons_array(pr->manNS, false, abs1, &(pr->moo));
    ap_interval_t** ap_interval_array = ap_abstract0_to_box(pr->manNS, abs2);
    itv_array_set_ap_interval_array(pr->itv, &(abs->gamma), ap_interval_array, pr->dim);
    ap_interval_array_free(ap_interval_array, pr->dim);
    ap_abstract0_free(pr->manNS, abs1);
    ap_abstract0_free(pr->manNS, abs2);
}
*/

void ap_abstract1_ns_meet_box_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_interval_t** box, size_t size)
{
    CALL();
    size_t i = 0;
    t1p_internal_t* pr = (t1p_internal_t*)man->internal;
    t1p_t* a = (t1p_t*)abstract1->abstract0->value;
    int cmp = 2;

    if (size != pr->dim) fatal("dimension of box and number of used noise symbols mismatch\n");


    size_t k = 0;
    ap_dim_t dim = 0;
    for(i=0; i<size; i++) {
	cmp = ap_interval_cmp(box[i], pr->ap_muu);
	if (cmp == -1) {
	    t1p_insert_constrained_nsym(pr, &dim, i, a);
	    a->gamma[dim] = ap_interval_alloc_set(box[i]);
	    k++;
	}
    }
    if (k) {
	a->hypercube = false; 
	a->abs = ap_abstract0_of_box(pr->manNS, 0, k, a->gamma);
    }
}

/*
ap_interval_t* t1p_create_pos(ap_manager_t* man)
{
    CALL();
    t1p_internal_t* pr = (t1p_internal_t*)man->internal;
    ap_interval_t* ap_interval = ap_interval_alloc();
    itv_t pos;
    itv_set_top(pos);
    bound_init(pos->inf);
    ap_interval_set_itv(pr->itv, ap_interval, pos);
    return ap_interval;
}
*/
