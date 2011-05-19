(** APRON Abstract value of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type 'a t = 'a Abstract0.t Common.val1
    (** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)

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

(** Print as a set of constraints *)
val print: 
  ?first:(unit, Format.formatter, unit) format ->
  ?sep:(unit, Format.formatter, unit) format ->
  ?last:(unit, Format.formatter, unit) format ->
  Format.formatter -> 'a t -> unit

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
val bottom : 'a Manager.t -> Environment.t -> 'a t

(** Create a top (universe) value with the given number of integer and real variables *)
val top : 'a Manager.t -> Environment.t -> 'a t

(** Abstract an hypercube.
[of_box man intdim realdim array] abstracts an hypercube defined by the array of intervals of size [intdim+realdim] *)
val of_box : 'a Manager.t -> Environment.t -> Linexpr1.t -> 'a t
(* ============================================================ *)
(** {3 Accessors} *)
(* ============================================================ *)

val manager : 'a t -> 'a Manager.t

(* ============================================================ *)
(** {3 Tests} *)
(* ============================================================ *)

(** Emptiness test *)
val is_bottom : 'a Manager.t -> 'a t -> bool

(** Universality test *)
val is_top : 'a Manager.t -> 'a t -> bool

(** Inclusion test. The 2 abstract values should be compatible. *)
val is_leq : 'a Manager.t -> 'a t -> 'a t -> bool

(** Equality test. The 2 abstract values should be compatible. *)
val is_eq : 'a Manager.t -> 'a t -> 'a t -> bool

(** Does the abstract value satisfy the linear constraint ? *)
val sat_lincons : 'a Manager.t -> 'a t -> Lincons1.t -> bool

(** Does the abstract value satisfy the tree expression constraint ? *)
val sat_tcons : 'a Manager.t -> 'a t -> Tcons1.t -> bool

(** Does the abstract value satisfy the constraint [dim in interval] ? *)
val sat_interval : 'a Manager.t -> 'a t -> Var.t -> 'a Coeff.tt -> bool

(** Is the dimension unconstrained in the abstract value ? If yes, this means that the existential quantification of the dimension does not change the value. *)
val is_var_unconstrained : 'a Manager.t -> 'a t -> Var.t -> bool
(* ============================================================ *)
(** {3 Extraction of properties} *)
(* ============================================================ *)

(** Return the interval of variation of the dimension in the abstract value. *)
val bound_variable : 'a Manager.t -> Coeff.t -> 'a t -> Var.t -> unit

(** Return the interval of variation of the linear expression in the abstract value.

Implement a form of linear programming, where the argument linear expression is the one to optimize under the constraints induced by the abstract value. *)
val bound_linexpr : 'a Manager.t -> Coeff.t -> 'a t -> Linexpr1.t -> unit

(** Return the interval of variation of the tree expression in the abstract value. *)
val bound_texpr : 'a Manager.t -> Coeff.t -> 'a t -> Texpr1.t -> unit

(** Convert the abstract value to an hypercube *)
val to_box : 'a Manager.t -> Linexpr1.t -> 'a t -> unit

(** Convert the abstract value to a conjunction of linear constraints. *)
val to_lincons_array : 'a Manager.t -> Lincons1.earray -> 'a t -> unit

(** Convert the abstract value to a conjunction of tree expression constraints. *)
val to_tcons_array : 'a Manager.t -> 'a t -> Tcons1.earray

(** Convert the abstract value to a set of generators that defines it. *)
val to_lingen_array : 'a Manager.t -> Lingen1.earray -> 'a t -> unit

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Meet and Join} *)
(* ============================================================ *)


(** Meet of 2 abstract values. *)
val meet : 'a Manager.t -> 'a t -> 'a t -> 'a t
(** Meet of a non empty array of abstract values. *)
val meet_array : 'a Manager.t -> 'a t array -> 'a t
(** Meet of an abstract value with an array of linear constraints. *)
val meet_lincons_array : 'a Manager.t -> 'a t -> Lincons1.earray -> 'a t
(** Meet of an abstract value with an array of tree expression constraints. *)
val meet_tcons_array : 'a Manager.t -> 'a t -> Tcons1.earray -> 'a t
(** Join of 2 abstract values. *)
val join : 'a Manager.t -> 'a t -> 'a t -> 'a t
(** Join of a non empty array of abstract values. *)
val join_array : 'a Manager.t -> 'a t array -> 'a t
(** Add the array of generators to the abstract value (time elapse operator).

 The generators should either lines or rays, not vertices. *)
val add_ray_array : 'a Manager.t -> 'a t -> Lingen1.earray -> 'a t

(** {5 Side-effect versions of the previous functions} *)

val meet_with : 'a Manager.t -> 'a t -> 'a t -> unit
val meet_lincons_array_with : 'a Manager.t -> 'a t -> Lincons1.earray -> unit
val meet_tcons_array_with : 'a Manager.t -> 'a t -> Tcons1.earray -> unit
val join_with : 'a Manager.t -> 'a t -> 'a t -> unit
val add_ray_array_with : 'a Manager.t -> 'a t -> Lingen1.earray -> unit
(* ============================================================ *)
(** {3 Assignements and Substitutions} *)
(* ============================================================ *)
(** Parallel assignement of an array of dimensions by an array of same size of linear expressions *)
val assign_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.earray -> 'a t option -> 'a t
(** Parallel substitution of an array of dimensions by an array of same size of linear expressions *)
val substitute_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.earray -> 'a t option -> 'a t
(** Parallel assignement of an array of dimensions by an array of same size of tree expressions *)
val assign_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.earray -> 'a t option -> 'a t
(** Parallel substitution of an array of dimensions by an array of same size of tree expressions *)
val substitute_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.earray -> 'a t option -> 'a t

(** {5 Side-effect versions of the previous functions} *)
val assign_linexpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.earray -> 'a t option -> unit
val substitute_linexpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.earray -> 'a t option -> unit
val assign_texpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.earray -> 'a t option -> unit
val substitute_texpr_array_with : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.earray -> 'a t option -> unit



(* ============================================================ *)
(** {3 Projections} *)
(* ============================================================ *)
(** These functions implements forgeting (existential quantification) of (array of) dimensions. Both functional and side-effect versions are provided. The Boolean, if true, adds a projection onto 0-plane. *)
val forget_array : 'a Manager.t -> 'a t -> Var.t array -> bool -> 'a t
val forget_array_with : 'a Manager.t -> 'a t -> Var.t array -> bool -> unit

(* ============================================================ *)
(** {3 Change of environments} *)
(* ============================================================ *)

val change_environment : 'a Manager.t -> 'a t -> Environment.t -> bool -> 'a t
val minimize_environment : 'a Manager.t -> 'a t -> 'a t
val rename_array : 'a Manager.t -> 'a t -> Var.t array -> Var.t array -> 'a t

(** {5 Side-effect versions of the previous functions} *)
val change_environment_with :
  'a Manager.t -> 'a t -> Environment.t -> bool -> unit
val minimize_environment_with : 'a Manager.t -> 'a t -> unit
val rename_array_with :
  'a Manager.t -> 'a t -> Var.t array -> Var.t array -> unit

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


(**     Expansion: [expand a dim n] expands the dimension [dim] into itself + [n]
    additional dimensions.  It results in (n+1) unrelated dimensions having
    same relations with other dimensions. The (n+1) dimensions are put as
    follows:

    - original dimension [dim]
    - if the dimension is integer, the n additional dimensions are put at the
      end of integer dimensions; if it is real, at the end of the real
      dimensions.
*)
val expand : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> 'a t
   (** Folding: [fold a tdim] fold the dimensions in the array [tdim] of size n>=1
    and put the result in the first dimension of the array. The other
    dimensions of the array are then removed (using
    ap_abstract0_permute_remove_dimensions).
  *)
val fold : 'a Manager.t -> 'a t -> Var.t array -> 'a t
val expand_with : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> unit
val fold_with : 'a Manager.t -> 'a t -> Var.t array -> unit

(* ============================================================ *)
(** {3 Widening} *)
(* ============================================================ *)
(** Widening *)
val widening : 'a Manager.t -> 'a t -> 'a t -> 'a t
val widening_threshold : 'a Manager.t -> 'a t -> 'a t -> Lincons1.earray -> 'a t
(* ============================================================ *)
(** {3 Closure operation} *)
(* ============================================================ *)
(** Closure: transform strict constraints into non-strict ones.*)
val closure : 'a Manager.t -> 'a t -> 'a t
(** Side-effect version *)
val closure_with : 'a Manager.t -> 'a t -> unit
(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

val of_lincons_array : 'a Manager.t -> Environment.t -> Lincons1.earray -> 'a t
val of_tcons_array   : 'a Manager.t -> Environment.t -> Tcons1.earray  -> 'a t
  (** Abstract a conjunction of constraints *)

(* ============================================================ *)
(** {3 Assignements and Substitutions by single expressions} *)
(* ============================================================ *)
val assign_linexpr : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> 'a t
val substitute_linexpr : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> 'a t
val assign_texpr : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t -> 'a t option -> 'a t
val substitute_texpr : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t -> 'a t option -> 'a t

(** {5 Side-effect versions of the previous functions} *)


val assign_linexpr_with : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> unit
val substitute_linexpr_with : 'a Manager.t -> 'a t -> Var.t -> Linexpr1.t -> 'a t option -> unit
val assign_texpr_with : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t -> 'a t option -> unit
val substitute_texpr_with : 'a Manager.t -> 'a t -> Var.t -> Texpr1.t -> 'a t option -> unit
