(* File generated from var.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t

(** APRON Variables *)

(** Constructor *)
external of_string : string -> t
	= "camlidl_var_ap_var_of_string"

(** Comparison function *)
external compare : t -> t -> int
	= "camlidl_var_ap_var_compare"

(** Conversion to string *)
external to_string : t -> string
	= "camlidl_var_ap_var_to_string"

(** Hash function *)
external hash : t -> int
	= "camlidl_var_ap_var_hash"


(** Printing function *)
val print : Format.formatter -> t -> unit


(** Initialisation of abstract type operations in C library *)
external set_var_operations : unit -> unit = "camlidl_apron_set_var_operations"
