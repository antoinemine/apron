(* File generated from abstract1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type 'a t = {
  mutable abstract0: 'a Abstract0.t;
  mutable env: Environment.t;
}

(** APRON Abstract values of level 1 *)
(** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)

type box1 = { mutable interval_array : Interval.t array; mutable box1_env : Environment.t }
(* ********************************************************************** *)
(** {2 General management} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Memory} *)
(* ============================================================ *)

(** Copy a value *)
val copy : 'a Manager.t -> 'a t -> 'a t

(** Return the abstract size of a value *)
val size : 'a Manager.t -> 'a t -> int

(* ============================================================ *)
(** {3 Control of internal representation} *)
(* ============================================================ *)

(** Minimize the size of the representation of the value. This may result in a later recomputation of internal information.*)
val minimize : 'a Manager.t -> 'a t -> unit

(** Put the abstract value in canonical form. (not yet clear definition) *)
val canonicalize : 'a Manager.t -> 'a t -> unit

val hash : 'a Manager.t -> 'a t -> int

(** [approximate man abs alg] perform some transformation on the abstract value, guided by the argument [alg]. The transformation may lose information.  The argument [alg] overrides the field algorithm of the structure of type [Manager.funopt] associated to ap_abstract0_approximate (commodity feature).*)
val approximate : 'a Manager.t -> 'a t -> int -> unit


(* ============================================================ *)
(** {3 Printing} *)
(* ============================================================ *)

(** Dump on the [stdout] C stream the internal representation of an abstract value, for debugging purposes *)
external fdump : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_fdump"


(** Print as a set of constraints *)
val print: Format.formatter -> 'a t -> unit
(* ============================================================ *)
(** {3 Serialization} *)
(* ============================================================ *)
(* ********************************************************************** *)
(** {2 Constructor, accessors, tests and property extraction} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Basic constructors} *)

(* ============================================================ *)
(** All these functions request explicitly an environment in their arguments. *)


(** Create a bottom (empty) value defined on the given environment *)
external bottom : 'a Manager.t -> Environment.t -> 'a t
	= "camlidl_abstract1_ap_abstract1_bottom"


(** Create a top (universe) value defined on the given environment *)
external top : 'a Manager.t -> Environment.t -> 'a t
	= "camlidl_abstract1_ap_abstract1_top"


(** Abstract an hypercube.

[of_box man env tvar tinterval] abstracts an hypercube defined by the arrays [tvar] and [tinterval]. The result is defined on the environment [env], which should contain all the variables in [tvar] (and defines their type) *)
external of_box : 'a Manager.t -> Environment.t -> Var.t array -> Interval.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_of_box"

(* ============================================================ *)
(** {3 Accessors} *)

(* ============================================================ *)

val manager : 'a t -> 'a Manager.t
val env : 'a t -> Environment.t
val abstract0 : 'a t -> 'a Abstract0.t
(** Return resp. the underlying manager, environment and abstract value of level 0 *)

(* ============================================================ *)
(** {3 Tests} *)

(* ============================================================ *)

(** Emptiness test *)
val is_bottom : 'a Manager.t -> 'a t -> bool

(** Universality test *)
val is_top : 'a Manager.t -> 'a t -> bool


(** Inclusion test. The 2 abstract values should be compatible. *)
external is_leq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract1_ap_abstract1_is_leq"


(** Equality test. The 2 abstract values should be compatible. *)
external is_eq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract1_ap_abstract1_is_eq"


(** Does the abstract value satisfy the linear constraint ? *)
external sat_lincons : 'a Manager.t -> 'a t -> Lincons1.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_lincons"


(** Does the abstract value satisfy the tree expression constraint ? *)
external sat_tcons : 'a Manager.t -> 'a t -> Tcons1.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_tcons"


(** Does the abstract value satisfy the constraint [dim in interval] ? *)
external sat_interval : 'a Manager.t -> 'a t -> Var.t -> Interval.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_interval"


(** Is the variable unconstrained in the abstract value ? If yes, this means that the existential quantification of the dimension does not change the value. *)
external is_variable_unconstrained : 'a Manager.t -> 'a t -> Var.t -> bool
	= "camlidl_abstract1_ap_abstract1_is_variable_unconstrained"

(* ============================================================ *)
(** {3 Extraction of properties} *)

(* ============================================================ *)

(** Return the interval of variation of the variable in the abstract value. *)
external bound_variable : 'a Manager.t -> 'a t -> Var.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_variable"


(** Return the interval of variation of the linear expression in the abstract value. 

Implement a form of linear programming, where the argument linear expression is the one to optimize under the constraints induced by the abstract value. *)
external bound_linexpr : 'a Manager.t -> 'a t -> Linexpr1.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_linexpr"


(** Return the interval of variation of the tree expression in the abstract value. *)
external bound_texpr : 'a Manager.t -> 'a t -> Texpr1.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_texpr"


(** Convert the abstract value to an hypercube *)
val to_box : 'a Manager.t -> 'a t -> box1

(** Convert the abstract value to a conjunction of linear constraints. *)
val to_lincons_array : 'a Manager.t -> 'a t -> Lincons1.earray
(** Convert the abstract value to a conjunction of tree expressions constraints. *)
val to_tcons_array : 'a Manager.t -> 'a t -> Tcons1.earray

(** Convert the abstract value to a set of generators that defines it. *)
val to_generator_array : 'a Manager.t -> 'a t -> Generator1.earray


(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Meet and Join} *)

(* ============================================================ *)
(** Meet of 2 abstract values. *)
external meet : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet"

(** Meet of a non empty array of abstract values. *)
external meet_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_array"

(** Meet of an abstract value with an array of linear constraints. *)
external meet_lincons_array : 'a Manager.t -> 'a t -> Lincons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_lincons_array"

(** Meet of an abstract value with an array of tree expressions constraints. *)
external meet_tcons_array : 'a Manager.t -> 'a t -> Tcons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_tcons_array"

(** Join of 2 abstract values. *)
external join : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_join"

(** Join of a non empty array of abstract values. *)
external join_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_join_array"

(** Add the array of generators to the abstract value (time elapse operator).

 The generators should either lines or rays, not vertices. *)
external add_ray_array : 'a Manager.t -> 'a t -> Generator1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_add_ray_array"


(** {5 Side-effect versions of the previous functions} *)

external meet_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_meet_with"

external meet_lincons_array_with : 'a Manager.t -> 'a t -> Lincons1.earray -> unit
	= "camlidl_abstract1_ap_abstract1_meet_lincons_array_with"

external meet_tcons_array_with : 'a Manager.t -> 'a t -> Tcons1.earray -> unit
	= "camlidl_abstract1_ap_abstract1_meet_tcons_array_with"

external join_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_join_with"

external add_ray_array_with : 'a Manager.t -> 'a t -> Generator1.earray -> unit
	= "camlidl_abstract1_ap_abstract1_add_ray_array_with"

(* ============================================================ *)
(** {3 Assignement and Substitutions} *)

(* ============================================================ *)
(** Parallel assignement of an array of dimensions by an array of same size of linear expressions *)
external assign_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_assign_linexpr_array"

(** Parallel substitution of an array of dimensions by an array of same size of linear expressions *)
external substitute_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_substitute_linexpr_array"

(** Parallel assignement of an array of dimensions by an array of same size of tree expressions *)
external assign_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_assign_texpr_array"

(** Parallel substitution of an array of dimensions by an array of same size of tree expressions *)
external substitute_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_substitute_texpr_array"


(** {5 Side-effect versions of the previous functions} *)

external assign_linexpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> unit
	= "camlidl_abstract1_ap_abstract1_assign_linexpr_array_with"

external substitute_linexpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> unit
	= "camlidl_abstract1_ap_abstract1_substitute_linexpr_array_with"

external assign_texpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> unit
	= "camlidl_abstract1_ap_abstract1_assign_texpr_array_with"

external substitute_texpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> unit
	= "camlidl_abstract1_ap_abstract1_substitute_texpr_array_with"

(* ============================================================ *)
(** {3 Projections} *)

(* ============================================================ *)
(** These functions implements forgeting (existential quantification) of (array of) variables. Both functional and side-effect versions are provided. The Boolean, if true, adds a projection onto 0-plane. *)


external forget_array : 'a Manager.t -> 'a t -> Var.t array -> bool -> 'a t
	= "camlidl_abstract1_ap_abstract1_forget_array"

external forget_array_with : 'a Manager.t -> 'a t -> Var.t array -> bool -> unit
	= "camlidl_abstract1_ap_abstract1_forget_array_with"

(* ============================================================ *)
(** {3 Change and permutation of dimensions} *)

(* ============================================================ *)
(** Change the environement of the abstract values.

Variables that are removed are first existentially quantified, and variables that are introduced are unconstrained. The Boolean, if true, adds a projection onto 0-plane for these ones. *)
external change_environment : 'a Manager.t -> 'a t -> Environment.t -> bool -> 'a t
	= "camlidl_abstract1_ap_abstract1_change_environment"

(** Remove from the environment of the abstract value and from the abstract value itself variables that are unconstrained in it. *)
external minimize_environment : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_minimize_environment"

(** Parallel renaming of the environment of the abstract value.

The new variables should not interfere with the variables that are not renamed. *)
external rename_array : 'a Manager.t -> 'a t -> Var.t array -> Var.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_rename_array"

external change_environment_with : 'a Manager.t -> 'a t -> Environment.t -> bool -> unit
	= "camlidl_abstract1_ap_abstract1_change_environment_with"

external minimize_environment_with : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_minimize_environment_with"

external rename_array_with : 'a Manager.t -> 'a t -> Var.t array -> Var.t array -> unit
	= "camlidl_abstract1_ap_abstract1_rename_array_with"

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


(** Expansion: [expand a var tvar] expands the variable [var] into itself and
	the additional variables in [tvar], which are given the same type as [var].

It results in (n+1) unrelated variables having
			same relations with other variables. The additional variables are added to the environment of 
the argument for making the environment of the result, so they should
not belong to the initial environement.
*)
external expand : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_expand"

 (** Folding: [fold a tvar] fold the variables in the array [tvar] of size n>=1
and put the result in the first variable of the array. The other
variables of the array are then removed, both from the environment and the abstract value.
*)
external fold : 'a Manager.t -> 'a t -> Var.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_fold"

external expand_with : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> unit
	= "camlidl_abstract1_ap_abstract1_expand_with"

external fold_with : 'a Manager.t -> 'a t -> Var.t array -> unit
	= "camlidl_abstract1_ap_abstract1_fold_with"

(* ============================================================ *)
(** {3 Widening} *)

(* ============================================================ *)
(** Widening *)
external widening : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_widening"

external widening_threshold : 'a Manager.t -> 'a t -> 'a t -> Lincons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_widening_threshold"

(* ============================================================ *)
(** {3 Closure operation} *)

(* ============================================================ *)
(** Closure: transform strict constraints into non-strict ones.*)
external closure : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_closure"

(** Side-effect version *)
external closure_with : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_closure_with"

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

val of_lincons_array : 'a Manager.t -> Environment.t -> Lincons1.earray -> 'a t
val of_tcons_array   : 'a Manager.t -> Environment.t -> Tcons1.earray   -> 'a t
(** Abstract a conjunction of constraints *)

val assign_linexpr     : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> 'a t
val substitute_linexpr : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> 'a t
val assign_texpr       : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t   -> 'a t option -> 'a t
val substitute_texpr   : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t   -> 'a t option -> 'a t
(** Assignement/Substitution of a single dimension by a single expression *)

val assign_linexpr_with     : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> unit
val substitute_linexpr_with : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> unit
val assign_texpr_with       : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t   -> 'a t option -> unit
val substitute_texpr_with   : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t   -> 'a t option -> unit
(** Side-effect version of the previous functions *)


(** Unification of 2 abstract values on their least common environment *)
external unify : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_unify"

(** Side-effect version *)
external unify_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_unify_with"

