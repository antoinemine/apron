(* File generated from linexpr0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t

(** APRON Linear expressions of level 0 *)

(** Create a linear expression. Its representation is sparse if [None] is provided, dense of size [size] if [Some size] is provided. *)
external make : int option -> t
	= "camlidl_linexpr0_ap_linexpr0_make"


val of_list : int option -> (Coeff.t * Dim.t) list -> Coeff.t option -> t
(** Combines {!make} and {!set_list} (see below) *)
val of_array : int option -> (Coeff.t * Dim.t) array -> Coeff.t option -> t
(** Combines {!make} and {!set_array} (see below) *)

(** In case of sparse representation, remove zero coefficients *)
external minimize : t -> unit
	= "camlidl_linexpr0_ap_linexpr0_minimize"

(** Copy *)
external copy : t -> t
	= "camlidl_linexpr0_ap_linexpr0_copy"

(** Comparison with lexicographic ordering using Coeff.cmp, terminating by constant *)
external compare : t -> t -> int
	= "camlidl_linexpr0_ap_linexpr0_compare"

(** Hashing function *)
external hash : t -> int
	= "camlidl_linexpr0_ap_linexpr0_hash"

(** Get the size of the linear expression (which may be sparse or dense) *)
external get_size : t -> int
	= "camlidl_linexpr0_ap_linexpr0_get_size"

(** Get the constant *)
external get_cst : t -> Coeff.t
	= "camlidl_linexpr0_ap_linexpr0_get_cst"

(** Get the coefficient corresponding to the dimension *)
external get_coeff : t -> int -> Coeff.t
	= "camlidl_linexpr0_ap_linexpr0_get_coeff"


val set_list : t -> (Coeff.t * Dim.t) list -> Coeff.t option -> unit
(** Set simultaneously a number of coefficients.

[set_list expr [(c1,1); (c2,2)] (Some cst)] assigns coefficients [c1] 
to dimension 1, coefficient [c2] to dimension 2, and coefficient [cst]
to the constant. If [(Some cst)] is replaced by [None],
the constant coefficient is not assigned. *)
val set_array : t -> (Coeff.t * Dim.t) array -> Coeff.t option -> unit
(** Set simultaneously a number of coefficients, as [set_list]. *)

(** Set the constant *)
external set_cst : t -> Coeff.t -> unit
	= "camlidl_linexpr0_ap_linexpr0_set_cst"

(** Set the coefficient corresponding to the dimension *)
external set_coeff : t -> int -> Coeff.t -> unit
	= "camlidl_linexpr0_ap_linexpr0_set_coeff"

(** Iter the function on the pairs coefficient/dimension of the linear expression *)

external iter: (Coeff.t -> Dim.t -> unit) -> t -> unit = "camlidl_expr0_linexpr0_iter"
 

(** Print a linear expression, using a function converting from dimensions to names *)
val print : (Dim.t -> string) -> Format.formatter -> t -> unit

