void wrapper_fprint(FILE* stream,
			 ap_manager_t* man, wrapper_t* a, char** name_of_dim)
{
  /*ap_tcons0_array_t cons;
  cons = wrapper_to_tcons_array(man, a);
  ap_tcons0_array_fprint(stream, &cons, name_of_dim);*/
  ap_lincons0_array_t cons;
  ap_lincons0_array_init(cons,AP_SCALAR_MPQ,0);
  wrapper_to_lincons_array(man,cons,a);
  ap_lincons0_array_fprint(stream,cons,name_of_dim);
  ap_lincons0_array_clear(cons);
}

void wrapper_fprintdiff(FILE* stream,
			     ap_manager_t* man,
			     wrapper_t* a1, wrapper_t* a2,
			     char** name_of_dim)
{
  assert(false);
}

void wrapper_fdump(FILE* stream, ap_manager_t* man, wrapper_t* a)
{
  static value *closure_fdump = NULL;
  if (closure_fdump == NULL) {
    closure_fdump = caml_named_value("fdump");
  }
  caml_callback2(*closure_fdump, ((wrapper_t*) man->internal)->val, a->val);
}
 
ap_membuf_t wrapper_serialize_raw(ap_manager_t* man, wrapper_t* a) 
{
  ap_membuf_t res = { NULL, 0 };
  assert(false);
  return(res);
}

wrapper_t* wrapper_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size) 
{
  assert(false);
  return(NULL);
}

static wrapper_t* wrapper_internal_alloc(void) 
{
  CAMLparam0();
  CAMLlocal1(v_man);
  wrapper_t* res;
  static value *closure_internal_alloc = NULL;
  if (closure_internal_alloc == NULL) {
    closure_internal_alloc = caml_named_value("manager_alloc");
  }
  res = wrapper_alloc(caml_callback(*closure_internal_alloc, Val_unit));
  CAMLreturnT(wrapper_t*, res);
}

static void wrapper_internal_free(wrapper_t* x) 
{
  if (x != NULL) {
    caml_remove_generational_global_root(&(x->val));
    free(x);
  }
}

static void callback_registering(void) 
{
  char* arg = '\0';
  caml_startup(&arg);
}

ap_manager_t* wrapper_manager_alloc(void)
{
  ap_manager_t* man;
  void** funptr;
  wrapper_t* internal;

  callback_registering();

  internal = wrapper_internal_alloc();
  man = ap_manager_alloc("ml2c wrapper manager", 
			 "1.0", 
			 internal, 
			 (void (*)(void*))wrapper_internal_free);

  funptr = man->funptr;

  funptr[AP_FUNID_COPY] = &wrapper_copy;
  funptr[AP_FUNID_FREE] = &wrapper_free;
  funptr[AP_FUNID_ASIZE] = &wrapper_size;
  funptr[AP_FUNID_MINIMIZE] = &wrapper_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &wrapper_canonicalize;
  funptr[AP_FUNID_HASH] = &wrapper_hash;
  funptr[AP_FUNID_APPROXIMATE] = &wrapper_approximate;
  funptr[AP_FUNID_FPRINT] = &wrapper_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &wrapper_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &wrapper_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &wrapper_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &wrapper_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &wrapper_bottom;
  funptr[AP_FUNID_TOP] = &wrapper_top;
  funptr[AP_FUNID_OF_BOX] = &wrapper_of_box;
  funptr[AP_FUNID_DIMENSION] = &wrapper_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &wrapper_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &wrapper_is_top;
  funptr[AP_FUNID_IS_LEQ] = &wrapper_is_leq;
  funptr[AP_FUNID_IS_EQ] = &wrapper_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &wrapper_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &wrapper_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &wrapper_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &wrapper_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &wrapper_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &wrapper_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &wrapper_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &wrapper_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &wrapper_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &wrapper_to_tcons_array;
  funptr[AP_FUNID_TO_LINGEN_ARRAY] = &wrapper_to_lingen_array;
  funptr[AP_FUNID_MEET] = &wrapper_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &wrapper_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &wrapper_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &wrapper_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &wrapper_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &wrapper_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &wrapper_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &wrapper_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &wrapper_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &wrapper_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &wrapper_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &wrapper_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &wrapper_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &wrapper_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &wrapper_forget_array;
  funptr[AP_FUNID_EXPAND] = &wrapper_expand;
  funptr[AP_FUNID_FOLD] = &wrapper_fold;
  funptr[AP_FUNID_WIDENING] = &wrapper_widening;
  funptr[AP_FUNID_CLOSURE] = &wrapper_closure;

  ap_manager_set_abort_if_exception(man, AP_EXC_TIMEOUT, false);
  ap_manager_set_abort_if_exception(man, AP_EXC_OUT_OF_SPACE, false);
  ap_manager_set_abort_if_exception(man, AP_EXC_OVERFLOW, false);
  
  return(man);
}

