(* File generated from coeff.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type union_5 =
  | Scalar of Scalar.t
  | Interval of Interval.t
and t = union_5

(** APRON Coefficients (either scalars or intervals) *)

let s_of_mpq x = Scalar (Scalar.of_mpq x)
let s_of_mpqf x = Scalar (Scalar.of_mpqf x)
let s_of_int x = Scalar (Scalar.of_int x)
let s_of_frac x y = Scalar (Scalar.of_frac x y)
let s_of_float x = Scalar (Scalar.of_float x)
let s_of_mpfr x = Scalar (Scalar.of_mpfr x)

let i_of_scalar inf sup = Interval (Interval.of_scalar inf sup)
let i_of_mpq x y = Interval (Interval.of_mpq x y)
let i_of_mpqf x y = Interval (Interval.of_mpqf x y)
let i_of_int x y = Interval (Interval.of_int x y)
let i_of_frac x y z w = Interval (Interval.of_frac x y z w)
let i_of_float x y = Interval (Interval.of_float x y)
let i_of_mpfr x y = Interval (Interval.of_mpfr x y)

let is_scalar c = match c with Scalar _ -> true | _ -> false
let is_interval c = match c with Interval _ -> true | _ -> false
let cmp c1 c2 = match (c1,c2) with
| (Scalar s1, Scalar s2) -> Scalar.cmp s1 s2
| (Interval i1, Interval i2) -> Interval.cmp i1 i2
| (Scalar _, _) -> (-3)
| _ -> 3
let equal c1 c2 = match (c1,c2) with
| (Scalar s1, Scalar s2) -> Scalar.equal s1 s2
| (Interval i1, Interval i2) -> Interval.equal i1 i2
| _ -> false
let is_zero c = match c with
| Scalar s -> (Scalar.sgn s)=0
| Interval i -> Interval.is_zero i
let equal_int c b = match c with
| Scalar s -> Scalar.equal_int s b
| Interval i -> Interval.equal_int i b
let neg c = match c with
| Scalar s -> Scalar (Scalar.neg s)
| Interval i -> Interval (Interval.neg i)
let reduce c = match c with
| Scalar _ -> c
| Interval i ->
if Scalar.equal i.Interval.inf i.Interval.sup
then Scalar i.Interval.inf
else c
let print fmt c = match c with
| Scalar s -> Scalar.print fmt s
| Interval i -> Interval.print fmt i

