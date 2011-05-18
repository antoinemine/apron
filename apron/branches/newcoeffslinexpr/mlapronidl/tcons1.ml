(** APRON tree constraints and array of tree constraints of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Tcons0.t Common.val1
type earray = Tcons0.t array Common.val1

(*  ********************************************************************** *)
(** {2 Constraints} *)
(*  ********************************************************************** *)

let make texpr1 typ =
  Common.make_val1 (Tcons0.make texpr1.Common.val0 typ) texpr1.Common.env
let of_lincons1 e = Common.make_val1 (Tcons0.of_lincons0 e.Common.val0) e.Common.env
let copy e = Common.make_val1 (Tcons0.copy e.Common.val0) e.Common.env
let print fmt cons =
  Tcons0.print
   (Environment.string_of_dim cons.Common.env)
   fmt cons.Common.val0

let get_typ cons = cons.Common.val0.Tcons0.typ
let set_typ cons typ = cons.Common.val0.Tcons0.typ <- typ
let get_mpq mpq cons = Mpq.set mpq cons.Common.val0.Tcons0.mpq
let set_mpq cons mpq = Mpq.set cons.Common.val0.Tcons0.mpq mpq
let get_texpr1 cons =  Common.make_val1 cons.Common.val0.Tcons0.texpr0 cons.Common.env

let extend_environment array env =
  let change = Environment.dimchange array.Common.env env in
  Common.make_val1 (Tcons0.add_dimensions array.Common.val0 change) env
let extend_environment_with array env =
  let change = Environment.dimchange array.Common.env env in
  Tcons0.add_dimensions_with array.Common.val0 change

(* ====================================================================== *)
(** {2 Type array} *)
(* ====================================================================== *)

let array_make env length =
  let cons0 = Tcons0.make (Texpr0.cst (Coeff.init_set_int (Common.D 0))) Common.EQ in
  Common.make_val1 (Array.make length cons0) env
let array_of_lincons1_array array = Common.make_val1 (Tcons0.array_of_lincons0_array array.Common.val0) array.Common.env
let array_length array = Array.length array.Common.val0
let array_copy array = Common.make_val1 (Array.map Tcons0.copy array.Common.val0) array.Common.env
let array_print ?first ?sep ?last fmt array =
  Tcons0.array_print ?first ?sep ?last (Environment.string_of_dim array.Common.env) fmt array.Common.val0

(** {3 Access} *)
let array_get_index array index = Common.make_val1 array.Common.val0.(index) array.Common.env
let array_set_index array index x =
  if (array.Common.env<>x.Common.env)  then raise (Invalid_argument ("Apron.Tcons1.array_set_index: the environments of the arguments should be the same"));
  array.Common.val0.(index) <- x.Common.val0

(** {3 Operations} *)
let array_extend_environment array env =
  let change = Environment.dimchange array.Common.env env in
  Common.make_val1 (Tcons0.array_add_dimensions array.Common.val0 change) env
let array_extend_environment_with array env =
  let change = Environment.dimchange array.Common.env env in
  Tcons0.array_add_dimensions_with array.Common.val0 change
