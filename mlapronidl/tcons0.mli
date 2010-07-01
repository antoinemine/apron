(* File generated from tcons0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable texpr0 : Texpr0.t;
  mutable typ : Lincons0.typ;
}


(** APRON tree expressions constraints of level 0 *)

type typ = Lincons0.typ =
| EQ
| SUPEQ
| SUP
| DISEQ
| EQMOD of Scalar.t


(** Make a tree expression constraint. Modifying later the tree expression expression
modifies correspondingly the tree expression constraint and conversely *)
val make : Texpr0.t -> typ -> t

(** Copy a tree expression constraint (deep copy) *)
val copy : t -> t

(** Convert a constraint type to a string ([=],[>=], or [>]) *)
val string_of_typ : typ -> string

(** Print a constraint *)
val print : (Dim.t -> string) -> Format.formatter -> t -> unit

