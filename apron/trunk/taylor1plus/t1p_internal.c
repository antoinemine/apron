/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2018 Khalil Ghorbal

*/


#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//#include <sys/mman.h>

#include "ap_generic.h"

#include "t1p_internal.h"
#include "t1p_itv_utils.h"

#include "t1p_representation.h"
#include "t1p_constructor.h"
#include "t1p_meetjoin.h"
#include "t1p_assign.h"
#include "t1p_resize.h"
#include "t1p_otherops.h"

//ap_manager_t* t1p_manager_alloc(ap_manager_t* manNS)
ap_manager_t* t1p_manager_alloc(void)
{
	CALL();
	ap_manager_t* man;
	void** funptr;

	/*if (!ap_fpu_init()) {
		fprintf(stderr,"t1p_manager_alloc cannot change the FPU rounding mode\n");
		return NULL;
	}*/
	ap_manager_t* manNS = box_manager_alloc();
	//ap_manager_t* manNS = oct_manager_alloc();
	//ap_manager_t* manNS = pk_manager_alloc(true);
	t1p_internal_t *t1p = t1p_internal_alloc(manNS);

	man = ap_manager_alloc("Taylor1+",/* Library name */
			"0.8", /* version */
			t1p, /* internal structure */
			(void (*)(void*))t1p_internal_free /* free function for internal */
			);

	funptr = man->funptr;

	/* Internal representation */
	/***************************/
	/* 1.Memory */
	funptr[AP_FUNID_COPY] = &t1p_copy;
	funptr[AP_FUNID_FREE] = &t1p_free;
	/*funptr[AP_FUNID_SIZE] = &t1p_size;*/
	funptr[AP_FUNID_ASIZE] = &t1p_size;
	/* 2.Control of internal representation */
	funptr[AP_FUNID_MINIMIZE] = &t1p_minimize;
	funptr[AP_FUNID_CANONICALIZE] = &t1p_canonicalize;
	funptr[AP_FUNID_HASH] = &t1p_hash;
	funptr[AP_FUNID_APPROXIMATE] = &t1p_approximate;
	/* 3.Printing */
	funptr[AP_FUNID_FPRINT] = &t1p_fprint;
	funptr[AP_FUNID_FPRINTDIFF] = &t1p_fprintdiff;
	funptr[AP_FUNID_FDUMP] = &t1p_fdump;
	/* 4.Serialisation */
	funptr[AP_FUNID_SERIALIZE_RAW] = &t1p_serialize_raw;
	funptr[AP_FUNID_DESERIALIZE_RAW] = &t1p_deserialize_raw;

	/* Constructors */
	/****************/
	/* 1.Basic constructors */
	funptr[AP_FUNID_BOTTOM] = &t1p_bottom;
	funptr[AP_FUNID_TOP] = &t1p_top;
	funptr[AP_FUNID_OF_BOX] = &t1p_of_box;
	/* 2.Accessors */
	funptr[AP_FUNID_DIMENSION] = &t1p_dimension;
	/* 3.Tests */
	funptr[AP_FUNID_IS_BOTTOM] = &t1p_is_bottom;
	funptr[AP_FUNID_IS_TOP] = &t1p_is_top;
	funptr[AP_FUNID_IS_LEQ] = &t1p_is_leq;
	funptr[AP_FUNID_IS_EQ] = &t1p_is_eq;
	funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &t1p_is_dimension_unconstrained;
	funptr[AP_FUNID_SAT_TCONS] = &t1p_sat_tcons; /*  */
	funptr[AP_FUNID_SAT_INTERVAL] = &t1p_sat_interval;
	funptr[AP_FUNID_SAT_LINCONS] = &t1p_sat_lincons;
	/* 4.Extraction of properties */
	funptr[AP_FUNID_BOUND_TEXPR] = &t1p_bound_texpr; /*  */
	funptr[AP_FUNID_BOUND_DIMENSION] = &t1p_bound_dimension;
	funptr[AP_FUNID_BOUND_LINEXPR] = &t1p_bound_linexpr;
	funptr[AP_FUNID_TO_BOX] = &t1p_to_box;
	funptr[AP_FUNID_TO_TCONS_ARRAY] = &t1p_to_tcons_array; /*  */
	funptr[AP_FUNID_TO_LINCONS_ARRAY] = &t1p_to_lincons_array;
	funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &t1p_to_generator_array;

	/* Meet and Join */
	/*****************/
	/* 1.Meet */
	funptr[AP_FUNID_MEET] = &t1p_meet; /* */
	funptr[AP_FUNID_MEET_ARRAY] = &t1p_meet_array; /*  */
	funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &t1p_meet_lincons_array; /*  */
	funptr[AP_FUNID_MEET_TCONS_ARRAY] = &t1p_meet_tcons_array; /*  */
	/* 2.Join */
	funptr[AP_FUNID_JOIN] = &t1p_join;
	funptr[AP_FUNID_JOIN_ARRAY] = &t1p_join_array;

	funptr[AP_FUNID_ADD_RAY_ARRAY] = &t1p_add_ray_array;

	/* Assign and Substitute */
	/*************************/
	funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &t1p_assign_linexpr_array;
	funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &t1p_substitute_linexpr_array;
	funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &t1p_assign_texpr_array;
	funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &t1p_substitute_texpr_array;

	/* Resize dimensions */
	/*********************/
	funptr[AP_FUNID_ADD_DIMENSIONS] = &t1p_add_dimensions;
	funptr[AP_FUNID_REMOVE_DIMENSIONS] = &t1p_remove_dimensions;
	funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &t1p_permute_dimensions;

	/* Other functions */
	/*******************/
	funptr[AP_FUNID_FORGET_ARRAY] = &t1p_forget_array;
	funptr[AP_FUNID_EXPAND] = &t1p_expand;
	funptr[AP_FUNID_FOLD] = &t1p_fold;
	funptr[AP_FUNID_WIDENING] = &t1p_widening;
	funptr[AP_FUNID_CLOSURE] = &t1p_closure;

	/* ?! */
	/******/
	/*
	   AP_FUNID_CHANGE_ENVIRONMENT
	   AP_FUNID_RENAME_ARRAY
	   AP_FUNID_SIZE2
	 */
	man->option.abort_if_exception[AP_EXC_INVALID_ARGUMENT] = false;
	return man;
}

int ap_manager_t1p_get_nsym(ap_manager_t* man)
{
        t1p_internal_t * pr = t1p_init_from_manager(man, AP_FUNID_UNKNOWN);
        return pr->dim;
}


// KG: The current three functions were removed as no longer needed for T1+ to work properly on CYGWI 
// 
//
//int get_clk_tck (void)
//{
//#ifdef CLOCKS_PER_SEC
//	return CLOCKS_PER_SEC;
//#else
//	int value = sysconf (_SC_CLK_TCK);
//	if (value < 0) {
//		fprintf(stderr,"sysconf: %s\n", strerror (errno));
//		abort();
//	}
//	return value;
//#endif
//}
//
///* TODO: Actually, these are called when allocating the internal structure of Taylor1+ domain .
// * If we have a multithreaded library, we should share these logging info between all threads ! */
//void log_init(void* addr, uint_t length, int fd)
//{
//
//#if (defined __CYGWIN__ || defined __MINGW32__)
//	uint_t page_size = 4096;
//#else
//	uint_t page_size = (uint_t) sysconf(_SC_PAGESIZE);
//#endif
//	char * path = getcwd(NULL, (uint_t)0);
//	if (!(path)) {
//		fprintf(stderr,"getcwd: %s\n",strerror(errno));
//		abort();
//	}
//	char * logfile = "/taylor1plus.log";
//	uint_t l = strlen(path) + strlen(logfile) + 1; /* +1 for the terminating null byte */
//	char * logpath = (char*)malloc(l*sizeof(char));
//	strcpy(logpath,path);
//	strcat(logpath,logfile);
//	fd = open(logpath, O_RDWR | O_CREAT, S_IRWXU);
//	if (fd == -1) {
//		fprintf(stderr,"open: %s\n",strerror(errno));
//		abort();
//	}
//	length = (uint_t)(1*page_size);
//	addr = mmap (NULL, length, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, (off_t)(0*page_size));
//	if (addr == MAP_FAILED) {
//		fprintf(stderr,"mmap: %s\n",strerror(errno));
//		abort();
//	}
//	free(path);
//	free(logpath);
//}
//
//void log_sync(void* addr, uint_t length, int fd)
//{
//	if (munmap(addr, length) == -1) {
//		fprintf(stderr,"munmap: %s\n",strerror(errno));
//		abort();
//	}
//	close(fd);
//}

/*
static inline ap_interval_t* t1p_nsymcons_get_gamma(t1p_internal_t * pr, uint_t nsymIndex, t1p_t* a)
{
    ap_dim_t dim;
    t1p_nsymcons_get_dimpos(pr, &dim, nsymIndex, a);
    return a->gamma[dim];
}
*/

/* back pointer to our internal structure from the manager */
t1p_internal_t* t1p_init_from_manager(ap_manager_t* man, ap_funid_t funid)
{
    t1p_internal_t* pr = (t1p_internal_t*)man->internal;
    pr->funid = funid;
    if (!(pr->man)) pr->man = man;
    return pr;
}

/**********************************/
/* Optimization Problem Functions */
/**********************************/
void optpr_init(t1p_internal_t* pr)
{
    pr->optpr = (optpr_problem_t*) malloc(sizeof(optpr_problem_t));
    optpr_problem_t* opt = pr->optpr;
    itv_init(opt->alpha0);
    itv_init(opt->beta);
    itv_init(opt->gamma);
    itv_set_int(opt->beta, 1);
    itv_set_int(opt->gamma, 1);
    opt->T = (optpr_indexinfo_t*) calloc(pr->dim,sizeof(optpr_indexinfo_t)); /* worst case, all symbols used */
    opt->litab = (Li*) calloc(4+pr->dim,sizeof(Li)); /* worst case, all symbols are in J (non constrained case) + les 4 droites des bordures de la region feasible */
    opt->sizeJ = 0; /* effective size of J */
    opt->size = 0; /* effective size of opt->indices (total nb noise symbols - nb input noise symbols) */
    itv_init(opt->optval);
}

void optpr_clear(t1p_internal_t* pr)
{
    optpr_problem_t* opt = pr->optpr;
    itv_clear(opt->alpha0);
    itv_clear(opt->beta);
    itv_clear(opt->gamma);
    uint_t i = 0;
    for (i=0; i<opt->sizeJ; i++) {
	itv_clear(opt->litab[i].abc[0]);
	itv_clear(opt->litab[i].abc[1]);
	itv_clear(opt->litab[i].abc[2]);
    }
    for (i=0; i<opt->size; i++) {
	itv_clear(opt->T[i].alphaixMalphaiy);
	itv_clear(opt->T[i].nsymitvx);
	itv_clear(opt->T[i].nsymitvy);
	opt->T[i].sign = 0;
	opt->T[i].I = 0;
	opt->T[i].pnsym = NULL;
    }
    free(opt->T); opt->T = NULL;
    free(opt->litab); opt->litab = NULL;
    opt->sizeJ = 0;
    opt->size = 0;
    itv_clear(opt->optval);
}

void optpr_u0_iszero(t1p_internal_t* pr)
{
}
void optpr_build(t1p_internal_t* pr, itv_t alphaix, itv_t alphaiy, t1p_nsym_t* pnsym, itv_t nsymItv1, itv_t nsymItv2, indicesSets_t I)
{
    itv_t tmp; itv_init(tmp);
    optpr_problem_t* opt = pr->optpr;

    itv_t alphaixMalphaiy; itv_init(alphaixMalphaiy);
    itv_sub(alphaixMalphaiy,alphaix,alphaiy);

    if (itv_is_eq(alphaix,alphaiy)) {
	opt->T[opt->size].sign = 0;
    } else if (itv_cmp(alphaix, alphaiy)) {
	opt->T[opt->size].sign = -1;
    } else if (itv_cmp(alphaiy, alphaix)) {
	opt->T[opt->size].sign = 1;
    } else if (itv_is_leq(tmp, alphaixMalphaiy)) {
	/* [0,0] \in alphaixMalphaiy <-> sign indefini */
	fatal("Sign de (alphaix - alphaiy) indefini \n");
    } else if (itv_is_pos(alphaixMalphaiy)) {
	opt->T[opt->size].sign = 1;
    } else if (itv_is_neg(alphaixMalphaiy)) {
	opt->T[opt->size].sign = -1;
    } else {
	fatal("Sign de [alphaix - alphaiy] indefini \n");
    }

    itv_init(opt->T[opt->size].alphaixMalphaiy);
    itv_init(opt->T[opt->size].nsymitvx);
    itv_init(opt->T[opt->size].nsymitvy);
    itv_init(opt->T[opt->size].alphaix);
    itv_init(opt->T[opt->size].alphaiy);

    itv_set(opt->T[opt->size].alphaixMalphaiy, alphaixMalphaiy);
    itv_set(opt->T[opt->size].nsymitvx, nsymItv1);
    itv_set(opt->T[opt->size].nsymitvy, nsymItv2);
    opt->T[opt->size].pnsym = pnsym;
    opt->T[opt->size].I = I;
    itv_set(opt->T[opt->size].alphaix,alphaix);
    itv_set(opt->T[opt->size].alphaiy,alphaiy);

    itv_t midx, midy, devx, devy;
    itv_init(midx); itv_init(midy); itv_init(devx); itv_init(devy);

    switch (I) {
	case IX:
	      {
		/* nsymItv1 is a point */
		itv_set(midx,nsymItv1);
		//itv_set_int(devx,-1); /* est ce utile ? */
		itv_middev(pr->itv, midy, devy, nsymItv2);
		itv_mul(pr->itv, tmp, alphaixMalphaiy, midx);
		itv_add(opt->alpha0, opt->alpha0, tmp);
		if (itv_is_eq(midx, midy)) {
		    /* do nothing */
		} else {
		    /* devy / |midx - midy| < 1 : maj de beta */
		    itv_sub(tmp, midx, midy);
		    itv_abs(tmp, tmp);
		    itv_div(pr->itv, tmp, devy, tmp);
		    if (num_cmp(bound_numref(tmp->inf),bound_numref(opt->beta->inf)) >= 0) {
			/* this index defines a border */
			//opt->indices[opt->size].isOnRightBorder = true;
			bound_set(opt->beta->inf, tmp->inf);
			bound_neg(opt->beta->sup, opt->beta->inf);
		    }
		}
		break;
	      }
	case IY:
	      {
		/* nsymItv2 is a point */
		itv_set(midy,nsymItv2);
		itv_middev(pr->itv, midx, devx, nsymItv1);
		itv_mul(pr->itv, tmp, alphaixMalphaiy, midy);
		itv_add(opt->alpha0, opt->alpha0, tmp);
		if (itv_is_eq(midx, midy)) {
		    /* do nothing */
		} else {
		    /* devx / |midx - midy| < 1 : maj de gamma */
		    itv_sub(tmp, midx, midy);
		    itv_abs(tmp, tmp);
		    itv_div(pr->itv, tmp, devx, tmp);
		    if (num_cmp(bound_numref(tmp->inf),bound_numref(opt->gamma->inf)) >= 0) {
			/* this index defines a border */
			//opt->indices[opt->size].isOnLeftBorder = true;
			bound_set(opt->gamma->inf, tmp->inf);
			bound_neg(opt->gamma->sup, opt->gamma->inf);
		    }
		}
		break;
	      }
	case IXY:
	      {
		/* nsymItv1 et nsymItv2 sont des points */
		itv_set(midx,nsymItv1);
		itv_set(midy,nsymItv2);
		itv_mul(pr->itv, tmp, alphaixMalphaiy, midy);
		itv_add(opt->alpha0, opt->alpha0, tmp);
		break;
	      }
	case J:
	      {
		itv_middev(pr->itv, midx, devx, nsymItv1);
		itv_middev(pr->itv, midy, devy, nsymItv2);
		opt->litab[opt->sizeJ].i = opt->size;
		if (opt->T[opt->size].sign == 1) {
		    /* alphaix > alphaiy */
		    itv_init(opt->litab[opt->sizeJ].abc[0]);
		    itv_init(opt->litab[opt->sizeJ].abc[1]);
		    itv_init(opt->litab[opt->sizeJ].abc[2]);
		    itv_add(opt->litab[opt->sizeJ].abc[0], devx, devy);
		    itv_sub(opt->litab[opt->sizeJ].abc[1], midx, midy);
		    itv_set(opt->litab[opt->sizeJ].abc[2], devy);
		    opt->sizeJ++;
		} else if (opt->T[opt->size].sign == -1) {
		    /* alphaix < alphaiy */
		    itv_init(opt->litab[opt->sizeJ].abc[0]);
		    itv_init(opt->litab[opt->sizeJ].abc[1]);
		    itv_init(opt->litab[opt->sizeJ].abc[2]);
		    itv_add(opt->litab[opt->sizeJ].abc[0], devx, devy);
		    itv_sub(opt->litab[opt->sizeJ].abc[1], midy, midx);
		    itv_set(opt->litab[opt->sizeJ].abc[2], devy);
		    opt->sizeJ++;
		} else {
		    ;
		}
		break;
	      }
	default:
	      {
		fatal("Unknown indices Set \n");
	      }
    }
    opt->size++;
}

int optpr_cramer(t1p_internal_t* pr, itv_t lambda, itv_t u0, itv_t * l1, itv_t * l2)
{


    if (itv_is_eq(l1[0],l2[0]) && itv_is_eq(l1[1],l2[1])) {
	if (itv_is_eq(l1[2],l2[2])) {
	    /* l1 = l2 */
	    return 0;
	} else {
	    /* l1 // l2 avec exactement même vecteur directeur */
	    return -1;
	}
    } else {
	itv_t ad, bc, det, tmp1, tmp2;
	itv_init(ad);
	itv_init(bc);
	itv_init(tmp1);
	itv_init(tmp2);
	itv_init(det);

	itv_mul(pr->itv,ad,l1[0],l2[1]);
	itv_mul(pr->itv,bc,l2[0],l1[1]);
	itv_sub(det, ad, bc);

	/* est ce que [0,0] est dan det ? (ici tmp1 = [0,0]) */
	if (itv_is_leq(tmp1,det)) {
	    if (itv_is_zero(det)) {
		/* l1 // l2  */
		return -1;
	    } else {
		/* matrice foireuse */
		fatal("Matrice de Cramer foireuse ... \n");
	    }
	}
	else {
	/* secants */
	    itv_mul(pr->itv,tmp1,l2[1],l1[2]);
	    itv_mul(pr->itv,tmp2,l1[1],l2[2]);
	    itv_sub(lambda,tmp1,tmp2);
	    itv_div(pr->itv,tmp1,lambda,det);
	    itv_set(lambda, tmp1);

	    itv_mul(pr->itv,tmp1,l1[0],l2[2]);
	    itv_mul(pr->itv,tmp2,l2[0],l1[2]);
	    itv_sub(u0,tmp1,tmp2);
	    itv_div(pr->itv,tmp1,u0,det);
	    itv_set(u0, tmp1);
	}

	itv_clear(ad);
	itv_clear(bc);
	itv_clear(tmp1);
	itv_clear(tmp2);
	itv_clear(det);
	return 1;
    }
}

bool optpr_isfeasible(t1p_internal_t* pr, itv_t lambda, itv_t u0)
{
    //printf("point: (");itv_print(lambda);printf(",");itv_print(u0);printf(")\n");
    itv_t zun, un;
    itv_t cond1, cond2;
    itv_t abs;
    itv_init(zun); itv_init(un);
    itv_init(cond1); itv_init(cond2);
    itv_init(abs);

    optpr_problem_t* opt = pr->optpr;

    itv_set_int2(zun,0,1);
    itv_set_int(un,1);
    bool res = itv_is_leq(lambda,zun);
    itv_mul(pr->itv,cond1,lambda,opt->gamma);
    itv_sub(cond2,un,lambda);
    itv_mul(pr->itv,cond2, cond2,opt->beta);
    itv_abs(abs, u0);

    /* TODO: faut voir si c'est 100% ok comme ca */
    res &= itv_cmp(abs, cond1);
    res &= itv_cmp(abs, cond2);

    itv_clear(zun); itv_clear(un);
    itv_clear(cond1); itv_clear(cond2);
    itv_clear(abs);

    return res;
}

void optpr_solve(t1p_internal_t* pr, itv_t alpha0x, itv_t alpha0, itv_t midgx, itv_t midgy, itv_t taux, itv_t tauy, t1p_aff_t* res)
{
    optpr_problem_t* opt = pr->optpr;
    uint_t nb = ((opt->sizeJ + 4)*(opt->sizeJ +3))/2; /* nb of intersections in the worst case */
    optpr_point_t* P = (optpr_point_t*)calloc(nb,sizeof(optpr_point_t));

    uint_t i = 0;
    uint_t j = 0;
    uint_t k = 0;
    uint_t nbP = 0;
    /* add borders to opt->litab */
    /* u0 = gamma*lambda : appele B4 stored in sizeJ-4 */
    itv_init(opt->litab[opt->sizeJ].abc[0]);
    itv_init(opt->litab[opt->sizeJ].abc[1]);
    itv_init(opt->litab[opt->sizeJ].abc[2]);
    itv_set(opt->litab[opt->sizeJ].abc[0],opt->gamma);
    itv_set_int(opt->litab[opt->sizeJ].abc[1], -1);
    itv_set_int(opt->litab[opt->sizeJ].abc[2],0);
    opt->sizeJ++;
    /* u0 = -gamma*lambda : B3 , sizeJ-3 */
    itv_init(opt->litab[opt->sizeJ].abc[0]);
    itv_init(opt->litab[opt->sizeJ].abc[1]);
    itv_init(opt->litab[opt->sizeJ].abc[2]);
    itv_neg(opt->litab[opt->sizeJ].abc[0],opt->gamma);
    itv_set_int(opt->litab[opt->sizeJ].abc[1], -1);
    itv_set_int(opt->litab[opt->sizeJ].abc[2],0);
    opt->sizeJ++;
    /* u0 = beta*(1 - lambda) : B2, sizeJ-2 */
    itv_init(opt->litab[opt->sizeJ].abc[0]);
    itv_init(opt->litab[opt->sizeJ].abc[1]);
    itv_init(opt->litab[opt->sizeJ].abc[2]);
    itv_set(opt->litab[opt->sizeJ].abc[0],opt->beta);
    itv_set_int(opt->litab[opt->sizeJ].abc[1], 1);
    itv_set(opt->litab[opt->sizeJ].abc[2],opt->beta);
    opt->sizeJ++;
    /* u0 = -beta*(1 - lambda) : B1, sizeJ-1 */
    itv_init(opt->litab[opt->sizeJ].abc[0]);
    itv_init(opt->litab[opt->sizeJ].abc[1]);
    itv_init(opt->litab[opt->sizeJ].abc[2]);
    itv_neg(opt->litab[opt->sizeJ].abc[0],opt->beta);
    itv_set_int(opt->litab[opt->sizeJ].abc[1], 1);
    itv_neg(opt->litab[opt->sizeJ].abc[2],opt->beta);
    opt->sizeJ++;

    itv_t u0, lambda;
    itv_t f, tmp1, tmp2, tmp3, ulambda, ulamdevy, lamdevx, u0midx, u0midy;
    itv_t midx, midy, devx, devy, abs;
    itv_t un, mun;
    itv_t midxMmidy;

    itv_init(midx); itv_init(midy); itv_init(devx); itv_init(devy);
    itv_init(f); itv_init(tmp1); itv_init(tmp2); itv_init(tmp3); itv_init(ulambda); 
    itv_init(ulamdevy); itv_init(lamdevx); itv_init(u0midx); itv_init(u0midy);
    itv_init(abs);
    itv_init(u0); itv_init(lambda);
    itv_init(un); 
    itv_init(mun);
    itv_init(midxMmidy);

    itv_set_int(un,1);
    itv_set_int(mun,-1);

    itv_add(opt->alpha0, opt->alpha0, alpha0);
    /* init optimal solution structure */
    itv_init(opt->optsol.u0);
    itv_init(opt->optsol.lambda);
    int* ui = (int*)calloc(opt->sizeJ,sizeof(int));
    opt->optsol.ui = (int*)calloc(opt->sizeJ,sizeof(int));
    itv_t** pcooru = (itv_t**)calloc(opt->size,sizeof(itv_t*));
    itv_t** pcooruSave = (itv_t**)calloc(opt->size,sizeof(itv_t*));
    itv_t** swap = (itv_t**)calloc(opt->size,sizeof(itv_t*));
    itv_t* cooru = (itv_t*)calloc(opt->size,sizeof(itv_t));
    itv_t* cooruSave = (itv_t*)calloc(opt->size,sizeof(itv_t));

    for (i=0; i<opt->size; i++) {
	itv_init(cooru[i]);
	pcooru[i] = &cooru[i];
	itv_init(cooruSave[i]);
	pcooruSave[i] = &cooruSave[i];
    }

    /* complexité (opt->sizeJ)^3 */
    for (i=0; i<opt->sizeJ; i++) {
	for (j=i+1; j<opt->sizeJ; j++) {
	    /* TODO: pour le moment on ne fait rien si deux lignes sont identiques: à optimiser */
	    if (optpr_cramer(pr, lambda, u0, opt->litab[i].abc, opt->litab[j].abc) > 0) {
		/* secant lines */
		if (optpr_isfeasible(pr, lambda, u0)) {
		    itv_init(P[nbP].u0);
		    itv_init(P[nbP].lambda);
		    itv_set(P[nbP].u0, u0);
		    itv_set(P[nbP].lambda, lambda);
		    /*	         (6)
		          (7) B4 /\ B2 (5)
		                /  \
		              (0) (4)
		                \  /
		          (1) B3 \/ B1 (3)
			         (2)
				(8) strictly feasible 
		    */
		    /*
		    if (j == opt->sizeJ - 4) {
			if (i == opt->sizeJ - 3) {
			    P[nbP].pos = 0;
			} else if (i == opt->sizeJ - 2) {
			    P[nbP].pos = 6;
			} else {
			    P[nbP].pos = 7;
			}
		    } else if (j == opt->sizeJ - 3) {
			if (i == opt->sizeJ - 1) {
			    P[nbP].pos = 2;
			} else {
			    P[nbP].pos = 1;
			}
		    } else if (j == opt->sizeJ - 2) {
			if (i == opt->sizeJ - 1) {
			    P[nbP].pos = 4;
			} else {
			    P[nbP].pos = 5;
			}
		    } else if (j == opt->sizeJ - 1) {
			P[nbP].pos = 3;
		    } else {
			P[nbP].pos = 8;
		    }
		    */
		    /* eval objfunc */
		    itv_mul(pr->itv, f, opt->alpha0, u0);
		    for (k=0; k<opt->sizeJ-4; k++) {
			itv_abs(abs, opt->T[opt->litab[k].i].alphaixMalphaiy);
			itv_middev(pr->itv, midx, devx, opt->T[opt->litab[k].i].nsymitvx);
			itv_middev(pr->itv, midy, devy, opt->T[opt->litab[k].i].nsymitvy);
			itv_mul(pr->itv,lamdevx,lambda,devx);
			itv_mul(pr->itv,u0midx,u0,midx);
			itv_sub(ulambda,un,lambda);
			itv_mul(pr->itv,ulamdevy,ulambda,devy);
			itv_mul(pr->itv,u0midy,u0,midy);
			if (opt->T[opt->litab[k].i].sign == 1) {
			    itv_add(tmp1,lamdevx,u0midx);
			    itv_add(tmp2,ulamdevy,u0midy);
			    if (k == i || k == j) {
				/* on sait que tmp1 doit etre egale a tmp2, du coup on prend l'intersection des deux */
				itv_meet(pr->itv,tmp3,tmp1,tmp2);
				ui[k] = 0;
			    } else if (itv_cmp(tmp1,tmp2)) {
				itv_set(tmp3,tmp1);
				ui[k] = -1;
			    } else if (itv_cmp(tmp2,tmp1)) {
				itv_set(tmp3,tmp2);
				ui[k] = 1;
			    } else if (itv_is_eq(tmp1,tmp2)) {
				/* des fois que les droites soient concordantes */
				itv_set(tmp3,tmp2);
				ui[k] = 0;
			    } else {
				fatal("que faire dans ce cas ... ? \n");
			    }
			} else if (opt->T[opt->litab[k].i].sign == -1) {
			    itv_sub(tmp1,lamdevx,u0midx);
			    itv_sub(tmp2,ulamdevy,u0midy);
			    if (k == i || k == j) {
				/* on sait que les deux points tmp1 doit etre egale a tmp2, du coup on prend l'intersection des deux */
				itv_meet(pr->itv,tmp3,tmp1,tmp2);
				ui[k] = 0;
			    } else if (itv_cmp(tmp1,tmp2)) {
				itv_set(tmp3,tmp1);
				ui[k] = -1;
			    } else if (itv_cmp(tmp2,tmp1)) {
				itv_set(tmp3,tmp2);
				ui[k] = 1;
			    } else if (itv_is_eq(tmp1,tmp2)) {
				itv_set(tmp3,tmp2);
				ui[k] = 0;
			    } else {
				itv_print(tmp1);printf("\t");itv_print(tmp2);printf("\n");
				fatal("que faire dans ce cas ? \n");
			    }
			} else {
			    /* opt->litab[k].sign = 0 ; do nothing */
			}
			itv_set(*pcooru[opt->litab[k].i],tmp3);
			if (opt->T[opt->litab[k].i].sign == -1) itv_neg(*pcooru[opt->litab[k].i],*pcooru[opt->litab[k].i]);
			itv_mul(pr->itv, tmp3, tmp3, abs);
			itv_add(f,f,tmp3);
		    }
		    /* comparer f avec le max */
		    //itv_print(f);
		    //printf("\n");
		    
		    if (itv_cmp(opt->optval,f)) {
			/* toutes les valeurs de f >= toutes les valeurs de max */
			itv_set(opt->optval,f);
			/* store the point */
			itv_set(opt->optsol.u0, u0);
			itv_set(opt->optsol.lambda, lambda);
			memcpy(opt->optsol.ui, ui, (opt->sizeJ)*sizeof(int));
			/* swap pour eviter une complexite de n^4 */
			memcpy(swap, pcooruSave, (opt->size)*sizeof(itv_t*));
			memcpy(pcooruSave, pcooru, (opt->size)*sizeof(itv_t*));
			memcpy(pcooru, swap, (opt->size)*sizeof(itv_t*));
		    } else if (itv_cmp(f,opt->optval)) {
			/* définitivement pas bon - je le mets surtout pour le else */
		    } else if (itv_is_leq(f,opt->optval)) {
			/* do nothing, le optval contient deja f */
		    } else {
			/* entrelacements des intervalles: prendre celui qui a la borne sup la plus grande et garder le point quand même */
			printf("f: ");itv_print(f);printf("optval: ");itv_print(opt->optval);printf("\n");
			fatal("A implementer .... \n");
		    }
		    nbP++;
		}
	    }
	}
    }
    itv_set(lambda,opt->optsol.lambda);
    itv_set(u0, opt->optsol.u0);
    itv_sub(ulambda, un, lambda);
    /* on a la dernière solution dans opt->optsol et sa valeur dans opt->optval */
    itv_div(pr->itv, tmp1, u0, lambda);
    itv_div(pr->itv, tmp2, u0, ulambda);
    /* creer deux linexp pour la dimension 0 */
    ap_manager_t* pk = pk_manager_alloc(false);
    /* nb of constraints in the worst case */
    ap_lincons0_array_t array = ap_lincons0_array_make(1 + 2*(opt->size));

    array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_DENSE, (uint_t)(1+opt->size));
    array.p[1].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_DENSE, (uint_t)(1+opt->size));
    array.p[2].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_DENSE, (uint_t)(1+opt->size));

    ap_interval_t* itv = ap_interval_alloc();
    /* MIDDEV */
    //itv_middev(pr->itv, tmp1, tmp2, u0);
    ap_interval_set_itv(pr->itv, itv, u0);
    ap_linexpr0_set_coeffinterval(array.p[2].linexpr0, (ap_dim_t)(0), itv);

    itv_t cst; itv_init(cst);

    itv_mul(pr->itv, cst, alpha0x, u0);

    for (j=0;j<opt->size;j++) {
	itv_middev(pr->itv, midx, devx, opt->T[j].nsymitvx);
	itv_middev(pr->itv, midy, devy, opt->T[j].nsymitvy);
	switch (opt->T[j].I) {
	    case IX:
		  {
		    itv_mul(pr->itv,tmp1,midx,u0);
		    itv_mul(pr->itv,tmp2,opt->T[j].alphaix,tmp1);
		    itv_add(cst,cst,tmp2);
		    /* MIDDEV */
		    //itv_middev(pr->itv, tmp2, tmp3, tmp1);
		    ap_interval_set_itv(pr->itv, itv, tmp1);
		    ap_linexpr0_set_coeffinterval(array.p[2].linexpr0, (ap_dim_t)(j+1), itv);
		    break;
		  }
	    case IY:
		  {
		    itv_mul(pr->itv,tmp1,midy,u0);
		    itv_mul(pr->itv,tmp2,opt->T[j].alphaix,tmp1);
		    itv_add(cst,cst,tmp2);
		    /* MIDDEV */
		    //itv_middev(pr->itv, tmp2, tmp3, tmp1);
		    ap_interval_set_itv(pr->itv, itv, tmp1);
		    ap_linexpr0_set_coeffinterval(array.p[2].linexpr0, (ap_dim_t)(j+1), itv);
		    break;
		  }
	    case IXY:
		  {
		    itv_mul(pr->itv,tmp1,midy,u0);
		    itv_mul(pr->itv,tmp2,opt->T[j].alphaix,tmp1);
		    itv_add(cst,cst,tmp2);
		    /* MIDDEV */
		    //itv_middev(pr->itv, tmp2, tmp3, tmp1);
		    ap_interval_set_itv(pr->itv, itv, tmp1);
		    ap_linexpr0_set_coeffinterval(array.p[2].linexpr0, (ap_dim_t)(j+1), itv);
		    break;
		  }
	    case J:
		  {
		    itv_mul(pr->itv,tmp2,opt->T[j].alphaix,*pcooruSave[j]);
		    itv_add(cst,cst,tmp2);
		    /* MIDDEV */
		    //itv_middev(pr->itv, tmp2, tmp3, *pcooruSave[j]);
		    ap_interval_set_itv(pr->itv, itv, *pcooruSave[j]);
		    ap_linexpr0_set_coeffinterval(array.p[2].linexpr0, (ap_dim_t)(j+1), itv);
		    break;
		  }
	    default: fatal("unknown set index \n");
	}
	itv_neg(tmp1,midx);
	ap_interval_set_itv(pr->itv, itv, tmp1);
	ap_linexpr0_set_coeffinterval(array.p[0].linexpr0, (ap_dim_t)(j+1), itv);
	ap_interval_set_itv(pr->itv, itv, midy);
	ap_linexpr0_set_coeffinterval(array.p[1].linexpr0, (ap_dim_t)(j+1), itv);
    }
    ap_linexpr0_set_coeff_scalar_int(array.p[0].linexpr0, (ap_dim_t)(0), -1);
    ap_linexpr0_set_coeff_scalar_int(array.p[1].linexpr0, (ap_dim_t)(0), 1);
    ap_interval_set_itv(pr->itv, itv, midgx);
    ap_linexpr0_set_cst_interval(array.p[0].linexpr0, itv);
    itv_neg(tmp1,midgy);
    ap_interval_set_itv(pr->itv, itv, tmp1);
    ap_linexpr0_set_cst_interval(array.p[1].linexpr0, itv);

    /* continuer le calcul de la constante de l'hyperplan */
    itv_mul(pr->itv,tmp1,lambda,opt->optval);
    itv_sub(cst,cst,tmp1);
    itv_mul(pr->itv,tmp1,lambda,ulambda);
    itv_sub(tmp2, taux, tauy);
    itv_mul(pr->itv, tmp3, tmp1, tmp2);
    itv_add(cst,cst, tmp3);
    itv_neg(cst,cst);
    itv_middev(pr->itv, tmp2, tmp3, cst);
    ap_interval_set_itv(pr->itv, itv, tmp2);
    ap_linexpr0_set_cst_interval(array.p[2].linexpr0, itv);
    array.p[2].constyp = AP_CONS_EQ;
    array.p[2].scalar = NULL;


    if (itv_is_leq(un,tmp1)) {
	/* rajouter contrainte hyperplan */
    } else if (itv_is_leq(mun,tmp1)) {
	/* rajouter contrainte hyperplan */
    } else {
	/* considerer que ce n'est ni un ni mun , machin = 0 */
	array.p[0].constyp = AP_CONS_EQ;
	array.p[0].scalar = NULL;
	array.p[1].constyp = AP_CONS_EQ;
	array.p[1].scalar = NULL;
    }
    if (itv_is_leq(un,tmp2)) {
	/* rajouter contrainte hyperplan */
    } else if (itv_is_leq(mun,tmp2)) {
	/* rajouter contrainte hyperplan */
    } else {
	/* considerer que ce n'est ni un ni mun , machin = 0 */
    }
    uint_t Nbcons = 3;
    for (i=0; i<opt->size; i++) {
	if (opt->T[i].sign == 0) {
	    /* alphaiz = alphaix = alphaiy */
	} else {
	    itv_middev(pr->itv, midx, devx, opt->T[i].nsymitvx);
	    itv_middev(pr->itv, midy, devy, opt->T[i].nsymitvy);
	    itv_sub(midxMmidy, midx, midy);
	    switch (opt->T[i].I) {
		case IX: 
		      {
			itv_mul(pr->itv, tmp1, midxMmidy, u0);
			itv_mul(pr->itv, tmp2, devy, ulambda);
			itv_div(pr->itv, tmp3, tmp1, tmp2);
			if (itv_is_leq(un, tmp3)) {
			    /* rajouter la contrainte alphazi >= alphaiy */
			} else if (itv_is_leq(mun, tmp3)) {
			    /* rajouter la contrainte alphazi <= alphaiy */
			} else {
			    /* alphaiz = alphaiy */
			}
			break;
		      }
		case IY:
		      {
			itv_mul(pr->itv, tmp1, midxMmidy, u0);
			itv_mul(pr->itv, tmp2, devx, lambda);
			itv_div(pr->itv, tmp3, tmp1, tmp2);
			itv_neg(tmp3,tmp3);
			if (itv_is_leq(un, tmp3)) {
			    /* rajouter la contrainte alphazi <= alphaix */
			} else if (itv_is_leq(mun, tmp3)) {
			    /* rajouter la contrainte alphazi >= alphaix */
			} else {
			    /* alphaiz = alphaix */
			}
			break;
		      }
		case IXY:
		      {
			/* no information */
			break;
		      }
		case J:
		      {
			if (opt->T[i].sign == 1) {
			    if (opt->optsol.ui[i] == 0) {
				/* alphaix >= alphaiz >= alphaiy */
				array.p[Nbcons].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (uint_t)1);
				ap_linexpr0_set_coeff_scalar_int(array.p[Nbcons].linexpr0, (ap_dim_t)(i+1), -1);
				ap_interval_set_itv(pr->itv, itv, opt->T[i].alphaix);
				ap_linexpr0_set_cst_interval(array.p[Nbcons].linexpr0, itv);
				array.p[Nbcons].constyp = AP_CONS_SUPEQ;
				array.p[Nbcons].scalar = NULL;
				Nbcons++;
				array.p[Nbcons].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (uint_t)1);
				ap_linexpr0_set_coeff_scalar_int(array.p[Nbcons].linexpr0, (ap_dim_t)(i+1), 1);
				itv_neg(tmp1, opt->T[i].alphaiy);
				ap_interval_set_itv(pr->itv, itv, tmp1);
				ap_linexpr0_set_cst_interval(array.p[Nbcons].linexpr0, itv);
				array.p[Nbcons].constyp = AP_CONS_SUPEQ;
				array.p[Nbcons].scalar = NULL;
				Nbcons++;
			    } else if (opt->optsol.ui[i] == 1) {
				/* contrainte alphaiz >= alphaiy */

				/* faut calculer l'autre signe */
				itv_mul(pr->itv, tmp1, midxMmidy, u0);
				itv_neg(tmp1,tmp1);
				itv_mul(pr->itv, tmp2, devy, ulambda);
				itv_add(tmp1, tmp1, tmp2);
				itv_mul(pr->itv, tmp3, lambda, devx);
				itv_div(pr->itv, tmp2, tmp1, tmp3);
				if (itv_is_leq(un, tmp2)) {
				    /* impossible */
				} else if (itv_is_leq(mun, tmp2)) {
				    /* rajouter la contrainte alphaiz >= alphaix */
				} else {
				    /* alphaiz = alphaix */
				}
			    } else if (opt->optsol.ui[i] == -1) {
				/* contrainte alphaiz <= alphaix */

				/* faut calculer l'autre signe */
				itv_mul(pr->itv, tmp1, midxMmidy, u0);
				itv_mul(pr->itv, tmp3, lambda, devx);
				itv_add(tmp1, tmp1, tmp3);
				itv_mul(pr->itv, tmp2, devy, ulambda);
				itv_div(pr->itv, tmp3, tmp1, tmp2);
				if (itv_is_leq(un, tmp3)) {
				    /* impossible */
				} else if (itv_is_leq(mun, tmp3)) {
				    /* rajouter la contrainte alphaiz <= alphaiy */
				} else {
				    /* alphaiz = alphaiy */
				}
			    }
			}
			break;
		      }
		default: fatal("unknown set \n");
	    }
	}
    }
    printf("\n");
    ap_lincons0_print(&array.p[0], NULL);
    printf("\n");
    ap_lincons0_print(&array.p[1], NULL);
    printf("\n");
    ap_lincons0_print(&array.p[2], NULL);
    printf("\n");
    array.size = Nbcons;
    ap_abstract0_t* obj = ap_abstract0_of_lincons_array(pk,0,(1+opt->size),&array);
    ap_abstract0_meet_lincons_array(pk, true, obj, &array);
    //ap_abstract0_fprint(stdout, pk, obj, NULL);

    double inf = 0.0; double sup = 0.0;
    for (i=0; i<3; i++) {
        ap_interval_t* gamma = ap_abstract0_bound_dimension(pk, obj, i);
	ap_double_set_scalar(&inf, gamma->inf, GMP_RNDU);
	ap_double_set_scalar(&sup, gamma->sup, GMP_RNDU);
	printf("x%d : [%f,%f]\n", i, inf, sup);
    }

    /* on doit construire la forme finale puis calculer tau^z */
    /* calcul de tauz */
    itv_t tauz; itv_init(tauz);
    itv_mul(pr->itv, tmp1, lambda, taux);
    itv_mul(pr->itv, tmp2, ulambda, tauy);
    itv_add(tmp3, tmp1, tmp2);
    itv_add(tauz, opt->optval, tmp3);
    printf("tauZ: ");itv_print(tauz);printf("\n");
    /*
    itv_print(opt->optval);
    printf("\n");
    itv_print(opt->optsol.lambda);
    printf("\n");
    itv_print(opt->optsol.u0);
    printf("\n");
    for (i=0;i<opt->sizeJ;i++) {
	printf("%d \t", opt->optsol.ui[i]);
    }
    printf("\n");
    */
    /* construction de la forme finale */
    t1p_aaterm_t *ptr = NULL;
    res = t1p_aff_alloc_init(pr);
    res->q = ptr = t1p_aaterm_alloc_init();
    ap_interval_t* itv_dim = NULL;
    for (i=0;i<opt->size+1;i++) {
	itv_dim = ap_abstract0_bound_dimension(pk, obj, (ap_dim_t)i);
	itv_set_ap_interval(pr->itv, tmp1, itv_dim);
	itv_middev(pr->itv, tmp2,tmp3,tmp1);
	if (i==0) itv_set(res->c,tmp2);
	else {
	    itv_set(ptr->coeff,tmp2);
	    ptr->pnsym = opt->T[i-1].pnsym;
	    res->end = ptr;
	    res->l++;
	    ptr->n = t1p_aaterm_alloc_init();
	    ptr=ptr->n;
	}
    }
    t1p_aaterm_free(pr, ptr);
    if (res->end) res->end->n = NULL;

    t1p_aff_nsym_create(pr, res, tauz, UN);

    t1p_aff_fprint(pr, stdout, res);

    /* free optimal solution structure */
    ap_interval_free(itv);

    itv_clear(opt->optsol.u0);
    itv_clear(opt->optsol.lambda);
    free(ui);
    free(opt->optsol.ui);
    free(P);
    itv_clear(midx); itv_clear(midy); itv_clear(devx); itv_clear(devy);
    itv_clear(f); itv_clear(tmp1); itv_clear(tmp2); itv_clear(tmp3); itv_clear(ulambda); 
    itv_clear(ulamdevy); itv_clear(lamdevx); itv_clear(u0midx); itv_clear(u0midy);
    itv_clear(abs);
    itv_clear(u0); itv_clear(lambda);
    itv_clear(un); 
    itv_clear(mun); 
    itv_clear(midxMmidy);
    array.size = 1 + 2*(opt->size);
    ap_lincons0_array_clear(&array);
    ap_manager_free(pk);
}

