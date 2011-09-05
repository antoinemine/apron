/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


/*****************************************/
/* MACROS (inspired from oct_internal.h) */
/*****************************************/
/* invalid argument exception */
#define arg_assert(cond,action)						\
  do { if (!(cond)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "assertion (%s) failed in %s at %s:%i",			\
	       #cond, __func__, __FILE__, __LINE__);		\
      ap_manager_raise_exception(pr->man,AP_EXC_INVALID_ARGUMENT,	\
				 pr->funid,buf_);			\
      action }								\
  } while(0)

/* malloc with safe-guard */
#define checked_malloc(ptr,t,nb,action)					\
  do {									\
    (ptr) = (t*)calloc(nb,sizeof(t));					\
    if (!(ptr)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "cannot allocate %s[%lu] for %s in %s at %s:%i",		\
	       #t, (long unsigned)(nb), #ptr,				\
	       __func__, __FILE__, __LINE__);				\
      ap_manager_raise_exception(pr->man,AP_EXC_OUT_OF_SPACE,		\
				 pr->funid,buf_);			\
      action }								\
  } while(0)

/* fatal misc errors */
#define fatal(strerr)							\
  do {									\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "%s in %s at %s:%i",					\
	       strerr, __func__, __FILE__, __LINE__);			\
      ap_manager_raise_exception(pr->man,AP_EXC_NOT_IMPLEMENTED,	\
				 pr->funid,buf_);			\
      abort();								\
  } while(0)

/* not yet implemented */
#define not_implemented()						\
  do {									\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "function %s not yet implemented at %s:%i",		\
	       __func__, __FILE__, __LINE__);				\
      ap_manager_raise_exception(pr->man,AP_EXC_NOT_IMPLEMENTED,	\
				 pr->funid,buf_);			\
      abort();								\
  } while(0)

/* switch based on afexpr discr */
#define T1P_SWITCH(afexp,itv,aff)	\
	switch(afexp->discr) {		\
	case T1P_ITV: 			\
		{			\
			itv;		\
			break;		\
		}			\
	case T1P_AFF:			\
		{			\
			aff;		\
			break;		\
		}			\
	default: fatal("Unknown afexpr type");	\
	}

/* neg, add, sub operations */
#define T1P_AFEXPR_BINARY_OP(op,itv_op)	\
	if (a->discr == T1P_ITV || b->discr == T1P_ITV) {		\
                itv_t itva, itvb, resus;				\
                t1p_afexpr_boxize(pr, itva, a);				\
                t1p_afexpr_boxize(pr, itvb, b);				\
		itv_op	\
                t1p_afexpr_init_itv(pr, res, resus);			\
        } else {							\
                t1p_afexpr_init_aff(pr, res);				\
                t1p_aff_##op(pr, res->val.aff, a->val.aff, b->val.aff);	\
        }								

#define T1P_AFEXPR_UNARY_OP(op,itv_op)					\
	if (a->discr == T1P_ITV) {               \
                itv_t itva, resus;                                \
                t1p_afexpr_boxize(pr, itva, a);                         \
                itv_op  \
                t1p_afexpr_init_itv(pr, res, resus);                    \
        } else {                                                        \
                t1p_afexpr_init_aff(pr, res);                           \
                t1p_aff_##op(pr, res->val.aff, a->val.aff); \
        } 

/* logging calls */
#ifdef _CALL_DEBUG
#define CALL()	printf("call %s (%s:%i)\n",__func__, __FILE__, __LINE__);
#endif
#ifndef _CALL_DEBUG
#define CALL()	;
#endif

