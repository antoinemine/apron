#define _AP_linyyy1_MARK_
#if defined(_AP_linexpr1_MARK_)
(** (Interval) linear expressions *)
#elif defined(_AP_lincons1_MARK_)
(** (Interval) linear constraints *)
#elif defined(_AP_linexpr1_MARK_)
(** Linear generators *)
#endif

type t = Linyyy0.t Common.val1
type earray = Linyyy0.earray Common.val1

(*  ********************************************************************** *)
(** {2 Linear expressions/constraints/generators} *)
(*  ********************************************************************** *)

(** {3 Pretty printing} *)
val print : Format.formatter -> t -> unit

(** {3 Initialization} *)
val init : Common.discr -> int -> Environment.t -> t
val init_set : t -> t
val init_set_linyyyD : LinyyyD.t -> Environment.t -> t
val init_set_linyyyMPQ : LinyyyMPQ.t -> Environment.t -> t
val init_set_linyyyMPFR : LinyyyMPFR.t -> Environment.t -> t

(** {3 Misc} *)
val resize : t -> int -> unit
val minimize : t -> unit

(** {3 Assignement (with possible conversion)} *)
val set : t -> t -> Common.num_internal -> bool
val set_linyyy0 : t -> Linyyy0.t -> Common.num_internal -> bool
val set_linyyyD : t -> LinyyyD.t -> Common.num_internal -> bool
val set_linyyyMPQ : t -> LinyyyMPQ.t -> Common.num_internal -> bool
val set_linyyyMPFR : t -> LinyyyMPFR.t -> Common.num_internal -> bool
val get_linyyyD : LinyyyD.t -> t -> Common.num_internal -> bool
val get_linyyyMPQ : LinyyyMPQ.t -> t -> Common.num_internal -> bool
val get_linyyyMPFR : LinyyyMPFR.t -> t -> Common.num_internal -> bool

(** {3 Access} *)
val iter : (Coeff.t -> Var.t -> unit) -> t -> unit
val get_cst : Coeff.t -> t -> Common.num_internal -> bool
val get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool
val set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool
val set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool

#if defined(_AP_lincons1_MARK_) || defined(_AP_lingen1_MARK_)
val get_linexpr1 : Linexpr1.t -> t -> Common.num_internal -> bool
val get_yyytyp : t -> Common.yyytyp
val set_linexpr1 : t -> Linexpr1.t -> Common.num_internal -> bool
val set_yyytyp : t -> Common.yyytyp -> unit
#endif
#if defined(_AP_lincons1_MARK_)
val get_mpq : Mpq.t -> t -> unit
val set_mpq : t -> 'a Mpq.tt -> unit
#endif

(** {3 Change of environment} *)
val extend_environment : t -> t -> Environment.t -> unit

(*  ********************************************************************** *)
(** {2 Arrays} *)
(*  ********************************************************************** *)

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
val array_length : earray -> int

(** {3 Assignements with conversions} *)
val array_set : earray -> earray -> Common.num_internal -> bool
val array_set_linyyyD_array : earray -> LinyyyD.earray -> Common.num_internal -> bool
val array_set_linyyyMPQ_array : earray -> LinyyyMPQ.earray -> Common.num_internal -> bool
val array_set_linyyyMPFR_array : earray -> LinyyyMPFR.earray -> Common.num_internal -> bool
val array_get_linyyyD_array : LinyyyD.earray -> earray -> Common.num_internal -> bool
val array_get_linyyyMPQ_array : LinyyyMPQ.earray -> earray -> Common.num_internal -> bool
val array_get_linyyyMPFR_array : LinyyyMPFR.earray -> earray -> Common.num_internal -> bool

(** {3 Access} *)
val array_set_index : earray -> int -> t -> Common.num_internal -> bool
val array_get_index : t -> earray -> int -> Common.num_internal -> bool
val array_set_index_linyyyD : earray -> int -> LinyyyD.t -> Common.num_internal -> bool
val array_set_index_linyyyMPQ : earray -> int -> LinyyyMPQ.t -> Common.num_internal -> bool
val array_set_index_linyyyMPFR : earray -> int -> LinyyyMPFR.t -> Common.num_internal -> bool
val array_get_index_linyyyD : LinyyyD.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linyyyMPQ : LinyyyMPQ.t -> earray -> int -> Common.num_internal -> bool
val array_get_index_linyyyMPFR : LinyyyMPFR.t -> earray -> int -> Common.num_internal -> bool

(** {3 Change of dimensions} *)
val array_extend_environment : earray -> earray -> Environment.t -> unit
