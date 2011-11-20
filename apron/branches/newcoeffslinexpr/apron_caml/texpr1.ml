(* File generated from texpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

open Common

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
type ('a,'b) gexpr = ('a,'b) Texpr0.gexpr =
  | Cst of 'a
  | DimVar of 'b
  | Unop of unop * ('a,'b) gexpr * typ * round
  | Binop of binop * ('a,'b) gexpr * ('a,'b) gexpr * typ * round
type expr = (Coeff.f Coeff.tt, Var.t) gexpr

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
  Texpr0.print_gexpr Coeff.print Var.to_string fmt expr
let print fmt expr =
  Texpr0.print
   (Environment.string_of_dim expr.env)
   fmt expr.val0
let array_print ?first ?sep ?last fmt array =
  Texpr0.array_print ?first ?sep ?last (Environment.string_of_dim array.env) fmt array.val0

(*  ********************************************************************** *)
(** {2 Expressions} *)
(*  ********************************************************************** *)

(** {3 Constructors and Destructor} *)

(** {4 General constructor (actually the most efficient)} *)
let rec expr0_of_expr1 env expr =
  Texpr0.map_gexpr (fun x -> x) (Environment.dim_of_var env) expr
let rec expr1_of_expr0 env expr =
  Texpr0.map_gexpr (fun x -> x) (Environment.var_of_dim env) expr

let of_expr env expr : t = make_val1 (Texpr0.of_expr (expr0_of_expr1 env expr)) env
let of_linexpr1 e = make_val1 (Texpr0.of_linexpr0 e.val0) e.env
let copy e = make_val1 (Texpr0.copy e.val0) e.env
let to_expr texpr1 =
  let expr0 = Texpr0.to_expr texpr1.val0 in
  expr1_of_expr0 texpr1.env expr0

(** {4 Incremental constructors} *)
let cst env x = make_val1 (Texpr0.cst x) env
let var env x = make_val1 (Texpr0.dim (Environment.dim_of_var env x)) env
let unop op x t r = make_val1 (Texpr0.unop x.val0 t r) x.env
let binop op x1 x2 t r =
  check_val1 x1 x2;
  make_val1 (Texpr0.binop x1.val0 x2.val0 t r) x1.env

(** {3 Tests} *)
let is_interval_cst (x:t) = Texpr0.is_interval_cst x.val0
let is_interval_linear (x:t) = Texpr0.is_interval_linear x.val0
let is_interval_polynomial (x:t) = Texpr0.is_interval_polynomial x.val0
let is_interval_polyfrac (x:t) = Texpr0.is_interval_polyfrac x.val0
let is_scalar (x:t) = Texpr0.is_scalar x.val0

(** {3 Operations} *)
let extend_environment array env =
  let change = Environment.dimchange array.env env in
  make_val1 (Texpr0.add_dimensions array.val0 change) env
let extend_environment_with array env =
  let change = Environment.dimchange array.env env in
  Texpr0.add_dimensions_with array.val0 change

(*  ********************************************************************** *)
(** {2 Arrays} *)
(*  ********************************************************************** *)

(** {3 Constructors and Destructor} *)
let array_make env length : earray =
  make_val1 (Array.make length (Texpr0.cst (Coeff.init_set_int (D()) 0))) env
let array_of_linexpr1_array array = make_val1 (Texpr0.array_of_linexpr0_array array.val0) array.env
let array_length array = Array.length array.val0
let array_copy array = make_val1 (Array.map Texpr0.copy array.val0) array.env

(** {3 Access} *)
let array_get_index array index = make_val1 array.val0.(index) array.env
let array_set_index array index x =
  check_val1 array x;
  array.val0.(index) <- x.val0

(** {3 Operations} *)
let array_extend_environment array env =
  let change = Environment.dimchange array.env env in
  make_val1 (Texpr0.array_add_dimensions array.val0 change) env
let array_extend_environment_with array env =
  let change = Environment.dimchange array.env env in
  Texpr0.array_add_dimensions_with array.val0 change
