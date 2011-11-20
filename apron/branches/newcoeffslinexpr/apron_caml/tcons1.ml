(** APRON tree constraints and array of tree constraints of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

open Common

type t = Tcons0.t val1
type earray = Tcons0.t array val1

(*  ********************************************************************** *)
(** {2 Constraints} *)
(*  ********************************************************************** *)

let make texpr1 typ ompq =
  make_val1 (Tcons0.make texpr1.val0 typ ompq) texpr1.env
let of_lincons1 e = make_val1 (Tcons0.of_lincons0 e.val0) e.env
let copy e = make_val1 (Tcons0.copy e.val0) e.env
let print fmt cons =
  Tcons0.print
   (Environment.string_of_dim cons.env)
   fmt cons.val0

let get_typ cons = cons.val0.Tcons0.typ
let get_mpq mpq cons = Mpq.set mpq cons.val0.Tcons0.mpq
let set_typ cons typ = cons.val0.Tcons0.typ <- typ
let set_mpq cons mpq = Mpq.set cons.val0.Tcons0.mpq mpq
let get_texpr1 cons =  make_val1 cons.val0.Tcons0.texpr0 cons.env

let extend_environment array env =
  let change = Environment.dimchange array.env env in
  make_val1 (Tcons0.add_dimensions array.val0 change) env
let extend_environment_with array env =
  let change = Environment.dimchange array.env env in
  Tcons0.add_dimensions_with array.val0 change

(* ====================================================================== *)
(** {2 Type array} *)
(* ====================================================================== *)

let array_make env length =
  let cons0 = Tcons0.make (Texpr0.cst (Coeff.init_set_int (D()) 0)) EQ None in
  make_val1 (Array.make length cons0) env
let array_of_lincons1_array array = make_val1 (Tcons0.array_of_lincons0_array array.val0) array.env
let array_length array = Array.length array.val0
let array_copy array = make_val1 (Array.map Tcons0.copy array.val0) array.env
let array_print ?first ?sep ?last fmt array =
  Tcons0.array_print ?first ?sep ?last (Environment.string_of_dim array.env) fmt array.val0

(** {3 Access} *)
let array_get_index array index = make_val1 array.val0.(index) array.env
let array_set_index array index x =
  check_val1 array x;
  array.val0.(index) <- x.val0

(** {3 Operations} *)
let array_extend_environment array env =
  let change = Environment.dimchange array.env env in
  make_val1 (Tcons0.array_add_dimensions array.val0 change) env
let array_extend_environment_with array env =
  let change = Environment.dimchange array.env env in
  Tcons0.array_add_dimensions_with array.val0 change
