(** APRON Abstract value of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

open Common

type 'a t = 'a Abstract0.t Common.val1
    (** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)

(* ********************************************************************** *)
(** {2 General management} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Memory} *)
(* ============================================================ *)

let copy man x = make_val1 (Abstract0.copy x.val0) x.env
let size man x = Abstract0.size man x.val0

(* ============================================================ *)
(** {3 Control of internal representation} *)
(* ============================================================ *)

let minimize man x = Abstract0.minimize man x.val0
let canonicalize man x = Abstract0.canonicalize man x.val0
let hash man x = (Environment.hash x.env) * 251 + (Abstract0.hash man x.val0) * 19
let approximate man x arg = Abstract0.approximate man x.val0 arg

(* ============================================================ *)
(** {3 Printing} *)
(* ============================================================ *)

let print ?first ?sep ?last fmt x =
  Abstract0.print ?first ?sep ?last (Environment.string_of_var x.env) fmt x.val0

(* ============================================================ *)
(** {3 Serialization} *)
(* ============================================================ *)

(* ********************************************************************** *)
(** {2 Constructor, accessors, tests and property extraction} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Basic constructors} *)
(* ============================================================ *)

let bottom man env = make_val1 (Abstract0.bottom man (Environment.dimension env)) env
let top man env = make_val1 (Abstract0.top man (Environment.dimension env)) env
let of_box man x = make_val1 (Abstract0.of_box man (Environment.dimension x.env)) x.env

(* ============================================================ *)
(** {3 Accessors} *)
(* ============================================================ *)

let manager x = Abstract0.manager x.val0

(* ============================================================ *)
(** {3 Tests} *)
(* ============================================================ *)

let is_bottom man x = Abstract0.is_bottom man x.val0
let is_top man x = Abstract0.is_top man x.val0
let bintest f = begin fun man x y ->
  check_val1 x y;
  f man x.val0 y.val0
end
let is_leq = bintest Abstract0.is_leq
let is_eq = bintest Abstract0.is_eq
let sat_lincons = bintest Abstract0.sat_lincons
let sat_tcons = bintest Abstract0.sat_tcons
let sat_interval man x var coeff = Abstract0.sat_interval man x.val0 (Environment.dim_of_var x.env var) coeff
let is_var_unconstrained man x var = Abstract0.is_dimension_unconstrained man x.val0 (Environment.dim_of_var x.env var)

(* ============================================================ *)
(** {3 Extraction of properties} *)
(* ============================================================ *)

let bound_variable man coeff x var = Abstract0.bound_dimension man coeff x.val0 (Environment.dim_of_var x.env var)
let bound_linexpr man coeff x y = check_val1 x y; Abstract0.bound_linexpr man coeff x.val0 y.val0
let bound_texpr man coeff x y = check_val1 x y; Abstract0.bound_texpr man coeff x.val0 y.val0
let to_box man x y =
  x.val0 <- Abstract0.to_box man x.val0 y.val0;
  x.env <- y.env
let to_lincons_array man x y =
  Abstract0.to_lincons_array man x.val0 y.val0;
  x.env <- y.env
let to_tcons_array man x y =
  make_val1 (Abstract0.to_tcons_array man x.val0 y.val0) y.env
let to_lingen_array man x y =
  Abstract0.to_lingen_array man x.val0 y.val0;
  x.env <- y.env

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)

(* ============================================================ *)
(** {3 Meet and Join} *)
(* ============================================================ *)

let binop f = begin fun man x y ->
  check_val1 x y;
  make_val1 (f man x.val0 y.val0) x.env
end
let arrayop f = begin fun man tx ->
  if tx=[||] then raise (Invalid_argument "The array should of size at least 1");
  for i=1 to pred (Array.length tx) do
    check_val1 tx.(0) tx.(i)
  done;
  make_val1 (f man (Array.map x -> x.val0) tx) tx.(0).env
end
let meet = binop Abstract0.meet
let meet_array = arrayop Abstract0.meet_array
let meet_lincons = binop Abstract0.meet_lincons
let meet_tcons = binop Abstract0.meet_tcons
let join = binop Abstract0.join
let join_array = arrayop Abstract0.join_array
let add_ray_array = binop Abstract0.add_ray_array

let binop_with f man x y =
  check_val1 x y;
  f man x.val0 y.val0

let meet_with = binop Abstract0.meet_with
let meet_lincons_with = binop Abstract0.meet_lincons_with
let meet_tcons_with = binop Abstract0.meet_tcons_with
let join_with = binop Abstract0.join_with
let add_ray_array_with = binop Abstract0.add_ray_array_with

(* ============================================================ *)
(** {3 Assignements and Substitutions} *)
(* ============================================================ *)

let transop f = begin fun man x tvar y optz ->
  check_val1 x y;
  let val0 =
    f man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) y.val0
      (match optz with None -> None | Some z -> check_val1 x z; z.val0)
  in
  make_val1 val0 x.env
end

let assign_linexpr_array = transop Abstract0.assign_linexpr_array
let substitute_linexpr_array = transop Abstract0.substitute_linexpr_array
let assign_texpr_array = transop Abstract0.assign_texpr_array
let substitute_texpr_array = transop Abstract0.substitute_texpr_array

let transop_with f = begin fun man x tvar y optz ->
  check_val1 x y;
  f man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) y.val0
    (match optz with None -> None | Some z -> check_val1 x z; z.val0)
end

let assign_linexpr_array_with = transop_with Abstract0.assign_linexpr_array_with
let substitute_linexpr_array_with = transop_with Abstract0.substitute_linexpr_array_with
let assign_texpr_array_with = transop_with Abstract0.assign_texpr_array_with
let substitute_texpr_array_with = transop_with Abstract0.substitute_texpr_array_with

(* ============================================================ *)
(** {3 Projections} *)
(* ============================================================ *)

let forget_array man x tvar b =
  make_val1 (Abstract0.forget_array man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) b) x.env
let forget_array_with man x tvar b =
  Abstract0.forget_array_with man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) b

(* ============================================================ *)
(** {3 Change and permutation of dimensions} *)
(* ============================================================ *)

let change_environment man x env b =
  let change2 = Environment.dimchange2 x.env env in
  make_val1 (Abstract0.apply_dimchange2 man x.val0 change2 b) env

let minimize_environment_aux man x =
  let dimension = Environment.dimension x.env in
  let lvar = ref [] in
  for i=0 to pred (dimension.Dim.intd + dimension.Dim.reald) do
    if Abstract0.is_dimension_unconstrained man x.val0 i then
      lvar := (Environment.var_of_dim x.env i) :: !lvar
  done;
  Array.of_list !lvar

let minimize_environment man x =
  let tvar = minimize_environment_aux man x in
  if tvar=[||] then x else begin
    let nenv = Environment.remove_vars tvar in
    change_environment man x nenv true
  end

let rename_array man x tvar1 tvar2 =
  let (nenv,perm) = Environment.rename x.env tvar1 tvar2 in
  make_val1 (Abstract0.permute_dimensions man x.val0 perm) nenv

let change_environment_with man x env b =
  let change2 = Environment.dimchange2 x.env env in
  Abstract0.apply_dimchange2_with man x.val0 change2 b;
  x.env <- env

let minimize_environment_with man x =
  let tvar = minimize_environment_aux man x in
  if tvar<>[||] then begin
    let nenv = Environment.remove_vars tvar in
    change_environment_with man x nenv true
  end
let rename_array_with man x tvar1 tvar2 =
  let (nenv,perm) = Environment.rename x.env tvar1 tvar2 in
  Abstract0.permute_dimensions_with man x.val0 perm;
  x.env <- nenv

(* ============================================================ *)
(** {3 Expansion and folding of dimensions} *)
(* ============================================================ *)

let expand man x var tvar =
  let dimension = Environment.dimension x.env in
  let dim = Environment.dim_of_var var in
  let val0 = Abstract0.expand man x.val0 dim (Array.length tvar) in
  let (nenv,perm) =
    Environment.add_perm
      x.env
      (if dim<dimension.Dim.intd then tvar else [||])
      (if dim<dimension.Dim.intd then [||] else tvar)
  in
  Abstract0.permute_dimensions_with man val0 perm;
  make_val1 val0 nenv

let expand_with man x var tvar =
  let dimension = Environment.dimension x.env in
  let dim = Environment.dim_of_var var in
  Abstract0.expand_with man x.val0 dim (Array.length tvar);
  let (nenv,perm) =
    Environment.add_perm
      x.env
      (if dim<dimension.Dim.intd then tvar else [||])
      (if dim<dimension.Dim.intd then [||] else tvar)
  in
  Abstract0.permute_dimensions_with man x.val0 perm;
  x.env <- nenv

let fold_aux env tvar =
  let dimension = Environment.dimension env in
  let nenv = Environment.remove env (Array.sub tvar 1 (pred (Array.length tvar))) in
  let tdim = Array.map (Environment.dim_of_var env) tvar in
  let dim = tdim.(0) in
  let tdim = Array.sort (-) tdim in
  let odimperm = 
    if (dim=tdim.(0)) then 
      None
    else begin
      let index = ref 0 in
      begin try
        for i=1 to pred (Array.length tdim) do
	  if tdim.(i)=dim then begin index := i; raise Exit end
        done;
        failwith ""
      with Exit ->
        ()
      end;
      let dimperm = Array.init (dimension.Dim.intd+Dimension.Dim.reald) (fun i -> i) in
      for rank=tdim.(0) to dim - !index do
        dimperm.(rank) = rank+1;
      done;
      dimperm.(dim - !index) = tdim.(0);
      Some(dimperm)
    end
  in
  (nenv,tdim,odimperm)

let fold man x tvar =
  let (nenv,tdim,odimperm) = fold_aux man x.env tvar in
let val0 = Abstract0.fold man x.val0 tdim in
  begin match odimperm with
  | Some dimperm -> Abstract0.permute_dimensions_with man val0 dimperm
  | None -> ()
  end;
  make_val1 val0 nenv
    
  let fold_with man x tvar =
    let (nenv,tdim,odimperm) = fold_aux man x.env tvar in
    Abstract0.fold_with man x.val0 tdim in
  begin match odimperm with
  | Some dimperm -> Abstract0.permute_dimensions_with man val0 dimperm
  | None -> ()
  end;
  x.env <- nenv

(* ============================================================ *)
(** {3 Widening} *)
(* ============================================================ *)

let widening = binop Abstract0.widening
let widening_threshold man x y z =
  check_val1 x y;
  check_val1 x z;
  make_val1 (Abstract0.widening_threshold man x.val0 y.val0 z.val0)

(* ============================================================ *)
(** {3 Closure operation} *)
(* ============================================================ *)

let closure man x = make_val1 (Abstract0.closure man x.val0) x.env
let closure_with man x = Abstract0.closure_with man x.val0

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

let of_lincons_array man env 

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
