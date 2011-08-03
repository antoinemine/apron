type kind =
  | Ap_manager_t
  | Wrapper_t 
  | Wrapper_array_t of int (* the integer denotes the index of the
			      argument containing the size of the array *)
  | Optional_wrapper_t
  | Ap_dim_t
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
  | Ap_texpr0_t
  | Ap_texpr0_array_t
  | Ap_tcons0_t
  | Ap_tcons0_array_t
  | Bool
  | Int
  | Size_t

type argument_spec = kind * string

type return_spec = 
  | Void
  | Value of kind
  | Side_effect of int

type funspec = 
    { ret : return_spec;
      name : string;
      args : argument_spec list;
      destructive_variant : int option;
    }

let c_type_of_kind k =
  match k with
    | Ap_manager_t -> "ap_manager_t*"
    | Wrapper_t -> "wrapper_t*"
    | Wrapper_array_t _ -> "wrapper_t**" 
    | Optional_wrapper_t -> "wrapper_t*"
    | Ap_dim_t -> "ap_dim_t"
    | Ap_dimension_t -> "ap_dimension_t"
    | Ap_dimchange_t -> "ap_dimchange_t"
    | Ap_dimperm_t -> "ap_dimperm_t"
    | Ap_coeff_t -> "ap_coeff_t"
    | Ap_lingen0_t -> "ap_lingen0_t"
    | Ap_lingen0_array_t -> "ap_lingen0_array_t"
    | Ap_linexpr0_t -> "ap_linexpr0_t"
    | Ap_linexpr0_array_t -> "ap_linexpr0_array_t"
    | Ap_lincons0_t -> "ap_lincons0_t"
    | Ap_lincons0_array_t -> "ap_lincons0_array_t"
    | Ap_texpr0_t -> "ap_texpr0_t*"
    | Ap_texpr0_array_t -> "ap_texpr0_array_t*"
    | Ap_tcons0_t -> "ap_tcons0_t*"
    | Ap_tcons0_array_t -> "ap_tcons0_array_t*"
    | Bool -> "bool"
    | Int -> "int"
    | Size_t -> "size_t"

let c_type_of_return_spec r =
  match r with
    | Void | Side_effect _ -> "void"
    | Value k -> c_type_of_kind k

let c2ml_function_of_kind k = 
  match k with
    | Ap_manager_t -> "camlidl_apron_manager_ptr_c2ml"
    | Wrapper_t -> assert false
    | Wrapper_array_t _ -> assert false
    | Optional_wrapper_t -> assert false (*"optional_wrapper_t_c2ml"*)
    | Ap_dim_t -> "camlidl_c2ml_dim_ap_dim_t"
    | Ap_dimension_t -> "camlidl_c2ml_dim_struct_ap_dimension_t"
    | Ap_dimchange_t -> "camlidl_apron_dimchange_c2ml"
    | Ap_dimperm_t -> "camlidl_c2ml_dim_struct_ap_dimperm_t"
    | Ap_coeff_t -> "camlidl_coeff_ptr_c2ml"
    | Ap_lingen0_t -> "camlidl_lingen0_ptr_c2ml"
    | Ap_lingen0_array_t -> "camlidl_lingen0_array_ptr_c2ml"
    | Ap_linexpr0_t -> "camlidl_linexpr0_ptr_c2ml"
    | Ap_linexpr0_array_t -> "camlidl_linexpr0_array_ptr_c2ml"
    | Ap_lincons0_t -> "camlidl_lincons0_ptr_c2ml"
    | Ap_lincons0_array_t -> "camlidl_lincons0_array_ptr_c2ml"
    | Ap_texpr0_t -> "camlidl_apron_texpr0_ptr_c2ml"
    | Ap_texpr0_array_t -> "camlidl_apron_texpr0_array_t_c2ml"
    | Ap_tcons0_t -> "camlidl_apron_tcons0_t_c2ml"
    | Ap_tcons0_array_t -> "camlidl_apron_tcons0_array_t_c2ml"
    | Bool -> "Val_bool"
    | Int | Size_t -> "Val_bool"

let ml2c_function_of_kind k = 
  match k with
    | Ap_manager_t -> "camlidl_apron_manager_ptr_ml2c"
    | Wrapper_t -> "wrapper_alloc"
    | Wrapper_array_t _ -> assert false
    | Optional_wrapper_t -> assert false
    | Ap_dim_t -> "camlidl_ml2c_dim_ap_dim_t"
    | Ap_dimension_t -> "camlidl_ml2c_dim_struct_ap_dimension_t"
    | Ap_dimchange_t -> "camlidl_apron_dimchange_ml2c"
    | Ap_dimperm_t -> "camlidl_ml2c_dim_struct_ap_dimperm_t"
    | Ap_coeff_t -> "camlidl_coeff_ptr_ml2c"
    | Ap_lingen0_t -> "camlidl_lingen0_ptr_ml2c"
    | Ap_lingen0_array_t -> "camlidl_lingen0_array_ptr_ml2c"
    | Ap_linexpr0_t -> "camlidl_linexpr0_ptr_ml2c"
    | Ap_linexpr0_array_t -> "camlidl_linexpr0_array_ptr_ml2c"
    | Ap_lincons0_t -> "camlidl_lincons0_ptr_ml2c"
    | Ap_lincons0_array_t -> "camlidl_lincons0_array_ptr_ml2c"
    | Ap_texpr0_t -> "camlidl_apron_texpr0_ptr_ml2c"
    | Ap_texpr0_array_t -> "camlidl_apron_texpr0_array_t_ml2c"
    | Ap_tcons0_t -> "camlidl_apron_tcons0_t_ml2c"
    | Ap_tcons0_array_t -> "camlidl_apron_tcons0_array_t_ml2c"
    | Bool -> "Bool_val"
    | Int -> "Int_val"
    | Size_t -> "(size_t) Int_val"

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
  Format.fprintf fmt ")@."
    
let print_local_declaration fmt f =
  let build_caml_local is_first (k, name) =
    match k with
      | Wrapper_t -> is_first
      | _ ->
	if not is_first then
	  Format.fprintf fmt ", @?";
	Format.fprintf fmt "v_%s@?" name;
	false
  in
  let n = List.fold_left (fun res (k, _) -> if k <> Wrapper_t then res+1 else res) 0 f.args in
  let p = match f.ret with 
    | Value _ -> n+1
    | _ -> n
  in
  Format.fprintf fmt "  CAMLparam0();@.";
  Format.fprintf fmt "  CAMLlocal%d(@?" p;
  let b = List.fold_left build_caml_local true f.args in
  begin
    match f.ret with
      | Void | Side_effect _ -> Format.printf ");@."
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
      | Void | Side_effect _ -> ()
      | Value _ ->
	let return_type = c_type_of_return_spec f.ret in
	Format.fprintf fmt "  %s res;@." return_type
  end;
  if List.exists (fun (k, _) -> match k with Wrapper_array_t _ -> true | _ -> false) f.args then
    Format.fprintf fmt "  size_t i;@.";
  Format.fprintf fmt "  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };@.";
  Format.fprintf fmt "  camlidl_ctx _ctx = &_ctxs;@."

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
      | Wrapper_t -> ()
      | Wrapper_array_t i ->
	let (_, size) = List.nth f.args i in
	Format.fprintf fmt "  v_%s = caml_alloc(%s, 0);@." name size;
	Format.fprintf fmt "  for (i = 0; i < %s; i++)@." size;
	Format.fprintf fmt "    Store_field(v_%s, i, %s[i]->val);@." name name;
      | Optional_wrapper_t ->
	Format.fprintf fmt "  if (%s == NULL)@." name;
	Format.fprintf fmt "    v_%s = Val_int(0);@." name;
	Format.fprintf fmt "  else {@.";
	Format.fprintf fmt "    v_%s = caml_alloc(1, 0);@." name;
	Format.fprintf fmt "    Store_field(v_%s, 0, %s->val);@." name name;
	Format.fprintf fmt "  }@."
      | Ap_dim_t | Ap_dimension_t | Ap_dimperm_t ->
	Format.fprintf fmt "  v_%s = %s(&%s, _ctx);@." name (c2ml_function_of_kind k) name	
      | _ -> Format.fprintf fmt "  v_%s = %s(&%s);@." name (c2ml_function_of_kind k) name
  in
  List.iter caml_local_conversion f.args

let print_result_conversion fmt f =
  match f.ret with
    | Void -> ()
    | Side_effect i ->
      let (k, name) = List.nth f.args i in
      begin
	match k with
	  | Ap_dim_t | Ap_dimension_t | Ap_dimchange_t | Ap_dimperm_t | Ap_tcons0_array_t -> 
	    Format.fprintf fmt "  %s(v_%s, &%s, _ctx);@." (ml2c_function_of_kind k) name name
	  | _ ->
	    Format.fprintf fmt "  %s(v_%s, &%s);@." (ml2c_function_of_kind k) name name
      end
    | Value k -> 
      match k with
	| Wrapper_t | Bool | Int | Size_t ->
	  Format.fprintf fmt "  res = %s(v_res);@." (ml2c_function_of_kind k)
	| Ap_dim_t | Ap_dimension_t | Ap_dimchange_t | Ap_dimperm_t | Ap_tcons0_array_t ->
	  Format.fprintf fmt "  %s(v_res, &res, _ctx);@." (ml2c_function_of_kind k) 
	| _ -> Format.fprintf fmt "  %s(v_res, &res);@." (ml2c_function_of_kind k) 

let print_callback fmt f =
  let print_assignement_return_value fmt () = 
    match f.ret with
      | Void | Side_effect _ -> ()
      | Value _ -> Format.fprintf fmt "v_res = @?"
  in
  let build_callback_argument fmt (k, name) = 
    match k with
      | Wrapper_t -> Format.fprintf fmt "%s->val@?" name
      | _ -> Format.fprintf fmt "v_%s@?" name
  in
  let print_callback_preamble fmt () =
    let n = List.length f.args in
    if n > 3 then
      ignore (List.fold_left (fun i arg -> 
	Format.fprintf fmt "  arg_tab[%d] = %a;@." i build_callback_argument arg;
	i+1) 0 f.args)
  in
  let print_callback_function fmt closure_name = 
    let n = List.length f.args in
    if n <= 3 then begin
      Format.fprintf fmt "caml_callback%d(*closure_%s@?" n closure_name;
      List.iter (fun arg -> Format.fprintf fmt ", %a@?" build_callback_argument arg) f.args
    end
    else
      Format.fprintf fmt "caml_callbackN(*closure_%s, %d, arg_tab@?" closure_name n;
    Format.fprintf fmt ")@?"
  in
  match f.destructive_variant with
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

let print_return fmt f =
  Format.fprintf fmt "  camlidl_free(_ctx);@.";
  match f.ret with
    | Void | Side_effect _ -> Format.fprintf fmt "  CAMLreturn0;@."
    | Value _ ->
      let return_type = c_type_of_return_spec f.ret in
      Format.fprintf fmt "  CAMLreturnT(%s, res);@." return_type

let print fmt f =
  Format.fprintf fmt "%a@?" print_signature f;
  Format.fprintf fmt "{@.";
  Format.fprintf fmt "%a@?" print_local_declaration f;
  Format.fprintf fmt "%a@?" print_closure_definition f;
  Format.fprintf fmt "%a@?" print_argument_conversion f;
  Format.fprintf fmt "%a@?" print_callback f;
  Format.fprintf fmt "%a@?" print_return f;
  Format.fprintf fmt "}@."

let _ = 
  let fun_list = 
    [
      { ret = Value Wrapper_t; name = "copy"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None };
      { ret = Value Size_t; name = "size"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Void; name = "minimize"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Void; name = "canonicalize"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Int; name = "hash"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Void; name = "approximate"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Int, "algorithm")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "bottom"; args = [(Ap_manager_t, "man"); (Ap_dimension_t, "dim")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "top"; args = [(Ap_manager_t, "man"); (Ap_dimension_t, "dim")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "of_box"; args = [(Ap_manager_t, "man"); (Ap_dimension_t, "dim"); (Ap_linexpr0_t, "box")]; destructive_variant = None }; 
      { ret = Value Ap_dimension_t; name = "dimension"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Bool; name = "is_bottom"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Bool; name = "is_top"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Bool; name = "is_leq"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a1"); (Wrapper_t, "a2")]; destructive_variant = None }; 
      { ret = Value Bool; name = "is_eq"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a1"); (Wrapper_t, "a2")]; destructive_variant = None }; 
      { ret = Value Bool; name = "sat_interval"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dim_t, "dim"); (Ap_coeff_t, "interval")]; destructive_variant = None }; 
      { ret = Value Bool; name = "sat_lincons"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_lincons0_t, "lincons")]; destructive_variant = None }; 
      { ret = Value Bool; name = "sat_tcons"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_tcons0_t, "tcons")]; destructive_variant = None }; 
      { ret = Value Bool; name = "is_dimension_unconstrained"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dim_t, "dim")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "bound_dimension"; args = [(Ap_manager_t, "man"); (Ap_coeff_t, "interval"); (Wrapper_t, "a"); (Ap_dim_t, "dim")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "bound_linexpr"; args = [(Ap_manager_t, "man"); (Ap_coeff_t, "interval"); (Wrapper_t, "a"); (Ap_linexpr0_t, "expr")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "bound_texpr"; args = [(Ap_manager_t, "man"); (Ap_coeff_t, "interval"); (Wrapper_t, "a"); (Ap_texpr0_t, "expr")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "to_lincons_array"; args = [(Ap_manager_t, "man"); (Ap_lincons0_array_t, "array"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Ap_tcons0_array_t; name = "to_tcons_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "to_box"; args = [(Ap_manager_t, "man"); (Ap_linexpr0_t, "box"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Side_effect 1; name = "to_lingen_array"; args = [(Ap_manager_t, "man"); (Ap_lingen0_array_t, "array"); (Wrapper_t, "a")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "meet"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a1"); (Wrapper_t, "a2")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "join"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a1"); (Wrapper_t, "a2")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "meet_array"; args = [(Ap_manager_t, "man"); (Wrapper_array_t 2, "tab"); (Size_t, "size")]; destructive_variant = None };
      { ret = Value Wrapper_t; name = "join_array"; args = [(Ap_manager_t, "man"); (Wrapper_array_t 2, "tab"); (Size_t, "size")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "meet_lincons_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_lincons0_array_t, "array")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "meet_tcons_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_tcons0_array_t, "array")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "add_ray_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_lingen0_array_t, "array")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "assign_linexpr_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "org"); (Ap_dim_t, "tdim"); (Ap_linexpr0_array_t, "texpr"); (Optional_wrapper_t, "dest")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "assign_texpr_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "org"); (Ap_dim_t, "tdim"); (Ap_texpr0_array_t, "array"); (Optional_wrapper_t, "dest")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "substitute_linexpr_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "org"); (Ap_dim_t, "tdim"); (Ap_linexpr0_array_t, "array"); (Optional_wrapper_t, "dest")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "substitute_texpr_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "org"); (Ap_dim_t, "tdim"); (Ap_texpr0_array_t, "array"); (Optional_wrapper_t, "dest")]; destructive_variant = Some 1}; 
      { ret = Value Wrapper_t; name = "forget_array"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dim_t, "tdim"); (Size_t, "size"); (Bool, "project")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "add_dimensions"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dimchange_t, "dimchange"); (Bool, "project")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "remove_dimensions"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dimchange_t, "dimchange")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "permute_dimensions"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dimperm_t, "perm")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "expand"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dim_t, "dim"); (Size_t, "n")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "fold"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a"); (Ap_dim_t, "tdim"); (Size_t, "size")]; destructive_variant = Some 1 }; 
      { ret = Value Wrapper_t; name = "widening"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a1"); (Wrapper_t, "a2")]; destructive_variant = None }; 
      { ret = Value Wrapper_t; name = "closure"; args = [(Ap_manager_t, "man"); (Wrapper_t, "a")]; destructive_variant = Some 1 };
    ]
  in
  List.iter (fun f -> Format.printf "%a@." print f) fun_list
