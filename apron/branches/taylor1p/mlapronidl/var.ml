(* File generated from var.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t

(** APRON Variables *)

external of_string : string -> t
	= "camlidl_var_ap_var_of_string"

external compare : t -> t -> int
	= "camlidl_var_ap_var_compare"

external to_string : t -> string
	= "camlidl_var_ap_var_to_string"

external hash : t -> int
	= "camlidl_var_ap_var_hash"


let print fmt v = Format.pp_print_string fmt (to_string v)


(** Initialisation of abstract type operations in C library *)
external set_var_operations : unit -> unit = "camlidl_apron_set_var_operations"

let _ = set_var_operations ()

