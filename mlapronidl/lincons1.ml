(* File generated from lincons1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Lincons0.t Common.val1
and earray = Lincons0.earray Common.val1

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lincons1_ap_lincons1_set_coeff"

let print fmt (x:t) = Lincons0.print (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let init (discr:Common.discr) length env : t = Common.make_val1 (Lincons0.init discr length) env
let init_set (x:t) : t = Common.make_val1 (Lincons0.init_set x.Common.val0) x.Common.env
let init_set_linconsD x env : t = Common.make_val1 (Lincons0.init_set_linconsD x) env
let init_set_linconsMPQ x env : t = Common.make_val1 (Lincons0.init_set_linconsMPQ x) env
let init_set_linconsMPFR x env : t = Common.make_val1 (Lincons0.init_set_linconsMPFR x) env
let resize (x:t) length = Lincons0.resize x.Common.val0 length
let minimize (x:t) = Lincons0.minimize x.Common.val0

let set (x:t) (y:t) intern = x.Common.env <- y.Common.env; Lincons0.set x.Common.val0 y.Common.val0 intern
let set_lincons0 (x:t) y intern = Lincons0.set x.Common.val0 y intern
let set_linconsD (x:t) y intern = Lincons0.set_linconsD x.Common.val0 y intern
let set_linconsMPQ (x:t) y intern = Lincons0.set_linconsMPQ x.Common.val0 y intern
let set_linconsMPFR (x:t) y intern = Lincons0.set_linconsMPFR x.Common.val0 y intern
let get_linconsD x (y:t) intern = Lincons0.get_linconsD x y.Common.val0 intern
let get_linconsMPQ x (y:t) intern = Lincons0.get_linconsMPQ x y.Common.val0 intern
let get_linconsMPFR x (y:t) intern = Lincons0.get_linconsMPFR x y.Common.val0 intern

let iter f x =
  match x.Common.val0 with
  | Common.D e -> LinconsD.iter1 (fun eitvD var -> f (Common.D eitvD) var) e x.Common.env
  | Common.MPQ e -> LinconsMPQ.iter1 (fun eitvMPQ var -> f (Common.MPQ eitvMPQ) var) e x.Common.env
  | Common.MPFR e -> LinconsMPFR.iter1 (fun eitvMPFR var -> f (Common.MPFR eitvMPFR) var) e x.Common.env
let extend_environment res expr newenv =
  res.Common.env <- newenv;
  Lincons0.extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

let array_print ?first ?sep ?last fmt (x:earray) = Lincons0.array_print ?first ?sep ?last (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let array_init (discr:Common.discr) length env : earray = Common.make_val1 (Lincons0.array_init discr length) env
let array_init_set (x:earray) : earray = Common.make_val1 (Lincons0.array_init_set x.Common.val0) x.Common.env
let array_init_set_linconsD_array x env : earray = Common.make_val1 (Lincons0.array_init_set_linconsD_array x) env
let array_init_set_linconsMPQ_array x env : earray = Common.make_val1 (Lincons0.array_init_set_linconsMPQ_array x) env
let array_init_set_linconsMPFR_array x env : earray = Common.make_val1 (Lincons0.array_init_set_linconsMPFR_array x) env
let array_resize (x:earray) length = Lincons0.array_resize x.Common.val0 length
let array_minimize (x:earray) = Lincons0.array_minimize x.Common.val0

let array_set (x:earray) (y:earray) intern = x.Common.env <- y.Common.env; Lincons0.array_set x.Common.val0 y.Common.val0 intern
let array_set_lincons0_array (x:earray) (y:Lincons0.earray) intern = Lincons0.array_set  x.Common.val0 y intern
let array_set_linconsD_array (x:earray) y intern = Lincons0.array_set_linconsD_array x.Common.val0 y intern
let array_set_linconsMPQ_array (x:earray) y intern = Lincons0.array_set_linconsMPQ_array x.Common.val0 y intern
let array_set_linconsMPFR_array (x:earray) y intern = Lincons0.array_set_linconsMPFR_array x.Common.val0 y intern
let array_get_linconsD_array x (y:earray) intern = Lincons0.array_get_linconsD_array x y.Common.val0 intern
let array_get_linconsMPQ_array x (y:earray) intern = Lincons0.array_get_linconsMPQ_array x y.Common.val0 intern
let array_get_linconsMPFR_array x (y:earray) intern = Lincons0.array_get_linconsMPFR_array x y.Common.val0 intern
let array_size x = Lincons0.array_size x.Common.val0

let array_set_index_linconsD (x:earray) i a intern = Lincons0.array_set_index_linconsD x.Common.val0 i a intern
let array_set_index_linconsMPQ (x:earray) i a intern = Lincons0.array_set_index_linconsMPQ x.Common.val0 i a intern
let array_set_index_linconsMPFR (x:earray) i a intern = Lincons0.array_set_index_linconsMPFR x.Common.val0 i a intern
let array_get_index_linconsD a (x:earray) i intern = Lincons0.array_get_index_linconsD a x.Common.val0 i intern
let array_get_index_linconsMPQ a (x:earray) i intern = Lincons0.array_get_index_linconsMPQ a x.Common.val0 i intern
let array_get_index_linconsMPFR a (x:earray) i intern = Lincons0.array_get_index_linconsMPFR a x.Common.val0 i intern
let array_set_index (x:earray) i a intern =
  if (x.Common.env<>a.Common.env) then raise (Invalid_argument ("Apron.Lincons1.array_set_index: the environments of the arguments should be the same"));
  Lincons0.array_set_index x.Common.val0 i a.Common.val0 intern
let array_get_index a (x:earray) i intern =
  a.Common.env <- x.Common.env;
  Lincons0.array_get_index a.Common.val0 x.Common.val0 i intern
let array_extend_environment res expr newenv =
  res.Common.env <- newenv;
  Lincons0.array_extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

