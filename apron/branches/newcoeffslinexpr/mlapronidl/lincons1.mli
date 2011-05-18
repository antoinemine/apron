(* File generated from lincons1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Lincons0.t Common.val1
and earray = Lincons0.earray Common.val1


(** {2 Linear expressions/constraints/generators} *)

(** {3 Pretty printing} *)
val print : Format.formatter -> t -> unit

(** {3 Initialization} *)
val init : Common.discr -> int -> Environment.t -> t
val init_set : t -> t
val init_set_linconsD : LinconsD.t -> Environment.t -> t
val init_set_linconsMPQ : LinconsMPQ.t -> Environment.t -> t
val init_set_linconsMPFR : LinconsMPFR.t -> Environment.t -> t

(** {3 Misc)} *)
val resize : t -> int -> unit
val minimize : t -> unit

(** {3 Assignement (with possible conversion)} *)
val set : t -> t -> Common.num_internal -> bool
val set_lincons0 : t -> Lincons0.t -> Common.num_internal -> bool
val set_linconsD : t -> LinconsD.t -> Common.num_internal -> bool
val set_linconsMPQ : t -> LinconsMPQ.t -> Common.num_internal -> bool
val set_linconsMPFR : t -> LinconsMPFR.t -> Common.num_internal -> bool
val get_linconsD : LinconsD.t -> t -> Common.num_internal -> bool
val get_linconsMPQ : LinconsMPQ.t -> t -> Common.num_internal -> bool
val get_linconsMPFR : LinconsMPFR.t -> t -> Common.num_internal -> bool

(** {3 Access} *)
val iter : (Coeff.t -> Var.t -> unit) -> t -> unit

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_set_coeff"

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
val array_set_linconsD_array : earray -> LinconsD.earray -> Common.num_internal -> bool
val array_set_linconsMPQ_array : earray -> LinconsMPQ.earray -> Common.num_internal -> bool
val array_set_linconsMPFR_array : earray -> LinconsMPFR.earray -> Common.num_internal -> bool
val array_get_linconsD_array : LinconsD.earray -> earray -> Common.num_internal -> bool
val array_get_linconsMPQ_array : LinconsMPQ.earray -> earray -> Common.num_internal -> bool
val array_get_linconsMPFR_array : LinconsMPFR.earray -> earray -> Common.num_internal -> bool

(** {3 Access} *)
val array_size : earray -> int
val array_set_index : earray -> int -> t -> Common.num_internal -> bool
val array_get_index : t -> earray -> int -> Common.num_internal -> bool
val array_set_index_linconsD : earray -> int -> LinconsD.t -> Common.num_internal -> bool
val array_set_index_linconsMPQ : earray -> int -> LinconsMPQ.t -> Common.num_internal -> bool
val array_set_index_linconsMPFR : earray -> int -> LinconsMPFR.t -> Common.num_internal -> bool
val array_get_index_linconsD : LinconsD.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linconsMPQ : LinconsMPQ.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linconsMPFR : LinconsMPFR.t -> earray -> int -> Common.num_internal -> bool

(** {3 Change of dimensions} *)
val array_extend_environment : earray -> earray -> Environment.t -> unit

