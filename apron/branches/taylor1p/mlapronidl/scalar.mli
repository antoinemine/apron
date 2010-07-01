(* File generated from scalar.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Float of float | Mpqf of Mpqf.t | Mpfrf of Mpfrf.t



(** APRON Scalar numbers. See {!Mpqf} for operations on GMP multiprecision rational numbers and {!Mpfr} for operations on MPFR multi-precision floating-point numbers. *)



val of_mpq : Mpq.t -> t
val of_mpqf : Mpqf.t -> t
val of_int : int -> t
val of_frac : int -> int -> t
(** Create a scalar of type [Mpqf] from resp.
- A multi-precision rational [Mpq.t] 
- A multi-precision rational [Mpqf.t] 
- an integer 
- a fraction [x/y]
*)

val of_mpfr : Mpfr.t -> t
val of_mpfrf : Mpfrf.t -> t
(** Create a scalar of type [Mpfrf] with the given value *)
val of_float : float -> t
(** Create a scalar of type [Float] with the given value *)
val of_infty : int -> t 
(** Create a scalar of type [Float] with the value multiplied by
infinity (resulting in minus infinity, zero, or infinity 
*)
val is_infty : t -> int
(** Infinity test.
[is_infty x] returns [-1] if x is [-oo], [1] if x is [+oo], and [0] if [x] is
finite. *)

val sgn : t -> int
(** Return the sign of the coefficient, which may be a negative value, zero
or a positive value. *)

val cmp : t -> t -> int
(** Compare two coefficients, possibly converting to [Mpqf.t].
[compare x y] returns a negative number if [x] is less than [y], 
[0] if they ar equal, and a positive number if [x] is greater than [y].
*)

val cmp_int : t -> int -> int
(** Compare a coefficient with an integer *)

val equal : t -> t -> bool
(** Equality test, possibly using a conversion to [Mpqf.t].
Return [true] if the 2 values are equal. Two infinite values of the same
signs are considered as equal. *)

val equal_int : t -> int -> bool
(** Equality test with an integer *)

val neg : t -> t
(** Negation *)
val to_string : t -> string
(** Conversion to string, using [string_of_double], [Mpqf.to_string] or [Mpfr.to_string]
*)

val print : Format.formatter -> t -> unit
(** Print a coefficient *)

