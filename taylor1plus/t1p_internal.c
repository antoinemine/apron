#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

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


int get_clk_tck (void)
{
#ifdef CLOCKS_PER_SEC
	return CLOCKS_PER_SEC;
#else
	int value = sysconf (_SC_CLK_TCK);
	if (value < 0) {
		fprintf(stderr,"sysconf: %s\n", strerror (errno));
		abort();
	}
	return value;
#endif
}

/* TODO: Actually, these are called when allocating the internal structure of Taylor1+ domain .
 * If we have a multithreaded library, we should share these logging info between all threads ! */
void log_init(void* addr, size_t length, int fd)
{

	size_t page_size = (size_t) sysconf(_SC_PAGESIZE);
	char * path = getcwd(NULL, (size_t)0);
	if (!(path)) {
		fprintf(stderr,"getcwd: %s\n",strerror(errno));
		abort();
	}
	char * logfile = "/taylor1plus.log";
	size_t l = strlen(path) + strlen(logfile) + 1; /* +1 for the terminating null byte */
	char * logpath = (char*)malloc(l*sizeof(char));
	strcpy(logpath,path);
	strcat(logpath,logfile);
	fd = open(logpath, O_RDWR | O_CREAT, S_IRWXU);
	if (fd == -1) {
		fprintf(stderr,"open: %s\n",strerror(errno));
		abort();
	}
	length = (size_t)(1*page_size);
	addr = mmap (NULL, length, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, (off_t)(0*page_size));
	if (addr == MAP_FAILED) {
		fprintf(stderr,"mmap: %s\n",strerror(errno));
		abort();
	}
	free(path);
	free(logpath);
}

void log_sync(void* addr, size_t length, int fd)
{
	if (munmap(addr, length) == -1) {
		fprintf(stderr,"munmap: %s\n",strerror(errno));
		abort();
	}
	close(fd);
}

/*
static inline ap_interval_t* t1p_nsymcons_get_gamma(t1p_internal_t * pr, size_t nsymIndex, t1p_t* a)
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


