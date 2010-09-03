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

#include "t1p_meetjoin.h"

/* Meet and Join */
/*****************/

/************************************************/
/* 1.Meet 					*/
/************************************************/
t1p_t* t1p_meet(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2)
/* TODO: destructive not used */
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man,AP_FUNID_MEET);
    arg_assert(a1->dims==a2->dims && a1->intdim==a2->intdim,abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### MEET OPERANDS (destructive %d)###\n",destructive);
    t1p_fprint(stdout, man, a1, 0x0);
    t1p_fprint(stdout, man, a2, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_t* res = NULL;
    size_t i = 0;
    size_t realdim = a1->dims - a1->intdim;
    size_t intdim = a1->intdim;
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    if (t1p_is_eq(man, a1, a2)) res = t1p_copy(man, a1);
    else if (tbool_or(t1p_is_bottom(man, a1), t1p_is_bottom(man, a2)) == tbool_true) {
	res = t1p_bottom(man, intdim, realdim);
    } else if (t1p_is_top(man, a1) == tbool_true) {
	res = t1p_copy(man, a2);
    } else if (t1p_is_top(man, a2) == tbool_true) {
	res = t1p_copy(man, a1);
    } else {
	res = t1p_alloc(man, intdim, realdim);
	//ap_abstract0_free(pr->manNS, res->abs);
	size_t k = 0;
	ap_dim_t j = 0;
	size_t dims1 = t1p_nsymcons_get_dimension(pr, a1);
	size_t dims2 = t1p_nsymcons_get_dimension(pr, a2);
	if (dims1 && dims2) {
	    size_t dim2 = 0;
	    /* au pire il y a toute la liste de l'un a rajouter dans l'autre */
	    ap_dimchange_t* dimchange2 = ap_dimchange_alloc(0, dims2);
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

	    ap_abstract0_meet(pr->manNS, true, res->abs, a2->abs);

	    /* update res->gamma */
	    t1p_update_nsymcons_gamma(pr, res);

	    ap_dimchange_add_invert(dimchange2);
	    ap_abstract0_remove_dimensions(pr->manNS, true, a2->abs, dimchange2);

	    dimchange2->realdim = dims2; ap_dimchange_free(dimchange2);
	} else if (dims1) {
	    res->nsymcons = memcpy((void *)res->nsymcons, (const void *)a1->nsymcons, dims1*sizeof(ap_dim_t));
	    for (i=0; i<dims1; i++) res->gamma[i] = ap_interval_alloc_set(a1->gamma[i]);
	    res->abs = ap_abstract0_copy(pr->manNS, a1->abs);
	} else if (dims2) {
	    res->nsymcons = memcpy((void *)res->nsymcons, (const void *)a2->nsymcons, dims2*sizeof(ap_dim_t));
	    for (i=0; i<dims2; i++) res->gamma[i] = ap_interval_alloc_set(a2->gamma[i]);
	    res->abs = ap_abstract0_copy(pr->manNS, a2->abs);
	} else {
	    /* non constrained abstract objects */
	}

	//res->abs = ap_abstract0_meet(pr->manNS, false, a1->abs, a2->abs);
	/* update res->gamma */
	//t1p_update_nsymcons_gamma(pr, res);
	itv_t tmp; itv_init(tmp);
	for (i=0; i<intdim+realdim; i++) {
	    /* update res->box */
	    itv_meet(pr->itv, res->box[i], a1->box[i], a2->box[i]);
	    if ((a1->paf[i]->q == NULL) && (a2->paf[i]->q == NULL)) {
		itv_meet(pr->itv, tmp, a1->paf[i]->c, a2->paf[i]->c);
		if (itv_is_bottom(pr->itv, tmp)) {
		    t1p_free(man, res);
		    res = t1p_bottom(man, intdim, realdim);
		    break;
		} else if (itv_is_top(tmp)) res->paf[i] = pr->top;
		else if (itv_has_finite_bound(tmp)) {
		    res->paf[i] = t1p_aff_alloc_init(pr);
		    t1p_aff_add_itv(pr, res->paf[i], tmp, IN);
		} else {
		    res->paf[i] = t1p_aff_alloc_init(pr);
		    itv_set(res->paf[i]->c, tmp);
		}
		res->paf[i]->pby++;
	    } else if (t1p_aff_is_eq(pr, a1->paf[i], a2->paf[i])) {
		res->paf[i] = a1->paf[i];
		res->paf[i]->pby++;
	    } else if (itv_is_point(pr->itv, res->box[i])) {
		res->paf[i] = t1p_aff_alloc_init(pr);
		itv_set(res->paf[i]->c, res->box[i]);
	    } else {
		fprintf(stdout, "destructive ? %d\n",destructive);
		t1p_fprint(stdout, man, a1, 0x0);
		t1p_fprint(stdout, man, a2, 0x0);
		//not_implemented();
		/* return a top instead */
		res->paf[i] = pr->top;
		res->paf[i]->pby++;
		itv_set_top(res->box[i]);
	    }
	}
	itv_clear(tmp);
    }
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of MEET ###\n");
    t1p_fprint(stdout, man, res, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

/**************/
/* meet array */
/**************/
t1p_t* t1p_meet_array(ap_manager_t* man, t1p_t** tab, size_t size)
{
	CALL();
	return (t1p_t*)ap_generic_meet_array(man, (void**)tab, size);
}

static inline bool t1p_eval_lincons_array(t1p_internal_t* pr, size_t* hash, t1p_aff_t** eps_linexp_array, ap_lincons0_array_t* eps_array, ap_lincons0_array_t* var_lincons_array, ap_lincons0_array_t* array, t1p_t* a)
{
    CALL();
    /* Requires: an array of varibale constraints */
    /* Ensures: an array of epsilons constraints */
    /* Ensures: an array of var constraints */
    bool res = true;
    size_t i = 0;
    size_t j = 0;
    size_t eps_dim = 0;	/* count the number of constraints that involves nsym */
    size_t var_dim = 0; /* count the number of constraints that doesn't involve nsym, either simplified when evaluating the constraint or doesn't exist from the begining */
    ap_linexpr0_t* linexpr0 = NULL;
    t1p_aff_t *tmp1, *tmp2, *tmp3, *sum;
    for (i=0; i<array->size; i++) {
	if (ap_lincons0_is_unsat(&(array->p[i]))) {
	    res = false;
	    break; /* break the for loop */
	} else {
	    /* recuperer l'expression lineaire et l'interpreter en eps */
	    linexpr0 = array->p[i].linexpr0;
	    sum = t1p_aff_alloc_init(pr);
	    itv_set_ap_coeff(pr->itv, sum->c, &(linexpr0->cst));
	    tmp1 = t1p_aff_alloc_init(pr);
	    switch (linexpr0->discr) {
		case AP_LINEXPR_DENSE :
		    for (j=0; j<linexpr0->size; j++) {
			/* j corresponds to the dimension of the variable */
			if (ap_coeff_zero(&(linexpr0->p.coeff[j]))) {;}
			else {
			    itv_set_ap_coeff(pr->itv, tmp1->c, &(linexpr0->p.coeff[j]));
			    tmp2 = t1p_aff_mul_constrained(pr, tmp1, a->paf[j], a);
			    tmp3 = t1p_aff_add(pr, tmp2, sum, a);
			    t1p_aff_free(pr, sum);
			    sum = tmp3;
			    t1p_aff_clear(pr, tmp1);
			    t1p_aff_free(pr, tmp2);
			}
		    }
		    break;
		case AP_LINEXPR_SPARSE :
		    for (j=0; j<linexpr0->size; j++) {
			/* j corresponds to the index of the linterm */
			if (ap_coeff_zero(&(linexpr0->p.linterm[j].coeff))) {;}
			else {
			    itv_set_ap_coeff(pr->itv, tmp1->c, &(linexpr0->p.linterm[j].coeff));
			    tmp2 = t1p_aff_mul_constrained(pr, tmp1, a->paf[linexpr0->p.linterm[j].dim], a);
			    tmp3 = t1p_aff_add(pr, tmp2, sum, a);
			    t1p_aff_free(pr, sum);
			    sum = tmp3;
			    t1p_aff_clear(pr, tmp1);
			    t1p_aff_free(pr, tmp2);
			}
		    }
		    break;
		default:
		    fatal("unknown linexpr0 Discriminant");
		    break;
	    }
	    eps_linexp_array[i] = sum;
	    if (sum->l == 0) {
		/* this is a variable constraint */
		/* TODO: now duplicate, not optimal  */
		if (ap_lincons0_is_unsat(&array->p[i])) {
		    res = false;
		    break;
		} else {
		    var_lincons_array->p[var_dim] = ap_lincons0_copy(&(array->p[i]));
		    var_dim++;
		}
	    } else {
		/* transformer cette forme en une contrainte lineaire (DENSE ou SPARSE a voir) */
		/* "sum" contains an affine form to transform into a constraint */
		ap_linexpr0_t* eps_linexpr0 = NULL;
		ap_coeff_t* coeff = ap_coeff_alloc(AP_COEFF_INTERVAL);
		ap_coeff_set_itv(pr->itv, coeff, sum->c);
		eps_linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
		ap_linexpr0_set_cst(eps_linexpr0, coeff);
		t1p_aaterm_t *p;
		/* sum->l donne la taille de la forme affine (ne comptabilise pas la constante) */
		size_t epscons_size = t1p_nsymcons_get_dimension(pr, a);
		/* au pire tous les symboles de la contrainte seront à rajouter */
		size_t new = 0;
		eps_linexpr0->p.linterm = (ap_linterm_t*)malloc(sum->l*sizeof(ap_linterm_t));
		eps_linexpr0->size = sum->l;
		size_t k = 0;
		ap_dim_t dim = 0;
		for (p=sum->q; p; p=p->n) {
		    ap_coeff_init(&eps_linexpr0->p.linterm[k].coeff, AP_COEFF_INTERVAL);
		    ap_coeff_set_itv(pr->itv, &eps_linexpr0->p.linterm[k].coeff, p->coeff);
		    t1p_insert_constrained_nsym(pr, &dim, p->pnsym->index, a);
		    eps_linexpr0->p.linterm[k].dim = dim;
		    k++;
		}
		ap_scalar_t* scalar = NULL;
		if (array->p[i].scalar) scalar = ap_scalar_alloc_set(array->p[i].scalar);
		eps_array->p[eps_dim] = ap_lincons0_make(array->p[i].constyp, eps_linexpr0, scalar);
		hash[eps_dim] = i;
		eps_dim++;
		ap_coeff_free(coeff);
	    }
	}
    }   
    ap_lincons0_array_resize(eps_array, eps_dim);
    ap_lincons0_array_resize(var_lincons_array, var_dim);
    return res;
}

/**********************/
/* meet lincons array */
/**********************/
t1p_t* t1p_meet_lincons_array(ap_manager_t* man, bool destructive, t1p_t* a, ap_lincons0_array_t* array)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_MEET_LINCONS_ARRAY);
    arg_assert(a && array, abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### MEET LINCONS ARRAY (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, a, NULL);
    ap_lincons0_array_fprint(stdout, array, NULL);
    fprintf(stdout, "### ### ###\n");
#endif

    size_t i = 0;
    ap_tcons0_array_t tcons0_array;
    tcons0_array.size = array->size;
    tcons0_array.p = (ap_tcons0_t*)malloc(array->size*sizeof(ap_tcons0_t));
    for (i=0; i<array->size; i++) {
	tcons0_array.p[i] = ap_tcons0_from_lincons0(&array->p[i]);
    }

    t1p_t* res = t1p_meet_tcons_array(man, destructive, a, &tcons0_array);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT OF MEET LINCONS ARRAY (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, res, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

/********************/
/* meet tcons array */
/********************/
t1p_t* t1p_meet_tcons_array(ap_manager_t* man, bool destructive, t1p_t* a, ap_tcons0_array_t* array)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_MEET_TCONS_ARRAY);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### MEET TCONS ARRAY (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, a, NULL);
    ap_tcons0_array_fprint(stdout, array, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    size_t i = 0;

    /*
    size_t size = array1->size;
    ap_tcons0_array_t arraybis = ap_tcons0_array_make(array1->size);
    for (i=0; i<array1->size; i++) {
	arraybis.p[i] = ap_tcons0_copy(&array1->p[i]);
    }
    ap_tcons0_array_t* array = &arraybis;
    */

    t1p_t* res = destructive ? a : t1p_copy(man, a);
    bool is_bottom = false;
    size_t kmax = 2; /* specifies the maximum number of iterations */

    bool* tchange = (bool*)calloc(2*a->dims, sizeof(bool));
    itv_t box; itv_init(box);
    itv_lincons_array_t itv_lincons_array;
    itv_lincons_array.size = 0;
    itv_lincons_array.p = NULL;
    if (!itv_intlinearize_ap_tcons0_array(pr->itv, &itv_lincons_array, array, res->box, res->intdim)) {
	size_t kmax = 2; /* specifies the maximum number of iterations */
	/* intervalonly is set to false which means try to improve all dimensions, not only the ones with an interval coefficient */
	if (itv_boxize_lincons_array(pr->itv, res->box, tchange, &itv_lincons_array, res->box, res->intdim, kmax, false)) {
	    /* there is some inferred bounds */
	    for (i=0; i<res->dims; i++) {
		if (tchange[2*i] || tchange[2*i + 1]) {
		    if (itv_is_bottom(pr->itv, res->box[i])) {
			is_bottom = true;
			break;
		    } else if (res->paf[i]->q == NULL) {
			t1p_aff_check_free(pr, res->paf[i]);
			res->paf[i] = t1p_aff_alloc_init(pr);
			itv_set(res->paf[i]->c, res->box[i]);
			res->paf[i]->pby++;
		    } /*else if (itv_is_point(pr->itv, res->box[i])) {
			ap_tcons0_array_resize(array, 1+(array->size));
			ap_interval_t* ap_itv = ap_interval_alloc();
			ap_interval_set_itv(pr->itv, ap_itv, res->box[i]);
			array->p[-1+(array->size)] = ap_tcons0_make(AP_CONS_EQ,ap_texpr0_binop(AP_TEXPR_SUB,ap_texpr0_dim(i),ap_texpr0_cst_interval(ap_itv),AP_RTYPE_REAL,AP_RDIR_UP),NULL);
			ap_interval_free(ap_itv);
		    }*/
		}
	    }
	} else {
	    /* nothing change */
	}
	itv_lincons_array_clear(&itv_lincons_array);
    } else {
	/* bottom */
	is_bottom = true;
    }
    if (!is_bottom) {
	/* texpr -> aff forme */
	ap_texpr0_t* texpr0 = NULL;
	t1p_aff_t** aff = malloc(array->size*sizeof(t1p_aff_t*));
	ap_linexpr0_t* linexpr0 = NULL;
	ap_lincons0_t lincons0;
	ap_interval_t cst;
	for (i=0; i<a->dims; i++) itv_set(a->paf[i]->itv, a->box[i]);
	for (i=0; i<array->size; i++) {
	    texpr0 = array->p[i].texpr0;
	    aff[i] = t1p_aff_eval_ap_texpr0(pr, texpr0, a);
	    ap_texpr0_t* toto = ap_texpr0_copy(texpr0);
	    ap_texpr0_free(toto);
	    if (aff[i]->q == NULL) {
		/* only the centers are involved in this constraint, already treated while updating res->box */
	    } else {
		/* infer constraints on noise symbols */
		linexpr0 = t1p_ap_linexpr0_set_aff(pr, aff[i], res);
		lincons0.constyp = array->p[i].constyp;
		lincons0.linexpr0 = linexpr0;
		lincons0.scalar = array->p[i].scalar;
		ap_lincons0_array_t eps_lincons_array;
		eps_lincons_array.size = 1;
		eps_lincons_array.p = &lincons0;
		ap_abstract0_meet_lincons_array(pr->manNS, true, res->abs, &eps_lincons_array);
		if (ap_abstract0_is_bottom(pr->manNS, res->abs)) {
		    is_bottom = true;
		    break;
		}
	    }
	    ap_linexpr0_free(linexpr0);
	}
	/* update res->gamma */
	t1p_update_nsymcons_gamma(pr, res);

	for (i=0; i<array->size; i++) {
	    /* update the abstract object with the new affine forms */
	    if (array->p[i].constyp == AP_CONS_EQ) {
		itv_t dummy; itv_init(dummy);
		t1p_aff_t* tmp, *tmp1;
		size_t j = 0;
		for (j=0; j<res->dims; j++) {
		    if (res->paf[j]->q) {
			t1p_aff_cons_eq_lambda(pr, &dummy, res->paf[j], aff[i], res);
			tmp = t1p_aff_mul_itv(pr, aff[i], dummy);
			itv_set(res->paf[j]->itv, res->box[j]);
			tmp1 = t1p_aff_add(pr, tmp, res->paf[j], res);
			t1p_aff_check_free(pr, res->paf[j]);
			res->paf[j] = tmp1;
			/* update res->box */
			itv_meet(pr->itv, res->box[j], res->box[j], res->paf[j]->itv);
			res->paf[j]->pby++;
			t1p_aff_free(pr, tmp);
		    }
		}
		itv_clear(dummy);
	    } else {
		/* do nothing, just update res->box */
		size_t j = 0;
		for (j=0; j<res->dims; j++) {
		    t1p_aff_bound(pr, box, res->paf[j], res);
		    itv_meet(pr->itv, res->box[j], res->box[j], box);
		}

	    }
	    t1p_aff_free(pr, aff[i]);
	}
	free(aff);
    }
    if (is_bottom) {
	size_t intdim = res->intdim;
	size_t realdim = res->dims - res->intdim;
	t1p_free(man, res);
	res = t1p_bottom(man, intdim, realdim);
    }
    //ap_tcons0_array_clear(array);
    free(tchange);
    itv_clear(box);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT OF MEET TCONS ARRAY (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, res, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

/************************************************/
/* 2.Join 					*/
/************************************************/
t1p_t* t1p_join(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2)
    /* TODO destructive not used  */
{
    CALL();
    size_t i = 0;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_JOIN);
    arg_assert(a1->dims==a2->dims && a1->intdim==a2->intdim,abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### JOIN OPERANDS (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, a1, NULL);
    t1p_fprint(stdout, man, a2, NULL);
    fprintf(stdout, "### ### ###\n");
#endif
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
	} else {
	    size_t k = 0;
	    ap_dim_t j = 0;
	    size_t dims1 = t1p_nsymcons_get_dimension(pr, a1);
	    size_t dims2 = t1p_nsymcons_get_dimension(pr, a2);
	    if (dims1 && dims2) {
		size_t dim2 = 0;
		/* au pire il y a toute la liste de l'un a rajouter dans l'autre */
		ap_dimchange_t* dimchange2 = ap_dimchange_alloc(0, dims2);
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
	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_top;
	itv_clear(tmp);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;

#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of JOIN (des %d) ###\n",destructive);
    t1p_fprint(stdout, man, res, NULL);
    fprintf(stdout, "### ### ###\n");
#endif

    return res;
}

/**************/
/* join array */
/**************/
t1p_t* t1p_join_array(ap_manager_t* man, t1p_t** tab, size_t size)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_JOIN_ARRAY);
    t1p_t* res = (t1p_t*)ap_generic_join_array(man, (void**)tab, size);
    return res;
}

/*****************/
/* add ray array */
/*****************/
t1p_t* t1p_add_ray_array(ap_manager_t* man, bool destructive, t1p_t* a, ap_generator0_array_t* array)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ADD_RAY_ARRAY);
    not_implemented();
}

