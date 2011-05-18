(** APRON tree constraints and array of tree constraints of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = Tcons0.t Common.val1
type earray = Tcons0.t array Common.val1

(*  ********************************************************************** *)
(** {2 Expressions} *)
(*  ********************************************************************** *)

(** {3 Constructors and Destructor} *)

(** Make a tree expression constraint. Modifying later the linear expression ({e not
    advisable}) modifies correspondingly the tree expression constraint and conversely,
    except for changes of environements *)
val make: Texpr1.t -> Common.constyp -> t
val of_lincons1 : Lincons1.t -> t
val copy : t -> t
  (** Copy (deep copy) *)

val print : Format.formatter -> t -> unit
  (** Print the tree expression constraint *)
val get_typ: t -> Common.constyp
  (** Get the constraint type *)
val set_typ: t -> Common.constyp -> unit
  (** Set the constraint type *)
val get_mpq : Mpq.t -> t -> unit
val set_mpq : t -> 'a Mpq.tt -> unit

val get_texpr1: t -> Texpr1.t
  (** Get the underlying linear expression. Modifying the linear
      expression ({e not advisable}) modifies correspondingly the tree
      expression constraint and conversely, except for changes of
      environements *)

(** {3 Operations} *)
val extend_environment : t -> Environment.t -> t
  (** Change the environment of the expression for a super-environement. Raise [Failure] if it is not the case *)
val extend_environment_with : t -> Environment.t -> unit
  (** Side-effet version of the previous function *)


(* ====================================================================== *)
(** {2 Type array} *)
(* ====================================================================== *)

val array_make : Environment.t -> int -> earray
  (** Make an array of tree expression constraints with the given
      size and defined on the given environement. The elements are
      initialized with the constraint 0=0. *)
val array_of_lincons1_array : Lincons1.earray -> earray
val array_length : earray -> int
  (** Get the size of the array *)

val array_print :
  ?first:(unit, Format.formatter, unit) format ->
  ?sep:(unit, Format.formatter, unit) format ->
  ?last:(unit, Format.formatter, unit) format ->
  Format.formatter -> earray -> unit
  (** Print an array of constraints *)

(** {3 Access} *)
val array_get_index : earray -> int -> t
val array_set_index : earray -> int -> t -> unit

(** {3 Operations} *)
val array_extend_environment : earray -> Environment.t -> earray
val array_extend_environment_with : earray -> Environment.t -> unit
