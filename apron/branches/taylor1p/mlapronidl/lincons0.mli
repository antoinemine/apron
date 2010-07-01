(* File generated from lincons0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable linexpr0 : Linexpr0.t;
  mutable typ : typ;
}


(** APRON Linear constraints of level 0 *)
and typ =
  | EQ
  | SUPEQ
  | SUP
  | DISEQ
  | EQMOD of Scalar.t


(** Make a linear constraint. Modifying later the linear expression
modifies correspondingly the linear constraint and conversely *)
val make : Linexpr0.t -> typ -> t

(** Copy a linear constraint (deep copy) *)
val copy : t -> t

(** Convert a constraint type to a string ([=],[>=], or [>]) *)
val string_of_typ : typ -> string

(** Print a constraint *)
val print : (Dim.t -> string) -> Format.formatter -> t -> unit

