(* File generated from ppl.idl *)


(*
 This file is part of the APRON Library, released under GPL license.
 Please read the COPYING file packaged in the distribution.
*)

(** Convex Polyhedra and Linear Congruences abstract domains (PPL wrapper) *)

(** This module is a wrapper around the Parma Polyhedra Library. *)



type loose
type strict
  (** Two flavors for convex polyhedra: loose or strict.

      Loose polyhedra cannot have strict inequality constraints like [x>0].
      They are algorithmically more efficient (less generators, simpler
      normalization).  Convex polyhedra are defined by the conjunction of a
      set of linear constraints of the form
      [a_0*x_0 + ... + a_n*x_n + b >= 0] or
      [a_0*x_0 + ... + a_n*x_n + b > 0]
      where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.
  *)

type grid
  (** Linear congruences.

      Linear congruences are defined by the conjunction of equality constraints
      modulo a rational number, of the form [a_0*x_0 + ... + a_n*x_n = b mod c],
      where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.
  *)

type 'a t
(** Type of convex polyhedra/linear congruences, where ['a] is [loose], [strict] or [grid].

    Abstract values which are convex polyhedra have the type
    [loose t Apron.AbstractX.t] or [strict t Apron.AbstractX.t].
    Abstract values which are conjunction of linear congruences equalities have the type
    [grid t Apron.AbstractX.t].
    Managers allocated by PPL have the type ['a t Apron.Manager.t].
*)

external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_loose"

external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_strict"

external manager_alloc_grid : unit -> grid t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_grid"


let manager_is_ppl man =
  let str = Apron.Manager.get_library man in
  let str =
    try String.sub str 0 3
    with Invalid_argument _ -> ""
  in
  (String.compare str "PPL")==0
let manager_of_ppl (man:'a t Apron.Manager.t) : 'b Apron.Manager.t = Obj.magic man
let manager_to_ppl (man:'a Apron.Manager.t) : 'b t Apron.Manager.t =
  if manager_is_ppl man then
    Obj.magic man
  else
    failwith "Ppl.to_ppl: the argument manager is not a Ppl manager"

let manager_is_ppl_loose man =
  let str = Apron.Manager.get_library man in
  (String.compare str "PPL::Polyhedron, loose mode")==0
let manager_of_ppl_loose (man:loose t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_ppl_loose (man:'a Apron.Manager.t) : loose t Apron.Manager.t =
  if manager_is_ppl_loose man then
    Obj.magic man
  else
    failwith "Ppl.to_ppl_loose: the argument manager is not a loose Ppl manager"

let manager_is_ppl_strict man =
  let str = Apron.Manager.get_library man in
  (String.compare str "PPL::Polyhedron, strict mode")==0
let manager_of_ppl_strict (man:strict t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_ppl_strict (man:'a Apron.Manager.t) : strict t Apron.Manager.t =
  if manager_is_ppl_strict man then
    Obj.magic man
  else
    failwith "Ppl.to_ppl_strict: the argument manager is not a strict Ppl manager"

let manager_is_ppl_grid man =
  let str = Apron.Manager.get_library man in
  (String.compare str "PPL::Grid")==0
let manager_of_ppl_grid (man:grid t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_ppl_grid (man:'a Apron.Manager.t) : grid t Apron.Manager.t =
  if manager_is_ppl_grid man then
    Obj.magic man
  else
    failwith "Ppl.to_ppl_grid: the argument manager is not a grid Ppl manager"

module Abstract0 = struct
  let is_ppl abs =
    manager_is_ppl (Apron.Abstract0.manager abs)
  let is_ppl_loose abs =
    manager_is_ppl_loose (Apron.Abstract0.manager abs)
  let is_ppl_strict abs =
    manager_is_ppl (Apron.Abstract0.manager abs)
  let is_ppl_grid abs =
    manager_is_ppl_grid (Apron.Abstract0.manager abs)
  let of_ppl (abs: 'a t Apron.Abstract0.t) : 'b Apron.Abstract0.t = Obj.magic abs
  let of_ppl_loose (abs: loose t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let of_ppl_strict (abs: strict t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let of_ppl_grid (abs: grid t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let to_ppl (abs:'a Apron.Abstract0.t) : 'b t Apron.Abstract0.t =
    if is_ppl abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract0.to_ppl: the argument value is not a ppl value"
  let to_ppl_loose (abs:'a Apron.Abstract0.t) : loose t Apron.Abstract0.t =
    if is_ppl_loose abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract0.to_ppl_loose: the argument value is not a loose ppl value"
  let to_ppl_strict (abs:'a Apron.Abstract0.t) : strict t Apron.Abstract0.t =
    if is_ppl_strict abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract0.to_ppl_strict: the argument value is not a strict ppl value"
  let to_ppl_grid (abs:'a Apron.Abstract0.t) : grid t Apron.Abstract0.t =
    if is_ppl_grid abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract0.to_ppl_grid: the argument value is not a grid ppl value"
end

module Abstract1 = struct
  let is_ppl abs =
    manager_is_ppl (Apron.Abstract1.manager abs)
  let is_ppl_loose abs =
    manager_is_ppl_loose (Apron.Abstract1.manager abs)
  let is_ppl_strict abs =
    manager_is_ppl (Apron.Abstract1.manager abs)
  let is_ppl_grid abs =
    manager_is_ppl_grid (Apron.Abstract1.manager abs)
  let of_ppl (abs: 'a t Apron.Abstract1.t) : 'b Apron.Abstract1.t = Obj.magic abs
  let of_ppl_loose (abs: loose t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let of_ppl_strict (abs: strict t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let of_ppl_grid (abs: grid t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let to_ppl (abs:'a Apron.Abstract1.t) : 'b t Apron.Abstract1.t =
    if is_ppl abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract1.to_ppl: the argument value is not a ppl value"
  let to_ppl_loose (abs:'a Apron.Abstract1.t) : loose t Apron.Abstract1.t =
    if is_ppl_loose abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract1.to_ppl_loose: the argument value is not a loose ppl value"
  let to_ppl_strict (abs:'a Apron.Abstract1.t) : strict t Apron.Abstract1.t =
    if is_ppl_strict abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract1.to_ppl_strict: the argument value is not a strict ppl value"
  let to_ppl_grid (abs:'a Apron.Abstract1.t) : grid t Apron.Abstract1.t =
    if is_ppl_grid abs then
      Obj.magic abs
    else
      failwith "Ppl.Abstract1.to_ppl_grid: the argument value is not a grid ppl value"
end

