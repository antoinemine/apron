(* File generated from generator1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable generator0: Generator0.t;
  mutable env: Environment.t;
}
and earray = {
  mutable generator0_array: Generator0.t array;
  mutable array_env: Environment.t;
}

(** APRON Generators and array of generators of level 1 *)

type typ = Generator0.typ = 
| LINE
| RAY
| VERTEX
| LINEMOD
| RAYMOD


(** Make a generator. Modifying later the linear expression ({e not
advisable}) modifies correspondingly the generator and conversely,
except for changes of environements *)
val make: Linexpr1.t -> Generator0.typ -> t

(** Copy (deep copy) *)
val copy: t -> t

(** Print the generator *)
val print : Format.formatter -> t -> unit

(** Get the generator type *)
val get_typ: t -> Generator0.typ

(** Iter the function on the pair coefficient/variable of the underlying linear
expression *)
val iter: (Coeff.t -> Var.t -> unit) -> t -> unit

(** Set the generator type *)
val set_typ: t -> Generator0.typ -> unit

(** Set simultaneously a number of coefficients.

[set_list expr [(c1,"x"); (c2,"y")]] assigns coefficients [c1] 
to variable ["x"] and coefficient [c2] to variable ["y"]. *)
val set_list : t -> (Coeff.t * Var.t) list -> unit

(** Set simultaneously a number of coefficients, as [set_list]. *)
val set_array : t -> (Coeff.t * Var.t) array -> unit


(** Get the coefficient of the variable in the underlying linear expression *)
external get_coeff : t -> Var.t -> Coeff.t
	= "camlidl_generator1_ap_generator1_get_coeff"

(** Set the coefficient of the variable in the underlying linear expression *)
external set_coeff : t -> Var.t -> Coeff.t -> unit
	= "camlidl_generator1_ap_generator1_set_coeff"

(** Change the environement of the generator for a super-environement. Raise [Failure] if it is not the case *)
external extend_environment : t -> Environment.t -> t
	= "camlidl_generator1_ap_generator1_extend_environment"

(** Side-effect version of the previous function *)
external extend_environment_with : t -> Environment.t -> unit
	= "camlidl_generator1_ap_generator1_extend_environment_with"

(* ====================================================================== *)
(** {2 Type earray} *)
(* ====================================================================== *)

(** Make an array of generators with the given size and defined on the
given environement. The elements are initialized with the line 0. *)
val array_make : Environment.t -> int -> earray

(** Print an array of generators *)
val array_print :
?first:(unit, Format.formatter, unit) format ->
?sep:(unit, Format.formatter, unit) format ->
?last:(unit, Format.formatter, unit) format ->
Format.formatter -> earray -> unit

(** Get the size of the array *)
val array_length : earray -> int

(** Get the element of the given index (which is not a copy) *)
val array_get : earray -> int -> t

(** Set the element of the given index (without any copy). The array and the
generator should be defined on the same environement; otherwise a [Failure]
exception is raised.*)
val array_set : earray -> int -> t -> unit

(** Change the environement of the array of generators for a super-environement. Raise [Failure] if it is not the case*)
external array_extend_environment : earray -> Environment.t -> earray
	= "camlidl_generator1_ap_generator1_array_extend_environment"

(** Side-effect version of the previous function *)
external array_extend_environment_with : earray -> Environment.t -> unit
	= "camlidl_generator1_ap_generator1_array_extend_environment_with"


(** Get the environement of the generator *)
val get_env: t -> Environment.t

(** Get the underlying linear expression. Modifying the linear expression ({e
not advisable}) modifies correspondingly the generator and
conversely, except for changes of environements *)
val get_linexpr1: t -> Linexpr1.t

(** Get the underlying generator of level 0. Modifying the generator
of level 0 ({e not advisable}) modifies correspondingly the generator
and conversely, except for changes of environements*)
val get_generator0: t -> Generator0.t

