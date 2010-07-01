(* File generated from scalar.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Float of float | Mpqf of Mpqf.t | Mpfrf of Mpfrf.t



(** APRON Scalar numbers. See {!Mpqf} for operations on GMP multiprecision rational numbers and {!Mpfr} for operations on MPFR multi-precision floating-point numbers. *)


let of_mpq x = Mpqf (Mpqf.of_mpq x)
let of_mpqf x = Mpqf x
let of_mpfr x = Mpfrf (Mpfrf.of_mpfr x)
let of_mpfrf x = Mpfrf x
let of_int x = Mpqf(Mpqf.of_int x)
let of_frac x y = Mpqf(Mpqf.of_frac x y)
let of_float x = Float(x)
let of_infty s = 
if s>0 then Float(Pervasives.infinity)
else if s<0 then Float(Pervasives.neg_infinity)
else Float(0.0)
let is_infty scalar =
match scalar with
| Mpqf x ->
let z = Mpqf.get_den x in
if Mpzf.sgn z <> 0 then 0
else begin
let z = Mpqf.get_num x in
let sgn = Mpzf.sgn z in
if sgn > 0 then 1 else if sgn < 0 then -1 else 0
end
| Mpfrf x ->
if Mpfrf.inf_p x then
if Mpfrf.sgn x > 0 then 1 else -1
else 0
| Float x ->
if x = Pervasives.infinity then 1
else if x = Pervasives.neg_infinity then -1
else 0
let sgn scalar =
match scalar with
| Mpqf x -> Mpqf.sgn x
| Mpfrf x -> Mpfrf.sgn x
| Float x -> if x > 0.0+.0.0 then 1 else if x < -.0.0 then -1 else 0
let to_mpqf = function
| Mpqf x -> x
| Mpfrf x -> Mpfrf.to_mpqf x
| Float x -> Mpqf.of_float x
let cmp c1 c2 =
let s1 = is_infty c1 in
let s2 = is_infty c2 in
if s1>s2 then 1
else if s1<s2 then -1
else if s1 != 0 then 0
else begin
match (c1,c2) with
| (Mpqf x1, Mpqf x2) -> Mpqf.cmp x1 x2
| (Mpfrf x1, Mpfrf x2) -> Mpfrf.cmp x1 x2
| (Float x1, Float x2) -> if x1>x2 then 1 else if x1<x2 then -1 else 0
| x1, x2 -> Mpqf.cmp (to_mpqf x1) (to_mpqf x2)
end
let equal c1 c2 =
let s1 = is_infty c1 in
let s2 = is_infty c2 in
if s1!=s2 then false
else if s1!=0 then true
else begin
match (c1,c2) with
| (Mpqf x1, Mpqf x2) -> Mpqf.equal x1 x2
| (Mpfrf x1, Mpfrf x2) -> Mpfrf.cmp x1 x2 = 0
| (Float x1, Float x2) -> x1 = x2
| x1, x2 -> Mpqf.equal (to_mpqf x1) (to_mpqf x2)
end
let cmp_int scalar n =
match scalar with
| Mpqf x -> Mpqf.cmp_int x n
| Mpfrf x -> Mpfrf.cmp_int x n
| Float x -> Pervasives.compare x (float_of_int n)
let equal_int scalar n =
match scalar with
| Mpqf x -> (Mpqf.cmp_int x n)=0
| Mpfrf x -> (Mpfrf.cmp_int x n)=0
| Float x -> x=(float_of_int n)
let neg scalar =
match scalar with
| Mpqf x -> Mpqf(Mpqf.neg x)
| Mpfrf x ->
let y = Mpfr.init2 (Mpfr.get_prec x) in
ignore (Mpfr.neg y x Mpfr.Up);
Mpfrf(Mpfrf.mpfrf y)
	     | Float x -> Float(-. x)
let to_string scalar =
match scalar with
| Mpqf x -> Mpqf.to_string x
| Mpfrf x -> Mpfrf.to_string x
| Float x -> string_of_float x
let print fmt scalar =
Format.pp_print_string fmt (to_string scalar)

