#define _AP_linyyy1_MARK_
#if defined(_AP_linexpr1_MARK_)
(** (Interval) linear expressions *)
#elif defined(_AP_lincons1_MARK_)
(** (Interval) linear constraints *)
#elif defined(_AP_linexpr1_MARK_)
(** Linear generators *)
#endif

type t = Linyyy0.t Common.val1
type earray = Linyyy0.earray Common.val1

(*  ********************************************************************** *)
(** {2 Linear expressions/constraints/generators} *)
(*  ********************************************************************** *)

let print fmt (x:t) = Linyyy0.print (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let init ?size (discr:Common.discr) env : t = Common.make_val1 (Linyyy0.init ?size discr) env
let init_set (x:t) : t = Common.make_val1 (Linyyy0.init_set x.Common.val0) x.Common.env
let init_set_linyyyD x env : t = Common.make_val1 (Linyyy0.init_set_linyyyD x) env
let init_set_linyyyMPQ x env : t = Common.make_val1 (Linyyy0.init_set_linyyyMPQ x) env
let init_set_linyyyMPFR x env : t = Common.make_val1 (Linyyy0.init_set_linyyyMPFR x) env
let resize (x:t) length = Linyyy0.resize x.Common.val0 length
let minimize (x:t) = Linyyy0.minimize x.Common.val0

let set (x:t) (y:t) intern = x.Common.env <- y.Common.env; Linyyy0.set x.Common.val0 y.Common.val0 intern
let set_linyyy0 (x:t) y intern = Linyyy0.set x.Common.val0 y intern
let set_linyyyD (x:t) y intern = Linyyy0.set_linyyyD x.Common.val0 y intern
let set_linyyyMPQ (x:t) y intern = Linyyy0.set_linyyyMPQ x.Common.val0 y intern
let set_linyyyMPFR (x:t) y intern = Linyyy0.set_linyyyMPFR x.Common.val0 y intern
let get_linyyyD x (y:t) intern = Linyyy0.get_linyyyD x y.Common.val0 intern
let get_linyyyMPQ x (y:t) intern = Linyyy0.get_linyyyMPQ x y.Common.val0 intern
let get_linyyyMPFR x (y:t) intern = Linyyy0.get_linyyyMPFR x y.Common.val0 intern

let iter f x =
  match x.Common.val0 with
  | Common.D e -> LinyyyD.iter1 (fun eitvD var -> f (Common.D eitvD) var) e x.Common.env
  | Common.MPQ e -> LinyyyMPQ.iter1 (fun eitvMPQ var -> f (Common.MPQ eitvMPQ) var) e x.Common.env
  | Common.MPFR e -> LinyyyMPFR.iter1 (fun eitvMPFR var -> f (Common.MPFR eitvMPFR) var) e x.Common.env
let get_cst coeff x intern = Linyyy0.get_cst coeff x.Common.val0 intern
let get_coeff coeff x var intern =
  Linyyy0.get_coeff coeff x.Common.val0 (Environment.dim_of_var x.Common.env var) intern
let set_cst x coeff intern = Linyyy0.set_cst x.Common.val0 coeff intern
let set_coeff x var coeff intern =
  Linyyy0.set_coeff x.Common.val0 (Environment.dim_of_var x.Common.env var) coeff intern

#if defined (_AP_lincons1_MARK_) || defined (_AP_lingen1_MARK_)
let get_linexpr1 (x:Linexpr1.t) (y:t) intern =
  x.Common.env <- y.Common.env;
  Linyyy0.get_linexpr0 x.Common.val0 y.Common.val0 intern
let get_yyytyp (x:t) =
  Linyyy0.get_yyytyp x.Common.val0
let set_linexpr1 (x:t) (y:Linexpr1.t) intern =
  x.Common.env <- y.Common.env;
  Linyyy0.set_linexpr0 x.Common.val0 y.Common.val0 intern
let set_yyytyp (x:t) t =
  Linyyy0.set_yyytyp x.Common.val0 t
#endif
#if defined (_AP_lincons1_MARK_)
let get_mpq mpq x = Linyyy0.get_mpq mpq x.Common.val0
let set_mpq x mpq = Linyyy0.set_mpq x.Common.val0 mpq
#endif

let extend_environment res expr newenv =
  Linyyy0.extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env;
  res.Common.env <- newenv

(*  ********************************************************************** *)
(** {2 Arrays} *)
(*  ********************************************************************** *)

let array_print ?first ?sep ?last fmt (x:earray) = Linyyy0.array_print ?first ?sep ?last (Environment.string_of_dim x.Common.env) fmt x.Common.val0
let array_init (discr:Common.discr) length env : earray = Common.make_val1 (Linyyy0.array_init discr length) env
let array_init_set (x:earray) : earray = Common.make_val1 (Linyyy0.array_init_set x.Common.val0) x.Common.env
let array_init_set_linyyyD_array x env : earray = Common.make_val1 (Linyyy0.array_init_set_linyyyD_array x) env
let array_init_set_linyyyMPQ_array x env : earray = Common.make_val1 (Linyyy0.array_init_set_linyyyMPQ_array x) env
let array_init_set_linyyyMPFR_array x env : earray = Common.make_val1 (Linyyy0.array_init_set_linyyyMPFR_array x) env
let array_resize (x:earray) length = Linyyy0.array_resize x.Common.val0 length
let array_minimize (x:earray) = Linyyy0.array_minimize x.Common.val0
let array_length x = Linyyy0.array_length x.Common.val0

let array_set (x:earray) (y:earray) intern = x.Common.env <- y.Common.env; Linyyy0.array_set x.Common.val0 y.Common.val0 intern
let array_set_linyyy0_array (x:earray) (y:Linyyy0.earray) intern = Linyyy0.array_set  x.Common.val0 y intern
let array_set_linyyyD_array (x:earray) y intern = Linyyy0.array_set_linyyyD_array x.Common.val0 y intern
let array_set_linyyyMPQ_array (x:earray) y intern = Linyyy0.array_set_linyyyMPQ_array x.Common.val0 y intern
let array_set_linyyyMPFR_array (x:earray) y intern = Linyyy0.array_set_linyyyMPFR_array x.Common.val0 y intern
let array_get_linyyyD_array x (y:earray) intern = Linyyy0.array_get_linyyyD_array x y.Common.val0 intern
let array_get_linyyyMPQ_array x (y:earray) intern = Linyyy0.array_get_linyyyMPQ_array x y.Common.val0 intern
let array_get_linyyyMPFR_array x (y:earray) intern = Linyyy0.array_get_linyyyMPFR_array x y.Common.val0 intern

let array_set_index (x:earray) i a intern =
  if (x.Common.env<>a.Common.env) then raise (Invalid_argument "Apron.Linyyy1.array_set_index: the environments of the arguments should be the same");
  Linyyy0.array_set_index x.Common.val0 i a.Common.val0 intern
let array_get_index a (x:earray) i intern =
  a.Common.env <- x.Common.env;
  Linyyy0.array_get_index a.Common.val0 x.Common.val0 i intern
let array_set_index_linyyyD (x:earray) i a intern = Linyyy0.array_set_index_linyyyD x.Common.val0 i a intern
let array_set_index_linyyyMPQ (x:earray) i a intern = Linyyy0.array_set_index_linyyyMPQ x.Common.val0 i a intern
let array_set_index_linyyyMPFR (x:earray) i a intern = Linyyy0.array_set_index_linyyyMPFR x.Common.val0 i a intern
let array_get_index_linyyyD a (x:earray) i intern = Linyyy0.array_get_index_linyyyD a x.Common.val0 i intern
let array_get_index_linyyyMPQ a (x:earray) i intern = Linyyy0.array_get_index_linyyyMPQ a x.Common.val0 i intern
let array_get_index_linyyyMPFR a (x:earray) i intern = Linyyy0.array_get_index_linyyyMPFR a x.Common.val0 i intern
let array_extend_environment res expr newenv =
  res.Common.env <- newenv;
  Linyyy0.array_extend_environment res.Common.val0 ~newenv expr.Common.val0 ~oldenv:expr.Common.env
