(* File generated from abstract0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type 'a t

(** APRON Abstract value of level 0 *)
(** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)


(** TO BE DOCUMENTED *)
external set_gc : int -> unit
	= "camlidl_abstract0_ap_abstract0_set_gc"

(* ********************************************************************** *)
(** {2 General management} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Memory} *)
(* ============================================================ *)

(** Copy a value *)
external copy : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_copy"


(** Return the abstract size of a value *)
external size : 'a Manager.t -> 'a t -> int
	= "camlidl_abstract0_ap_abstract0_size"

(* ============================================================ *)
(** {3 Control of internal representation} *)
(* ============================================================ *)

(** Minimize the size of the representation of the value. This may result in a later recomputation of internal information.*)
external minimize : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_minimize"


(** Put the abstract value in canonical form. (not yet clear definition) *)
external canonicalize : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_canonicalize"

external hash : 'a Manager.t -> 'a t -> int
	= "camlidl_abstract0_ap_abstract0_hash"


(** [approximate man abs alg] perform some transformation on the abstract value, guided by the argument [alg]. The transformation may lose information.  The argument [alg] overrides the field algorithm of the structure of type [Manager.funopt] associated to ap_abstract0_approximate (commodity feature).*)
external approximate : 'a Manager.t -> 'a t -> int -> unit
	= "camlidl_abstract0_ap_abstract0_approximate"

(* ============================================================ *)
(** {3 Printing} *)
(* ============================================================ *)

(** Dump on the [stdout] C stream the internal representation of an abstract value, for debugging purposes *)
external fdump : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_fdump"


(** Print as a set of constraints *)
val print: (int -> string) -> Format.formatter -> 'a t -> unit
(* ============================================================ *)
(** {3 Serialization} *)
(* ============================================================ *)
(* ********************************************************************** *)
(** {2 Constructor, accessors, tests and property extraction} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Basic constructors} *)
(* ============================================================ *)

(** Create a bottom (empty) value with the given number of integer and real variables *)
external bottom : 'a Manager.t -> int -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_bottom"


(** Create a top (universe) value with the given number of integer and real variables *)
external top : 'a Manager.t -> int -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_top"


(** Abstract an hypercube.

[of_box man intdim realdim array] abstracts an hypercube defined by the array of intervals of size [intdim+realdim] *)
external of_box : 'a Manager.t -> int -> int -> Interval.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_of_box"

(* ============================================================ *)
(** {3 Accessors} *)
(* ============================================================ *)


external dimension : 'a Manager.t -> 'a t -> Dim.dimension
	= "camlidl_abstract0_ap_abstract0_dimension"

external manager : 'a t -> 'a Manager.t
	= "camlidl_abstract0_ap_abstract0_manager"

(* ============================================================ *)
(** {3 Tests} *)
(* ============================================================ *)

(** Emptiness test *)
external is_bottom : 'a Manager.t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_bottom"


(** Universality test *)
external is_top : 'a Manager.t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_top"


(** Inclusion test. The 2 abstract values should be compatible. *)
external is_leq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_leq"


(** Equality test. The 2 abstract values should be compatible. *)
external is_eq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_eq"


(** Does the abstract value satisfy the linear constraint ? *)
external sat_lincons : 'a Manager.t -> 'a t -> Lincons0.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_lincons"


(** Does the abstract value satisfy the tree expression constraint ? *)
external sat_tcons : 'a Manager.t -> 'a t -> Tcons0.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_tcons"


(** Does the abstract value satisfy the constraint [dim in interval] ? *)
external sat_interval : 'a Manager.t -> 'a t -> Dim.t -> Interval.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_interval"


(** Is the dimension unconstrained in the abstract value ? If yes, this means that the existential quantification of the dimension does not change the value. *)
external is_dimension_unconstrained : 'a Manager.t -> 'a t -> Dim.t -> bool
	= "camlidl_abstract0_ap_abstract0_is_dimension_unconstrained"

(* ============================================================ *)
(** {3 Extraction of properties} *)
(* ============================================================ *)

(** Return the interval of variation of the dimension in the abstract value. *)
external bound_dimension : 'a Manager.t -> 'a t -> Dim.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_dimension"


(** Return the interval of variation of the linear expression in the abstract value.

Implement a form of linear programming, where the argument linear expression is the one to optimize under the constraints induced by the abstract value. *)
external bound_linexpr : 'a Manager.t -> 'a t -> Linexpr0.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_linexpr"


(** Return the interval of variation of the tree expression in the abstract value. *)
external bound_texpr : 'a Manager.t -> 'a t -> Texpr0.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_texpr"


(** Convert the abstract value to an hypercube *)
external to_box : 'a Manager.t -> 'a t -> Interval.t array
	= "camlidl_abstract0_ap_abstract0_to_box"


(** Convert the abstract value to a conjunction of linear constraints. *)
external to_lincons_array : 'a Manager.t -> 'a t -> Lincons0.t array
	= "camlidl_abstract0_ap_abstract0_to_lincons_array"


(** Convert the abstract value to a conjunction of tree expression constraints. *)
external to_tcons_array : 'a Manager.t -> 'a t -> Tcons0.t array
	= "camlidl_abstract0_ap_abstract0_to_tcons_array"


(** Convert the abstract value to a set of generators that defines it. *)
external to_generator_array : 'a Manager.t -> 'a t -> Generator0.t array
	= "camlidl_abstract0_ap_abstract0_to_generator_array"

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Meet and Join} *)
(* ============================================================ *)


(** Meet of 2 abstract values. *)
external meet : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet"

(** Meet of a non empty array of abstract values. *)
external meet_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_array"

(** Meet of an abstract value with an array of linear constraints. *)
external meet_lincons_array : 'a Manager.t -> 'a t -> Lincons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_lincons_array"

(** Meet of an abstract value with an array of tree expression constraints. *)
external meet_tcons_array : 'a Manager.t -> 'a t -> Tcons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_tcons_array"

(** Join of 2 abstract values. *)
external join : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_join"

(** Join of a non empty array of abstract values. *)
external join_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_join_array"

(** Add the array of generators to the abstract value (time elapse operator).

 The generators should either lines or rays, not vertices. *)
external add_ray_array : 'a Manager.t -> 'a t -> Generator0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_add_ray_array"


(** {5 Side-effect versions of the previous functions} *)

external meet_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_meet_with"

external meet_lincons_array_with : 'a Manager.t -> 'a t -> Lincons0.t array -> unit
	= "camlidl_abstract0_ap_abstract0_meet_lincons_array_with"

external meet_tcons_array_with : 'a Manager.t -> 'a t -> Tcons0.t array -> unit
	= "camlidl_abstract0_ap_abstract0_meet_tcons_array_with"

external join_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_join_with"

external add_ray_array_with : 'a Manager.t -> 'a t -> Generator0.t array -> unit
	= "camlidl_abstract0_ap_abstract0_add_ray_array_with"

(* ============================================================ *)
(** {3 Assignements and Substitutions} *)
(* ============================================================ *)
(** Parallel assignement of an array of dimensions by an array of same size of linear expressions *)
external assign_linexpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_assign_linexpr_array"

(** Parallel substitution of an array of dimensions by an array of same size of linear expressions *)
external substitute_linexpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_substitute_linexpr_array"

(** Parallel assignement of an array of dimensions by an array of same size of tree expressions *)
external assign_texpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_assign_texpr_array"

(** Parallel substitution of an array of dimensions by an array of same size of tree expressions *)
external substitute_texpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_substitute_texpr_array"


(** {5 Side-effect versions of the previous functions} *)


external assign_linexpr_array_with : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> unit
	= "camlidl_abstract0_ap_abstract0_assign_linexpr_array_with"

external substitute_linexpr_array_with : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> unit
	= "camlidl_abstract0_ap_abstract0_substitute_linexpr_array_with"

external assign_texpr_array_with : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> unit
	= "camlidl_abstract0_ap_abstract0_assign_texpr_array_with"

external substitute_texpr_array_with : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> unit
	= "camlidl_abstract0_ap_abstract0_substitute_texpr_array_with"




(* ============================================================ *)
(** {3 Projections} *)
(* ============================================================ *)
(** These functions implements forgeting (existential quantification) of (array of) dimensions. Both functional and side-effect versions are provided. The Boolean, if true, adds a projection onto 0-plane. *)


external forget_array : 'a Manager.t -> 'a t -> Dim.t array -> bool -> 'a t
	= "camlidl_abstract0_ap_abstract0_forget_array"

external forget_array_with : 'a Manager.t -> 'a t -> Dim.t array -> bool -> unit
	= "camlidl_abstract0_ap_abstract0_forget_array_with"

(* ============================================================ *)
(** {3 Change and permutation of dimensions} *)
(* ============================================================ *)


external add_dimensions : 'a Manager.t -> 'a t -> Dim.change -> bool -> 'a t
	= "camlidl_abstract0_ap_abstract0_add_dimensions"

external remove_dimensions : 'a Manager.t -> 'a t -> Dim.change -> 'a t
	= "camlidl_abstract0_ap_abstract0_remove_dimensions"

external apply_dimchange2 : 'a Manager.t -> 'a t -> Dim.change2 -> bool -> 'a t
	= "camlidl_abstract0_ap_abstract0_apply_dimchange2"

external permute_dimensions : 'a Manager.t -> 'a t -> Dim.perm -> 'a t
	= "camlidl_abstract0_ap_abstract0_permute_dimensions"


(** {5 Side-effect versions of the previous functions} *)


external add_dimensions_with : 'a Manager.t -> 'a t -> Dim.change -> bool -> unit
	= "camlidl_abstract0_ap_abstract0_add_dimensions_with"

external remove_dimensions_with : 'a Manager.t -> 'a t -> Dim.change -> unit
	= "camlidl_abstract0_ap_abstract0_remove_dimensions_with"

external apply_dimchange2_with : 'a Manager.t -> 'a t -> Dim.change2 -> bool -> unit
	= "camlidl_abstract0_ap_abstract0_apply_dimchange2_with"

external permute_dimensions_with : 'a Manager.t -> 'a t -> Dim.perm option -> unit
	= "camlidl_abstract0_ap_abstract0_permute_dimensions_with"




(* ============================================================ *)
(** {3 Expansion and folding of dimensions} *)
(* ============================================================ *)

(**
These functions allows to expand one dimension into several ones having the
same properties with respect to the other dimensions, and to fold several
dimensions into one. Formally,

- expand P(x,y,z) z w = P(x,y,z) inter P(x,y,w) if z is expanded in z and w
- fold Q(x,y,z,w) z w = exists w:Q(x,y,z,w) union (exist z:Q(x,y,z,w))(z<-w)
if z and w are folded onto z
*)


(** Expansion: [expand a dim n] expands the dimension [dim] into itself + [n]
additional dimensions.  It results in (n+1) unrelated dimensions having
same relations with other dimensions. The (n+1) dimensions are put as
follows: 

- original dimension [dim]
- if the dimension is integer, the n additional dimensions are put at the
end of integer dimensions; if it is real, at the end of the real
dimensions.
*)
external expand : 'a Manager.t -> 'a t -> Dim.t -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_expand"

 (** Folding: [fold a tdim] fold the dimensions in the array [tdim] of size n>=1
and put the result in the first dimension of the array. The other
dimensions of the array are then removed (using
ap_abstract0_permute_remove_dimensions).
*)
external fold : 'a Manager.t -> 'a t -> Dim.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_fold"

external expand_with : 'a Manager.t -> 'a t -> Dim.t -> int -> unit
	= "camlidl_abstract0_ap_abstract0_expand_with"

external fold_with : 'a Manager.t -> 'a t -> Dim.t array -> unit
	= "camlidl_abstract0_ap_abstract0_fold_with"

(* ============================================================ *)
(** {3 Widening} *)
(* ============================================================ *)
(** Widening *)
external widening : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_widening"

external widening_threshold : 'a Manager.t -> 'a t -> 'a t -> Lincons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_widening_threshold"

(* ============================================================ *)
(** {3 Closure operation} *)
(* ============================================================ *)
(** Closure: transform strict constraints into non-strict ones.*)
external closure : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_closure"

(** Side-effect version *)
external closure_with : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_closure_with"

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

val of_lincons_array : 'a Manager.t -> int -> int -> Lincons0.t array -> 'a t
val of_tcons_array   : 'a Manager.t -> int -> int -> Tcons0.t   array -> 'a t
(** Abstract a conjunction of constraints *)

val assign_linexpr     : 'a Manager.t -> 'a t -> Dim.t -> Linexpr0.t -> 'a t option -> 'a t
val substitute_linexpr : 'a Manager.t -> 'a t -> Dim.t -> Linexpr0.t -> 'a t option -> 'a t
val assign_texpr       : 'a Manager.t -> 'a t -> Dim.t -> Texpr0.t   -> 'a t option -> 'a t
val substitute_texpr   : 'a Manager.t -> 'a t -> Dim.t -> Texpr0.t   -> 'a t option -> 'a t
(** Assignement/Substitution of a single dimension by a single expression *)

val assign_linexpr_with     : 'a Manager.t -> 'a t -> Dim.t -> Linexpr0.t -> 'a t option -> unit
val substitute_linexpr_with : 'a Manager.t -> 'a t -> Dim.t -> Linexpr0.t -> 'a t option -> unit
val assign_texpr_with       : 'a Manager.t -> 'a t -> Dim.t -> Texpr0.t   -> 'a t option -> unit
val substitute_texpr_with   : 'a Manager.t -> 'a t -> Dim.t -> Texpr0.t   -> 'a t option -> unit
(** Side-effect version of the previous functions *)



(** General use *)
val print_array :
?first:(unit, Format.formatter, unit) format ->
?sep:(unit, Format.formatter, unit) format ->
?last:(unit, Format.formatter, unit) format ->
(Format.formatter -> 'a -> unit) -> Format.formatter -> 'a array -> unit

