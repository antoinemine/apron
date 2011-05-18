(* File generated from linexpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Linexpr0.t Common.val1
and earray = Linexpr0.earray Common.val1


(** {2 Linear expressions/constraints/generators} *)

(** {3 Pretty printing} *)
val print : Format.formatter -> t -> unit

(** {3 Initialization} *)
val init : Common.discr -> int -> Environment.t -> t
val init_set : t -> t
val init_set_linexprD : LinexprD.t -> Environment.t -> t
val init_set_linexprMPQ : LinexprMPQ.t -> Environment.t -> t
val init_set_linexprMPFR : LinexprMPFR.t -> Environment.t -> t

(** {3 Misc)} *)
val resize : t -> int -> unit
val minimize : t -> unit

(** {3 Assignement (with possible conversion)} *)
val set : t -> t -> Common.num_internal -> bool
val set_linexpr0 : t -> Linexpr0.t -> Common.num_internal -> bool
val set_linexprD : t -> LinexprD.t -> Common.num_internal -> bool
val set_linexprMPQ : t -> LinexprMPQ.t -> Common.num_internal -> bool
val set_linexprMPFR : t -> LinexprMPFR.t -> Common.num_internal -> bool
val get_linexprD : LinexprD.t -> t -> Common.num_internal -> bool
val get_linexprMPQ : LinexprMPQ.t -> t -> Common.num_internal -> bool
val get_linexprMPFR : LinexprMPFR.t -> t -> Common.num_internal -> bool

(** {3 Access} *)
val iter : (Coeff.t -> Var.t -> unit) -> t -> unit

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_set_coeff"

(** {3 Change of environment} *)
val extend_environment : t -> t -> Environment.t -> unit


(** {2 Arrays} *)
(** {3  Pretty printing} *)

val array_print :
  ?first:(unit, Format.formatter, unit) format ->
  ?sep:(unit, Format.formatter, unit) format ->
  ?last:(unit, Format.formatter, unit) format ->
 Format.formatter -> earray -> unit

(** {3 Initialization and Assignement Functions} *)
val array_init : Common.discr -> int -> Environment.t -> earray
val array_init_set : earray -> earray
val array_resize : earray -> int -> unit
val array_minimize : earray -> unit

(** {3 Assignements with conversions} *)

val array_set : earray -> earray -> Common.num_internal -> bool
val array_set_linexprD_array : earray -> LinexprD.earray -> Common.num_internal -> bool
val array_set_linexprMPQ_array : earray -> LinexprMPQ.earray -> Common.num_internal -> bool
val array_set_linexprMPFR_array : earray -> LinexprMPFR.earray -> Common.num_internal -> bool
val array_get_linexprD_array : LinexprD.earray -> earray -> Common.num_internal -> bool
val array_get_linexprMPQ_array : LinexprMPQ.earray -> earray -> Common.num_internal -> bool
val array_get_linexprMPFR_array : LinexprMPFR.earray -> earray -> Common.num_internal -> bool

(** {3 Access} *)
val array_size : earray -> int
val array_set_index : earray -> int -> t -> Common.num_internal -> bool
val array_get_index : t -> earray -> int -> Common.num_internal -> bool
val array_set_index_linexprD : earray -> int -> LinexprD.t -> Common.num_internal -> bool
val array_set_index_linexprMPQ : earray -> int -> LinexprMPQ.t -> Common.num_internal -> bool
val array_set_index_linexprMPFR : earray -> int -> LinexprMPFR.t -> Common.num_internal -> bool
val array_get_index_linexprD : LinexprD.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linexprMPQ : LinexprMPQ.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linexprMPFR : LinexprMPFR.t -> earray -> int -> Common.num_internal -> bool

(** {3 Change of dimensions} *)
val array_extend_environment : earray -> earray -> Environment.t -> unit

