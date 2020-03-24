/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#include "t1p_internal.h"
#include "t1p_representation.h"
//#include "t1p_constructor.h"

#include "ap_dimension.h"
#include "ap_lincons0.h"
#include "ap_manager.h"
#include "ap_interval.h"
#include "ap_tcons0.h"
#include "ap_generator0.h"

/****************/
/* Constructors */
/****************/
/* 1.Basic constructors */
t1p_t* t1p_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
    CALL();
    size_t i;
    size_t dims = intdim + realdim;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_BOTTOM);
    t1p_t* res = t1p_alloc(man, intdim, realdim);
    for (i=0; i<dims; i++) {
	res->paf[i] = pr->bot;
	res->paf[i]->pby++;
	itv_set_bottom(res->box[i]);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return res;
}

t1p_t* t1p_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
    CALL();
    size_t i;
    size_t dims = intdim + realdim;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_TOP);
    t1p_t* res = t1p_alloc(man, intdim, realdim);
    for (i=0; i<dims; i++) {
	res->paf[i] = pr->top;
	res->paf[i]->pby++;
	itv_set_top(res->box[i]);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return res;
}

/* Abstract an hypercube defined by the array of intervals of size intdim+realdim */
t1p_t* t1p_of_box(ap_manager_t* man, size_t intdim, size_t realdim, ap_interval_t** tinterval)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_OF_BOX);
    //itv_t* itv_array = itv_array_alloc(intdim+realdim);
    itv_t* itv_array;
    itv_array_set_ap_interval_array(pr->itv, &itv_array, tinterval, intdim+realdim);
    t1p_t* res = t1p_alloc(man,intdim,realdim);
    size_t i = 0;
    for (i=0; i<intdim+realdim; i++) {
	itv_set(res->box[i], itv_array[i]);
	res->paf[i] = t1p_aff_alloc_init(pr);
	if (itv_is_bottom(pr->itv, itv_array[i])) res->paf[i] = pr->bot;
	else if (itv_is_top(itv_array[i])) res->paf[i] = pr->top;
	else if (itv_is_point(pr->itv, itv_array[i])) itv_set(res->paf[i]->c, itv_array[i]);
	else if (itv_has_finite_bound(itv_array[i])) t1p_aff_add_itv(pr, res->paf[i], itv_array[i], IN);
	else itv_set(res->paf[i]->c, itv_array[i]);
	res->paf[i]->pby++;
    }
    itv_array_free(itv_array,intdim+realdim);
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return res;
}

/* 2.Accessors */
/* return dimension (type ap_dimension_t) */
ap_dimension_t t1p_dimension(ap_manager_t* man, t1p_t* a)
{
    CALL();
    ap_dimension_t res;
    res.intdim = a->intdim;
    res.realdim = a->dims - a->intdim;
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return res;
}

/* 3.Tests */
tbool_t t1p_is_bottom(ap_manager_t* man, t1p_t* a)
{
    CALL();
    size_t i;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_IS_BOTTOM);
    bool res = itv_is_bottom(pr->itv, a->box[0]);
    for (i=1; i<a->dims; i++) {
	res &= itv_is_bottom(pr->itv, a->box[i]);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return tbool_of_bool(res);
}

tbool_t t1p_is_top(ap_manager_t* man, t1p_t* a)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_IS_TOP);
    size_t i;
    bool res = itv_is_top(a->box[0]);
    for (i=1; i<a->dims; i++) {
	res &= itv_is_top(a->box[i]);
    }
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return tbool_of_bool(res);
}

tbool_t t1p_is_eq(ap_manager_t* man, t1p_t* a, t1p_t* b)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_IS_EQ);
    arg_assert(a && b && (a->dims == b->dims), abort(););
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    ap_dimension_t dim1 = ap_abstract0_dimension(pr->manNS, a->abs);
    ap_dimension_t dim2 = ap_abstract0_dimension(pr->manNS, b->abs);
    if ( (dim1.intdim != dim2.intdim) || (dim1.realdim != dim2.realdim) ) return tbool_of_bool(false);
    else if (!ap_abstract0_is_eq(pr->manNS, a->abs, b->abs)) return tbool_of_bool(false);
    else if (a == b) return tbool_of_bool(true);
    else {
        size_t i = 0;
        bool res = true;
        for (i=0; i<a->dims; i++) {
            // Previous implementation was over conservative. 
            // Detected by Alexandra-Olimpia Bugariu on June 7th 2018 
            res = t1p_aff_is_eq(pr, a->paf[i], b->paf[i]) && itv_is_eq(a->box[i], b->box[i]);
            if (!res) break;
        }
        return tbool_of_bool(res);
    }
}

tbool_t t1p_is_leq(ap_manager_t* man, t1p_t* a, t1p_t* b)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_IS_LEQ);
    arg_assert(a && b && (a->dims == b->dims), abort(););
#ifdef _T1P_DEBUG
    fprintf(stdout, "### IS LESS or EQUAL %tx < %tx ###\n",(intptr_t)a,(intptr_t)b);
//    t1p_fprint(stdout, man, a, 0x0);
 //   t1p_fprint(stdout, man, b, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    size_t i;
    bool ok;
    tbool_t res;

    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;

    if (t1p_is_bottom(man, a) == tbool_true) res = tbool_true;
    else if (t1p_is_bottom(man, b) == tbool_true) res = tbool_false;
    else if (t1p_is_top(man, b) == tbool_true) res = tbool_true;
    else if  (t1p_is_top(man, a) == tbool_true) res = tbool_false;
    else if (t1p_is_eq(man, a, b)) res = tbool_true;
    else {
	/* General case */
	ok = true;
	if (a->hypercube) {
	    if (b->hypercube) {
		ok = true;
	    } else {
		ok = false;
	    }
	} else {
	    if (b->hypercube) {
		ok = true;
	    } else {
		/* compare interval concretisations of constrained eps */
		size_t sizea = t1p_nsymcons_get_dimension(pr, a);
		size_t sizeb = t1p_nsymcons_get_dimension(pr, b);
		ap_dim_t dima = 0;
		ap_dim_t dimb = 0;
		for (i=0; i<sizea; i++)	{
		    if(t1p_nsymcons_get_dimpos(pr, &dima, i, a)) {
			if (t1p_nsymcons_get_dimpos(pr, &dimb, i, b)) {
			    if (ap_interval_is_leq(a->gamma[dima], b->gamma[dimb])) {
				ok &= true;
			    } else {
				ok &= false;
				break;
			    }
			} else {
			    ok &= true;
			}
		    } else {
			if (t1p_nsymcons_get_dimpos(pr, &dimb, i, b)) {
			    ok &= false;
			    break;
			} else {
			    ok &= true;
			}
		    }
		}
	    }
	}
	if (ok) {
	    for (i=0; i<a->dims; i++) {
		if (!itv_is_leq(a->box[i], b->box[i])) {
		    ok = false;
		    break;
		}
		else if (a->paf[i] != b->paf[i]) {
		    itv_set(a->paf[i]->itv, a->box[i]);
		    itv_set(b->paf[i]->itv, b->box[i]);
		    ok &= t1p_aff_is_leq_constrained(pr, a->paf[i], b->paf[i], a, b);
		}
		/* at the first occurence of "top" or "false" we no longer need to continue */
		//if (!ok) break;
	    }
	}
	res = tbool_of_bool(ok);
    }
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of IS LESS or EQUAL ###\n");
    fprintf(stdout, "### %d ###\n",res);
#endif
    return res;
}


tbool_t t1p_is_dimension_unconstrained(ap_manager_t* man, t1p_t* a, ap_dim_t dim)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_IS_DIMENSION_UNCONSTRAINED);
	not_implemented();
}

tbool_t t1p_sat_tcons(ap_manager_t* man, t1p_t* a, ap_tcons0_t* tcons)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_SAT_TCONS);
	not_implemented();
}

tbool_t t1p_sat_interval(ap_manager_t* man, t1p_t* a, ap_interval_t* interval)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_SAT_INTERVAL);
	not_implemented();
}

tbool_t t1p_sat_lincons(ap_manager_t* man, t1p_t* a, ap_lincons0_t* lincons)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_SAT_LINCONS);
	not_implemented();
}

/* 4.Extraction of properties */
ap_interval_t* t1p_bound_texpr(ap_manager_t* man, t1p_t* a, ap_texpr0_t* expr)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_BOUND_TEXPR);
	not_implemented();
}

ap_interval_t* t1p_bound_dimension(ap_manager_t* man, t1p_t* a, ap_dim_t dim)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_BOUND_DIMENSION);
    ap_interval_t* ap_itv = ap_interval_alloc();
    ap_interval_set_itv(pr->itv, ap_itv, a->box[dim]);
    return ap_itv;
}

ap_interval_t* t1p_bound_linexpr(ap_manager_t* man, t1p_t* a, ap_linexpr0_t* expr)
{
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_BOUND_LINEXPR);
	not_implemented();
}

ap_interval_t** t1p_to_box(ap_manager_t* man, t1p_t* a)
{
    /* operation couteuse */
    /* la transformation ap_interval <-> itv est couteuse */
    /* TODO:savoir si la transformation est exact et mettre a jour la valeur de flag_exact */
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_TO_BOX);
    ap_interval_t **res = (ap_interval_t**)malloc(a->dims*sizeof(ap_interval_t*));
    size_t i = 0;
    //itv_t tmp; itv_init(tmp);
    for (i=0; i<a->dims; i++) {
	//t1p_aff_boxize(pr, tmp, a->paf[i], a);
	res[i] = ap_interval_alloc();
	ap_interval_set_itv(pr->itv, res[i], a->box[i]);
	//ap_interval_set_itv(pr->itv, res[i], tmp);
    }
    //itv_clear(tmp);
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    return res;
}

ap_tcons0_array_t t1p_to_tcons_array(ap_manager_t* man, t1p_t* a)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_TO_TCONS_ARRAY);
    not_implemented();
}

ap_lincons0_array_t t1p_to_lincons_array(ap_manager_t* man, t1p_t* a)
    /* Taken from box_to_lincons_array */
    /* TODO: use constraints in eps domain to deduce constraints on variable ? */
{
    CALL();
    size_t i;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_TO_LINCONS_ARRAY);
    ap_lincons0_array_t array;

    size_t nbdims = a->dims;

    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
    if (nbdims==0){
	array = ap_lincons0_array_make(0);
    }
    else if (t1p_is_bottom(man,a) == tbool_true){
	array = ap_lincons0_array_make(1);
	array.p[0] = ap_lincons0_make_unsat();
    }
    else {
	size_t size;
	ap_linexpr0_t* expr;
	ap_scalar_t* scalar;
	bool point;

	size = 0;
	//itv_t* tmp = itv_array_alloc(nbdims);
	for (i=0;i<nbdims;i++){
	    //t1p_aff_boxize(pr, tmp[i], a->paf[i], a);
	    //if (!bound_infty(tmp[i]->inf)) size++;
	    if (!bound_infty(a->box[i]->inf)) size++;
	    point = itv_is_point(pr->itv,a->box[i]);
	    if (!point && !bound_infty(a->box[i]->sup)) size++;
	    //point = itv_is_point(pr->itv,tmp[i]);
	    //if (!point && !bound_infty(tmp[i]->sup)) size++;
	}
	array = ap_lincons0_array_make(size);
	size = 0;
	for (i=0;i<nbdims;i++){
	    point = false;
	    //if (!bound_infty(tmp[i]->inf)){
	    if (!bound_infty(a->box[i]->inf)){
		expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
		ap_coeff_set_scalar_int(&expr->p.linterm[0].coeff, 1);
		expr->p.linterm[0].dim = i;

		ap_coeff_reinit(&expr->cst,AP_COEFF_SCALAR,AP_SCALAR_DOUBLE);
		scalar = expr->cst.val.scalar;
		//ap_scalar_set_bound(scalar,tmp[i]->inf);
		ap_scalar_set_bound(scalar,a->box[i]->inf);

		point = itv_is_point(pr->itv,a->box[i]);
		//point = itv_is_point(pr->itv,tmp[i]);
		array.p[size].constyp = point ? AP_CONS_EQ : AP_CONS_SUPEQ;
		array.p[size].linexpr0 = expr;
		size++;
	    }
	    //if (!point && !bound_infty(tmp[i]->sup)){
	    if (!point && !bound_infty(a->box[i]->sup)){
		expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
		ap_coeff_set_scalar_int(&expr->p.linterm[0].coeff, -1);
		expr->p.linterm[0].dim = i;

		ap_coeff_reinit(&expr->cst,AP_COEFF_SCALAR,AP_SCALAR_DOUBLE);
		ap_scalar_set_bound(expr->cst.val.scalar,a->box[i]->sup);
		//ap_scalar_set_bound(expr->cst.val.scalar,tmp[i]->sup);

		array.p[size].constyp = AP_CONS_SUPEQ;
		array.p[size].linexpr0 = expr;
		size++;
	    }
	}
	//itv_array_free(tmp, nbdims);
	}
	//fprintf(stdout, "end lincons\n");
	return array;
	}

ap_generator0_array_t t1p_to_generator_array(ap_manager_t* man, t1p_t* a)
  {
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_TO_GENERATOR_ARRAY);
    not_implemented();
  }
