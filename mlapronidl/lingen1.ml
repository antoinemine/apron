(* File generated from lingen1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Lingen0.t Common.val1
and earray = Lingen0.earray Common.val1

external get_cst : Coeff.t -> t -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_get_cst"
external get_coeff : Coeff.t -> t -> Var.t -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_get_coeff"
external set_cst : t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_set_cst"
external set_coeff : t -> Var.t -> 'a Coeff.tt -> Common.num_internal -> bool	= "camlidl_lingen1_ap_lingen1_set_coeff"

let print fmt (x:t) = Lingen0.print (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let init (discr:Common.discr) length env : t = Common.make_val1 (Lingen0.init discr length) env
let init_set (x:t) : t = Common.make_val1 (Lingen0.init_set x.Common.val0) x.Common.env
let init_set_lingenD x env : t = Common.make_val1 (Lingen0.init_set_lingenD x) env
let init_set_lingenMPQ x env : t = Common.make_val1 (Lingen0.init_set_lingenMPQ x) env
let init_set_lingenMPFR x env : t = Common.make_val1 (Lingen0.init_set_lingenMPFR x) env
let resize (x:t) length = Lingen0.resize x.Common.val0 length
let minimize (x:t) = Lingen0.minimize x.Common.val0

let set (x:t) (y:t) intern = x.Common.env <- y.Common.env; Lingen0.set x.Common.val0 y.Common.val0 intern
let set_lingen0 (x:t) y intern = Lingen0.set x.Common.val0 y intern
let set_lingenD (x:t) y intern = Lingen0.set_lingenD x.Common.val0 y intern
let set_lingenMPQ (x:t) y intern = Lingen0.set_lingenMPQ x.Common.val0 y intern
let set_lingenMPFR (x:t) y intern = Lingen0.set_lingenMPFR x.Common.val0 y intern
let get_lingenD x (y:t) intern = Lingen0.get_lingenD x y.Common.val0 intern
let get_lingenMPQ x (y:t) intern = Lingen0.get_lingenMPQ x y.Common.val0 intern
let get_lingenMPFR x (y:t) intern = Lingen0.get_lingenMPFR x y.Common.val0 intern

let iter f x =
  match x.Common.val0 with
  | Common.D e -> LingenD.iter1 (fun eitvD var -> f (Common.D eitvD) var) e x.Common.env
  | Common.MPQ e -> LingenMPQ.iter1 (fun eitvMPQ var -> f (Common.MPQ eitvMPQ) var) e x.Common.env
  | Common.MPFR e -> LingenMPFR.iter1 (fun eitvMPFR var -> f (Common.MPFR eitvMPFR) var) e x.Common.env
let extend_environment res expr newenv =
  res.Common.env <- newenv;
  Lingen0.extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

let array_print ?first ?sep ?last fmt (x:earray) = Lingen0.array_print ?first ?sep ?last (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let array_init (discr:Common.discr) length env : earray = Common.make_val1 (Lingen0.array_init discr length) env
let array_init_set (x:earray) : earray = Common.make_val1 (Lingen0.array_init_set x.Common.val0) x.Common.env
let array_init_set_lingenD_array x env : earray = Common.make_val1 (Lingen0.array_init_set_lingenD_array x) env
let array_init_set_lingenMPQ_array x env : earray = Common.make_val1 (Lingen0.array_init_set_lingenMPQ_array x) env
let array_init_set_lingenMPFR_array x env : earray = Common.make_val1 (Lingen0.array_init_set_lingenMPFR_array x) env
let array_resize (x:earray) length = Lingen0.array_resize x.Common.val0 length
let array_minimize (x:earray) = Lingen0.array_minimize x.Common.val0

let array_set (x:earray) (y:earray) intern = x.Common.env <- y.Common.env; Lingen0.array_set x.Common.val0 y.Common.val0 intern
let array_set_lingen0_array (x:earray) (y:Lingen0.earray) intern = Lingen0.array_set  x.Common.val0 y intern
let array_set_lingenD_array (x:earray) y intern = Lingen0.array_set_lingenD_array x.Common.val0 y intern
let array_set_lingenMPQ_array (x:earray) y intern = Lingen0.array_set_lingenMPQ_array x.Common.val0 y intern
let array_set_lingenMPFR_array (x:earray) y intern = Lingen0.array_set_lingenMPFR_array x.Common.val0 y intern
let array_get_lingenD_array x (y:earray) intern = Lingen0.array_get_lingenD_array x y.Common.val0 intern
let array_get_lingenMPQ_array x (y:earray) intern = Lingen0.array_get_lingenMPQ_array x y.Common.val0 intern
let array_get_lingenMPFR_array x (y:earray) intern = Lingen0.array_get_lingenMPFR_array x y.Common.val0 intern
let array_size x = Lingen0.array_size x.Common.val0

let array_set_index_lingenD (x:earray) i a intern = Lingen0.array_set_index_lingenD x.Common.val0 i a intern
let array_set_index_lingenMPQ (x:earray) i a intern = Lingen0.array_set_index_lingenMPQ x.Common.val0 i a intern
let array_set_index_lingenMPFR (x:earray) i a intern = Lingen0.array_set_index_lingenMPFR x.Common.val0 i a intern
let array_get_index_lingenD a (x:earray) i intern = Lingen0.array_get_index_lingenD a x.Common.val0 i intern
let array_get_index_lingenMPQ a (x:earray) i intern = Lingen0.array_get_index_lingenMPQ a x.Common.val0 i intern
let array_get_index_lingenMPFR a (x:earray) i intern = Lingen0.array_get_index_lingenMPFR a x.Common.val0 i intern
let array_set_index (x:earray) i a intern =
  if (x.Common.env<>a.Common.env) then raise (Invalid_argument ("Apron.Lingen1.array_set_index: the environments of the arguments should be the same"));
  Lingen0.array_set_index x.Common.val0 i a.Common.val0 intern
let array_get_index a (x:earray) i intern =
  a.Common.env <- x.Common.env;
  Lingen0.array_get_index a.Common.val0 x.Common.val0 i intern
let array_extend_environment res expr newenv =
  res.Common.env <- newenv;
  Lingen0.array_extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env

