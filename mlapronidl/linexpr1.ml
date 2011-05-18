(* File generated from linexpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Linexpr0.t Common.val1
and earray = Linexpr0.earray Common.val1

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_linexpr1_ap_linexpr1_set_coeff"

let print fmt (x:t) = Linexpr0.print (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let init (discr:Common.discr) length env : t = Common.make_val1 (Linexpr0.init discr length) env
let init_set (x:t) : t = Common.make_val1 (Linexpr0.init_set x.Common.val0) x.Common.env
let init_set_linexprD x env : t = Common.make_val1 (Linexpr0.init_set_linexprD x) env
let init_set_linexprMPQ x env : t = Common.make_val1 (Linexpr0.init_set_linexprMPQ x) env
let init_set_linexprMPFR x env : t = Common.make_val1 (Linexpr0.init_set_linexprMPFR x) env
let resize (x:t) length = Linexpr0.resize x.Common.val0 length
let minimize (x:t) = Linexpr0.minimize x.Common.val0

let set (x:t) (y:t) intern = x.Common.env <- y.Common.env; Linexpr0.set x.Common.val0 y.Common.val0 intern
let set_linexpr0 (x:t) y intern = Linexpr0.set x.Common.val0 y intern
let set_linexprD (x:t) y intern = Linexpr0.set_linexprD x.Common.val0 y intern
let set_linexprMPQ (x:t) y intern = Linexpr0.set_linexprMPQ x.Common.val0 y intern
let set_linexprMPFR (x:t) y intern = Linexpr0.set_linexprMPFR x.Common.val0 y intern
let get_linexprD x (y:t) intern = Linexpr0.get_linexprD x y.Common.val0 intern
let get_linexprMPQ x (y:t) intern = Linexpr0.get_linexprMPQ x y.Common.val0 intern
let get_linexprMPFR x (y:t) intern = Linexpr0.get_linexprMPFR x y.Common.val0 intern

let iter f x =
  match x.Common.val0 with
  | Common.D e -> LinexprD.iter1 (fun eitvD var -> f (Common.D eitvD) var) e x.Common.env
  | Common.MPQ e -> LinexprMPQ.iter1 (fun eitvMPQ var -> f (Common.MPQ eitvMPQ) var) e x.Common.env
  | Common.MPFR e -> LinexprMPFR.iter1 (fun eitvMPFR var -> f (Common.MPFR eitvMPFR) var) e x.Common.env
let extend_environment res expr newenv =
  res.Common.env <- newenv;
  Linexpr0.extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

let array_print ?first ?sep ?last fmt (x:earray) = Linexpr0.array_print ?first ?sep ?last (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let array_init (discr:Common.discr) length env : earray = Common.make_val1 (Linexpr0.array_init discr length) env
let array_init_set (x:earray) : earray = Common.make_val1 (Linexpr0.array_init_set x.Common.val0) x.Common.env
let array_init_set_linexprD_array x env : earray = Common.make_val1 (Linexpr0.array_init_set_linexprD_array x) env
let array_init_set_linexprMPQ_array x env : earray = Common.make_val1 (Linexpr0.array_init_set_linexprMPQ_array x) env
let array_init_set_linexprMPFR_array x env : earray = Common.make_val1 (Linexpr0.array_init_set_linexprMPFR_array x) env
let array_resize (x:earray) length = Linexpr0.array_resize x.Common.val0 length
let array_minimize (x:earray) = Linexpr0.array_minimize x.Common.val0

let array_set (x:earray) (y:earray) intern = x.Common.env <- y.Common.env; Linexpr0.array_set x.Common.val0 y.Common.val0 intern
let array_set_linexpr0_array (x:earray) (y:Linexpr0.earray) intern = Linexpr0.array_set  x.Common.val0 y intern
let array_set_linexprD_array (x:earray) y intern = Linexpr0.array_set_linexprD_array x.Common.val0 y intern
let array_set_linexprMPQ_array (x:earray) y intern = Linexpr0.array_set_linexprMPQ_array x.Common.val0 y intern
let array_set_linexprMPFR_array (x:earray) y intern = Linexpr0.array_set_linexprMPFR_array x.Common.val0 y intern
let array_get_linexprD_array x (y:earray) intern = Linexpr0.array_get_linexprD_array x y.Common.val0 intern
let array_get_linexprMPQ_array x (y:earray) intern = Linexpr0.array_get_linexprMPQ_array x y.Common.val0 intern
let array_get_linexprMPFR_array x (y:earray) intern = Linexpr0.array_get_linexprMPFR_array x y.Common.val0 intern
let array_size x = Linexpr0.array_size x.Common.val0

let array_set_index_linexprD (x:earray) i a intern = Linexpr0.array_set_index_linexprD x.Common.val0 i a intern
let array_set_index_linexprMPQ (x:earray) i a intern = Linexpr0.array_set_index_linexprMPQ x.Common.val0 i a intern
let array_set_index_linexprMPFR (x:earray) i a intern = Linexpr0.array_set_index_linexprMPFR x.Common.val0 i a intern
let array_get_index_linexprD a (x:earray) i intern = Linexpr0.array_get_index_linexprD a x.Common.val0 i intern
let array_get_index_linexprMPQ a (x:earray) i intern = Linexpr0.array_get_index_linexprMPQ a x.Common.val0 i intern
let array_get_index_linexprMPFR a (x:earray) i intern = Linexpr0.array_get_index_linexprMPFR a x.Common.val0 i intern
let array_set_index (x:earray) i a intern =
  if (x.Common.env<>a.Common.env) then raise (Invalid_argument ("Apron.Linexpr1.array_set_index: the environments of the arguments should be the same"));
  Linexpr0.array_set_index x.Common.val0 i a.Common.val0 intern
let array_get_index a (x:earray) i intern =
  a.Common.env <- x.Common.env;
  Linexpr0.array_get_index a.Common.val0 x.Common.val0 i intern
let array_extend_environment res expr newenv =
  res.Common.env <- newenv;
  Linexpr0.array_extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

