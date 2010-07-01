(* File generated from texpr0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t
and unop = 
  | Neg
  | Cast
  | Sqrt (** *)
  (** Unary operators *)
and binop = 
  | Add
  | Sub
  | Mul
  | Div
  | Mod (** *)
  (** Binary operators *)
and typ = 
  | Real
  | Int
  | Single
  | Double
  | Extended
  | Quad (** *)
  (** Destination type for rounding *)
and round = 
  | Near
  | Zero
  | Up
  | Down
  | Rnd (** *)
  (** Rounding direction *)

(** APRON tree expressions of level 0 *)


(** User type for tree expressions *) 
type expr = 
| Cst of Coeff.t 
| Dim of Dim.t 
| Unop of unop * expr * typ * round 
| Binop of binop * expr * expr * typ * round 

(** {2 Constructors and Destructor} *)
(** General constructor (actually the most efficient *)
external of_expr : expr -> t = "camlidl_texpr0_ap_texpr0_of_expr"

(** Copy *)
external copy : t -> t
	= "camlidl_texpr0_ap_texpr0_copy"

(** Conversion *)
external of_linexpr : Linexpr0.t -> t
	= "camlidl_texpr0_ap_texpr0_of_linexpr"

(** General destructor *)
external to_expr : t -> expr = "camlidl_texpr0_ap_texpr0_to_expr"


(** {3 Incremental constructors} *)

external cst : Coeff.t -> t
	= "camlidl_texpr0_ap_texpr0_cst"

external dim : Dim.t -> t
	= "camlidl_texpr0_ap_texpr0_dim"

external unop : unop -> t -> typ -> round -> t
	= "camlidl_texpr0_ap_texpr0_unop"

external binop : binop -> typ -> round -> t -> t -> t
	= "camlidl_texpr0_ap_texpr0_binop"


(** {2 Tests} *)

external is_interval_cst : t -> bool
	= "camlidl_texpr0_ap_texpr0_is_interval_cst"

external is_interval_linear : t -> bool
	= "camlidl_texpr0_ap_texpr0_is_interval_linear"

external is_interval_polynomial : t -> bool
	= "camlidl_texpr0_ap_texpr0_is_interval_polynomial"

external is_interval_polyfrac : t -> bool
	= "camlidl_texpr0_ap_texpr0_is_interval_polyfrac"

external is_scalar : t -> bool
	= "camlidl_texpr0_ap_texpr0_is_scalar"


(** {2 Printing} *)


val string_of_unop  : unop  -> string
val string_of_binop : binop -> string
val string_of_typ   : typ   -> string
val string_of_round : round -> string
val print_unop  :  Format.formatter -> unop  -> unit
val print_binop :  Format.formatter -> binop -> unit
val print_typ   :  Format.formatter -> typ   -> unit
val print_round :  Format.formatter -> round -> unit
val print_expr : (Dim.t -> string) -> Format.formatter -> expr -> unit
(** Print a tree expression, using a function converting from dimensions to names *)
val print : (Dim.t -> string) -> Format.formatter -> t -> unit
(** Print an abstract tree expression, using a function converting from dimensions to names *)

(** {2 Internal usage for level 1} *)

val print_sprint_unop : unop -> typ -> round -> string
val print_sprint_binop : binop -> typ -> round -> string
val print_precedence_of_unop : unop -> int
val print_precedence_of_binop : binop -> int

