(* File generated from interval.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable inf: Scalar.t;
  mutable sup: Scalar.t;
}



(** APRON Intervals on scalars *)



let of_scalar inf sup = { inf = inf; sup = sup }
let of_infsup = of_scalar
let of_mpq x y = { 
inf = Scalar.of_mpq x;
sup = Scalar.of_mpq y
}
let of_mpqf x y = {
inf = Scalar.of_mpqf x;
sup = Scalar.of_mpqf y
}
let of_int x y = {
inf = Scalar.of_int x;
sup = Scalar.of_int y
}
let of_frac x y z w = {
inf = Scalar.of_frac x y;
sup = Scalar.of_frac z w
}
let of_float x y = {
inf = Scalar.of_float x;
sup = Scalar.of_float y
}
let of_mpfr x y = {
inf = Scalar.of_mpfr x;
sup = Scalar.of_mpfr y
}
let is_top itv =
Scalar.is_infty itv.inf < 0 && Scalar.is_infty itv.sup > 0
let is_bottom itv =
Scalar.cmp itv.inf itv.sup > 0
let is_leq itv1 itv2 =
Scalar.cmp itv1.inf itv2.inf >= 0 &&
Scalar.cmp itv1.sup itv2.sup <= 0
let cmp itv1 itv2 =
let s1 = Scalar.cmp itv1.inf itv2.inf in
let s2 = Scalar.cmp itv1.sup itv2.sup in
if s1=0 && s2=0 then 0
else if s1>=0 && s2<=0 then -1
else if s1<=0 && s2>=0 then 1
else if s1<=0 then -2
else 2
let equal itv1 itv2 =
Scalar.equal itv1.inf itv2.inf &&
Scalar.equal itv1.sup itv2.sup 
let is_zero itv =
Scalar.sgn itv.inf=0 && Scalar.sgn itv.sup = 0
let equal_int itv b = 
Scalar.equal_int itv.inf b && Scalar.equal_int itv.sup b
let neg itv = 
{ inf = Scalar.neg itv.sup; sup = Scalar.neg itv.inf }
let top = { inf = Scalar.Float neg_infinity; sup = Scalar.Float infinity }
let bottom = { inf = Scalar.Float infinity; sup = Scalar.Float neg_infinity }
let set_infsup itv inf sup =
itv.inf <- inf;
itv.sup <- sup
let set_top itv =
itv.inf <- Scalar.Float neg_infinity;
itv.sup <- Scalar.Float infinity
let set_bottom itv =
itv.inf <- Scalar.Float infinity;
itv.sup <- Scalar.Float neg_infinity
let print fmt itv =
Format.fprintf fmt "[@[<hv>%a;@ %a@]]"
Scalar.print itv.inf Scalar.print itv.sup

