(* File generated from texpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Texpr0.t Common.val1
type earray = Texpr0.t array Common.val1

(** APRON tree expressions of level 1 *)

(** Unary operators *)
type unop = Texpr0.unop =
  | Neg
  | Cast
  | Sqrt

(** Binary operators *)
type binop = Texpr0.binop =
  | Add
  | Sub
  | Mul
  | Div
  | Mod

(** Destination type for rounding *)
type typ = Texpr0.typ =
  | Real
  | Int
  | Single
  | Double
  | Extended
  | Quad

(** Rounding direction *)
type round = Texpr0.round =
  | Near
  | Zero
  | Up
  | Down
  | Rnd

(** User type for tree expressions *)
type 'a gexpr = 'a Texpr0.gexpr =
  | Cst of Coeff.f Coeff.tt
  | DimVar of 'a
  | Unop of unop * 'a gexpr * typ * round
  | Binop of binop * 'a gexpr * 'a gexpr * typ * round
type expr = Var.t gexpr


(** {2 Printing} *)



val string_of_unop  : unop  -> string
val string_of_binop : binop -> string
val string_of_typ   : typ   -> string
val string_of_round : round -> string
val print_unop  :  Format.formatter -> unop  -> unit
val print_binop :  Format.formatter -> binop -> unit
val print_typ   :  Format.formatter -> typ   -> unit
val print_round :  Format.formatter -> round -> unit
val print_expr : Format.formatter -> expr -> unit
  (** Print a tree expression *)
val print : Format.formatter -> t -> unit
  (** Print an abstract tree expression *)

(** {2 Expressions} *)

(** {3 Constructors and Destructor} *)

(** General constructor (actually the most efficient) *)
val of_expr : Environment.t -> expr -> t
val to_expr : t -> expr
val copy : t -> t
val of_linexpr1 : Linexpr1.t -> t

(** {4 Incremental constructors} *)

external cst : Environment.t -> 'a Coeff.tt -> t	= "camlidl_texpr1_ap_texpr1_cst"
external var : Environment.t -> Var.t -> t	= "camlidl_texpr1_ap_texpr1_var"
external unop : Texpr0.unop -> t -> Texpr0.typ -> Texpr0.round -> t	= "camlidl_texpr1_ap_texpr1_unop"
external binop : Texpr0.binop -> t -> t -> Texpr0.typ -> Texpr0.round -> t	= "camlidl_texpr1_ap_texpr1_binop"

(** {3 Tests} *)

val is_interval_cst : t -> bool
val is_interval_linear : t -> bool
val is_interval_polynomial : t -> bool
val is_interval_polyfrac : t -> bool
val is_scalar : t -> bool

(** {3 Operations} *)
(** Change the environment of the expression for a super-environement. Raise [Failure] if it is not the case *)
external extend_environment : t -> Environment.t -> t	= "camlidl_texpr1_ap_texpr1_extend_environment"
(** Side-effet version of the previous function *)
external extend_environment_with : t -> Environment.t -> unit	= "camlidl_texpr1_ap_texpr1_extend_environment_with"

(** {2 Arrays} *)

(** {3 Constructors and Destructor} *)
val array_make : int -> earray
val array_size : earray -> int
val array_of_lincons1_array : Lincons1.earray -> earray

(** {3 Access} *)y
val array_get_index : earray -> int -> ty
val array_set_index : earray -> int -> t -> unity

(** {3 Operations} *)
val array_extend_environment : earray -> env -> earray
val array_extend_environment_with : earray -> env -> unit
