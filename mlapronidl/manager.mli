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

(** Get the name of the effective library which allocated the manager *)
external get_library : 'a t -> string
	= "camlidl_manager_ap_manager_get_library"

(** Get the version of the effective library which allocated the manager *)
external get_version : 'a t -> string
	= "camlidl_manager_ap_manager_get_version"

(** Return the default options for any function ([0] or [false] for al fields) *)
external funopt_make : unit -> funopt
	= "camlidl_manager_ap_funopt_make"

(** Get the options sets for the function. The result is a copy of the internal structure and may be freely modified. [funid] should be different from [Funid_change_environment] and [Funid_rename_array] (no option associated to them). *)
external get_funopt : 'a t -> funid -> funopt
	= "camlidl_manager_ap_manager_get_funopt"

(** Set the options for the function. [funid] should be different from [Funid_change_environment] and [Funid_rename_array] (no option associated to them). *)
external set_funopt : 'a t -> funid -> funopt -> unit
	= "camlidl_manager_ap_manager_set_funopt"

(** Get the corresponding result flag *)
external get_flag_exact : 'a t -> bool
	= "camlidl_manager_ap_manager_get_flag_exact"

(** Get the corresponding result flag *)
external get_flag_best : 'a t -> bool
	= "camlidl_manager_ap_manager_get_flag_best"

(** Exception raised by functions of the interface *)
exception Error of exclog
val string_of_funid: funid -> string
val string_of_exc: exc -> string
val print_funid: Format.formatter -> funid -> unit
val print_funopt: Format.formatter -> funopt -> unit
val print_exc: Format.formatter -> exc -> unit
val print_exclog: Format.formatter -> exclog -> unit
  (** Printing functions *)
(** Set / get the global manager used for deserialization *)
external set_deserialize : 'a t -> unit
	= "camlidl_manager_ap_manager_set_deserialize"

external get_deserialize : unit -> 'a t
	= "camlidl_manager_ap_manager_get_deserialize"

