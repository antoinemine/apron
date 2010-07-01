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

let copy man x = { abstract0 = Abstract0.copy man x.abstract0; env = x.env }
let size man x = Abstract0.size man x.abstract0

(* ============================================================ *)
(** {3 Control of internal representation} *)
(* ============================================================ *)

let minimize man x = Abstract0.minimize man x.abstract0
let canonicalize man x = Abstract0.canonicalize man x.abstract0
let hash man x = 5*(Environment.hash x.env) + Abstract0.hash man x.abstract0
let approximate man x n = Abstract0.approximate man x.abstract0 n

(* ============================================================ *)
(** {3 Printing} *)
(* ============================================================ *)
external fdump : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_fdump"

(* ============================================================ *)
(** {3 Serialization} *)
(* ============================================================ *)
(* ********************************************************************** *)
(** {2 Constructor, accessors, tests and property extraction} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Basic constructors} *)

(* ============================================================ *)
external bottom : 'a Manager.t -> Environment.t -> 'a t
	= "camlidl_abstract1_ap_abstract1_bottom"

external top : 'a Manager.t -> Environment.t -> 'a t
	= "camlidl_abstract1_ap_abstract1_top"

external of_box : 'a Manager.t -> Environment.t -> Var.t array -> Interval.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_of_box"

(* ============================================================ *)
(** {3 Accessors} *)

(* ============================================================ *)

let manager x = Abstract0.manager x.abstract0
let env x = x.env
let abstract0 x = x.abstract0

(* ============================================================ *)
(** {3 Tests} *)

(* ============================================================ *)

let is_bottom man x = Abstract0.is_bottom man x.abstract0
let is_top man x = Abstract0.is_top man x.abstract0

external is_leq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract1_ap_abstract1_is_leq"

external is_eq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract1_ap_abstract1_is_eq"

external sat_lincons : 'a Manager.t -> 'a t -> Lincons1.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_lincons"

external sat_tcons : 'a Manager.t -> 'a t -> Tcons1.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_tcons"

external sat_interval : 'a Manager.t -> 'a t -> Var.t -> Interval.t -> bool
	= "camlidl_abstract1_ap_abstract1_sat_interval"

external is_variable_unconstrained : 'a Manager.t -> 'a t -> Var.t -> bool
	= "camlidl_abstract1_ap_abstract1_is_variable_unconstrained"

(* ============================================================ *)
(** {3 Extraction of properties} *)

(* ============================================================ *)
external bound_variable : 'a Manager.t -> 'a t -> Var.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_variable"

external bound_linexpr : 'a Manager.t -> 'a t -> Linexpr1.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_linexpr"

external bound_texpr : 'a Manager.t -> 'a t -> Texpr1.t -> Interval.t
	= "camlidl_abstract1_ap_abstract1_bound_texpr"


let to_lincons_array man x = {
Lincons1.lincons0_array = Abstract0.to_lincons_array man x.abstract0;
Lincons1.array_env = x.env
}
let to_tcons_array man x = {
Tcons1.tcons0_array = Abstract0.to_tcons_array man x.abstract0;
Tcons1.array_env = x.env
}
let to_generator_array man x = {
Generator1.generator0_array = Abstract0.to_generator_array man x.abstract0;
Generator1.array_env = x.env
}
let to_box man x = {
interval_array = Abstract0.to_box man x.abstract0;
box1_env = x.env
}

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Meet and Join} *)

(* ============================================================ *)
external meet : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet"

external meet_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_array"

external meet_lincons_array : 'a Manager.t -> 'a t -> Lincons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_lincons_array"

external meet_tcons_array : 'a Manager.t -> 'a t -> Tcons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_meet_tcons_array"

external join : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_join"

external join_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_join_array"

external add_ray_array : 'a Manager.t -> 'a t -> Generator1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_add_ray_array"

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
external assign_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_assign_linexpr_array"

external substitute_linexpr_array : 'a Manager.t -> 'a t -> Var.t array -> Linexpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_substitute_linexpr_array"

external assign_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_assign_texpr_array"

external substitute_texpr_array : 'a Manager.t -> 'a t -> Var.t array -> Texpr1.t array -> 'a t option -> 'a t
	= "camlidl_abstract1_ap_abstract1_substitute_texpr_array"

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
external forget_array : 'a Manager.t -> 'a t -> Var.t array -> bool -> 'a t
	= "camlidl_abstract1_ap_abstract1_forget_array"

external forget_array_with : 'a Manager.t -> 'a t -> Var.t array -> bool -> unit
	= "camlidl_abstract1_ap_abstract1_forget_array_with"

(* ============================================================ *)
(** {3 Change and permutation of dimensions} *)

(* ============================================================ *)
external change_environment : 'a Manager.t -> 'a t -> Environment.t -> bool -> 'a t
	= "camlidl_abstract1_ap_abstract1_change_environment"

external minimize_environment : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_minimize_environment"

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
external expand : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_expand"

external fold : 'a Manager.t -> 'a t -> Var.t array -> 'a t
	= "camlidl_abstract1_ap_abstract1_fold"

external expand_with : 'a Manager.t -> 'a t -> Var.t -> Var.t array -> unit
	= "camlidl_abstract1_ap_abstract1_expand_with"

external fold_with : 'a Manager.t -> 'a t -> Var.t array -> unit
	= "camlidl_abstract1_ap_abstract1_fold_with"

(* ============================================================ *)
(** {3 Widening} *)

(* ============================================================ *)
external widening : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_widening"

external widening_threshold : 'a Manager.t -> 'a t -> 'a t -> Lincons1.earray -> 'a t
	= "camlidl_abstract1_ap_abstract1_widening_threshold"

(* ============================================================ *)
(** {3 Closure operation} *)

(* ============================================================ *)
external closure : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_closure"

external closure_with : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_closure_with"

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

let of_lincons_array man env array =
let res = top man env in
meet_lincons_array_with man res array;
res
let of_tcons_array man env array =
let res = top man env in
meet_tcons_array_with man res array;
res
let assign_linexpr man abs dim expr odest =
assign_linexpr_array man abs [|dim|] [|expr|] odest
let assign_texpr man abs dim expr odest =
assign_texpr_array man abs [|dim|] [|expr|] odest
let substitute_linexpr man abs dim expr odest =
substitute_linexpr_array man abs [|dim|] [|expr|] odest
let substitute_texpr man abs dim expr odest =
substitute_texpr_array man abs [|dim|] [|expr|] odest
let assign_linexpr_with man abs dim expr odest =
assign_linexpr_array_with man abs [|dim|] [|expr|] odest
let assign_texpr_with man abs dim expr odest =
assign_texpr_array_with man abs [|dim|] [|expr|] odest
let substitute_linexpr_with man abs dim expr odest =
substitute_linexpr_array_with man abs [|dim|] [|expr|] odest
let substitute_texpr_with man abs dim expr odest =
substitute_texpr_array_with man abs [|dim|] [|expr|] odest

external unify : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract1_ap_abstract1_unify"

external unify_with : 'a Manager.t -> 'a t -> 'a t -> unit
	= "camlidl_abstract1_ap_abstract1_unify_with"


let print fmt a =
let man = manager a in
if is_bottom man a then
Format.pp_print_string fmt "bottom"
else if is_top man a then
Format.pp_print_string fmt "top"
else begin
let tab = to_lincons_array man a in
Lincons1.array_print fmt tab;
end

