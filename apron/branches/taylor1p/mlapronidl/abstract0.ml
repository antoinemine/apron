(* File generated from abstract0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type 'a t

(** APRON Abstract value of level 0 *)
(** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)

external set_gc : int -> unit
	= "camlidl_abstract0_ap_abstract0_set_gc"

(* ********************************************************************** *)
(** {2 General management} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Memory} *)
(* ============================================================ *)
external copy : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_copy"

external size : 'a Manager.t -> 'a t -> int
	= "camlidl_abstract0_ap_abstract0_size"

(* ============================================================ *)
(** {3 Control of internal representation} *)
(* ============================================================ *)
external minimize : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_minimize"

external canonicalize : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_canonicalize"

external hash : 'a Manager.t -> 'a t -> int
	= "camlidl_abstract0_ap_abstract0_hash"

external approximate : 'a Manager.t -> 'a t -> int -> unit
	= "camlidl_abstract0_ap_abstract0_approximate"

(* ============================================================ *)
(** {3 Printing} *)
(* ============================================================ *)
external fdump : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_fdump"

(* ============================================================ *)
(** {3 Serialization} *)
(* ============================================================ *)
(* ********************************************************************** *)
(** {2 Constructor, accessors, tests and property extraction} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Basic constructors} *)
(* ============================================================ *)
external bottom : 'a Manager.t -> int -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_bottom"

external top : 'a Manager.t -> int -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_top"

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
external is_bottom : 'a Manager.t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_bottom"

external is_top : 'a Manager.t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_top"

external is_leq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_leq"

external is_eq : 'a Manager.t -> 'a t -> 'a t -> bool
	= "camlidl_abstract0_ap_abstract0_is_eq"

external sat_lincons : 'a Manager.t -> 'a t -> Lincons0.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_lincons"

external sat_tcons : 'a Manager.t -> 'a t -> Tcons0.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_tcons"

external sat_interval : 'a Manager.t -> 'a t -> Dim.t -> Interval.t -> bool
	= "camlidl_abstract0_ap_abstract0_sat_interval"

external is_dimension_unconstrained : 'a Manager.t -> 'a t -> Dim.t -> bool
	= "camlidl_abstract0_ap_abstract0_is_dimension_unconstrained"

(* ============================================================ *)
(** {3 Extraction of properties} *)
(* ============================================================ *)
external bound_dimension : 'a Manager.t -> 'a t -> Dim.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_dimension"

external bound_linexpr : 'a Manager.t -> 'a t -> Linexpr0.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_linexpr"

external bound_texpr : 'a Manager.t -> 'a t -> Texpr0.t -> Interval.t
	= "camlidl_abstract0_ap_abstract0_bound_texpr"

external to_box : 'a Manager.t -> 'a t -> Interval.t array
	= "camlidl_abstract0_ap_abstract0_to_box"

external to_lincons_array : 'a Manager.t -> 'a t -> Lincons0.t array
	= "camlidl_abstract0_ap_abstract0_to_lincons_array"

external to_tcons_array : 'a Manager.t -> 'a t -> Tcons0.t array
	= "camlidl_abstract0_ap_abstract0_to_tcons_array"

external to_generator_array : 'a Manager.t -> 'a t -> Generator0.t array
	= "camlidl_abstract0_ap_abstract0_to_generator_array"

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Meet and Join} *)
(* ============================================================ *)
external meet : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet"

external meet_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_array"

external meet_lincons_array : 'a Manager.t -> 'a t -> Lincons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_lincons_array"

external meet_tcons_array : 'a Manager.t -> 'a t -> Tcons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_meet_tcons_array"

external join : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_join"

external join_array : 'a Manager.t -> 'a t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_join_array"

external add_ray_array : 'a Manager.t -> 'a t -> Generator0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_add_ray_array"

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
external assign_linexpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_assign_linexpr_array"

external substitute_linexpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Linexpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_substitute_linexpr_array"

external assign_texpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_assign_texpr_array"

external substitute_texpr_array : 'a Manager.t -> 'a t -> Dim.t array -> Texpr0.t array -> 'a t option -> 'a t
	= "camlidl_abstract0_ap_abstract0_substitute_texpr_array"

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
external expand : 'a Manager.t -> 'a t -> Dim.t -> int -> 'a t
	= "camlidl_abstract0_ap_abstract0_expand"

external fold : 'a Manager.t -> 'a t -> Dim.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_fold"

external expand_with : 'a Manager.t -> 'a t -> Dim.t -> int -> unit
	= "camlidl_abstract0_ap_abstract0_expand_with"

external fold_with : 'a Manager.t -> 'a t -> Dim.t array -> unit
	= "camlidl_abstract0_ap_abstract0_fold_with"

(* ============================================================ *)
(** {3 Widening} *)
(* ============================================================ *)
external widening : 'a Manager.t -> 'a t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_widening"

external widening_threshold : 'a Manager.t -> 'a t -> 'a t -> Lincons0.t array -> 'a t
	= "camlidl_abstract0_ap_abstract0_widening_threshold"

(* ============================================================ *)
(** {3 Closure operation} *)
(* ============================================================ *)
external closure : 'a Manager.t -> 'a t -> 'a t
	= "camlidl_abstract0_ap_abstract0_closure"

external closure_with : 'a Manager.t -> 'a t -> unit
	= "camlidl_abstract0_ap_abstract0_closure_with"

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

let of_lincons_array man intdim realdim array =
let res = top man intdim realdim in
meet_lincons_array_with man res array;
res
let of_tcons_array man intdim realdim array =
let res = top man intdim realdim in
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


let print_array
?(first=("[|@[<hov>":(unit,Format.formatter,unit) format))
?(sep = (";@ ":(unit,Format.formatter,unit) format))
?(last = ("@]|]":(unit,Format.formatter,unit) format))
(print_elt: Format.formatter -> 'a -> unit)
(fmt:Format.formatter)
(array: 'a array)
: unit
=
if array=[||] then begin
Format.fprintf fmt first;
Format.fprintf fmt last;
end
else begin
Format.fprintf fmt first;
let first = ref true in
Array.iter
(begin fun e ->
if !first then first := false else Format.fprintf fmt sep;
print_elt fmt e
end)
array
;
Format.fprintf fmt last;
end

let print assoc fmt a =
let man = manager a in
if is_bottom man a then
	Format.pp_print_string fmt "bottom"
else if is_top man a then
Format.pp_print_string fmt "top"
else begin
let tab = to_lincons_array man a in
print_array (Lincons0.print assoc) fmt tab;
end

