/*
 * avo_transfer.c
 *
 * Assignment, substitution, guard transfer functions.
 *
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"
#include "ap_generic.h"

/* ============================================================ */
/* Expression classification */
/* ============================================================ */

uexpr avo_uexpr_of_linexpr(avo_internal_t* pr, bound_t* dst,
			   ap_linexpr0_t* e, size_t dim)
{

#define CLASS_COEFF(idx,coef)						\
  if (!bound_cmp_int(dst[2*idx+2],-coef) &&				\
      !bound_cmp_int(dst[2*idx+3],coef)) {				\
    if (u.type==ZERO) { u.type = UNARY;  u.i = idx; u.coef_i = coef; }	\
    else              { u.type = BINARY; u.j = idx; u.coef_j = coef; }	\
    continue;								\
  }

#define CLASS_VAR(idx)							\
  if (u.type==EMPTY) continue;						\
  if (!bound_sgn(dst[2*idx+2]) && !bound_sgn(dst[2*idx+3])) continue;	\
  if (u.type>=BINARY) { u.type = OTHER; continue; }			\
  CLASS_COEFF(idx,1);							\
  CLASS_COEFF(idx,-1);							\
  u.type = OTHER;

  uexpr u = { ZERO, 0, 0, 0, 0 };
  size_t i;
  if (avo_bounds_of_coeff(pr,dst[0],dst[1],e->cst,false)) u.type = EMPTY;
  switch (e->discr) {
   case AP_LINEXPR_DENSE:
    arg_assert(e->size<=dim,return u;);
    for (i=0;i<e->size;i++) {
      if (avo_bounds_of_coeff(pr,dst[2*i+2],dst[2*i+3],e->p.coeff[i],false))
    	  u.type = EMPTY;
      CLASS_VAR(i);
    }
    for (;i<dim;i++) {
      bound_set_int(dst[2*i+2],0);
      bound_set_int(dst[2*i+3],0);
    }
    break;
   case AP_LINEXPR_SPARSE:
    for (i=0;i<dim;i++) {
      bound_set_int(dst[2*i+2],0);
      bound_set_int(dst[2*i+3],0);
    }
    for (i=0;i<e->size;i++) {
      size_t d = e->p.linterm[i].dim;
      arg_assert(d<dim,return u;);
      if (avo_bounds_of_coeff(pr,dst[2*d+2],dst[2*d+3],e->p.linterm[i].coeff,false))
    	  u.type = EMPTY;
      CLASS_VAR(d);
    }
    break;
   default: arg_assert(0,return u;);
  }
  return u;
}

/* for debugging */
static void avo_print_uexpr(uexpr u, bound_t* dst, size_t dim)
{
  size_t i;
  printf("[-");
  bound_print(dst[0]); printf(","); bound_print(dst[1]);
  printf("]");
  for (i=0;i<dim;i++) {
    printf(" + [-");
    bound_print(dst[2*i+2]); printf(","); bound_print(dst[2*i+3]);
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

/* set *exact to 1 if all all constraints are avOctagonal ones
   return true if empty
 */
bool avo_hmat_add_lincons(avo_internal_t* pr, bound_t* b,bound_t *nsc, size_t dim,
		      ap_lincons0_array_t* ar, bool* exact,
		      bool* respect_closure)
{
  size_t i, j, k, ui, uj;
  size_t var_pending = 0; /* delay incremental closure as long as possible */
  int closure_pending = 0;
  int order;

  *exact = 1;
  for (i=0;i<ar->size;i++) {
    ap_constyp_t c = ar->p[i].constyp;
    uexpr u;

    switch (c) {
      /* skipped */
     case AP_CONS_EQMOD:
         continue;
     /* handled */
     case AP_CONS_DISEQ:
     case AP_CONS_EQ:
     case AP_CONS_SUPEQ:
     case AP_CONS_SUP:
      break;
      /* error */
     default:
      assert(0);
    }

    /* now handle ==, >=, >, != */
    u = avo_uexpr_of_linexpr(pr,pr->tmp,ar->p[i].linexpr0,dim);
    switch (u.type) {
    case EMPTY:
      /* empty constraint: no added information */
      break;

    case ZERO:
      if ((c==AP_CONS_SUPEQ && bound_sgn(pr->tmp[1])>=0) ||
	  /* [-a,b] >= 0 <=> b >= 0 */
	  (c==AP_CONS_SUP && bound_sgn(pr->tmp[1])>0) ||
	  /* [-a,b] > 0 <=> b > 0 */
	  (c==AP_CONS_EQ && bound_sgn(pr->tmp[0])>=0 && bound_sgn(pr->tmp[1])>=0) ||
	  /* [-a,b] = 0 <=> a >= 0 && b >= 0 */
	  (c==AP_CONS_DISEQ && (bound_sgn(pr->tmp[0])>0 || bound_sgn(pr->tmp[1])<0))
	  /* [-a,b] != 0 <=> a > 0 || b < 0 */
	  )
    	  ; /* trivial */
      else return true; /* unsatisfiable */
      break;

    case UNARY:
      /* can we delay incremental closure further? */
      if (*respect_closure && closure_pending && var_pending!=u.i) {
    	  if (avo_hmat_close_incremental(b,nsc,dim,var_pending)) return true;
      }
      closure_pending = 1;
      var_pending = u.i;

      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      bound_mul_2(pr->tmp[0],pr->tmp[0]); /*lower bound*/
      bound_mul_2(pr->tmp[1],pr->tmp[1]); /*upper bound*/

      /*      c_i X_i + [-a,b] != 0
       * <=> -c_i X_i < b || c_i X_i < a
       * <=> -|c_i| X_i < min(|a|,|b|)
       */
      if (c==AP_CONS_DISEQ){
    	  bound_abs(pr->tmp[0],pr->tmp[0]);
    	  bound_abs(pr->tmp[1],pr->tmp[1]);
    	  bound_min(pr->tmp[0],pr->tmp[0],pr->tmp[1]);
          int order=bound_cmp(pr->tmp[0],b[avo_matpos((2*dim+ui),(2*dim+ui)^1)]); /* -2*|xi|< */
          if(order<=0 ){
        	  bound_set(b[avo_matpos((2*dim+ui),(2*dim+ui)^1)],pr->tmp[0]);
              bound_set_int(nsc[avo_matpos((2*dim+ui),(2*dim+ui)^1)],0);
          }
      }
      else{
		  order=bound_cmp(pr->tmp[1], b[avo_matpos(ui,ui^1)]);   /* 2*xi <= */
		  if(order<0 ){
			  bound_set(b[avo_matpos(ui,ui^1)],pr->tmp[1]);
			  if (c==AP_CONS_SUPEQ || c==AP_CONS_EQ)
				  bound_set_infty(nsc[avo_matpos(ui,ui^1)],1);
			  else if(c==AP_CONS_SUP){
				  *exact = 0;
				  bound_set_int(nsc[avo_matpos(ui,ui^1)],0);
			  }
		  }
		  else if(order==0 && c==AP_CONS_SUP && bound_cmp_int(nsc[avo_matpos(ui,ui^1)],0)>0 ){
			  bound_set_int(nsc[avo_matpos(ui,ui^1)],0);
		  }

		   /*  c_i X_i + [-a,b] >= 0 <=> -c_i X_i <= b */
		   /*  c_i X_i + [-a,b] <= 0 <=>  c_i X_i <= a */
		  if (c==AP_CONS_EQ){
			   int order=bound_cmp(pr->tmp[0],b[avo_matpos(ui^1,ui)]);
			   if(order<0 ){
				   bound_set(b[avo_matpos(ui^1,ui)],pr->tmp[0]);
				   bound_set_infty(nsc[avo_matpos(ui^1,ui)],1);
			   }
		  }
      }
      break;

     case BINARY:
      /* can we delay incremental closure further? */
      if (*respect_closure && closure_pending &&
	  var_pending!=u.i && var_pending!=u.j) {
    	  if (avo_hmat_close_incremental(b,nsc,dim,var_pending)) return true;
      }
      *respect_closure = 1;
      closure_pending = 1;
      var_pending = (var_pending==u.j) ? u.j : u.i;

      if ( u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if ( u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;

	  order=bound_cmp(pr->tmp[1],b[avo_matpos2(uj,ui^1)]);
      if(order<0 ){
    	  bound_set(b[avo_matpos2(uj,ui^1)],pr->tmp[1]);
    	  if (c==AP_CONS_SUPEQ || c==AP_CONS_EQ)
    		  bound_set_infty(nsc[avo_matpos2(uj,ui^1)],1);
    	  else if(c==AP_CONS_SUP){
    		  *exact = 0;
    		  bound_set_int(nsc[avo_matpos2(uj,ui^1)],0);
    	  }
      }
	  else if(order==0 && c==AP_CONS_SUP && bound_cmp_int(nsc[avo_matpos2(uj,ui^1)],0)>0 ){
		  bound_set_int(nsc[avo_matpos2(uj,ui^1)],0);
	  }

      /*  c_i X_i + c_j X_j + [-a,b] >= 0 <=> -c_i X_i - c_j X_j <= b */
      /*  c_i X_i + c_j X_j + [-a,b] <= 0 <=>  c_i X_i + c_j X_j <= a */
      if (c==AP_CONS_EQ){
          int order=bound_cmp(pr->tmp[0],b[avo_matpos2(uj^1,ui)]);
          if(order<0 ){
        	  bound_set(b[avo_matpos2(uj^1,ui)],pr->tmp[0]);
              bound_set_infty(nsc[avo_matpos2(uj^1,ui)],1);
          }
      }
      break;

    case OTHER:
      {
		/* general, approximated case */
		bound_t tmpa, tmpb, Cb, cb;
		int Cinf = 0;            /* number of infinite upper bounds */
		size_t Cj1 = 0, Cj2 = 0; /* variable index with infinite bound */
		int cinf = 0;            /* number of infinite lower bounds */
		size_t cj1 = 0, cj2 = 0; /* variable index with infinite bound */
		*respect_closure = false; /* do not respect closure */

		bound_init(tmpa); bound_init(tmpb); bound_init(Cb); bound_init(cb);

		/* compute 2 * upper bound, ignoring components leading to +oo */
		bound_mul_2(cb,pr->tmp[0]);
		bound_mul_2(Cb,pr->tmp[1]);
		for (j=0;j<dim;j++) {
		  bounds_mul(tmpa,tmpb, b[avo_matpos(2*j,2*j+1)],b[avo_matpos(2*j+1,2*j)],
				 pr->tmp[2*j+2],pr->tmp[2*j+3], pr->tmp+2*(dim+1));
		  if (bound_infty(tmpa)) { cinf++; cj2 = cj1; cj1 = j; }
		  else bound_badd(cb,tmpa);
		  if (bound_infty(tmpb)) { Cinf++; Cj2 = Cj1; Cj1 = j; }
		  else bound_badd(Cb,tmpb);
		}

		/* upper bound */
		if (bound_infty(Cb)) ;
		else if (!Cinf) {
		  /* no infinite bound: derive quadratic number of bounds */
		  for (j=0;j<dim;j++) {
			if (bound_cmp_int(pr->tmp[2*j+2],-1)<=0 &&
			!bound_infty(b[avo_matpos(2*j+1,2*j)])) {
			  /* -x <= expr-x <= max(expr) - max x */
			  bound_sub(tmpa,Cb,b[avo_matpos(2*j+1,2*j)]);
			  uj = 2*j+1;
			}
			else if (bound_cmp_int(pr->tmp[2*j+3],-1)<=0 &&
				 !bound_infty(b[avo_matpos(2*j,2*j+1)])) {
			  /* x <= expr+x <= max(expr) - max(-x) */
			  bound_sub(tmpa,Cb,b[avo_matpos(2*j,2*j+1)]);
			  uj = 2*j;
			}
			else continue;
			for (k=j+1;k<dim;k++) {
			  if (bound_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
			  !bound_infty(b[avo_matpos(2*k+1,2*k)])) {
				/* (+/-)x -y <= max(expr) - max((+/-)x) - max y */
				bound_sub(tmpb,tmpa,b[avo_matpos(2*k+1,2*k)]);
				bound_div_2(tmpb,tmpb);
				bound_bmin(b[avo_matpos(2*k,uj)],tmpb);
			  }
			  else if (bound_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
				   !bound_infty(b[avo_matpos(2*k,2*k+1)])) {
				/* (+/-)x +y <= max(expr) - max((+/-)x) - max (-y) */
				bound_sub(tmpb,tmpa,b[avo_matpos(2*k,2*k+1)]);
				bound_div_2(tmpb,tmpb);
				bound_bmin(b[avo_matpos(2*k+1,uj)],tmpb);
			  }
			}
		  }
		}

		else if (Cinf==1) {
		  /* one infinite bound: derive linear number of bounds */
		  if (!bound_cmp_int(pr->tmp[2*Cj1+3],-1) &&
			  !bound_cmp_int(pr->tmp[2*Cj1+2], 1)) uj = 2*Cj1;
		  else if (!bound_cmp_int(pr->tmp[2*Cj1+3], 1) &&
			   !bound_cmp_int(pr->tmp[2*Cj1+2],-1)) uj = 2*Cj1+1;
		  else goto Cbrk;
		  for (k=0;k<dim;k++) {
			if (k==Cj1) continue;
			if (bound_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
			!bound_infty(b[avo_matpos(2*k+1,2*k)])) {
			  /* (+/-)x -y <= max(expr) - max((+/-)x) - max y */
			  bound_sub(tmpb,Cb,b[avo_matpos(2*k+1,2*k)]);
			  bound_div_2(tmpb,tmpb);
			  bound_bmin(b[avo_matpos2(2*k,uj)],tmpb);
			}
			else if (bound_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
				 !bound_infty(b[avo_matpos(2*k,2*k+1)])) {
			  /* (+/-)x +y <= max(expr) - max((+/-)x) - max (-y) */
			  bound_sub(tmpb,Cb,b[avo_matpos(2*k,2*k+1)]);
			  bound_div_2(tmpb,tmpb);
			  bound_bmin(b[avo_matpos2(2*k+1,uj)],tmpb);
			}
		  }
		}

		else if (Cinf==2) {
		  /* two infinite bounds: derive just one bound */
		  if (!bound_cmp_int(pr->tmp[2*Cj1+3],-1) &&
			  !bound_cmp_int(pr->tmp[2*Cj1+2], 1)) ui = 2*Cj1;
		  else if (!bound_cmp_int(pr->tmp[2*Cj1+3], 1) &&
			   !bound_cmp_int(pr->tmp[2*Cj1+2],-1)) ui = 2*Cj1+1;
		  else goto Cbrk;
		  if (!bound_cmp_int(pr->tmp[2*Cj2+3],-1) &&
			  !bound_cmp_int(pr->tmp[2*Cj2+2], 1)) uj = 2*Cj2;
		  else if (!bound_cmp_int(pr->tmp[2*Cj2+3], 1) &&
			   !bound_cmp_int(pr->tmp[2*Cj2+2],-1)) uj = 2*Cj2+1;
		  else goto Cbrk;
		  bound_div_2(tmpa,Cb);
		  bound_bmin(b[avo_matpos2(uj^1,ui)],tmpa);
		}

		/* if more than two infinite bounds: do nothing */
        Cbrk:
		  /* lower bound */
		  if (c==AP_CONS_EQ) {
			if (bound_infty(cb)) ;
			else if (!cinf) {
			  for (j=0;j<dim;j++) {
				if (bound_cmp_int(pr->tmp[2*j+3],-1)<=0 &&
				!bound_infty(b[avo_matpos(2*j+1,2*j)])) {
				  bound_sub(tmpa,cb,b[avo_matpos(2*j+1,2*j)]);
				  uj = 2*j+1;
				}
				else if (bound_cmp_int(pr->tmp[2*j+2],-1)<=0 &&
					 !bound_infty(b[avo_matpos(2*j,2*j+1)])) {
				  bound_sub(tmpa,cb,b[avo_matpos(2*j,2*j+1)]);
				  uj = 2*j;
				}
				else continue;
				for (k=j+1;k<dim;k++) {
				  if (bound_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
				  !bound_infty(b[avo_matpos(2*k+1,2*k)])) {
					bound_sub(tmpb,tmpa,b[avo_matpos(2*k+1,2*k)]);
					bound_div_2(tmpb,tmpb);
					bound_bmin(b[avo_matpos(2*k,uj)],tmpb);
				  }
				  else if (bound_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
					   !bound_infty(b[avo_matpos(2*k,2*k+1)])) {
					bound_sub(tmpb,tmpa,b[avo_matpos(2*k,2*k+1)]);
					bound_div_2(tmpb,tmpb);
					bound_bmin(b[avo_matpos(2*k+1,uj)],tmpb);
				  }
				}
			  }
			}
			else if (cinf==1) {
			  if (!bound_cmp_int(pr->tmp[2*cj1+2],-1) &&
				  !bound_cmp_int(pr->tmp[2*cj1+3], 1)) uj = 2*cj1;
			  else if (!bound_cmp_int(pr->tmp[2*cj1+2], 1) &&
				   !bound_cmp_int(pr->tmp[2*cj1+3],-1)) uj = 2*cj1+1;
			  else goto cbrk;
			  for (k=0;k<dim;k++) {
				if (k==cj1) continue;
				if (bound_cmp_int(pr->tmp[2*k+3],-1)<=0 &&
				!bound_infty(b[avo_matpos(2*k+1,2*k)])) {
				  bound_sub(tmpb,cb,b[avo_matpos(2*k+1,2*k)]);
				  bound_div_2(tmpb,tmpb);
				  bound_bmin(b[avo_matpos2(2*k,uj)],tmpb);
				}
				else if (bound_cmp_int(pr->tmp[2*k+2],-1)<=0 &&
					 !bound_infty(b[avo_matpos(2*k,2*k+1)])) {
				  bound_sub(tmpb,cb,b[avo_matpos(2*k,2*k+1)]);
				  bound_div_2(tmpb,tmpb);
				  bound_bmin(b[avo_matpos2(2*k+1,uj)],tmpb);
				}
			  }
			}
			else if (cinf==2) {
			  if (!bound_cmp_int(pr->tmp[2*cj1+2],-1) &&
				  !bound_cmp_int(pr->tmp[2*cj1+3], 1)) ui = 2*cj1;
			  else if (!bound_cmp_int(pr->tmp[2*cj1+2], 1) &&
				   !bound_cmp_int(pr->tmp[2*cj1+3],-1)) ui = 2*cj1+1;
			  else goto cbrk;
			  if (!bound_cmp_int(pr->tmp[2*cj2+2],-1) &&
				  !bound_cmp_int(pr->tmp[2*cj2+3], 1)) uj = 2*cj2;
			  else if (!bound_cmp_int(pr->tmp[2*cj2+2], 1) &&
				   !bound_cmp_int(pr->tmp[2*cj2+3],-1)) uj = 2*cj2+1;
			  else goto cbrk;
			  bound_div_2(tmpa,cb);
			  bound_bmin(b[avo_matpos2(uj^1,ui)],tmpa);
			}
		  }
        cbrk:
		  bound_clear(tmpa); bound_clear(tmpb); bound_clear(Cb); bound_clear(cb);
		  *exact = 0;
      }
      break;

    default: assert(0);
    }
  }

  /* apply pending incremental closure now */
  if (*respect_closure && closure_pending)
    if (avo_hmat_close_incremental(b,nsc,dim,var_pending)) return true;

  return false;
}

void avo_hmat_add_generators(avo_internal_t* pr, bound_t* b, size_t dim,
			 ap_generator0_array_t* ar)
{
  uexpr u;
  size_t i,j,k;
  bound_t* bb;

  /* add vertices */
  for (i=0;i<ar->size;i++) {
    if (ar->p[i].gentyp!=AP_GEN_VERTEX) continue;
    avo_bounds_of_generator(pr,pr->tmp,ar->p[i].linexpr0,dim);
    for (bb=b,j=0;j<2*dim;j++)
      for (k=0;k<=(j|1);k++,bb++) {
		bound_add(pr->tmp[2*dim],pr->tmp[j],pr->tmp[k^1]);
		bound_max(*bb,*bb,pr->tmp[2*dim]);
      }
  }

  /* add others */
  for (i=0;i<ar->size;i++) {
    switch (ar->p[i].gentyp) {

      /* rays */
    case AP_GEN_RAY:
    case AP_GEN_RAYMOD:
      avo_bounds_of_generator(pr,pr->tmp+2,ar->p[i].linexpr0,dim);
      for (bb=b,j=0;j<2*dim;j++)
		for (k=0;k<=(j|1);k++,bb++) {
		  bound_add(pr->tmp[0],pr->tmp[2+(k^1)],pr->tmp[2+j]);
		  if (bound_sgn(pr->tmp[0])>0) bound_set_infty(*bb,1);
		}
      break;

      /* lines */
    case AP_GEN_LINE:
    case AP_GEN_LINEMOD:
      avo_bounds_of_generator(pr,pr->tmp+2,ar->p[i].linexpr0,dim);
      for (bb=b,j=0;j<2*dim;j++)
		for (k=0;k<=(j|1);k++,bb++) {
		  bound_add(pr->tmp[0],pr->tmp[2+(k^1)],pr->tmp[2+j]);
		  bound_add(pr->tmp[1],pr->tmp[2+(j^1)],pr->tmp[2+k]);
		  if (bound_sgn(pr->tmp[0])>0 ||
			  bound_sgn(pr->tmp[1])>0) bound_set_infty(*bb,1);
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


avo_t* avo_meet_lincons_array(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_lincons0_array_t* array)
{
  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY,2*(2*a->dim+8));
  if (!a->closed && !a->m)
    /* definitively empty */
    return avo_set_mat_nsc(pr,a,NULL,NULL,NULL,destructive);
  else {
    bool exact, respect_closure;
    size_t i;
    bound_t* m = a->closed ? a->closed : a->m;
    bound_t * nsc = a->nsc;
    /* can / should we try to respect closure */
    respect_closure = (m==a->closed) && (pr->funopt->algorithm>=0);

    if (!destructive) m = avo_hmat_copy(pr,m,a->dim);
    if (!destructive) nsc = avo_hmat_copy(pr,nsc,a->dim);
    /* go */
    if (avo_hmat_add_lincons(pr,m,nsc,a->dim,array,&exact,&respect_closure)) {
      /* empty */
      if (!destructive) avo_hmat_free(pr,m,a->dim);
      if (!destructive) avo_hmat_free(pr,nsc,a->dim);
      return avo_set_mat_nsc(pr,a,NULL,NULL,NULL,destructive);
    }
    else {
      /* exact if avOctagonal constraints & no conversion error */
      if (num_incomplete || !exact) flag_incomplete;
      else if (pr->conv) flag_conv;
      if (respect_closure) return avo_set_mat_nsc(pr,a,NULL,m,nsc,destructive);
      else return avo_set_mat_nsc(pr,a,m,NULL,nsc,destructive);
    }
  }
}

avo_t* avo_meet_tcons_array(ap_manager_t* man,
			    bool destructive, avo_t* a,
			    ap_tcons0_array_t* array)
{
  return ap_generic_meet_intlinearize_tcons_array(man,destructive,a,array,
#if defined(NUM_NUMFLT)
				     AP_SCALAR_DOUBLE
#else
				     AP_SCALAR_MPQ
#endif
				     ,
				     AP_LINEXPR_INTLINEAR,
				     (void*) &avo_meet_lincons_array);
}

avo_t* avo_add_ray_array(ap_manager_t* man,
			 bool destructive, avo_t* a,
			 ap_generator0_array_t* array)
{
  // fprintf(stdout, "AP_FUNID_ADD_RAY_ARRAY\n"); fflush(stdout);
  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,2*(2*a->dim+2));
  bound_t *m;
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m)
    /* definitively empty */
    return avo_set_mat_nsc(pr,a,NULL,NULL,NULL,destructive);
  else {
    size_t i;
    if (!destructive) m = avo_hmat_copy(pr,m,a->dim);
    avo_hmat_add_generators(pr,m,a->dim,array);
    /* result is best on Q if closed and no conversion errors */
    man->result.flag_exact = false;
    if (num_incomplete || a->intdim) flag_incomplete;
    else if (!a->closed) flag_algo;
    else if (pr->conv) flag_conv;
    return avo_set_mat_nsc(pr,a,m,NULL,NULL,destructive);
  }
}

/* ============================================================ */
/* Assignement and Substitutions */
/* ============================================================ */

/* internal helper function: forget all about dimension d */
static inline void avo_hmat_forget_var(bound_t* m, size_t dim, size_t d)
{
  size_t k;
  for (k=0;k<2*d;k++) {
    bound_set_infty(m[avo_matpos2(2*d,k)],1);
    bound_set_infty(m[avo_matpos2(2*d+1,k)],1);
    bound_set_infty(m[avo_matpos2(2*d+2*dim,k)],1);
    bound_set_infty(m[avo_matpos2(2*d+1+2*dim,k)],1);
  }
  for (k=2*d+2;k<4*dim;k++) {
    bound_set_infty(m[avo_matpos2(k,2*d)],1);
    bound_set_infty(m[avo_matpos2(k,2*d+1)],1);
    bound_set_infty(m[avo_matpos2(k,2*d+2*dim)],1);
    bound_set_infty(m[avo_matpos2(k,2*d+1+2*dim)],1);
  }
  bound_set_infty(m[avo_matpos2(2*d,2*d+1)],1);
  bound_set_infty(m[avo_matpos2(2*d+1,2*d)],1);
  bound_set_infty(m[avo_matpos2(2*d+2*dim,2*d+1+2*dim)],1);
  bound_set_infty(m[avo_matpos2(2*d+1+2*dim,2*d+2*dim)],1);
  bound_set_infty(m[avo_matpos2(2*d,2*d+1+2*dim)],1);
  bound_set_infty(m[avo_matpos2(2*d+1,2*d+2*dim)],1);
  bound_set_infty(m[avo_matpos2(2*d+2*dim,2*d+1)],1);
  bound_set_infty(m[avo_matpos2(2*d+1+2*dim,2*d)],1);
}

/* internal helper function: apply assignment */
/* if *respect_closure=true, try to respect closure,
   when returning, *respect_closure=true only if this was possible
 */
static void avo_hmat_assign(avo_internal_t* pr, uexpr u, bound_t* m, bound_t * nsc,size_t dim,
			size_t d, bool* respect_closure)
{
  size_t i,k;
  for(i = 0; i < 4 * dim ; i++)
  {
   bound_set_infty(m[avo_matpos2(2*d+2*dim,i)],1);
   bound_set_infty(m[avo_matpos2(2*d+2*dim+1,i)],1);
   bound_set_infty(nsc[avo_matpos2(2*d+2*dim,i)],1);
   bound_set_infty(nsc[avo_matpos2(2*d+2*dim+1,i)],1);
   bound_set_infty(m[avo_matpos2(i,2*d+2*dim)],1);
   bound_set_infty(m[avo_matpos2(i,2*d+2*dim+1)],1);
   bound_set_infty(nsc[avo_matpos2(i,2*d+2*dim)],1);
   bound_set_infty(nsc[avo_matpos2(i,2*d+2*dim+1)],1);
  }
   if (u.type==ZERO ) {
    /* X <- [-a,b], non-invertible */
    if (*respect_closure) {
      /* 'respect closure' version */
      for (k=0;k<2*d;k++) {
		bound_div_2(pr->tmp[2],m[avo_matpos(k^1,k)]);
		bound_add(m[avo_matpos2(2*d,k)],pr->tmp[2],pr->tmp[0]);
		bound_set(nsc[avo_matpos2(2*d,k)],nsc[avo_matpos(k^1,k)]);
		bound_add(m[avo_matpos2(2*d+1,k)],pr->tmp[2],pr->tmp[1]);
		bound_set(nsc[avo_matpos2(2*d+1,k)],nsc[avo_matpos(k^1,k)]);
		bound_set_infty(m[avo_matpos2(2*d+1+2*dim,k)],1);
		bound_set_infty(m[avo_matpos2(2*d+2*dim,k)],1);
		bound_set_infty(nsc[avo_matpos2(2*d+1+2*dim,k)],1);
		bound_set_infty(nsc[avo_matpos2(2*d+2*dim,k)],1);
      }
      for (k=2*d+2;k<4*dim;k++) {
		bound_div_2(pr->tmp[2],m[avo_matpos2(k,k^1)]);
		bound_add(m[avo_matpos2(k,2*d)],pr->tmp[2],pr->tmp[1]);
		bound_set(nsc[avo_matpos2(k,2*d)],nsc[avo_matpos2(k,k^1)]);
		bound_add(m[avo_matpos2(k,2*d+1)],pr->tmp[2],pr->tmp[0]);
		bound_set(nsc[avo_matpos2(k,2*d+1)],nsc[avo_matpos2(k,k^1)]);

		bound_set_infty(m[avo_matpos2(k,2*d+2*dim)],1);
		bound_set_infty(m[avo_matpos2(k,2*d+2*dim+1)],1);
		bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim)],1);
		bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim+1)],1);
      }
    }
    else {
      /* plain version */
      avo_hmat_forget_var(m,dim,d);
      avo_hmat_forget_var(nsc,dim,d);
    }

    bound_mul_2(m[avo_matpos(2*d,2*d+1)],pr->tmp[0]);
    bound_mul_2(m[avo_matpos(2*d+1,2*d)],pr->tmp[1]);
    bound_set_infty(m[avo_matpos(2*d+2*dim,2*d+1+2*dim)],1);
    bound_set_infty(m[avo_matpos(2*d+2*dim+1,2*d+2*dim)],1);

  }

  else if (u.type==UNARY && u.i!=d) {
    /* X <- c_i X_i + [-a,b], X_i!=X, non-invertible */
    avo_hmat_forget_var(m,dim,d);
    avo_hmat_forget_var(nsc,dim,d);
    i = 2*u.i + (u.coef_i==1 ? 0 : 1);
    bound_set(m[avo_matpos2(2*d,i)],pr->tmp[0]);
    bound_set(m[avo_matpos2(i,2*d)],pr->tmp[1]);
    if (*respect_closure)
      avo_hmat_close_incremental(m,nsc,dim,d);
  }

  else if (u.type==UNARY && u.coef_i==-1) {
    /* X <- - X + [-a,b], invertible */
    /* always respect closure */
    for (k=0;k<2*d;k++) {
      bound_set(pr->tmp[2],m[avo_matpos2(2*d,k)]);
      bound_add(m[avo_matpos2(2*d,k)],m[avo_matpos2(2*d+1,k)],pr->tmp[0]);
      bound_set(nsc[avo_matpos2(2*d,k)],nsc[avo_matpos2(2*d+1,k)]);
      bound_add(m[avo_matpos2(2*d+1,k)],pr->tmp[2],pr->tmp[1]);
      bound_set(nsc[avo_matpos2(2*d+1,k)],nsc[avo_matpos2(2*d,k)]);
      bound_set_infty(m[avo_matpos2(2*d+1+2*dim,k)],1);
      bound_set_infty(m[avo_matpos2(2*d+2*dim,k)],1);
      bound_set_infty(nsc[avo_matpos2(2*d+1+2*dim,k)],1);
      bound_set_infty(nsc[avo_matpos2(2*d+2*dim,k)],1);
    }
    for (k=2*d+2;k<4*dim;k++) {
      bound_set(pr->tmp[2],m[avo_matpos2(k,2*d)]);
      bound_add(m[avo_matpos2(k,2*d)],m[avo_matpos2(k,2*d+1)],pr->tmp[1]);
      bound_set(nsc[avo_matpos2(k,2*d)],nsc[avo_matpos2(k,2*d+1)]);
      bound_add(m[avo_matpos2(k,2*d+1)],pr->tmp[2],pr->tmp[0]);
      bound_set(nsc[avo_matpos2(k,2*d+1)],nsc[avo_matpos2(k,2*d)]);
      bound_set_infty(m[avo_matpos2(k,2*d+2*dim)],1);
      bound_set_infty(m[avo_matpos2(k,2*d+2*dim+1)],1);
      bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim)],1);
      bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim+1)],1);
    }
    bound_mul_2(pr->tmp[0],pr->tmp[0]);
    bound_mul_2(pr->tmp[1],pr->tmp[1]);
    bound_set(pr->tmp[2],m[avo_matpos(2*d+1,2*d)]);
    bound_add(m[avo_matpos(2*d+1,2*d)],m[avo_matpos(2*d,2*d+1)],pr->tmp[1]);

    bound_add(m[avo_matpos(2*d,2*d+1)],pr->tmp[2],pr->tmp[0]);

  }

  else if (u.type==UNARY && u.coef_i==1) {
    /* X <- X + [-a,b], invertible */
    /* always respect closure */
    for (k=0;k<2*d;k++) {
      bound_badd(m[avo_matpos2(2*d,k)],pr->tmp[0]);
      bound_badd(m[avo_matpos2(2*d+1,k)],pr->tmp[1]);
      bound_set_infty(m[avo_matpos2(2*d+1+2*dim,k)],1);
      bound_set_infty(m[avo_matpos2(2*d+2*dim,k)],1);
      bound_set_infty(nsc[avo_matpos2(2*d+1+2*dim,k)],1);
      bound_set_infty(nsc[avo_matpos2(2*d+2*dim,k)],1);

    }
    for (k=2*d+2;k<4*dim;k++) {
      bound_badd(m[avo_matpos2(k,2*d)],pr->tmp[1]);
      bound_badd(m[avo_matpos2(k,2*d+1)],pr->tmp[0]);
      bound_set_infty(m[avo_matpos2(k,2*d+2*dim)],1);
      bound_set_infty(m[avo_matpos2(k,2*d+2*dim+1)],1);
      bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim)],1);
      bound_set_infty(nsc[avo_matpos2(k,2*d+2*dim+1)],1);
    }
    bound_mul_2(pr->tmp[0],pr->tmp[0]);
    bound_mul_2(pr->tmp[1],pr->tmp[1]);
    bound_badd(m[avo_matpos(2*d+1,2*d)],pr->tmp[1]);
    bound_badd(m[avo_matpos(2*d,2*d+1)],pr->tmp[0]);
  }

  else {
    /* general, approximated case */

    bound_t tmpa, tmpb, Cb, cb;
    int Cinf = 0;          /* number of infinite upper bounds */
    int cinf = 0;          /* number of infinite lower bounds */
    size_t Ci = 0, ci = 0; /* variable index with infinite bound */

    *respect_closure = false; /* does not respect closure */

    bound_init(tmpa); bound_init(tmpb); bound_init(Cb); bound_init(cb);

    /* compute 2 * expression bounds, ignoring components leading to +oo */
    bound_mul_2(cb,pr->tmp[0]);
    bound_mul_2(Cb,pr->tmp[1]);
    for (i=0;i<dim;i++) {
      bounds_mul(tmpa,tmpb, m[avo_matpos(2*i,2*i+1)],m[avo_matpos(2*i+1,2*i)],
		 pr->tmp[2*i+2],pr->tmp[2*i+3], pr->tmp+2*(dim+1));
      if (bound_infty(tmpa)) { cinf++; ci = i; } else bound_badd(cb,tmpa);
      if (bound_infty(tmpb)) { Cinf++; Ci = i; } else bound_badd(Cb,tmpb);
    }

    avo_hmat_forget_var(m,dim,d);
    avo_hmat_forget_var(nsc,dim,d);

    /* upper bounds */
    if (!Cinf) {
      /* no bound is infinite */
      bound_set(m[avo_matpos(2*d+1,2*d)],Cb); /* bound for x */
      for (i=0;i<dim;i++)  {
		if (i==d) continue;
		if (bound_cmp_int(pr->tmp[2*i+2],-1)<=0 &&
			!bound_infty(m[avo_matpos(2*i+1,2*i)])) {
		  /* bound for x-y */
		  bound_sub(tmpa,Cb,m[avo_matpos(2*i+1,2*i)]);
		  bound_div_2(m[avo_matpos2(2*i,2*d)],tmpa);
		}
		else if (bound_cmp_int(pr->tmp[2*i+3],-1)<=0 &&
			 !bound_infty(m[avo_matpos(2*i,2*i+1)])) {
		  /* bound for x+y */
		  bound_sub(tmpa,Cb,m[avo_matpos(2*i,2*i+1)]);
		  bound_div_2(m[avo_matpos2(2*i+1,2*d)],tmpa);
		}
      }
    }
    else if (Cinf==1) {
      /* exactly one bound is infinite, X_d+/-X_Cinf may still be finite */
      if (Ci!=d) {
		if (!bound_cmp_int(pr->tmp[2*i+3],1) &&
			!bound_cmp_int(pr->tmp[2*i+2],-1))
		  bound_div_2(m[avo_matpos2(2*Ci,2*d)],Cb);
		else if (!bound_cmp_int(pr->tmp[2*i+3],-1) &&
			 !bound_cmp_int(pr->tmp[2*i+2],1))
		  bound_div_2(m[avo_matpos2(2*Ci+1,2*d)],Cb);
      }
    }

    /* lower bounds */
    if (!cinf) {
      /* no bound is infinite */
      bound_set(m[avo_matpos(2*d,2*d+1)],cb); /* bound for -x */
      for (i=0;i<dim;i++)  {
		if (i==d) continue;
		if (bound_cmp_int(pr->tmp[2*i+2],-1)<=0 &&
			!bound_infty(m[avo_matpos(2*i,2*i+1)])) {
		  /* bound for -x+y */
		  bound_sub(tmpa,cb,m[avo_matpos(2*i,2*i+1)]);
		  bound_div_2(m[avo_matpos2(2*i+1,2*d+1)],tmpa);
		}
		else if (bound_cmp_int(pr->tmp[2*i+3],-1)<=0 &&
			 !bound_infty(m[avo_matpos(2*i+1,2*i)])) {
		  /* bound for -x-y */
		  bound_sub(tmpa,cb,m[avo_matpos(2*i+1,2*i)]);
		  bound_div_2(m[avo_matpos2(2*i,2*d+1)],tmpa);
		}
      }
    }
    else if (cinf==1) {
      /* exactly one bound is infinite, -X_d+/-X_Cinf may still be finite */
      if (ci!=d) {
		if (!bound_cmp_int(pr->tmp[2*i+3],1) &&
			!bound_cmp_int(pr->tmp[2*i+2],-1))
		  bound_div_2(m[avo_matpos2(2*d,2*ci)],cb);
		else if (!bound_cmp_int(pr->tmp[2*i+3],-1) &&
			 !bound_cmp_int(pr->tmp[2*i+2],1))
		  bound_div_2(m[avo_matpos2(2*d,2*ci+1)],cb);
      }
    }

    bound_clear(tmpa); bound_clear(tmpb); bound_clear(Cb); bound_clear(cb);
  }
}


/* internal helper function: apply substitution, retrun true if empty */
static bool avo_hmat_subst(avo_internal_t* pr, uexpr u, bound_t* m,bound_t * nsc, size_t dim,
		       size_t d, bound_t* dst, bool* respect_closure)
{
  size_t i,k;
  if (u.type==ZERO ) {
    /* X -> [-a,b], non-invertible */

    *respect_closure = false; /* TODO: does it respect closure? */

    /* test satisfiability */
    bound_mul_2(pr->tmp[2],pr->tmp[0]);
    bound_mul_2(pr->tmp[3],pr->tmp[1]);
    bound_add(pr->tmp[2],pr->tmp[2],m[avo_matpos(2*d+1,2*d)]);
    bound_add(pr->tmp[3],pr->tmp[3],m[avo_matpos(2*d,2*d+1)]);
    if (bound_sgn(pr->tmp[2])<0 || bound_sgn(pr->tmp[3])<0) return true;

    /* infer unary contraints cX from binary constraints on cX + c'Xd */
    for (i=0;i<2*d;i++) {
      bound_add(pr->tmp[2],pr->tmp[0],m[avo_matpos(2*d+1,i^1)]);
      bound_add(pr->tmp[3],pr->tmp[1],m[avo_matpos(2*d,i^1)]);
      bound_mul_2(pr->tmp[2],pr->tmp[2]);
      bound_mul_2(pr->tmp[3],pr->tmp[3]);
      bound_bmin(m[avo_matpos(i,i^1)],pr->tmp[2]);
      bound_bmin(m[avo_matpos(i,i^1)],pr->tmp[3]);
    }
    for (i=2*d+2;i<2*dim;i++) {
      bound_add(pr->tmp[2],pr->tmp[0],m[avo_matpos(i,2*d)]);
      bound_add(pr->tmp[3],pr->tmp[1],m[avo_matpos(i,2*d+1)]);
      bound_mul_2(pr->tmp[2],pr->tmp[2]);
      bound_mul_2(pr->tmp[3],pr->tmp[3]);
      bound_bmin(m[avo_matpos(i,i^1)],pr->tmp[2]);
      bound_bmin(m[avo_matpos(i,i^1)],pr->tmp[3]);
    }
    avo_hmat_forget_var(m,dim,d);
    return false;
  }

  else if (u.type==UNARY && u.i!=d) {
    k = u.i*2 + (u.coef_i==1 ? 0 : 1 );
    /* X -> cX_i + [-a,b], X_i!=X, non-invertible */

    *respect_closure = false; /* TODO: does it respect closure? */

    /* test satisfiability */
    bound_add(pr->tmp[2],pr->tmp[0],m[avo_matpos2(k,2*d)]);
    bound_add(pr->tmp[3],pr->tmp[1],m[avo_matpos2(2*d,k)]);
    if (bound_sgn(pr->tmp[2])<0 || bound_sgn(pr->tmp[3])<0) return true;

    /* infer binary constraints by substitution */
    for (i=0;i<2*d;i++) {
      bound_add(pr->tmp[2],pr->tmp[0],m[avo_matpos(2*d+1,i)]);
      bound_add(pr->tmp[3],pr->tmp[1],m[avo_matpos(2*d,i)]);
      bound_bmin(m[avo_matpos2(k^1,i)],pr->tmp[2]);
      bound_bmin(m[avo_matpos2(k,i)],pr->tmp[3]);
    }
    for (i=2*d+2;i<2*dim;i++) {
      bound_add(pr->tmp[2],pr->tmp[0],m[avo_matpos(i^1,2*d)]);
      bound_add(pr->tmp[3],pr->tmp[1],m[avo_matpos(i^1,2*d+1)]);
      bound_bmin(m[avo_matpos2(k^1,i)],pr->tmp[2]);
      bound_bmin(m[avo_matpos2(k,i)],pr->tmp[3]);
    }

    /* infer unary constraints by substitution */
    bound_mul_2(pr->tmp[2],pr->tmp[0]);
    bound_mul_2(pr->tmp[3],pr->tmp[1]);
    bound_badd(pr->tmp[2],m[avo_matpos(2*d+1,d*2)]);
    bound_badd(pr->tmp[3],m[avo_matpos(2*d,d*2+1)]);
    bound_bmin(m[avo_matpos(k^1,k)],pr->tmp[2]);
    bound_bmin(m[avo_matpos(k,k^1)],pr->tmp[3]);

    avo_hmat_forget_var(m,dim,d);
    return false;
  }

  else if (u.type==UNARY && u.coef_i==-1) {
    /* X -> - X + [-a,b], invertible */
    /* equivalent to X <- -X + [-a,b] */
    avo_hmat_assign(pr,u,m,nsc,dim,d,respect_closure);
    return false;
  }

  else if (u.type==UNARY && u.coef_i==1) {
    /* X -> X + [-a,b], invertible */
    /* equivalent to X <- X + [-b,a] */
    bound_set(pr->tmp[dim*2+3],pr->tmp[0]);
    bound_set(pr->tmp[0],pr->tmp[1]);
    bound_set(pr->tmp[1],pr->tmp[dim*2+3]);
    avo_hmat_assign(pr,u,m,nsc,dim,d,respect_closure);
    return false;
  }

  else {
    /* general, approximated case */

    /* TODO */

    /* for now, respects closure... */

    avo_hmat_forget_var(m,dim,d);
    return false;
  }
}

/* common simple case: X_d -> X_k */
static void avo_hmat_subst_var(bound_t* m, size_t dim, size_t d, size_t k)
{
  size_t i;
  /* infer binary constraints by substitution */
  for (i=0;i<2*d;i++) {
    bound_bmin(m[avo_matpos2(2*k+1,i)],m[avo_matpos(2*d+1,i)]);
    bound_bmin(m[avo_matpos2(2*k,i)],m[avo_matpos(2*d,i)]);
  }
  for (i=2*d+2;i<2*dim;i++) {
    bound_bmin(m[avo_matpos2(2*k+1,i)],m[avo_matpos(i^1,2*d)]);
    bound_bmin(m[avo_matpos2(2*k,i)],m[avo_matpos(i^1,2*d+1)]);
  }

  /* infer unary constraints by substitution */
  bound_bmin(m[avo_matpos(2*k+1,2*k)],m[avo_matpos(2*d+1,d*2)]);
  bound_bmin(m[avo_matpos(2*k,2*k+1)],m[avo_matpos(2*d,d*2+1)]);
  avo_hmat_forget_var(m,dim,d);
}

/* variable d is definitely positive or negative */
avo_t* avo_assign_linexpr_d_fixed_sign(avo_internal_t* pr,
			  bool destructive, avo_t* a,
			  ap_dim_t d, ap_linexpr0_t* expr,
			  avo_t* dest)
{
  bound_t* m;
  bound_t* nsc;
  bool respect_closure;
  avo_t* a1;
  ap_manager_t* man=pr->man;

  arg_assert(d<a->dim,return NULL;);
  uexpr u = avo_uexpr_of_linexpr(pr,pr->tmp,expr,a->dim);

  if (!destructive)	  a1 = avo_copy_internal(pr,a);
  else 	  a1 = a;

  if (dest && !dest->closed && !dest->m)
    /* definitively empty due to dest*/
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);

  if (u.type==EMPTY)
    /* definitively empty due to empty expression */
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);

  /* useful to close only for non-invertible assignments */
  if ((u.type!=UNARY || u.i!=d) && pr->funopt->algorithm>=0)
    avo_cache_closure(pr,a1);
  m = a1->closed ? a1->closed : a1->m;
  nsc = a1->nsc;
  if (!m) return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true); /* empty */

  /* can / should we try to respect the closure */
  respect_closure = (m==a1->closed) && (pr->funopt->algorithm>=0) && (!dest);

  /* go */
  avo_hmat_assign(pr,u,m,nsc,a1->dim,d,&respect_closure);

  /* exact on Q if zeroary or unary, closed arg and no conv error */
  if (u.type==BINARY || u.type==OTHER) flag_incomplete;
  else if (num_incomplete || a1->intdim) flag_incomplete;
  else if (!a1->closed) flag_algo;
  else if (pr->conv) flag_conv;

  /* intersect with dest */
  if (dest) {
    bound_t* m_d = dest->closed ? dest->closed : dest->m;
    bound_t* nsc_d = dest->nsc;
    for (size_t i=0;i<avo_matsize(a1->dim);i++)
    {
    	bound_bmin_nsc(&(m[i]), &(nsc[i]), m_d[i], nsc_d[i]);
    }
  }

 return avo_set_mat_nsc(pr,a1,m,NULL,nsc,true);
}

/* destructive: false (not changing a) */
/* dest: to be intersected with result */
avo_t* avo_assign_linexpr(ap_manager_t* man,
			  bool destructive, avo_t* a,
			  ap_dim_t d, ap_linexpr0_t* expr,
			  avo_t* dest)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY,2*(2*a->dim+1+5));
  bound_t *m = a->closed ? a->closed : a->m;
  bound_t *nsc = a->nsc;
  avo_t *res;

  if (bound_cmp_int(m[avo_matpos2(2*d+1,2*d)], 0)<=0 || bound_cmp_int(m[avo_matpos2(2*d,2*d+1)],0)<=0)
  {   /* variable d is definitely positive or negative */
	  res = avo_assign_linexpr_d_fixed_sign(pr,destructive,a,d,expr,dest);
	  return res;
  }
  else
  {
	  avo_t *a1,*a2,*a12;
	  bound_t* m1 = avo_hmat_alloc(pr,a->dim);
	  bound_t* m2 = avo_hmat_alloc(pr,a->dim);
	  bound_t* nsc1=avo_hmat_alloc(pr,a->dim);
	  bound_t* nsc2=avo_hmat_alloc(pr,a->dim);

	  bound_set_array(m1,m,avo_matsize(a->dim));
	  bound_set_array(m2,m,avo_matsize(a->dim));
	  bound_set_array(nsc1,nsc,avo_matsize(a->dim));
	  bound_set_array(nsc2,nsc,avo_matsize(a->dim));
	  bound_set_int(m1[avo_matpos2(2*d+1,2*d)],0);
	  bound_set_infty(nsc1[avo_matpos2(2*d+1,2*d)],1);
	  bound_set_int(m2[avo_matpos2(2*d,2*d+1)],0);
	  bound_set_infty(nsc2[avo_matpos2(2*d+1,2*d)],1);

	  a1 = avo_alloc_internal(pr,a->dim,a->intdim);
	  a1->closed = m1;
	  a1->nsc = nsc1;
	  a1->m = NULL;
	  a2 = avo_alloc_internal(pr,a->dim,a->intdim);
	  a2->closed = m2;
	  a2->nsc = nsc2;
	  a2->m = NULL;
	  avo_hmat_close_incremental(m1,nsc1,a->dim, d);
	  avo_hmat_close_incremental(m2,nsc2,a->dim, d);
	  a1 = avo_assign_linexpr_d_fixed_sign(pr,true,a1,d,expr,NULL);
	  a2 = avo_assign_linexpr_d_fixed_sign(pr,true,a2,d,expr,NULL);
	  a12 = avo_join(man,false,a1,a2);
	  /* intersect with dest */
	  if (dest) {
	    bound_t* m_d = dest->closed ? dest->closed : dest->m;
	    bound_t* nsc_d = dest->nsc;
	    for (size_t i=0;i<avo_matsize(a->dim);i++)
	    {
	    	bound_bmin_nsc(&(a12->m[i]), &(a12->nsc[i]), m_d[i], nsc_d[i]);
	    }
	  }
	  if (!destructive)	  res = a12;
	  else{
		  avo_set_mat_nsc(pr,a,a12->m,a12->closed,a12->nsc,true);
		  res = a;
		  free(a12);
	  }
	  avo_free_internal(pr,a1);	  avo_free_internal(pr,a2);
	  return res;
  }
}


avo_t* avo_substitute_linexpr(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_dim_t d, ap_linexpr0_t* expr,
			      avo_t* dest)
{
  	//fprintf(stdout, "AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY\n"); fflush(stdout);
  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,2*(2*a->dim+1+5));
  uexpr u = avo_uexpr_of_linexpr(pr,pr->tmp,expr,a->dim);
  bound_t *m, *m2, *nsc, *nsc2;
  avo_t* a1;
  bool respect_closure;
  arg_assert(d<a->dim,return NULL;);

  m2   = dest ? (dest->closed ? dest->closed : dest->m) : NULL;
  nsc2 = dest ? (dest->nsc) : NULL;

  if (!destructive)	  a1 = avo_copy_internal(pr,a);
  else 	  a1 = a;

  if (dest && !m2)
    /* definitively empty due to dest*/
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);

  if (u.type==EMPTY)
    /* definitively empty due to empty expression */
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);

  /* useful to close only for non-invertible substitution */
  if ((u.type!=UNARY || u.i!=d) && pr->funopt->algorithm>=0)
    avo_cache_closure(pr,a1);
  m = a1->closed ? a1->closed : a1->m;
  nsc = a1->nsc;
  if (!m) return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true); /* empty */
  /* can / should we try to respect the closure */
  respect_closure = (m==a1->closed) && (pr->funopt->algorithm>=0) && (!dest);
  /* go */
  if (avo_hmat_subst(pr,u,m,a1->nsc,a1->dim,d,(bound_t*)m2,&respect_closure)) {
    /* empty */
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
  }

  if (u.type==BINARY || u.type==OTHER) flag_incomplete;
  else if (num_incomplete || a1->intdim) flag_incomplete;
  else if (!a1->closed) flag_algo;
  else if (pr->conv) flag_conv;

  /* intersect with dest */
  if (m2) {
    size_t i;
    for (i=0;i<avo_matsize(a1->dim);i++){
      bound_min(m[i],m[i],m2[i]);
      bound_min(nsc[i],nsc[i],nsc2[i]);
    }
  }

  if (respect_closure) return avo_set_mat_nsc(pr,a1,NULL,m,nsc,true);
  else return avo_set_mat_nsc(pr,a1,m,NULL,nsc,true);
}

avo_t* avo_assign_linexpr_array(ap_manager_t* man,
				bool destructive, avo_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				avo_t* dest)
{
	//fprintf(stdout, "AP_FUNID_ASSIGN_LINEXPR_ARRAY\n"); fflush(stdout);
  if (size==1) 
    return avo_assign_linexpr(man,destructive,a,tdim[0],texpr[0],dest);

  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY,2*(2*a->dim+size+5));
  avo_t* a1;
  ap_dim_t* d = (ap_dim_t*) pr->tmp2;
  bound_t *m, *m1;
  bound_t *nsc,*nsc1;
  size_t i;
  ap_dim_t p = a->dim;
  int inexact = 0;
  bool respect_closure = false;

  if (!destructive)	  a1 = avo_copy_internal(pr,a);
  else 	  a1 = a;

  /* checks */
  arg_assert(size>0,return NULL;);
  for (i=0;i<a1->dim;i++) d[i] = 0;
  for (i=0;i<size;i++) {
    arg_assert(tdim[i]<a1->dim,return NULL;);
    arg_assert(!d[tdim[i]],return NULL;); /* tdim has duplicate */
    d[tdim[i]] = 1;
  }

  if (dest && !dest->closed && !dest->m)
    /* definitively empty due to dest*/
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a1);
  m = a1->closed ? a1->closed : a1->m;
  nsc = a1->nsc;
  if (!m) return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true); /* empty */

  /* add temporary dimensions to hold destination variables */
  m1 = avo_hmat_alloc_top(pr,a1->dim+size);
  nsc1 = avo_hmat_alloc_top(pr,a1->dim+size);
  bound_set_array(m1,m,avo_matsize(a1->dim));
  bound_set_array(nsc1,nsc,avo_matsize(a1->dim));
  /* coefs in expr for temporary dimensions are set to 0 */
  for (i=0;i<2*size;i++)
    bound_set_int(pr->tmp[2*a1->dim+i+2],0);
  /* perform assignments */
  for (i=0;i<size;i++) {
    uexpr u = avo_uexpr_of_linexpr(pr,pr->tmp,texpr[i],a1->dim);
    if (u.type==EMPTY) {
      avo_hmat_free(pr,m1,a1->dim+size);
      avo_hmat_free(pr,nsc1,a1->dim+size);
      return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
    }

    if (u.type==BINARY || u.type==OTHER) inexact = 1;

    avo_hmat_assign(pr,u,m1,nsc1,a1->dim+size,a1->dim+i,&respect_closure);
  }

  /* now close & remove temporary variables */
  if (pr->funopt->algorithm>=0) {
    if (avo_hmat_close(m1,nsc1,a1->dim+size)) {
      /* empty */
      avo_hmat_free(pr,m1,a1->dim+size);
      avo_hmat_free(pr,nsc1,a1->dim+size);
      return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
    }
  }
  else flag_algo;

  for (i=0;i<a1->dim;i++) d[i] = i;
  for (i=0;i<size;i++) {
    d[a1->dim+i] = tdim[i];
    d[tdim[i]] = a1->dim;
  }
  avo_hmat_permute(m,m1,a1->dim,a1->dim+size,d);
  avo_hmat_permute(nsc,nsc1,a1->dim,a1->dim+size,d);
  avo_hmat_free(pr,m1,a1->dim+size);
  avo_hmat_free(pr,nsc1,a1->dim+size);

  /* intersect with dest */
  if (dest) {
    bound_t* m2 = dest->closed ? dest->closed : dest->m;
    bound_t* nsc2 = dest->nsc;
    size_t i;
    for (i=0;i<avo_matsize(a1->dim);i++)
    {
      bound_min(m[i],m[i],m2[i]);
      if(bound_cmp(m[i],m2[i])==0&&bound_cmp_int(nsc[i],1)>0)
    	  bound_set(nsc[i],nsc2[i]);
    }
  }

  if (inexact || a1->intdim) flag_incomplete;
  else if (!a1->closed) flag_algo;
  else if (pr->conv) flag_conv;

  return avo_set_mat_nsc(pr,a1,m,NULL,nsc,true);
}

avo_t* avo_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, avo_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    avo_t* dest)
{
   //fprintf(stdout, "AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY\n"); fflush(stdout);

  if (size==1) 
    return avo_substitute_linexpr(man,destructive,a,tdim[0],texpr[0],dest);

  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
			  2*(2*a->dim+size+5));
  ap_dim_t* d = (ap_dim_t*) pr->tmp2;
  bound_t *m, *m1, *m2;
  avo_t* a1;
  size_t i,j;
  ap_dim_t p = a->dim;
  int inexact = 0;
  bool respect_closure = false; /* TODO */

  if (!destructive)	  a1 = avo_copy_internal(pr,a);
  else 	  a1 = a;

  /* checks */
  arg_assert(size>0,return NULL;);
  for (i=0;i<a1->dim;i++) d[i] = 0;
  for (i=0;i<size;i++) {
    arg_assert(tdim[i]<a->dim,return NULL;);
    arg_assert(!d[tdim[i]],return NULL;); /* tdim has duplicate */
    d[tdim[i]] = 1;
  }

  m2 = dest ? (dest->closed ? dest->closed : dest->m) : NULL;
  if (dest && !m2)
    /* definitively empty due to dest*/
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a1);
  m = a1->closed ? a1->closed : a1->m;
  if (!m) return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true); /* empty */

  /* add temporary dimensions to hold destination variables */
  m1 = avo_hmat_alloc_top(pr,a1->dim+size);
  bound_set_array(m1,m,avo_matsize(a1->dim));

  /* susbstitute org with temp variables */
  for (i=0;i<size;i++) {
    size_t dst = 2*(a1->dim+i), src = 2*tdim[i];
    for (j=0;j<src;j++) {
      bound_bmin(m1[avo_matpos(dst+1,j)],m1[avo_matpos(src+1,j)]);
      bound_bmin(m1[avo_matpos(dst,j)],m1[avo_matpos(src,j)]);
    }
    for (j=src+2;j<2*a1->dim+2*size;j++) {
      bound_bmin(m1[avo_matpos2(dst+1,j)],m1[avo_matpos(j^1,src)]);
      bound_bmin(m1[avo_matpos2(dst,j)],m1[avo_matpos(j^1,src+1)]);
    }
    bound_bmin(m1[avo_matpos(dst+1,dst)],m1[avo_matpos(src+1,src)]);
    bound_bmin(m1[avo_matpos(dst,dst+1)],m1[avo_matpos(src,src+1)]);
    avo_hmat_forget_var(m1,a1->dim+size,tdim[i]);
  }

  /* coefs in expr for temporary dimensions are set to 0 */
  for (i=0;i<2*size;i++)
    bound_set_int(pr->tmp[2*a1->dim+i+2],0);

  /* perform substitutions */
  for (i=0;i<size;i++) {
    uexpr u = avo_uexpr_of_linexpr(pr,pr->tmp,texpr[i],a1->dim);

    if (u.type==EMPTY) {
      avo_hmat_free(pr,m1,a1->dim+size);
      return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
    }

    if (u.type==BINARY || u.type==OTHER) inexact = 1;

    if (avo_hmat_subst(pr,u,m1,a1->nsc,a1->dim+size,a1->dim+i,m2,
		   &respect_closure)) {
      /* empty */
      avo_hmat_free(pr,m1,a1->dim+size);
      return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
    }
  }

  /* now close */
  if (pr->funopt->algorithm>=0) {
    if (avo_hmat_close(m1,a1->nsc,a1->dim+size)) {
      /* empty */
      avo_hmat_free(pr,m1,a1->dim+size);
      return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,true);
    }
  }
  else flag_algo;

  /* remove temp */
  bound_set_array(m,m1,avo_matsize(a1->dim));
  avo_hmat_free(pr,m1,a1->dim+size);

  /* intersect with dest */
  if (m2) {
    size_t i;
    for (i=0;i<avo_matsize(a1->dim);i++)
      bound_min(m[i],m[i],m2[i]);
  }

  if (inexact || a1->intdim) flag_incomplete;
  else if (!a1->closed) flag_algo;
  else if (pr->conv) flag_conv;

  return avo_set_mat_nsc(pr,a1,m,NULL,NULL,true);
}

avo_t* avo_assign_texpr_array(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      avo_t* dest)
{
  return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
avo_t* avo_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, avo_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  avo_t* dest)
{
  return ap_generic_substitute_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
