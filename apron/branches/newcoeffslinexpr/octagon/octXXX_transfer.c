/*
 * octXXX_transfer.c
 *
 * Assignment, substitution, guard transfer functions.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "octXXX.h"
#include "octXXX_internal.h"
#include "ap_generic.h"


/* ============================================================ */
/* Expression classification */
/* ============================================================ */

uexpr octXXX_uexpr_of_linexpr0(octXXX_internal_t* pr, boundXXX_t* dst,
			       ap_linexpr0_t e, size_t dim)
{

#define CLASS_COEFF(idx,coef)						\
  if (!boundXXX_cmp_int(dst[2*idx+2],-coef) &&				\
      !boundXXX_cmp_int(dst[2*idx+3],coef)) {				\
    if (u.type==ZERO) { u.type = UNARY;  u.i = idx; u.coef_i = coef; }	\
    else              { u.type = BINARY; u.j = idx; u.coef_j = coef; }	\
    continue;								\
  }

#define CLASS_VAR(idx)							\
  if (u.type==EMPTY) continue;						\
  if (!boundXXX_sgn(dst[2*idx+2]) && !boundXXX_sgn(dst[2*idx+3])) continue;	\
  if (u.type>=BINARY) { u.type = OTHER; continue; }			\
  CLASS_COEFF(idx,1);							\
  CLASS_COEFF(idx,-1);							\
  u.type = OTHER;

  uexpr u = { ZERO, 0, 0, 0, 0 };
  size_t i;
  ap_coeff_t coeff;
  ap_dim_t d;

  ap_linexpr0_cstref(coeff,e);

  if (boundsXXX_of_coeff(pr,dst[0],dst[1],coeff,false)) u.type = EMPTY;
  for (i=0;i<dim;i++) {
    boundXXX_set_int(dst[2*i+2],0);
    boundXXX_set_int(dst[2*i+3],0);
  }
  ap_linexpr0_ForeachLinterm(e,i,d,coeff){
    arg_assert(d<dim,return u;);
    if (boundsXXX_of_coeff(pr,dst[2*d+2],dst[2*d+3],coeff,false))
      u.type = EMPTY;
    CLASS_VAR(d);
  }
  return u;
}

/* for debugging */
static void print_uexpr(uexpr u, boundXXX_t* dst, size_t dim)
{
  size_t i;
  printf("[-");
  boundXXX_print(dst[0]); printf(","); boundXXX_print(dst[1]);
  printf("]");
  for (i=0;i<dim;i++) {
    printf(" + [-");
    boundXXX_print(dst[2*i+2]); printf(","); boundXXX_print(dst[2*i+3]);
    printf("]v%i",(int)i);
  }
  printf(" >= 0 ");
  switch (u.type) {
  case ZERO:   printf(" zeroary\n"); break;
  case UNARY:  printf(" unary v%i=%i\n",(int)u.i,u.coef_i); break;
  case BINARY: printf(" binary v%i=%i v%i=%i\n",(int)u.i,u.coef_i,(int)u.j,u.coef_j); break;
  default:     printf("\n");
  }
  fflush(stdout);
}


/* ============================================================ */
/* Adding constraints / generators */
/* ============================================================ */

/* set *exact to 1 if all all constraints are octagonal ones
   return true if empty
 */
bool hmatXXX_add_lincons0_array(octXXX_internal_t* pr, boundXXX_t* b, size_t dim,
				ap_lincons0_array_t ar, bool* exact,
				bool* respect_closure)
{
  size_t i, j, k, ui, uj, size;
  ap_linexpr0_t expr;
  ap_lincons0_t cons;
  size_t var_pending = 0; /* delay incremental closure as long as possible */
  int closure_pending = 0;
  *exact = 1;
  size = ap_lincons0_array_size(ar);

  for (i=0;i<size;i++) {
    ap_lincons0_array_ref_index(cons,ar,i);
    ap_lincons0_linexpr0ref(expr,cons);
    ap_constyp_t c = ap_lincons0_get_constyp(cons);
    uexpr u;

    switch (c) {

      /* skipped */
    case AP_CONS_EQMOD:
    case AP_CONS_DISEQ:
      *exact = 0;
      continue;

      /* handled */
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      break;

      /* error */
    default:
      assert(0);
    }


    /* now handle ==, >=, > */

    u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,dim);

    switch (u.type) {

    case EMPTY:
      /* empty constraint: no added information */
      break;

    case ZERO:
      if ((c==AP_CONS_SUPEQ && boundXXX_sgn(pr->tmp[1])>=0) ||
	  /* [-a,b] >= 0 <=> b >= 0 */
	  (c==AP_CONS_SUP && boundXXX_sgn(pr->tmp[1])>0) ||
	  /* [-a,b] > 0 <=> b > 0 */
	  (c==AP_CONS_EQ && boundXXX_sgn(pr->tmp[0])>=0 && boundXXX_sgn(pr->tmp[1])>=0)
	  /* [-a,b] = 0 <=> a >= 0 && b >= 0 */
      )
	; /* trivial */
      else return true; /* unsatisfiable */
      break;

    case UNARY:

      /* can we delay incremental closure further? */
      if (*respect_closure && closure_pending && var_pending!=u.i) {
	if (hmatXXX_close_incremental(b,dim,var_pending)) return true;
      }
      closure_pending = 1;
      var_pending = u.i;

      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      boundXXX_mul_2exp(pr->tmp[0],pr->tmp[0],1);
      boundXXX_mul_2exp(pr->tmp[1],pr->tmp[1],1);
      boundXXX_bmin(b[matpos(ui,ui^1)],pr->tmp[1]);
      /*  c_i X_i + [-a,b] >= 0 <=> -c_i X_i <= b */
      if (c==AP_CONS_EQ) boundXXX_bmin(b[matpos(ui^1,ui)],pr->tmp[0]);
      /*  c_i X_i + [-a,b] <= 0 <=>  c_i X_i <= a */
      if (c==AP_CONS_SUP) *exact = 0; /* not exact for strict constraints */
      break;

    case BINARY:

      /* can we delay incremental closure further? */
      if (*respect_closure && closure_pending &&
	  var_pending!=u.i && var_pending!=u.j) {
	if (hmatXXX_close_incremental(b,dim,var_pending)) return true;
      }
      closure_pending = 1;
      var_pending = (var_pending==u.j) ? u.j : u.i;

      if ( u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if ( u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;
      boundXXX_bmin(b[matpos2(uj,ui^1)],pr->tmp[1]);
      /* c_i X_i + c_j X_j + [-a,b] >= 0 <=> -c_i X_i - c_j X_j <= b */
      if (c==AP_CONS_EQ)
	boundXXX_bmin(b[matpos2(uj^1,ui)],pr->tmp[0]);
      /* c_i X_i + c_j X_j + [-a,b] <= 0 <=>  c_i X_i + c_j X_j <= a */
      if (c==AP_CONS_SUP)
	*exact = 0; /* not exact for strict constraints */
      break;

    case OTHER:
      {
	/* general, approximated case */

	boundXXX_t tmpa, tmpb, Cb, cb;
	int Cinf = 0;            /* number of infinite upper bounds */
	size_t Cj1 = 0, Cj2 = 0; /* variable index with infinite bound */
	int cinf = 0;            /* number of infinite lower bounds */
	size_t cj1 = 0, cj2 = 0; /* variable index with infinite bound */

	*respect_closure = false; /* do not respect closure */

	boundXXX_init(tmpa); boundXXX_init(tmpb);
	boundXXX_init(Cb); boundXXX_init(cb);

	/* compute 2 * upper bound, ignoring components leading to +oo */
	boundXXX_mul_2exp(cb,pr->tmp[0],1);
	boundXXX_mul_2exp(Cb,pr->tmp[1],1);
	for (j=0;j<dim;j++) {
	  boundsXXX_mul(tmpa,tmpb, b[matpos(2*j,2*j+1)],b[matpos(2*j+1,2*j)],
			pr->tmp[2*j+2],pr->tmp[2*j+3], pr->tmp+2*(dim+1));
	  if (boundXXX_infty(tmpa)){
	    cinf++; cj2 = cj1; cj1 = j;
	  }
	  else
	    boundXXX_badd(cb,tmpa);
	  if (boundXXX_infty(tmpb)){
	    Cinf++; Cj2 = Cj1; Cj1 = j;
	  }
	  else
	    boundXXX_badd(Cb,tmpb);
	}

	/* upper bound */
	if (boundXXX_infty(Cb)) ;

	else if (!Cinf) {
	  /* no infinite bound: derive quadratic number of bounds */
	  for (j=0;j<dim;j++) {
	    if (boundXXX_cmp_int(pr->tmp[2*j+2],-1)<=0 &&
		!boundXXX_infty(b[matpos(2*j+1,2*j)])) {
	      /* -x <= expr-x <= max(expr) - max x */
	      boundXXX_sub(tmpa,Cb,b[matpos(2*j+1,2*j)]);
	      uj = 2*j+1;
	    }
	    else if (boundXXX_cmp_int(pr->tmp[2*j+3],-1)<=0 &&
		     !boundXXX_infty(b[matpos(2*j,2*j+1)])) {
	      /* x <= expr+x <= max(expr) - max(-x) */
	      boundXXX_sub(tmpa,Cb,b[matpos(2*j,2*j+1)]);
	      uj = 2*j;
	    }
	    else continue;
	    for (k=j+1;k<dim;k++) {
	      if (boundXXX_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
		  !boundXXX_infty(b[matpos(2*k+1,2*k)])) {
		/* (+/-)x -y <= max(expr) - max((+/-)x) - max y */
		boundXXX_sub(tmpb,tmpa,b[matpos(2*k+1,2*k)]);
		boundXXX_mul_2exp(tmpb,tmpb,-1);
		boundXXX_bmin(b[matpos(2*k,uj)],tmpb);
	      }
	      else if (boundXXX_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
		       !boundXXX_infty(b[matpos(2*k,2*k+1)])) {
		/* (+/-)x +y <= max(expr) - max((+/-)x) - max (-y) */
		boundXXX_sub(tmpb,tmpa,b[matpos(2*k,2*k+1)]);
		boundXXX_mul_2exp(tmpb,tmpb,-1);
		boundXXX_bmin(b[matpos(2*k+1,uj)],tmpb);
	      }
	    }
	  }
	}

	else if (Cinf==1) {
	  /* one infinite bound: derive linear number of bounds */
	  if (!boundXXX_cmp_int(pr->tmp[2*Cj1+3],-1) &&
	      !boundXXX_cmp_int(pr->tmp[2*Cj1+2], 1))
	    uj = 2*Cj1;
	  else if (!boundXXX_cmp_int(pr->tmp[2*Cj1+3], 1) &&
		   !boundXXX_cmp_int(pr->tmp[2*Cj1+2],-1))
	    uj = 2*Cj1+1;
	  else
	    goto Cbrk;
	  for (k=0;k<dim;k++) {
	    if (k==Cj1) continue;
	    if (boundXXX_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
		!boundXXX_infty(b[matpos(2*k+1,2*k)])) {
	      /* (+/-)x -y <= max(expr) - max((+/-)x) - max y */
	      boundXXX_sub(tmpb,Cb,b[matpos(2*k+1,2*k)]);
	      boundXXX_mul_2exp(tmpb,tmpb,-1);
	      boundXXX_bmin(b[matpos2(2*k,uj)],tmpb);
	    }
	    else if (boundXXX_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
		     !boundXXX_infty(b[matpos(2*k,2*k+1)])) {
	      /* (+/-)x +y <= max(expr) - max((+/-)x) - max (-y) */
	      boundXXX_sub(tmpb,Cb,b[matpos(2*k,2*k+1)]);
	      boundXXX_mul_2exp(tmpb,tmpb,-1);
	      boundXXX_bmin(b[matpos2(2*k+1,uj)],tmpb);
	    }
	  }
	}

	else if (Cinf==2) {
	  /* two infinite bounds: derive just one bound */
	  if (!boundXXX_cmp_int(pr->tmp[2*Cj1+3],-1) &&
	      !boundXXX_cmp_int(pr->tmp[2*Cj1+2], 1))
	    ui = 2*Cj1;
	  else if (!boundXXX_cmp_int(pr->tmp[2*Cj1+3], 1) &&
		   !boundXXX_cmp_int(pr->tmp[2*Cj1+2],-1))
	    ui = 2*Cj1+1;
	  else
	    goto Cbrk;
	  if (!boundXXX_cmp_int(pr->tmp[2*Cj2+3],-1) &&
	      !boundXXX_cmp_int(pr->tmp[2*Cj2+2], 1))
	    uj = 2*Cj2;
	  else if (!boundXXX_cmp_int(pr->tmp[2*Cj2+3], 1) &&
		   !boundXXX_cmp_int(pr->tmp[2*Cj2+2],-1))
	    uj = 2*Cj2+1;
	  else
	    goto Cbrk;
	  boundXXX_mul_2exp(tmpa,Cb,-1);
	  boundXXX_bmin(b[matpos2(uj^1,ui)],tmpa);
	}

	/* if more than two infinite bounds: do nothing */

      Cbrk:

	/* lower bound */
	if (c==AP_CONS_EQ) {

	  if (boundXXX_infty(cb)) ;
	  else if (!cinf) {
	    for (j=0;j<dim;j++) {
	      if (boundXXX_cmp_int(pr->tmp[2*j+3],-1)<=0 &&
		  !boundXXX_infty(b[matpos(2*j+1,2*j)])) {
		boundXXX_sub(tmpa,cb,b[matpos(2*j+1,2*j)]);
		uj = 2*j+1;
	      }
	      else if (boundXXX_cmp_int(pr->tmp[2*j+2],-1)<=0 &&
		       !boundXXX_infty(b[matpos(2*j,2*j+1)])) {
		boundXXX_sub(tmpa,cb,b[matpos(2*j,2*j+1)]);
		uj = 2*j;
	      }
	      else continue;
	      for (k=j+1;k<dim;k++) {
		if (boundXXX_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
		    !boundXXX_infty(b[matpos(2*k+1,2*k)])) {
		  boundXXX_sub(tmpb,tmpa,b[matpos(2*k+1,2*k)]);
		  boundXXX_mul_2exp(tmpb,tmpb,-1);
		  boundXXX_bmin(b[matpos(2*k,uj)],tmpb);
		}
		else if (boundXXX_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
			 !boundXXX_infty(b[matpos(2*k,2*k+1)])) {
		  boundXXX_sub(tmpb,tmpa,b[matpos(2*k,2*k+1)]);
		  boundXXX_mul_2exp(tmpb,tmpb,-1);
		  boundXXX_bmin(b[matpos(2*k+1,uj)],tmpb);
		}
	      }
	    }
	  }
	  else if (cinf==1) {
	    if (!boundXXX_cmp_int(pr->tmp[2*cj1+2],-1) &&
		!boundXXX_cmp_int(pr->tmp[2*cj1+3], 1))
	      uj = 2*cj1;
	    else if (!boundXXX_cmp_int(pr->tmp[2*cj1+2], 1) &&
		     !boundXXX_cmp_int(pr->tmp[2*cj1+3],-1))
	      uj = 2*cj1+1;
	    else
	      goto cbrk;
	    for (k=0;k<dim;k++) {
	      if (k==cj1) continue;
	      if (boundXXX_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
		  !boundXXX_infty(b[matpos(2*k+1,2*k)])) {
		boundXXX_sub(tmpb,cb,b[matpos(2*k+1,2*k)]);
		boundXXX_mul_2exp(tmpb,tmpb,-1);
		boundXXX_bmin(b[matpos2(2*k,uj)],tmpb);
	      }
	      else if (boundXXX_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
		       !boundXXX_infty(b[matpos(2*k,2*k+1)])) {
		boundXXX_sub(tmpb,cb,b[matpos(2*k,2*k+1)]);
		boundXXX_mul_2exp(tmpb,tmpb,-1);
		boundXXX_bmin(b[matpos2(2*k+1,uj)],tmpb);
	      }
	    }
	  }
	  else if (cinf==2) {
	    if (!boundXXX_cmp_int(pr->tmp[2*cj1+2],-1) &&
		!boundXXX_cmp_int(pr->tmp[2*cj1+3], 1))
	      ui = 2*cj1;
	    else if (!boundXXX_cmp_int(pr->tmp[2*cj1+2], 1) &&
		     !boundXXX_cmp_int(pr->tmp[2*cj1+3],-1))
	      ui = 2*cj1+1;
	    else
	      goto cbrk;
	    if (!boundXXX_cmp_int(pr->tmp[2*cj2+2],-1) &&
		!boundXXX_cmp_int(pr->tmp[2*cj2+3], 1))
	      uj = 2*cj2;
	    else if (!boundXXX_cmp_int(pr->tmp[2*cj2+2], 1) &&
		     !boundXXX_cmp_int(pr->tmp[2*cj2+3],-1))
	      uj = 2*cj2+1;
	    else
	      goto cbrk;
	    boundXXX_mul_2exp(tmpa,cb,-1);
	    boundXXX_bmin(b[matpos2(uj^1,ui)],tmpa);
	  }
	}

      cbrk:

	boundXXX_clear(tmpa); boundXXX_clear(tmpb);
	boundXXX_clear(Cb); boundXXX_clear(cb);
	*exact = 0;
      }
      break;

    default: assert(0);
    }
  }

  /* apply pending incremental closure now */
  if (*respect_closure && closure_pending)
    if (hmatXXX_close_incremental(b,dim,var_pending)) return true;

  return false;
}

void hmatXXX_add_lingen0_array(octXXX_internal_t* pr, boundXXX_t* b, size_t dim,
			       ap_lingen0_array_t ar)
{
  uexpr u;
  size_t i,j,k,size;
  boundXXX_t* bb;
  ap_lingen0_t gen;
  ap_linexpr0_t expr;
  ap_gentyp_t gentyp;
  /* add vertices */
  size = ap_lingen0_array_size(ar);
  for (i=0;i<size;i++) {
    ap_lingen0_array_ref_index(gen,ar,i);
    gentyp = ap_lingen0_get_gentyp(gen);
    if (gentyp!=AP_GEN_VERTEX) continue;
    ap_lingen0_linexpr0ref(expr,gen);
    boundsXXX_of_lingen0(pr,pr->tmp,expr,dim);
    for (bb=b,j=0;j<2*dim;j++)
      for (k=0;k<=(j|1);k++,bb++) {
	boundXXX_add(pr->tmp[2*dim],pr->tmp[j],pr->tmp[k^1]);
	boundXXX_max(*bb,*bb,pr->tmp[2*dim]);
      }
  }

  /* add others */
  for (i=0;i<size;i++) {
   ap_lingen0_array_ref_index(gen,ar,i);
   gentyp = ap_lingen0_get_gentyp(gen);
   ap_lingen0_linexpr0ref(expr,gen);
   switch (gentyp) {

      /* rays */
    case AP_GEN_RAY:
    case AP_GEN_RAYMOD:
      boundsXXX_of_lingen0(pr,pr->tmp+2,expr,dim);
      for (bb=b,j=0;j<2*dim;j++)
	for (k=0;k<=(j|1);k++,bb++) {
	  boundXXX_add(pr->tmp[0],pr->tmp[2+(k^1)],pr->tmp[2+j]);
	  if (boundXXX_sgn(pr->tmp[0])>0) boundXXX_set_infty(*bb,1);
	}
      break;

      /* lines */
    case AP_GEN_LINE:
    case AP_GEN_LINEMOD:
      boundsXXX_of_lingen0(pr,pr->tmp+2,expr,dim);
      for (bb=b,j=0;j<2*dim;j++)
	for (k=0;k<=(j|1);k++,bb++) {
	  boundXXX_add(pr->tmp[0],pr->tmp[2+(k^1)],pr->tmp[2+j]);
	  boundXXX_add(pr->tmp[1],pr->tmp[2+(j^1)],pr->tmp[2+k]);
	  if (boundXXX_sgn(pr->tmp[0])>0 ||
	      boundXXX_sgn(pr->tmp[1])>0) boundXXX_set_infty(*bb,1);
	}
      break;

    case AP_GEN_VERTEX: continue; /* already done */
    default: assert(0);
    }
  }
}



/* ============================================================ */
/* Meet constraints and Join generators */
/* ============================================================ */


octXXX_t* octXXX_meet_lincons_array(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_lincons0_array_t array)
{
  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY,2*(a->dim+8));
  if (!a->closed && !a->m)
    /* definitively empty */
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  else {
    bool exact, respect_closure;
    size_t i;
    boundXXX_t* m = a->closed ? a->closed : a->m;

    /* can / should we try to respect closure */
    respect_closure = (m==a->closed) && (pr->option.meet_lincons_array_closure);

    if (!destructive) m = hmatXXX_copy(pr,m,a->dim);

    /* go */
    if (hmatXXX_add_lincons0_array(pr,m,a->dim,array,&exact,&respect_closure)) {
      /* empty */
      if (!destructive) hmatXXX_free(pr,m,a->dim);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }
    else {
      /* exact if octagonal constraints & no conversion error */
      if (numXXX_incomplete || !exact) flag_incomplete;
      else if (pr->conv) flag_conv;

      if (respect_closure) return octXXX_set_mat(pr,a,NULL,m,destructive);
      else return octXXX_set_mat(pr,a,m,NULL,destructive);
    }
  }
}

octXXX_t* octXXX_meet_tcons_array(ap_manager_t* man,
			    bool destructive, octXXX_t* a,
			    ap_tcons0_array_t* array)
{
  return
    ap_generic_meet_intlinearize_tcons_array(
	man,destructive,a,array,
	numXXX_scalar, AP_LINEXPR_INTLINEAR,
	false,1,false,
	(void*(*)(ap_manager_t*, bool, void*, ap_lincons0_array_t))octXXX_meet_lincons_array
    );
}

octXXX_t* octXXX_add_ray_array(ap_manager_t* man,
			 bool destructive, octXXX_t* a,
			 ap_lingen0_array_t array)
{
  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,2*(a->dim+2));
  boundXXX_t *m;
  if (pr->option.add_ray_array_closure) octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m)
    /* definitively empty */
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  else {
    size_t i;
    if (!destructive) m = hmatXXX_copy(pr,m,a->dim);
    hmatXXX_add_lingen0_array(pr,m,a->dim,array);
    /* result is best on Q if closed and no conversion errors */
    man->result.flag_exact = false;
    if (numXXX_incomplete || a->intdim) flag_incomplete;
    else if (!a->closed) flag_algo;
    else if (pr->conv) flag_conv;
    return octXXX_set_mat(pr,a,m,NULL,destructive);
  }
}


/* ============================================================ */
/* Assignement and Substitutions */
/* ============================================================ */

/* internal helper function: forget all about dimension d */
static inline void hmatXXX_forget_var(boundXXX_t* m, size_t dim, size_t d)
{
  size_t k;
  for (k=0;k<2*d;k++) {
    boundXXX_set_infty(m[matpos(2*d,k)],1);
    boundXXX_set_infty(m[matpos(2*d+1,k)],1);
  }
  for (k=2*d+2;k<2*dim;k++) {
    boundXXX_set_infty(m[matpos(k,2*d)],1);
    boundXXX_set_infty(m[matpos(k,2*d+1)],1);
  }
  boundXXX_set_infty(m[matpos(2*d,2*d+1)],1);
  boundXXX_set_infty(m[matpos(2*d+1,2*d)],1);
}

/* internal helper function: apply assignment */
/* if *respect_closure=true, try to respect closure,
   when returning, *respect_closure=true only if this was possible
 */
static void hmatXXX_assign(octXXX_internal_t* pr, uexpr u, boundXXX_t* m, size_t dim,
			size_t d, bool* respect_closure)
{
  size_t i,k;

  if (u.type==ZERO ) {
    /* X <- [-a,b], non-invertible */
    if (*respect_closure) {
      /* 'respect closure' version */
      for (k=0;k<2*d;k++) {
	boundXXX_mul_2exp(pr->tmp[2],m[matpos(k^1,k)],-1);
	boundXXX_add(m[matpos(2*d,k)],pr->tmp[2],pr->tmp[0]);
	boundXXX_add(m[matpos(2*d+1,k)],pr->tmp[2],pr->tmp[1]);
      }
      for (k=2*d+2;k<2*dim;k++) {
	boundXXX_mul_2exp(pr->tmp[2],m[matpos(k,k^1)],-1);
	boundXXX_add(m[matpos(k,2*d)],pr->tmp[2],pr->tmp[1]);
	boundXXX_add(m[matpos(k,2*d+1)],pr->tmp[2],pr->tmp[0]);
      }
    }
    else {
      /* plain version */
      hmatXXX_forget_var(m,dim,d);
    }

    boundXXX_mul_2exp(m[matpos(2*d,2*d+1)],pr->tmp[0],1);
    boundXXX_mul_2exp(m[matpos(2*d+1,2*d)],pr->tmp[1],1);
  }

  else if (u.type==UNARY && u.i!=d) {
    /* X <- c_i X_i + [-a,b], X_i!=X, non-invertible */
    hmatXXX_forget_var(m,dim,d);
    i = 2*u.i + (u.coef_i==1 ? 0 : 1);
    boundXXX_set(m[matpos2(2*d,i)],pr->tmp[0]);
    boundXXX_set(m[matpos2(i,2*d)],pr->tmp[1]);
    if (*respect_closure)
      hmatXXX_close_incremental(m,dim,d);
  }

  else if (u.type==UNARY && u.coef_i==-1) {
    /* X <- - X + [-a,b], invertible */
    /* always respect closure */
    for (k=0;k<2*d;k++) {
      boundXXX_set(pr->tmp[2],m[matpos(2*d,k)]);
      boundXXX_add(m[matpos(2*d,k)],m[matpos(2*d+1,k)],pr->tmp[0]);
      boundXXX_add(m[matpos(2*d+1,k)],pr->tmp[2],pr->tmp[1]);
    }
    for (k=2*d+2;k<2*dim;k++) {
      boundXXX_set(pr->tmp[2],m[matpos(k,2*d)]);
      boundXXX_add(m[matpos(k,2*d)],m[matpos(k,2*d+1)],pr->tmp[1]);
      boundXXX_add(m[matpos(k,2*d+1)],pr->tmp[2],pr->tmp[0]);
    }
    boundXXX_mul_2exp(pr->tmp[0],pr->tmp[0],1);
    boundXXX_mul_2exp(pr->tmp[1],pr->tmp[1],1);
    boundXXX_set(pr->tmp[2],m[matpos(2*d+1,2*d)]);
    boundXXX_add(m[matpos(2*d+1,2*d)],m[matpos(2*d,2*d+1)],pr->tmp[1]);
    boundXXX_add(m[matpos(2*d,2*d+1)],pr->tmp[2],pr->tmp[0]);
  }

  else if (u.type==UNARY && u.coef_i==1) {
    /* X <- X + [-a,b], invertible */
    /* always respect closure */
    for (k=0;k<2*d;k++) {
      boundXXX_badd(m[matpos(2*d,k)],pr->tmp[0]);
      boundXXX_badd(m[matpos(2*d+1,k)],pr->tmp[1]);
    }
    for (k=2*d+2;k<2*dim;k++) {
      boundXXX_badd(m[matpos(k,2*d)],pr->tmp[1]);
      boundXXX_badd(m[matpos(k,2*d+1)],pr->tmp[0]);
    }
    boundXXX_mul_2exp(pr->tmp[0],pr->tmp[0],1);
    boundXXX_mul_2exp(pr->tmp[1],pr->tmp[1],1);
    boundXXX_badd(m[matpos(2*d+1,2*d)],pr->tmp[1]);
    boundXXX_badd(m[matpos(2*d,2*d+1)],pr->tmp[0]);
  }

  else {
    /* general, approximated case */

    boundXXX_t tmpa, tmpb, Cb, cb;
    int Cinf = 0;          /* number of infinite upper bounds */
    int cinf = 0;          /* number of infinite lower bounds */
    size_t Ci = 0, ci = 0; /* variable index with infinite bound */

    *respect_closure = false; /* does not respect closure */

    boundXXX_init(tmpa); boundXXX_init(tmpb); boundXXX_init(Cb); boundXXX_init(cb);

    /* compute 2 * expression bounds, ignoring components leading to +oo */
    boundXXX_mul_2exp(cb,pr->tmp[0],1);
    boundXXX_mul_2exp(Cb,pr->tmp[1],1);
    for (i=0;i<dim;i++) {
      boundsXXX_mul(tmpa,tmpb, m[matpos(2*i,2*i+1)],m[matpos(2*i+1,2*i)],
		 pr->tmp[2*i+2],pr->tmp[2*i+3], pr->tmp+2*(dim+1));
      if (boundXXX_infty(tmpa)) { cinf++; ci = i; } else boundXXX_badd(cb,tmpa);
      if (boundXXX_infty(tmpb)) { Cinf++; Ci = i; } else boundXXX_badd(Cb,tmpb);
    }

    hmatXXX_forget_var(m,dim,d);

    /* upper bounds */
    if (!Cinf) {
      /* no bound is infinite */
      boundXXX_set(m[matpos(2*d+1,2*d)],Cb); /* bound for x */
      for (i=0;i<dim;i++)  {
	if (i==d) continue;
	if (boundXXX_cmp_int(pr->tmp[2*i+2],-1)<=0 &&
	    !boundXXX_infty(m[matpos(2*i+1,2*i)])) {
	  /* bound for x-y */
	  boundXXX_sub(tmpa,Cb,m[matpos(2*i+1,2*i)]);
	  boundXXX_mul_2exp(m[matpos2(2*i,2*d)],tmpa,-1);
	}
	else if (boundXXX_cmp_int(pr->tmp[2*i+3],-1)<=0 &&
		 !boundXXX_infty(m[matpos(2*i,2*i+1)])) {
	  /* bound for x+y */
	  boundXXX_sub(tmpa,Cb,m[matpos(2*i,2*i+1)]);
	  boundXXX_mul_2exp(m[matpos2(2*i+1,2*d)],tmpa,-1);
	}
      }
    }
    else if (Cinf==1) {
      /* exactly one bound is infinite, X_d+/-X_Cinf may still be finite */
      if (Ci!=d) {
	if (!boundXXX_cmp_int(pr->tmp[2*i+3],1) &&
	    !boundXXX_cmp_int(pr->tmp[2*i+2],-1))
	  boundXXX_mul_2exp(m[matpos2(2*Ci,2*d)],Cb,-1);
	else if (!boundXXX_cmp_int(pr->tmp[2*i+3],-1) &&
		 !boundXXX_cmp_int(pr->tmp[2*i+2],1))
	  boundXXX_mul_2exp(m[matpos2(2*Ci+1,2*d)],Cb,-1);
      }
    }

    /* lower bounds */
    if (!cinf) {
      /* no bound is infinite */
      boundXXX_set(m[matpos(2*d,2*d+1)],cb); /* bound for -x */
      for (i=0;i<dim;i++)  {
	if (i==d) continue;
	if (boundXXX_cmp_int(pr->tmp[2*i+2],-1)<=0 &&
	    !boundXXX_infty(m[matpos(2*i,2*i+1)])) {
	  /* bound for -x+y */
	  boundXXX_sub(tmpa,cb,m[matpos(2*i,2*i+1)]);
	  boundXXX_mul_2exp(m[matpos2(2*i+1,2*d+1)],tmpa,-1);
	}
	else if (boundXXX_cmp_int(pr->tmp[2*i+3],-1)<=0 &&
		 !boundXXX_infty(m[matpos(2*i+1,2*i)])) {
	  /* bound for -x-y */
	  boundXXX_sub(tmpa,cb,m[matpos(2*i+1,2*i)]);
	  boundXXX_mul_2exp(m[matpos2(2*i,2*d+1)],tmpa,-1);
	}
      }
    }
    else if (cinf==1) {
      /* exactly one bound is infinite, -X_d+/-X_Cinf may still be finite */
      if (ci!=d) {
	if (!boundXXX_cmp_int(pr->tmp[2*i+3],1) &&
	    !boundXXX_cmp_int(pr->tmp[2*i+2],-1))
	  boundXXX_mul_2exp(m[matpos2(2*d,2*ci)],cb,-1);
	else if (!boundXXX_cmp_int(pr->tmp[2*i+3],-1) &&
		 !boundXXX_cmp_int(pr->tmp[2*i+2],1))
	  boundXXX_mul_2exp(m[matpos2(2*d,2*ci+1)],cb,-1);
      }
    }

    boundXXX_clear(tmpa); boundXXX_clear(tmpb); boundXXX_clear(Cb); boundXXX_clear(cb);
  }
}


/* internal helper function: apply substitution, retrun true if empty */
static bool hmatXXX_subst(octXXX_internal_t* pr, uexpr u, boundXXX_t* m, size_t dim,
		       size_t d, boundXXX_t* dst, bool* respect_closure)
{
  size_t i,k;
  if (u.type==ZERO ) {
    /* X -> [-a,b], non-invertible */

    *respect_closure = false; /* TODO: does it respect closure? */

    /* test satisfiability */
    boundXXX_mul_2exp(pr->tmp[2],pr->tmp[0],1);
    boundXXX_mul_2exp(pr->tmp[3],pr->tmp[1],1);
    boundXXX_add(pr->tmp[2],pr->tmp[2],m[matpos(2*d+1,2*d)]);
    boundXXX_add(pr->tmp[3],pr->tmp[3],m[matpos(2*d,2*d+1)]);
    if (boundXXX_sgn(pr->tmp[2])<0 || boundXXX_sgn(pr->tmp[3])<0) return true;

    /* infer unary contraints cX from binary constraints on cX + c'Xd */
    for (i=0;i<2*d;i++) {
      boundXXX_add(pr->tmp[2],pr->tmp[0],m[matpos(2*d+1,i^1)]);
      boundXXX_add(pr->tmp[3],pr->tmp[1],m[matpos(2*d,i^1)]);
      boundXXX_mul_2exp(pr->tmp[2],pr->tmp[2],1);
      boundXXX_mul_2exp(pr->tmp[3],pr->tmp[3],1);
      boundXXX_bmin(m[matpos(i,i^1)],pr->tmp[2]);
      boundXXX_bmin(m[matpos(i,i^1)],pr->tmp[3]);
    }
    for (i=2*d+2;i<2*dim;i++) {
      boundXXX_add(pr->tmp[2],pr->tmp[0],m[matpos(i,2*d)]);
      boundXXX_add(pr->tmp[3],pr->tmp[1],m[matpos(i,2*d+1)]);
      boundXXX_mul_2exp(pr->tmp[2],pr->tmp[2],1);
      boundXXX_mul_2exp(pr->tmp[3],pr->tmp[3],1);
      boundXXX_bmin(m[matpos(i,i^1)],pr->tmp[2]);
      boundXXX_bmin(m[matpos(i,i^1)],pr->tmp[3]);
    }
    hmatXXX_forget_var(m,dim,d);
    return false;
  }

  else if (u.type==UNARY && u.i!=d) {
    k = u.i*2 + (u.coef_i==1 ? 0 : 1 );
    /* X -> cX_i + [-a,b], X_i!=X, non-invertible */

    *respect_closure = false; /* TODO: does it respect closure? */

    /* test satisfiability */
    boundXXX_add(pr->tmp[2],pr->tmp[0],m[matpos2(k,2*d)]);
    boundXXX_add(pr->tmp[3],pr->tmp[1],m[matpos2(2*d,k)]);
    if (boundXXX_sgn(pr->tmp[2])<0 || boundXXX_sgn(pr->tmp[3])<0) return true;

    /* infer binary constraints by substitution */
    for (i=0;i<2*d;i++) {
      boundXXX_add(pr->tmp[2],pr->tmp[0],m[matpos(2*d+1,i)]);
      boundXXX_add(pr->tmp[3],pr->tmp[1],m[matpos(2*d,i)]);
      boundXXX_bmin(m[matpos2(k^1,i)],pr->tmp[2]);
      boundXXX_bmin(m[matpos2(k,i)],pr->tmp[3]);
    }
    for (i=2*d+2;i<2*dim;i++) {
      boundXXX_add(pr->tmp[2],pr->tmp[0],m[matpos(i^1,2*d)]);
      boundXXX_add(pr->tmp[3],pr->tmp[1],m[matpos(i^1,2*d+1)]);
      boundXXX_bmin(m[matpos2(k^1,i)],pr->tmp[2]);
      boundXXX_bmin(m[matpos2(k,i)],pr->tmp[3]);
    }

    /* infer unary constraints by substitution */
    boundXXX_mul_2exp(pr->tmp[2],pr->tmp[0],1);
    boundXXX_mul_2exp(pr->tmp[3],pr->tmp[1],1);
    boundXXX_badd(pr->tmp[2],m[matpos(2*d+1,d*2)]);
    boundXXX_badd(pr->tmp[3],m[matpos(2*d,d*2+1)]);
    boundXXX_bmin(m[matpos(k^1,k)],pr->tmp[2]);
    boundXXX_bmin(m[matpos(k,k^1)],pr->tmp[3]);

    hmatXXX_forget_var(m,dim,d);
    return false;
  }

  else if (u.type==UNARY && u.coef_i==-1) {
    /* X -> - X + [-a,b], invertible */
    /* equivalent to X <- -X + [-a,b] */
    hmatXXX_assign(pr,u,m,dim,d,respect_closure);
    return false;
  }

  else if (u.type==UNARY && u.coef_i==1) {
    /* X -> X + [-a,b], invertible */
    /* equivalent to X <- X + [-b,a] */
    boundXXX_set(pr->tmp[dim*2+3],pr->tmp[0]);
    boundXXX_set(pr->tmp[0],pr->tmp[1]);
    boundXXX_set(pr->tmp[1],pr->tmp[dim*2+3]);
    hmatXXX_assign(pr,u,m,dim,d,respect_closure);
    return false;
  }

  else {
    /* general, approximated case */

    /* TODO */

    /* for now, respects closure... */

    hmatXXX_forget_var(m,dim,d);
    return false;
  }
}

/* common simple case: X_d -> X_k */
static void hmatXXX_subst_var(boundXXX_t* m, size_t dim, size_t d, size_t k)
{
  size_t i;
  /* infer binary constraints by substitution */
  for (i=0;i<2*d;i++) {
    boundXXX_bmin(m[matpos2(2*k+1,i)],m[matpos(2*d+1,i)]);
    boundXXX_bmin(m[matpos2(2*k,i)],m[matpos(2*d,i)]);
  }
  for (i=2*d+2;i<2*dim;i++) {
    boundXXX_bmin(m[matpos2(2*k+1,i)],m[matpos(i^1,2*d)]);
    boundXXX_bmin(m[matpos2(2*k,i)],m[matpos(i^1,2*d+1)]);
  }

  /* infer unary constraints by substitution */
  boundXXX_bmin(m[matpos(2*k+1,2*k)],m[matpos(2*d+1,d*2)]);
  boundXXX_bmin(m[matpos(2*k,2*k+1)],m[matpos(2*d,d*2+1)]);
  hmatXXX_forget_var(m,dim,d);
}


octXXX_t* octXXX_assign_linexpr(ap_manager_t* man,
				bool destructive, octXXX_t* a,
				ap_dim_t d, ap_linexpr0_t expr,
				octXXX_t* dest)
{
  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY,2*(a->dim+1+5));
  uexpr u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,a->dim);
  boundXXX_t* m;
  bool respect_closure;
  arg_assert(d<a->dim,return NULL;);

  if (dest && !dest->closed && !dest->m)
    /* definitively empty due to dest*/
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);

  if (u.type==EMPTY)
    /* definitively empty due to empty expression */
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);

  /* useful to close only for non-invertible assignments */
  if ((u.type!=UNARY || u.i!=d) && pr->option.assign_closure)
    octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) return octXXX_set_mat(pr,a,NULL,NULL,destructive); /* empty */

  /* can / should we try to respect the closure */
  respect_closure = (m==a->closed) && (pr->option.assign_closure) && (!dest);

  if (!destructive) m = hmatXXX_copy(pr,m,a->dim);

  /* go */
  hmatXXX_assign(pr,u,m,a->dim,d,&respect_closure);

  /* exact on Q if zeroary or unary, closed arg and no conv error */
  if (u.type==BINARY || u.type==OTHER) flag_incomplete;
  else if (numXXX_incomplete || a->intdim) flag_incomplete;
  else if (!a->closed) flag_algo;
  else if (pr->conv) flag_conv;

  /* intersect with dest */
  if (dest) {
    boundXXX_t* m2 = dest->closed ? dest->closed : dest->m;
    size_t i;
    for (i=0;i<matsize(a->dim);i++)
      boundXXX_min(m[i],m[i],m2[i]);
  }

  if (respect_closure) return octXXX_set_mat(pr,a,NULL,m,destructive);
  else return octXXX_set_mat(pr,a,m,NULL,destructive);
}

octXXX_t* octXXX_substitute_linexpr(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_dim_t d, ap_linexpr0_t expr,
			      octXXX_t* dest)
{
  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,2*(a->dim+1+5));
  uexpr u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,a->dim);
  boundXXX_t * m, *m2;
  bool respect_closure;
  arg_assert(d<a->dim,return NULL;);

  m2 = dest ? (dest->closed ? dest->closed : dest->m) : NULL;

  if (dest && !m2)
    /* definitively empty due to dest*/
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);

  if (u.type==EMPTY)
    /* definitively empty due to empty expression */
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);

  /* useful to close only for non-invertible substitution */
  if ((u.type!=UNARY || u.i!=d) && pr->option.substitute_closure)
    octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) return octXXX_set_mat(pr,a,NULL,NULL,destructive); /* empty */

  /* can / should we try to respect the closure */
  respect_closure = (m==a->closed) && (pr->option.substitute_closure) && (!dest);

  if (!destructive) m = hmatXXX_copy(pr,m,a->dim);

  /* go */
  if (hmatXXX_subst(pr,u,m,a->dim,d,(boundXXX_t*)m2,&respect_closure)) {
    /* empty */
    if (!destructive) hmatXXX_free(pr,m,a->dim);
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  }

  if (u.type==BINARY || u.type==OTHER) flag_incomplete;
  else if (numXXX_incomplete || a->intdim) flag_incomplete;
  else if (!a->closed) flag_algo;
  else if (pr->conv) flag_conv;

  /* intersect with dest */
  if (m2) {
    size_t i;
    for (i=0;i<matsize(a->dim);i++)
      boundXXX_min(m[i],m[i],m2[i]);
  }

  if (respect_closure) return octXXX_set_mat(pr,a,NULL,m,destructive);
  else return octXXX_set_mat(pr,a,m,NULL,destructive);
}

octXXX_t* octXXX_assign_linexpr_array(
    ap_manager_t* man,
    bool destructive, octXXX_t* a,
    ap_dim_t* tdim,
    ap_linexpr0_array_t texpr,
    octXXX_t* dest)
{
  ap_linexpr0_t expr;
  size_t size = ap_linexpr0_array_size(texpr);

  if (size==1){
    ap_linexpr0_array_ref_index(expr,texpr,0);
    return octXXX_assign_linexpr(man,destructive,a,tdim[0],expr,dest);
  }

  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY,2*(a->dim+size+5));
  ap_dim_t* d = (ap_dim_t*) pr->tmp2;
  boundXXX_t *m, *mm;
  size_t i;
  ap_dim_t p = a->dim;
  int inexact = 0;
  bool respect_closure = false; /* TODO */

  /* checks */
  arg_assert(size>0,return NULL;);
  for (i=0;i<a->dim;i++) d[i] = 0;
  for (i=0;i<size;i++) {
    arg_assert(tdim[i]<a->dim,return NULL;);
    arg_assert(!d[tdim[i]],return NULL;); /* tdim has duplicate */
    d[tdim[i]] = 1;
  }

  if (dest && !dest->closed && !dest->m)
    /* definitively empty due to dest*/
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  if (pr->option.assign_closure) octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) return octXXX_set_mat(pr,a,NULL,NULL,destructive); /* empty */

  /* add temporary dimensions to hold destination variables */
  mm = hmatXXX_alloc_top(pr,a->dim+size);
  boundXXX_set_array(mm,m,matsize(a->dim));

  /* coefs in expr for temporary dimensions are set to 0 */
  for (i=0;i<2*size;i++)
    boundXXX_set_int(pr->tmp[2*a->dim+i+2],0);

  /* perform assignments */
  for (i=0;i<size;i++) {
    ap_linexpr0_array_ref_index(expr,texpr,i);
    uexpr u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,a->dim);

    if (u.type==EMPTY) {
      hmatXXX_free(pr,mm,a->dim+size);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }

    if (u.type==BINARY || u.type==OTHER) inexact = 1;

    hmatXXX_assign(pr,u,mm,a->dim+size,a->dim+i,&respect_closure);
  }

  /* now close & remove temporary variables */
  if (pr->option.assign_closure) {
    if (hmatXXX_close(mm,a->dim+size)) {
      /* empty */
      hmatXXX_free(pr,mm,a->dim+size);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }
  }
  else flag_algo;
  if (!destructive) m = hmatXXX_alloc(pr,a->dim);
  for (i=0;i<a->dim;i++) d[i] = i;
  for (i=0;i<size;i++) {
    d[a->dim+i] = tdim[i];
    d[tdim[i]] = a->dim;
  }
  hmatXXX_permute(m,mm,a->dim,a->dim+size,d);
  hmatXXX_free(pr,mm,a->dim+size);

  /* intersect with dest */
  if (dest) {
    boundXXX_t* m2 = dest->closed ? dest->closed : dest->m;
    size_t i;
    for (i=0;i<matsize(a->dim);i++)
      boundXXX_min(m[i],m[i],m2[i]);
  }

  if (inexact || a->intdim) flag_incomplete;
  else if (!a->closed) flag_algo;
  else if (pr->conv) flag_conv;

  return octXXX_set_mat(pr,a,m,NULL,destructive);
}

octXXX_t* octXXX_substitute_linexpr_array(
    ap_manager_t* man,
    bool destructive, octXXX_t* a,
    ap_dim_t* tdim,
    ap_linexpr0_array_t texpr,
    octXXX_t* dest)
{
  ap_linexpr0_t expr;
  size_t size = ap_linexpr0_array_size(texpr);

  if (size==1){
    ap_linexpr0_array_ref_index(expr,texpr,0);
    return octXXX_substitute_linexpr(man,destructive,a,tdim[0],expr,dest);
  }

  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
			     2*(a->dim+size+5));
  ap_dim_t* d = (ap_dim_t*) pr->tmp2;
  boundXXX_t *m, *mm, *m2;
  size_t i,j;
  ap_dim_t p = a->dim;
  int inexact = 0;
  bool respect_closure = false; /* TODO */

  /* checks */
  arg_assert(size>0,return NULL;);
  for (i=0;i<a->dim;i++) d[i] = 0;
  for (i=0;i<size;i++) {
    arg_assert(tdim[i]<a->dim,return NULL;);
    arg_assert(!d[tdim[i]],return NULL;); /* tdim has duplicate */
    d[tdim[i]] = 1;
  }

  m2 = dest ? (dest->closed ? dest->closed : dest->m) : NULL;
  if (dest && !m2)
    /* definitively empty due to dest*/
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  if (pr->option.substitute_closure) octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) return octXXX_set_mat(pr,a,NULL,NULL,destructive); /* empty */

  /* add temporary dimensions to hold destination variables */
  mm = hmatXXX_alloc_top(pr,a->dim+size);
  boundXXX_set_array(mm,m,matsize(a->dim));

  /* susbstitute org with temp variables */
  for (i=0;i<size;i++) {
    size_t dst = 2*(a->dim+i), src = 2*tdim[i];
    for (j=0;j<src;j++) {
      boundXXX_bmin(mm[matpos(dst+1,j)],mm[matpos(src+1,j)]);
      boundXXX_bmin(mm[matpos(dst,j)],mm[matpos(src,j)]);
    }
    for (j=src+2;j<2*a->dim+2*size;j++) {
      boundXXX_bmin(mm[matpos2(dst+1,j)],mm[matpos(j^1,src)]);
      boundXXX_bmin(mm[matpos2(dst,j)],mm[matpos(j^1,src+1)]);
    }
    boundXXX_bmin(mm[matpos(dst+1,dst)],mm[matpos(src+1,src)]);
    boundXXX_bmin(mm[matpos(dst,dst+1)],mm[matpos(src,src+1)]);
    hmatXXX_forget_var(mm,a->dim+size,tdim[i]);
  }

  /* coefs in expr for temporary dimensions are set to 0 */
  for (i=0;i<2*size;i++)
    boundXXX_set_int(pr->tmp[2*a->dim+i+2],0);

  /* perform substitutions */
  for (i=0;i<size;i++) {
    ap_linexpr0_array_ref_index(expr,texpr,i);
    uexpr u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,a->dim);

    if (u.type==EMPTY) {
      hmatXXX_free(pr,mm,a->dim+size);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }

    if (u.type==BINARY || u.type==OTHER) inexact = 1;

    if (hmatXXX_subst(pr,u,mm,a->dim+size,a->dim+i,m2,
		      &respect_closure)) {
      /* empty */
      hmatXXX_free(pr,mm,a->dim+size);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }
  }

  /* now close */
  if (pr->option.substitute_closure) {
    if (hmatXXX_close(mm,a->dim+size)) {
      /* empty */
      hmatXXX_free(pr,mm,a->dim+size);
      return octXXX_set_mat(pr,a,NULL,NULL,destructive);
    }
  }
  else flag_algo;

  /* remove temp */
  if (!destructive) m = hmatXXX_copy(pr,mm,a->dim);
  else boundXXX_set_array(m,mm,matsize(a->dim));
  hmatXXX_free(pr,mm,a->dim+size);

  /* intersect with dest */
  if (m2) {
    size_t i;
    for (i=0;i<matsize(a->dim);i++)
      boundXXX_min(m[i],m[i],m2[i]);
  }

  if (inexact || a->intdim) flag_incomplete;
  else if (!a->closed) flag_algo;
  else if (pr->conv) flag_conv;

  return octXXX_set_mat(pr,a,m,NULL,destructive);
}

octXXX_t* octXXX_assign_texpr_array(ap_manager_t* man,
				    bool destructive, octXXX_t* a,
				    ap_dim_t* tdim,
				    ap_texpr0_array_t* texpr,
				    octXXX_t* dest)
{
  return
    ap_generic_asssub_texpr_array(
	true, man,destructive,a,tdim,texpr,dest
    );
}
octXXX_t* octXXX_substitute_texpr_array(ap_manager_t* man,
					bool destructive, octXXX_t* a,
					ap_dim_t* tdim,
					ap_texpr0_array_t* texpr,
					octXXX_t* dest)
{
  return
    ap_generic_asssub_texpr_array(
	false, man,destructive,a,tdim,texpr,dest
    );
}
