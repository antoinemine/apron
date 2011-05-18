(* File generated from texpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

(*  ********************************************************************** *)
(** {2 Types} *)
(*  ********************************************************************** *)

type t = Texpr0.t Common.val1
    (** Types of expressions *)
type earray = Texpr0.t array Common.val1
    (** Types of arrays of expressions *)

(** Unary operators *)
type unop =   Texpr0.unop = Neg | Cast | Sqrt

(** Binary operators *)
type binop = Texpr0.binop = Add | Sub | Mul | Div | Mod

(** Destination type for rounding *)
type typ =   Texpr0.typ =   Real | Int | Single | Double | Extended | Quad

 (** Rounding direction *)
type round = Texpr0.round = Near | Zero | Up | Down | Rnd

(** User type for tree expressions *)
type 'a gexpr = 'a Texpr0.gexpr =
  | Cst of Coeff.f Coeff.tt
  | DimVar of 'a
  | Unop of unop * 'a gexpr * typ * round
  | Binop of binop * 'a gexpr * 'a gexpr * typ * round
type expr = Var.t gexpr

(*  ********************************************************************** *)
(** {2 Printing} *)
(*  ********************************************************************** *)
let string_of_unop = Texpr0.string_of_unop
let string_of_binop = Texpr0.string_of_binop
let string_of_typ = Texpr0.string_of_typ
let string_of_round = Texpr0.string_of_round
let print_unop = Texpr0.print_unop
let print_binop = Texpr0.print_binop
let print_typ = Texpr0.print_typ
let print_round = Texpr0.print_round
let print_expr fmt (expr:expr) =
  Texpr0.print_gexpr Var.to_string fmt expr
let print fmt expr =
  Texpr0.print
   (Environment.string_of_dim expr.Common.env)
   fmt expr.Common.val0
let array_print ?first ?sep ?last fmt array =
  Texpr0.array_print ?first ?sep ?last (Environment.string_of_dim array.Common.env) fmt array.Common.val0

(*  ********************************************************************** *)
(** {2 Expressions} *)
(*  ********************************************************************** *)

(** {3 Constructors and Destructor} *)

(** {4 General constructor (actually the most efficient)} *)
let rec expr0_of_expr1 env expr =
  Texpr0.map_gexpr (Environment.dim_of_var env) expr
let rec expr1_of_expr0 env expr =
  Texpr0.map_gexpr (Environment.var_of_dim env) expr

let of_expr env expr : t = Common.make_val1 (Texpr0.of_expr (expr0_of_expr1 env expr)) env
let of_linexpr1 e = Common.make_val1 (Texpr0.of_linexpr0 e.Common.val0) e.Common.env
let copy e = Common.make_val1 (Texpr0.copy e.Common.val0) e.Common.env
let to_expr texpr1 =
  let expr0 = Texpr0.to_expr texpr1.Common.val0 in
  expr1_of_expr0 texpr1.Common.env expr0

(** {4 Incremental constructors} *)
let cst env x = Common.make_val1 (Texpr0.cst x) env
let var env x = Common.make_val1 (Texpr0.dim (Environment.dim_of_var env x)) env
let unop op x t r = Common.make_val1 (Texpr0.unop x.Common.val0 t r) x.Common.env
let binop op x1 x2 t r =
  if x1.Common.env<>x2.Common.env then raise (Invalid_argument "Texpr1.binop: two operands are not based on the same environment");
  Common.make_val1 (Texpr0.binop x1.Common.val0 x2.Common.val0 t r) x1.Common.env

(** {3 Tests} *)
let is_interval_cst (x:t) = Texpr0.is_interval_cst x.Common.val0
let is_interval_linear (x:t) = Texpr0.is_interval_linear x.Common.val0
let is_interval_polynomial (x:t) = Texpr0.is_interval_polynomial x.Common.val0
let is_interval_polyfrac (x:t) = Texpr0.is_interval_polyfrac x.Common.val0
let is_scalar (x:t) = Texpr0.is_scalar x.Common.val0

(** {3 Operations} *)
let extend_environment array env =
  let change = Environment.dimchange array.Common.env env in
  Common.make_val1 (Texpr0.add_dimensions array.Common.val0 change) env
let extend_environment_with array env =
  let change = Environment.dimchange array.Common.env env in
  Texpr0.add_dimensions_with array.Common.val0 change

(*  ********************************************************************** *)
(** {2 Arrays} *)
(*  ********************************************************************** *)

(** {3 Constructors and Destructor} *)
let array_make env length : earray =
  Common.make_val1 (Array.make length (Texpr0.cst (Coeff.init_set_int (Common.D 0)))) env
let array_of_linexpr1_array array = Common.make_val1 (Texpr0.array_of_linexpr0_array array.Common.val0) array.Common.env
let array_length array = Array.length array.Common.val0
let array_copy array = Common.make_val1 (Array.map Texpr0.copy array.Common.val0) array.Common.env

(** {3 Access} *)
let array_get_index array index = Common.make_val1 array.Common.val0.(index) array.Common.env
let array_set_index array index x =
  if (array.Common.env<>x.Common.env)  then raise (Invalid_argument ("Apron.Texpr1.array_set_index: the environments of the arguments should be the same"));
  array.Common.val0.(index) <- x.Common.val0

(** {3 Operations} *)
let array_extend_environment array env =
  let change = Environment.dimchange array.Common.env env in
  Common.make_val1 (Texpr0.array_add_dimensions array.Common.val0 change) env
let array_extend_environment_with array env =
  let change = Environment.dimchange array.Common.env env in
  Texpr0.array_add_dimensions_with array.Common.val0 change
