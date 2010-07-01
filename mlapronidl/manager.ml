(* File generated from manager.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type funid = 
| Funid_unknown
| Funid_copy
| Funid_free
| Funid_asize
| Funid_minimize
| Funid_canonicalize
| Funid_hash
| Funid_approximate
| Funid_fprint
| Funid_fprintdiff
| Funid_fdump
| Funid_serialize_raw
| Funid_deserialize_raw
| Funid_bottom
| Funid_top
| Funid_of_box
| Funid_dimension
| Funid_is_bottom
| Funid_is_top
| Funid_is_leq
| Funid_is_eq
| Funid_is_dimension_unconstrained
| Funid_sat_interval
| Funid_sat_lincons
| Funid_sat_tcons
| Funid_bound_dimension
| Funid_bound_linexpr
| Funid_bound_texpr
| Funid_to_box
| Funid_to_lincons_array
| Funid_to_tcons_array
| Funid_to_generator_array
| Funid_meet
| Funid_meet_array
| Funid_meet_lincons_array
| Funid_meet_tcons_array
| Funid_join
| Funid_join_array
| Funid_add_ray_array
| Funid_assign_linexpr_array
| Funid_substitute_linexpr_array
| Funid_assign_texpr_array
| Funid_substitute_texpr_array
| Funid_add_dimensions
| Funid_remove_dimensions
| Funid_permute_dimensions
| Funid_forget_array
| Funid_expand
| Funid_fold
| Funid_widening
| Funid_closure
| Funid_change_environment
| Funid_rename_array

and funopt = {
  algorithm: int;
  timeout: int;
  max_object_size: int;
  flag_exact_wanted: bool;
  flag_best_wanted: bool;
}
and exc = 
| Exc_none
| Exc_timeout
| Exc_out_of_space
| Exc_overflow
| Exc_invalid_argument
| Exc_not_implemented

and exclog = {
  exn: exc;
  funid: funid;
  msg: string;
}
and 'a t

(** APRON Managers *)
(** The type parameter ['a] allows to distinguish managers allocated by different underlying abstract domains. *)

(** Concerning the other types,

- [funid] defines identifiers for the generic function working on abstrat values;

- [funopt] defines the options associated to generic functions; 

- [exc] defines the different kind of exceptions; 

- [exclog] defines the exceptions raised by APRON functions.

*)

external get_library : 'a t -> string
	= "camlidl_manager_ap_manager_get_library"

external get_version : 'a t -> string
	= "camlidl_manager_ap_manager_get_version"

external funopt_make : unit -> funopt
	= "camlidl_manager_ap_funopt_make"

external get_funopt : 'a t -> funid -> funopt
	= "camlidl_manager_ap_manager_get_funopt"

external set_funopt : 'a t -> funid -> funopt -> unit
	= "camlidl_manager_ap_manager_set_funopt"

external get_flag_exact : 'a t -> bool
	= "camlidl_manager_ap_manager_get_flag_exact"

external get_flag_best : 'a t -> bool
	= "camlidl_manager_ap_manager_get_flag_best"

exception Error of exclog
let _ = Callback.register_exception "apron exception" (Error { exn=Exc_none; funid = Funid_bottom; msg = "dummy"; })

let string_of_funid = function
| Funid_unknown -> "Funid_unknown"
| Funid_copy -> "Funid_copy"
| Funid_free -> "Funid_free"
| Funid_asize -> "Funid_asize"
| Funid_minimize -> "Funid_minimize"
| Funid_canonicalize -> "Funid_canonicalize"
| Funid_hash -> "Funid_hash"
| Funid_approximate -> "Funid_approximate"
| Funid_fprint -> "Funid_fprint"
| Funid_fprintdiff -> "Funid_fprintdiff"
| Funid_fdump -> "Funid_fdump"
| Funid_serialize_raw -> "Funid_serialize_raw"
| Funid_deserialize_raw -> "Funid_deserialize_raw"
| Funid_bottom -> "Funid_bottom"
| Funid_top -> "Funid_top"
| Funid_of_box -> "Funid_of_box"
| Funid_dimension -> "Funid_dimension"
| Funid_is_bottom -> "Funid_is_bottom"
| Funid_is_top -> "Funid_is_top"
| Funid_is_leq -> "Funid_is_leq"
| Funid_is_eq -> "Funid_is_eq"
| Funid_is_dimension_unconstrained -> "Funid_is_dimension_unconstrained"
| Funid_sat_interval -> "Funid_sat_interval"
| Funid_sat_lincons -> "Funid_sat_lincons"
| Funid_sat_tcons -> "Funid_sat_tcons"
| Funid_bound_dimension -> "Funid_bound_dimension"
| Funid_bound_linexpr -> "Funid_bound_linexpr"
| Funid_bound_texpr -> "Funid_bound_texpr"
| Funid_to_box -> "Funid_to_box"
| Funid_to_lincons_array -> "Funid_to_lincons_array"
| Funid_to_tcons_array -> "Funid_to_tcons_array"
| Funid_to_generator_array -> "Funid_to_generator_array"
| Funid_meet -> "Funid_meet"
| Funid_meet_array -> "Funid_meet_array"
| Funid_meet_lincons_array -> "Funid_meet_lincons_array"
| Funid_meet_tcons_array -> "Funid_meet_tcons_array"
| Funid_join -> "Funid_join"
| Funid_join_array -> "Funid_join_array"
| Funid_add_ray_array -> "Funid_add_ray_array"
| Funid_assign_linexpr_array -> "Funid_assign_linexpr_array"
| Funid_substitute_linexpr_array -> "Funid_substitute_linexpr_array"
| Funid_assign_texpr_array -> "Funid_assign_texpr_array"
| Funid_substitute_texpr_array -> "Funid_substitute_texpr_array"
| Funid_add_dimensions -> "Funid_add_dimensions"
| Funid_remove_dimensions -> "Funid_remove_dimensions"
| Funid_permute_dimensions -> "Funid_permute_dimensions"
| Funid_forget_array -> "Funid_forget_array"
| Funid_expand -> "Funid_expand"
| Funid_fold -> "Funid_fold"
| Funid_widening -> "Funid_widening"
| Funid_closure -> "Funid_closure"
| Funid_change_environment -> "Funid_change_environment"
| Funid_rename_array -> "Funid_rename_array"

let string_of_exc = function
| Exc_none -> "Exc_none"
| Exc_timeout -> "Exc_timeout"
| Exc_out_of_space -> "Exc_out_of_space"
| Exc_overflow -> "Exc_overflow"
| Exc_invalid_argument -> "Exc_invalid_argument"
| Exc_not_implemented -> "Exc_not_implemented"

let print_funid fmt x = Format.pp_print_string fmt (string_of_funid x)
let print_exc fmt x = Format.pp_print_string fmt (string_of_exc x)
let print_exclog fmt x =
Format.fprintf fmt "{ @[ exn = %a;@ funid = %a;@ msg = %s;@] }"
    print_exc x.exn print_funid x.funid x.msg
let print_funopt fmt x =
Format.fprintf fmt "{ @[ algorithm = %i;@ timeout = %i;@ max_object_size = %i;@ flag_exact_wanted = %b;@ flag_best_wanted = %b;@] }"
    x.algorithm x.timeout x.max_object_size x.flag_exact_wanted x.flag_best_wanted

external set_deserialize : 'a t -> unit
	= "camlidl_manager_ap_manager_set_deserialize"

external get_deserialize : unit -> 'a t
	= "camlidl_manager_ap_manager_get_deserialize"

external init: unit -> unit = "camlidl_apron_init"

let _ = init()
