type kind =
  | Ap_manager_ptr
  | Wrapper_ptr
  | Wrapper_array_t of string (* the integer denotes the index of the
			      argument containing the size of the array *)
  | Optional_wrapper_ptr
  | Ap_dim_t
  | Ap_dim_array_t of string
  | Ap_dimension_t
  | Ap_dimchange_t
  | Ap_dimperm_t
  | Ap_coeff_t
  | Ap_lingen0_t
  | Ap_lingen0_array_t
  | Ap_linexpr0_t
  | Ap_linexpr0_array_t
  | Ap_lincons0_t
  | Ap_lincons0_array_t
  | Ap_texpr0_ptr
  | Ap_texpr0_array_ptr
  | Ap_tcons0_t
  | Ap_tcons0_ptr
  | Ap_tcons0_array_t
  | Ap_tcons0_array_ptr
  | Bool
  | Int
  | Size_t
  | Array_size

type argument_spec = kind * string

type return_spec =
  | Void
  | Value of kind

type funspec =
    { ret : return_spec;
      name : string;
      args : argument_spec list;
      destructive_variant : int option;
    }

let c_type_of_kind k =
  match k with
    | Ap_manager_ptr -> "ap_manager_t*"
    | Wrapper_ptr -> "wrapper_t*"
    | Wrapper_array_t _ -> "wrapper_t**"
    | Optional_wrapper_ptr -> "wrapper_t*"
    | Ap_dim_t -> "ap_dim_t"
    | Ap_dim_array_t _ -> "ap_dim_t*"
    | Ap_dimension_t -> "ap_dimension_t"
    | Ap_dimchange_t -> "ap_dimchange_t*"
    | Ap_dimperm_t -> "ap_dimperm_t*"
    | Ap_coeff_t -> "ap_coeff_t"
    | Ap_lingen0_t -> "ap_lingen0_t"
    | Ap_lingen0_array_t -> "ap_lingen0_array_t"
    | Ap_linexpr0_t -> "ap_linexpr0_t"
    | Ap_linexpr0_array_t -> "ap_linexpr0_array_t"
    | Ap_lincons0_t -> "ap_lincons0_t"
    | Ap_lincons0_array_t -> "ap_lincons0_array_t"
    | Ap_texpr0_ptr -> "ap_texpr0_t*"
    | Ap_texpr0_array_ptr -> "ap_texpr0_array_t*"
    | Ap_tcons0_t -> "ap_tcons0_t"
    | Ap_tcons0_ptr -> "ap_tcons0_t*"
    | Ap_tcons0_array_t -> "ap_tcons0_array_t"
    | Ap_tcons0_array_ptr -> "ap_tcons0_array_t*"
    | Bool -> "bool"
    | Int -> "int"
    | Size_t | Array_size -> "size_t"

let c_type_of_return_spec r =
  match r with
    | Void -> "void"
    | Value k -> c_type_of_kind k

let c2ml_function_of_kind k =
  match k with
    | Ap_manager_ptr -> "camlidl_apron_manager_ptr_c2ml"
    | Wrapper_ptr -> assert false
    | Wrapper_array_t _ -> assert false
    | Optional_wrapper_ptr -> assert false
    | Ap_dim_t -> "camlidl_c2ml_dim_ap_dim_t"
    | Ap_dim_array_t _ -> assert false
    | Ap_dimension_t -> "camlidl_c2ml_dim_struct_ap_dimension_t"
    | Ap_dimchange_t -> "camlidl_apron_dimchange_c2ml"
    | Ap_dimperm_t -> "camlidl_apron_dimperm_c2ml"
    | Ap_coeff_t -> "camlidl_coeff_ptr_c2ml_nonfinal"
    | Ap_lingen0_t -> "camlidl_lingen0_ptr_c2ml_nonfinal"
    | Ap_lingen0_array_t -> "camlidl_lingen0_array_ptr_c2ml_nonfinal"
    | Ap_linexpr0_t -> "camlidl_linexpr0_ptr_c2ml_nonfinal"
    | Ap_linexpr0_array_t -> "camlidl_linexpr0_array_ptr_c2ml_nonfinal"
    | Ap_lincons0_t -> "camlidl_lincons0_ptr_c2ml_nonfinal"
    | Ap_lincons0_array_t -> "camlidl_lincons0_array_ptr_c2ml_nonfinal"
    | Ap_texpr0_ptr -> "camlidl_apron_texpr0_ptr_c2ml_nonfinal"
    | Ap_texpr0_array_ptr -> "camlidl_apron_texpr0_array_t_c2ml_nonfinal"
    | Ap_tcons0_t | Ap_tcons0_ptr -> "camlidl_apron_tcons0_t_c2ml_nonfinal"
    | Ap_tcons0_array_t | Ap_tcons0_array_ptr -> "camlidl_apron_tcons0_array_t_c2ml_nonfinal"
    | Bool -> "Val_bool"
    | Int | Size_t | Array_size -> "Val_int"

let ml2c_function_of_kind k =
  match k with
    | Ap_manager_ptr -> "camlidl_apron_manager_ptr_ml2c"
    | Wrapper_ptr -> "wrapper_alloc"
    | Wrapper_array_t _ -> assert false
    | Optional_wrapper_ptr -> assert false
    | Ap_dim_t -> "camlidl_ml2c_dim_ap_dim_t"
    | Ap_dimension_t -> "camlidl_ml2c_dim_struct_ap_dimension_t"
    | Ap_tcons0_array_t | Ap_tcons0_array_ptr -> "camlidl_apron_tcons0_array_t_ml2c"
    | Bool -> "Bool_val"
    | Int -> "Int_val"
    | Size_t -> "(size_t) Int_val"
    | _ -> failwith ""
(*
  | Ap_dim_array_t -> assert false
  | Ap_dimchange_t -> "camlidl_apron_dimchange_ml2c"
    | Ap_dimperm_t -> "camlidl_apron_dimperm_ml2c"
    | Ap_coeff_t -> "camlidl_coeff_ptr_ml2c"
    | Ap_lingen0_t -> "camlidl_lingen0_ptr_ml2c"
    | Ap_lingen0_array_t -> "camlidl_lingen0_array_ptr_ml2c"
    | Ap_linexpr0_t -> "camlidl_linexpr0_ptr_ml2c"
    | Ap_linexpr0_array_t -> "camlidl_linexpr0_array_ptr_ml2c"
    | Ap_lincons0_t -> "camlidl_lincons0_ptr_ml2c"
    | Ap_lincons0_array_t -> "camlidl_lincons0_array_ptr_ml2c"
    | Ap_texpr0_ptr -> "camlidl_apron_texpr0_ptr_ml2c"
    | Ap_texpr0_array_ptr -> "camlidl_apron_texpr0_array_t_ml2c"
    | Ap_tcons0_t | Ap_tcons0_ptr -> "camlidl_apron_tcons0_t_ml2c"
*)
(*
let nonfinalize_of_kind k =
  match k with
    | Ap_coeff_t -> "ap_coeff_ptr_nonfinalize"
    | Ap_linexpr0_t -> "ap_linexpr0_ptr_nonfinalize"
    | Ap_linexpr0_array_t -> "ap_linexpr0_array_ptr_nonfinalize"
    | Ap_lincons0_t -> "ap_lincons0_ptr_nonfinalize"
    | Ap_lincons0_array_t -> "ap_lincons0_array_ptr_nonfinalize"
    | Ap_lingen0_t -> "ap_lingen0_ptr_nonfinalize"
    | Ap_lingen0_array_t -> "ap_lingen0_array_ptr_nonfinalize"
    | Ap_texpr0_ptr -> "ap_texpr0_ptr_nonfinalize"
    | Ap_texpr0_array_ptr -> "ap_texpr0_array_t_nonfinalize"
    | Ap_tcons0_t | Ap_tcons0_ptr -> "ap_tcons0_t_nonfinalize"
    | Ap_tcons0_array_t | Ap_tcons0_array_ptr -> "ap_tcons0_array_t_nonfinalize"
    | _ -> assert false
*)

let print_signature fmt f =
  let build_argument n (k, name) =
    if n > 0 then
      Format.fprintf fmt ", @?";
    if f.destructive_variant = Some n then
      Format.fprintf fmt "bool destructive, @?";
    Format.fprintf fmt "%s %s@?" (c_type_of_kind k) name;
    n+1
  in
  let return_type = c_type_of_return_spec f.ret in
  Format.fprintf fmt "%s wrapper_%s(@?" return_type f.name;
  ignore (List.fold_left build_argument 0 f.args);
  Format.fprintf fmt ")@?"

let print_local_declaration fmt f =
  let build_caml_local is_first (k, name) =
    match k with
      | Wrapper_ptr | Ap_manager_ptr -> is_first
      | _ ->
	if not is_first then
	  Format.fprintf fmt ", @?";
	Format.fprintf fmt "v_%s@?" name;
	false
  in
  let n = List.fold_left
    (fun res (k, _) ->
      match k with
	| Wrapper_ptr | Ap_manager_ptr | Array_size -> res
	| _ -> res+1) 0 f.args in
  let p = match f.ret with
    | Value _ -> n+1
    | _ -> n
  in
  Format.fprintf fmt "  CAMLparam0();@.";
  if p > 0 then begin
    Format.fprintf fmt "  CAMLlocal%d(@?" p;
    let fargs = List.filter (fun (k,name) -> k<>Array_size) f.args in
    let b = List.fold_left build_caml_local true fargs in
    match f.ret with
      | Void -> Format.fprintf fmt ");@."
      | Value _ ->
	if not b then
	  Format.fprintf fmt ", @?";
	Format.fprintf fmt "v_res);@."
  end;

  let q = List.length f.args in
  if q > 3 then
    Format.fprintf fmt "  value arg_tab[%d];@." q;
  begin
    match f.ret with
      | Void -> ()
      | Value _ ->
	let return_type = c_type_of_return_spec f.ret in
	Format.fprintf fmt "  %s res;@." return_type
  end;
  if List.exists
    (begin fun (k, _) -> match k with
    | Wrapper_array_t _ | Ap_dim_array_t _ -> true
    | _ -> false
    end)
    f.args
  then
    Format.fprintf fmt "  size_t i;@.";
  if List.exists (fun (k, _) -> match k with Ap_dim_t | Ap_dim_array_t _ | Ap_dimension_t -> true | _ -> false) f.args ||
    (match f.ret with Value Ap_dim_t | Value Ap_dimension_t -> true | _ -> false) then begin
    Format.fprintf fmt "  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };@.";
    Format.fprintf fmt "  camlidl_ctx _ctx = &_ctxs;@."
  end

let print_closure_definition fmt f =
  Format.fprintf fmt "  static value *closure_%s = NULL;@." f.name;
  Format.fprintf fmt "  if (closure_%s == NULL) {@." f.name;
  Format.fprintf fmt "    closure_%s = caml_named_value(\"%s\");@." f.name f.name;
  Format.fprintf fmt "  }@.";
  if f.destructive_variant <> None then begin
    Format.fprintf fmt "  static value *closure_%s_with = NULL;@." f.name;
    Format.fprintf fmt "  if (closure_%s_with == NULL) {@." f.name;
    Format.fprintf fmt "    closure_%s_with = caml_named_value(\"%s_with\");@." f.name f.name;
    Format.fprintf fmt "  }@.";
  end

let print_argument_conversion fmt f =
  let caml_local_conversion (k, name) =
    match k with
      | Wrapper_ptr | Ap_manager_ptr -> ()
      | Wrapper_array_t size ->
	Format.fprintf fmt "  v_%s = caml_alloc(%s, 0);@." name size;
	Format.fprintf fmt "  for (i = 0; i < %s; i++)@." size;
	Format.fprintf fmt "    Store_field(v_%s, i, %s[i]->val);@." name name;
      | Ap_dim_array_t size ->
	Format.fprintf fmt "  v_%s = caml_alloc(%s, 0);@." name size;
	Format.fprintf fmt "  for (i = 0; i < %s; i++){@." size;
	Format.fprintf fmt "    value vdim = %s(&%s[i], _ctx);@." (c2ml_function_of_kind Ap_dim_t) name;
	Format.fprintf fmt "    Store_field(v_%s, i, vdim);@." name;
	Format.fprintf fmt "  }@.";
      | Optional_wrapper_ptr ->
	Format.fprintf fmt "  if (%s == NULL)@." name;
	Format.fprintf fmt "    v_%s = Val_int(0);@." name;
	Format.fprintf fmt "  else {@.";
	Format.fprintf fmt "    v_%s = caml_alloc(1, 0);@." name;
	Format.fprintf fmt "    Store_field(v_%s, 0, %s->val);@." name name;
	Format.fprintf fmt "  }@."
      | Ap_dim_t | Ap_dimension_t ->
	Format.fprintf fmt "  v_%s = %s(&%s, _ctx);@." name (c2ml_function_of_kind k) name
      | Ap_dimchange_t | Ap_dimperm_t
      | Ap_coeff_t
      | Ap_linexpr0_t | Ap_linexpr0_array_t
      | Ap_lincons0_t | Ap_lincons0_array_t
      | Ap_lingen0_t | Ap_lingen0_array_t
      | Ap_texpr0_ptr -> begin
	  Format.fprintf fmt "  v_%s = %s(%s);@." name (c2ml_function_of_kind k) name;
	end
      | Ap_tcons0_t | Ap_tcons0_array_t -> begin
	  Format.fprintf fmt "  v_%s = %s(&%s);@." name (c2ml_function_of_kind k) name;
(*
  Format.fprintf fmt "  %s(v_%s);@." (nonfinalize_of_kind k) name
*)
      end
      | Ap_texpr0_array_ptr | Ap_tcons0_ptr | Ap_tcons0_array_ptr -> begin
	Format.fprintf fmt "  v_%s = %s(%s);@." name (c2ml_function_of_kind k) name;
(*
	Format.fprintf fmt "  %s(v_%s);@." (nonfinalize_of_kind k) name
*)
      end
      | Bool | Int | Size_t ->
	Format.fprintf fmt "  v_%s = %s(%s);@." name (c2ml_function_of_kind k) name
      | Array_size -> ()
      | _ ->
	Format.fprintf fmt "  v_%s = %s(&%s);@." name (c2ml_function_of_kind k) name
  in
  List.iter caml_local_conversion f.args

let print_result_conversion fmt f =
  match f.ret with
    | Void -> ()
    | Value k ->
      match k with
	| Wrapper_ptr | Bool | Int | Size_t ->
	  Format.fprintf fmt "  res = %s(v_res);@." (ml2c_function_of_kind k)
	| Ap_dim_t | Ap_dimension_t ->
	  Format.fprintf fmt "  %s(v_res, &res, _ctx);@." (ml2c_function_of_kind k)
	| Ap_dim_array_t _ -> assert false;
	| Ap_coeff_t | Ap_linexpr0_t | Ap_linexpr0_array_t | Ap_lincons0_t
	| Ap_lincons0_array_t | Ap_lingen0_t | Ap_lingen0_array_t | Ap_texpr0_ptr
	| Ap_tcons0_t -> failwith ""
	| Ap_tcons0_array_t -> begin
(*
	  Format.fprintf fmt "  %s(v_res);@." (nonfinalize_of_kind k);
*)
	  Format.fprintf fmt "  %s(v_res, &res);@." (ml2c_function_of_kind k)
	end
	| Ap_texpr0_array_ptr | Ap_tcons0_ptr ->
	    failwith ""
	| Ap_tcons0_array_ptr -> begin
(*
	  Format.fprintf fmt "  %s(v_res);@." (nonfinalize_of_kind k);
*)
	  Format.fprintf fmt "  %s(v_res, res);@."(ml2c_function_of_kind k)
	end
	| _ ->
	  Format.fprintf fmt "  %s(v_res, &res);@." (ml2c_function_of_kind k)

let print_callback fmt f =
  let print_assignement_return_value fmt () =
    match f.ret with
      | Void -> ()
      | Value _ -> Format.fprintf fmt "v_res = @?"
  in
  let build_callback_argument fmt (k, name) =
    match k with
      | Wrapper_ptr -> Format.fprintf fmt "%s->val@?" name
      | Ap_manager_ptr -> Format.fprintf fmt "((wrapper_t*) %s->internal)->val@?" name
      | _ -> Format.fprintf fmt "v_%s@?" name
  in
  let print_callback_preamble fmt () =
    let fargs = List.filter (fun (k,name) -> k<>Array_size) f.args in
    let n = List.length f.args in
    if n > 3 then
      ignore (List.fold_left (fun i arg ->
	Format.fprintf fmt "  arg_tab[%d] = %a;@." i build_callback_argument arg;
	i+1) 0 fargs)
  in
  let print_callback_function fmt closure_name =
    let fargs = List.filter (fun (k,name) -> k<>Array_size) f.args in
    let n = List.length fargs in
    if n <= 3 then begin
      if n <> 1 then
	Format.fprintf fmt "caml_callback%d(*closure_%s@?" n closure_name
      else
	Format.fprintf fmt "caml_callback(*closure_%s@?" closure_name;
      List.iter (fun arg -> Format.fprintf fmt ", %a@?" build_callback_argument arg) fargs
    end
    else
      Format.fprintf fmt "caml_callbackN(*closure_%s, %d, arg_tab@?" closure_name n;
    Format.fprintf fmt ")@?"
  in
  if true then Format.fprintf fmt "  caml_gc_full_major(Val_unit);@.";
  begin match f.destructive_variant with
    | None ->
      Format.fprintf fmt "%a@?" print_callback_preamble ();
      Format.fprintf fmt "  %a%a;@." print_assignement_return_value () print_callback_function f.name;
      Format.fprintf fmt "%a@?" print_result_conversion f
    | Some i ->
      Format.fprintf fmt "%a@?" print_callback_preamble ();
      Format.fprintf fmt "  if (destructive) {@.";
      Format.fprintf fmt "    if (closure_%s_with == NULL)@." f.name;
      let (_, arg_name) = List.nth f.args i in
      Format.fprintf fmt "      wrapper_modify(%s, %a);@." arg_name print_callback_function f.name;
      Format.fprintf fmt "    else@.";
      Format.fprintf fmt "      %a;@." print_callback_function (f.name^"_with");
      Format.fprintf fmt "    res = %s;@." arg_name;
      Format.fprintf fmt "  }@.";
      Format.fprintf fmt "  else {@.";
      Format.fprintf fmt "    %a%a;@." print_assignement_return_value () print_callback_function f.name;
      Format.fprintf fmt "  %a@?" print_result_conversion f;
      Format.fprintf fmt "  }@."
  end;
  if true then Format.fprintf fmt "  caml_gc_full_major(Val_unit);@.";
  ()

let print_return fmt f =
  if List.exists (fun (k, _) -> match k with Ap_dim_t | Ap_dim_array_t _ | Ap_dimension_t -> true | _ -> false) f.args ||
    (match f.ret with Value Ap_dim_t | Value Ap_dimension_t -> true | _ -> false) then
    Format.fprintf fmt "  camlidl_free(_ctx);@.";
  match f.ret with
    | Void -> Format.fprintf fmt "  CAMLreturn0;@."
    | Value _ ->
      let return_type = c_type_of_return_spec f.ret in
      Format.fprintf fmt "  CAMLreturnT(%s, res);@." return_type

let print_c fmt f =
  Format.fprintf fmt "%a@." print_signature f;
  Format.fprintf fmt "{@.";
  Format.fprintf fmt "%a@?" print_local_declaration f;
  Format.fprintf fmt "%a@?" print_closure_definition f;
  Format.fprintf fmt "%a@?" print_argument_conversion f;
  Format.fprintf fmt "%a@?" print_callback f;
  Format.fprintf fmt "%a@?" print_return f;
  Format.fprintf fmt "}@."

let print_h fmt = Format.fprintf fmt "%a;@." print_signature

let fun_list =
  [
    { ret = Value Wrapper_ptr; name = "copy"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Size_t; name = "size"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Void; name = "minimize"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Void; name = "canonicalize"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Int; name = "hash"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Void; name = "approximate"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Int, "algorithm")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "bottom"; args = [(Ap_manager_ptr, "man"); (Ap_dimension_t, "dim")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "top"; args = [(Ap_manager_ptr, "man"); (Ap_dimension_t, "dim")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "of_box"; args = [(Ap_manager_ptr, "man"); (Ap_dimension_t, "dim"); (Ap_linexpr0_t, "box")]; destructive_variant = None };
    { ret = Value Ap_dimension_t; name = "dimension"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Bool; name = "is_bottom"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Bool; name = "is_top"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Bool; name = "is_leq"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a1"); (Wrapper_ptr, "a2")]; destructive_variant = None };
    { ret = Value Bool; name = "is_eq"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a1"); (Wrapper_ptr, "a2")]; destructive_variant = None };
    { ret = Value Bool; name = "sat_interval"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dim_t, "dim"); (Ap_coeff_t, "interval")]; destructive_variant = None };
    { ret = Value Bool; name = "sat_lincons"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_lincons0_t, "lincons")]; destructive_variant = None };
    { ret = Value Bool; name = "sat_tcons"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_tcons0_ptr, "tcons")]; destructive_variant = None };
    { ret = Value Bool; name = "is_dimension_unconstrained"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dim_t, "dim")]; destructive_variant = None };
    { ret = Void; name = "bound_dimension"; args = [(Ap_manager_ptr, "man"); (Ap_coeff_t, "interval"); (Wrapper_ptr, "a"); (Ap_dim_t, "dim")]; destructive_variant = None };
    { ret = Void; name = "bound_linexpr"; args = [(Ap_manager_ptr, "man"); (Ap_coeff_t, "interval"); (Wrapper_ptr, "a"); (Ap_linexpr0_t, "expr")]; destructive_variant = None };
    { ret = Void; name = "bound_texpr"; args = [(Ap_manager_ptr, "man"); (Ap_coeff_t, "interval"); (Wrapper_ptr, "a"); (Ap_texpr0_ptr, "expr")]; destructive_variant = None };
    { ret = Void; name = "to_lincons_array"; args = [(Ap_manager_ptr, "man"); (Ap_lincons0_array_t, "array"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Ap_tcons0_array_t; name = "to_tcons_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Void; name = "to_box"; args = [(Ap_manager_ptr, "man"); (Ap_linexpr0_t, "box"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Void; name = "to_lingen_array"; args = [(Ap_manager_ptr, "man"); (Ap_lingen0_array_t, "array"); (Wrapper_ptr, "a")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "meet"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a1"); (Wrapper_ptr, "a2")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "join"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a1"); (Wrapper_ptr, "a2")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "meet_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_array_t "size", "tab"); (Array_size, "size")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "join_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_array_t "size", "tab"); (Array_size, "size")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "meet_lincons_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_lincons0_array_t, "array")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "meet_tcons_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_tcons0_array_ptr, "array")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "add_ray_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_lingen0_array_t, "array")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "assign_linexpr_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "org"); (Ap_dim_array_t "ap_linexpr0_array_size(array)", "tdim"); (Ap_linexpr0_array_t, "array"); (Optional_wrapper_ptr, "dest")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "assign_texpr_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "org"); (Ap_dim_array_t "array->size", "tdim"); (Ap_texpr0_array_ptr, "array"); (Optional_wrapper_ptr, "dest")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "substitute_linexpr_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "org"); (Ap_dim_array_t "ap_linexpr0_array_size(array)", "tdim"); (Ap_linexpr0_array_t, "array"); (Optional_wrapper_ptr, "dest")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "substitute_texpr_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "org"); (Ap_dim_array_t "array->size", "tdim"); (Ap_texpr0_array_ptr, "array"); (Optional_wrapper_ptr, "dest")]; destructive_variant = Some 1};
    { ret = Value Wrapper_ptr; name = "forget_array"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dim_array_t "size", "tdim"); (Array_size, "size"); (Bool, "project")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "add_dimensions"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dimchange_t, "dimchange"); (Bool, "project")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "remove_dimensions"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dimchange_t, "dimchange")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "permute_dimensions"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dimperm_t, "perm")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "expand"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dim_t, "dim"); (Size_t, "n")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "fold"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a"); (Ap_dim_array_t "size", "tdim"); (Array_size, "size")]; destructive_variant = Some 1 };
    { ret = Value Wrapper_ptr; name = "widening"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a1"); (Wrapper_ptr, "a2")]; destructive_variant = None };
    { ret = Value Wrapper_ptr; name = "closure"; args = [(Ap_manager_ptr, "man"); (Wrapper_ptr, "a")]; destructive_variant = Some 1 };
  ]

let preamble_h = ref None
let postamble_h = ref None
let preamble_c = ref None
let postamble_c = ref None
let output_h = ref None
let output_c = ref None

let copy fmt file_name =
  try
    let c_in = open_in file_name in
    try
      while true do
	let s = input_line c_in in
	Format.fprintf fmt "%s@." s
      done;
      assert false
    with End_of_file ->
      close_in c_in
  with Sys_error _ -> invalid_arg ("Cannot read file "^file_name^".")

let generate () =
  let generate_aux print preamble postamble output =
    match output with
      | None -> ()
      | Some output ->
	begin
	  try
	    let c_out = open_out output in
	    let fmt_out = Format.formatter_of_out_channel c_out in
	    begin
	      match preamble with
		| None -> ()
		| Some preamble -> Format.fprintf fmt_out "%a@." copy preamble
	    end;
	    List.iter (fun f -> Format.fprintf fmt_out "%a@." print f) fun_list;
	    begin
	      match postamble with
		| None -> ()
		| Some postamble -> Format.fprintf fmt_out "%a@." copy postamble
	    end;
	    close_out c_out
	  with Sys_error _ -> invalid_arg ("Cannot open file "^output^" for writing.")
	end;
  in
  generate_aux print_c !preamble_c !postamble_c !output_c;
  generate_aux print_h !preamble_h !postamble_h !output_h

let _ =
  Arg.parse
    [
      ("-preamble-h", Arg.String (fun s -> preamble_h := Some s), "Set .h preamble file");
      ("-preamble-c", Arg.String (fun s -> preamble_c := Some s), "Set .c preamble file");
      ("-postamble-h", Arg.String (fun s -> postamble_h := Some s), "Set .h postamble file");
      ("-postamble-c", Arg.String (fun s -> postamble_c := Some s), "Set .c postamble file");
      ("-output-h", Arg.String (fun s -> output_h := Some s), "Set .h output file");
      ("-output-c", Arg.String (fun s -> output_c := Some s), "Set .c output file")
    ]
    (fun _ -> ()) "Automatic generator of wrappring functions from OCaml to C";
  generate ()
