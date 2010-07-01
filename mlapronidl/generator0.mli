(* File generated from generator0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type typ =
  | LINE
  | RAY
  | VERTEX
  | LINEMOD
  | RAYMOD
and t = {
  mutable linexpr0: Linexpr0.t;
  mutable typ: typ;
}


(** APRON Generators of level 0 *)

(** Making a generator. The constant coefficient of the linear expression is
ignored. Modifying later the linear expression modifies correspondingly the
generator and conversely. *)
val make : Linexpr0.t -> typ -> t

(** Copy a generator (deep copy) *)
val copy : t -> t

(** Convert a generator type to a string ([LIN],[RAY], or [VTX]) *)
val string_of_typ : typ -> string

(** Print a generator *)
val print : (Dim.t -> string) -> Format.formatter -> t -> unit

