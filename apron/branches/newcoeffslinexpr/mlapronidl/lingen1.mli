(* File generated from lingen1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Lingen0.t Common.val1
and earray = Lingen0.earray Common.val1


(** {2 Linear expressions/constraints/generators} *)

(** {3 Pretty printing} *)
val print : Format.formatter -> t -> unit

(** {3 Initialization} *)
val init : Common.discr -> int -> Environment.t -> t
val init_set : t -> t
val init_set_lingenD : LingenD.t -> Environment.t -> t
val init_set_lingenMPQ : LingenMPQ.t -> Environment.t -> t
val init_set_lingenMPFR : LingenMPFR.t -> Environment.t -> t

(** {3 Misc)} *)
val resize : t -> int -> unit
val minimize : t -> unit

(** {3 Assignement (with possible conversion)} *)
val set : t -> t -> Common.num_internal -> bool
val set_lingen0 : t -> Lingen0.t -> Common.num_internal -> bool
val set_lingenD : t -> LingenD.t -> Common.num_internal -> bool
val set_lingenMPQ : t -> LingenMPQ.t -> Common.num_internal -> bool
val set_lingenMPFR : t -> LingenMPFR.t -> Common.num_internal -> bool
val get_lingenD : LingenD.t -> t -> Common.num_internal -> bool
val get_lingenMPQ : LingenMPQ.t -> t -> Common.num_internal -> bool
val get_lingenMPFR : LingenMPFR.t -> t -> Common.num_internal -> bool

(** {3 Access} *)
val iter : (Coeff.t -> Var.t -> unit) -> t -> unit

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_set_coeff"

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
val array_set_lingenD_array : earray -> LingenD.earray -> Common.num_internal -> bool
val array_set_lingenMPQ_array : earray -> LingenMPQ.earray -> Common.num_internal -> bool
val array_set_lingenMPFR_array : earray -> LingenMPFR.earray -> Common.num_internal -> bool
val array_get_lingenD_array : LingenD.earray -> earray -> Common.num_internal -> bool
val array_get_lingenMPQ_array : LingenMPQ.earray -> earray -> Common.num_internal -> bool
val array_get_lingenMPFR_array : LingenMPFR.earray -> earray -> Common.num_internal -> bool

(** {3 Access} *)
val array_size : earray -> int
val array_set_index : earray -> int -> t -> Common.num_internal -> bool
val array_get_index : t -> earray -> int -> Common.num_internal -> bool
val array_set_index_lingenD : earray -> int -> LingenD.t -> Common.num_internal -> bool
val array_set_index_lingenMPQ : earray -> int -> LingenMPQ.t -> Common.num_internal -> bool
val array_set_index_lingenMPFR : earray -> int -> LingenMPFR.t -> Common.num_internal -> bool
val array_get_index_lingenD : LingenD.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_lingenMPQ : LingenMPQ.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_lingenMPFR : LingenMPFR.t -> earray -> int -> Common.num_internal -> bool

(** {3 Change of dimensions} *)
val array_extend_environment : earray -> earray -> Environment.t -> unit

