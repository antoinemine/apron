(* File generated from texpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable texpr0: Texpr0.t;
  mutable env: Environment.t;
}

(** APRON Expressions of level 1 *)

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
type expr = 
| Cst of Coeff.t 
| Var of Var.t 
| Unop of unop * expr * typ * round 
| Binop of binop * expr * expr * typ * round 

(** {2 Constructors and Destructor} *)
(** General constructor (actually the most efficient) *)
val of_expr : Environment.t -> expr -> t

(** Copy *)
val copy : t -> t

(** Conversion *)
val of_linexpr : Linexpr1.t -> t

(** General destructor *)
val to_expr : t -> expr


(** {3 Incremental constructors} *)

external cst : Environment.t -> Coeff.t -> t
	= "camlidl_texpr1_ap_texpr1_cst"

external var : Environment.t -> Var.t -> t
	= "camlidl_texpr1_ap_texpr1_var"

external unop : Texpr0.unop -> t -> Texpr0.typ -> Texpr0.round -> t
	= "camlidl_texpr1_ap_texpr1_unop"

external binop : Texpr0.binop -> t -> t -> Texpr0.typ -> Texpr0.round -> t
	= "camlidl_texpr1_ap_texpr1_binop"


(** {2 Tests} *)

val is_interval_cst : t -> bool 
val is_interval_linear : t -> bool 
val is_interval_polynomial : t -> bool 
val is_interval_polyfrac : t -> bool 
val is_scalar : t -> bool 


(** {2 Operations} *)

(** Change the environment of the expression for a super-environement. Raise [Failure] if it is not the case *)
external extend_environment : t -> Environment.t -> t
	= "camlidl_texpr1_ap_texpr1_extend_environment"

(** Side-effet version of the previous function *)
external extend_environment_with : t -> Environment.t -> unit
	= "camlidl_texpr1_ap_texpr1_extend_environment_with"


(** Get the underlying expression of level 0 (which is not a copy). *)
val get_texpr0: t -> Texpr0.t

(** Get the environement of the expression *)
val get_env: t -> Environment.t



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

