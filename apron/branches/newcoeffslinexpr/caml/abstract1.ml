(** APRON Abstract value of level 1 *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

open Common

type 'a t = 'a Abstract0.t Common.val1
    (** The type parameter ['a] allows to distinguish abstract values with different underlying abstract domains. *)

let linexpr0_add_dimensions val0 dimchange =
  let nval0 = Linexpr0.init (Common.discr val0) in
  Linexpr0.add_dimensions nval0 val0 dimchange;
  nval0
let lincons0_add_dimensions val0 dimchange =
  let nval0 = Lincons0.init (Common.discr val0) in
  Lincons0.add_dimensions nval0 val0 dimchange;
  nval0
let linexpr0_array_add_dimensions val0 dimchange =
  let nval0 = Linexpr0.array_init (Common.discr val0) 0 in
  Linexpr0.array_add_dimensions nval0 val0 dimchange;
  nval0
let lincons0_array_add_dimensions val0 dimchange =
  let nval0 = Lincons0.array_init (Common.discr val0) 0 in
  Lincons0.array_add_dimensions nval0 val0 dimchange;
  nval0
let lingen0_array_add_dimensions val0 dimchange =
  let nval0 = Lingen0.array_init (Common.discr val0) 0 in
  Lingen0.array_add_dimensions nval0 val0 dimchange;
  nval0

(* ********************************************************************** *)
(** {2 General management} *)
(* ********************************************************************** *)
(* ============================================================ *)
(** {3 Memory} *)
(* ============================================================ *)

let copy man x = make_val1 (Abstract0.copy man x.val0) x.env
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
  Abstract0.print ?first ?sep ?last (Environment.string_of_dim x.env) fmt x.val0

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
let of_box man env x =
  let x0 =
    if x.env==env || Environment.equal x.env env then
      x.val0
  else
    try
      let dimchange = Environment.dimchange x.env env in
      linexpr0_add_dimensions x.val0 dimchange;
    with Failure _ ->
      raise (Invalid_argument "The environment is not a superenvironment of the environment of the box")
  in
  make_val1 (Abstract0.of_box man (Environment.dimension env) x0) env

(* ============================================================ *)
(** {3 Accessors} *)
(* ============================================================ *)

let manager x = Abstract0.manager x.val0

(* ============================================================ *)
(** {3 Tests} *)
(* ============================================================ *)

let is_bottom man x = Abstract0.is_bottom man x.val0
let is_top man x = Abstract0.is_top man x.val0
let bintest op man x y =
  check_val1 x y;
  op man x.val0 y.val0
let is_leq man x y = bintest Abstract0.is_leq man x y
let is_eq man x y = bintest Abstract0.is_eq man x y
let bintest_add op add man x y =
  let y0 = check_val1_add x y add in
  op man x.val0 y0
let sat_lincons man x y = bintest_add Abstract0.sat_lincons lincons0_add_dimensions man x y
let sat_tcons man x y = bintest_add Abstract0.sat_tcons Tcons0.add_dimensions man x y
let sat_interval man x var coeff = Abstract0.sat_interval man x.val0 (Environment.dim_of_var x.env var) coeff
let is_var_unconstrained man x var = Abstract0.is_dimension_unconstrained man x.val0 (Environment.dim_of_var x.env var)

(* ============================================================ *)
(** {3 Extraction of properties} *)
(* ============================================================ *)

let bound_variable man coeff x var = Abstract0.bound_dimension man coeff x.val0 (Environment.dim_of_var x.env var)
let bound_linexpr man coeff x y =
  let y0 = check_val1_add x y linexpr0_add_dimensions in
  Abstract0.bound_linexpr man coeff x.val0 y0
let bound_texpr man coeff x y =
  let y0 = check_val1_add x y Texpr0.add_dimensions in
  Abstract0.bound_texpr man coeff x.val0 y0
let to_box man x y =
  Abstract0.to_box man x.val0 y.val0;
  x.env <- y.env
let to_lincons_array man x y =
  Abstract0.to_lincons_array man x.val0 y.val0;
  x.env <- y.env
let to_tcons_array man x =
  make_val1 (Abstract0.to_tcons_array man x.val0) x.env
let to_lingen_array man x y =
  Abstract0.to_lingen_array man x.val0 y.val0;
  x.env <- y.env

(* ********************************************************************** *)
(** {2 Operations} *)
(* ********************************************************************** *)

(* ============================================================ *)
(** {3 Meet and Join} *)
(* ============================================================ *)

let binop op man x y =
  check_val1 x y;
  make_val1 (op man x.val0 y.val0) x.env
let binop_add op add man x y =
  let y0 = check_val1_add x y add in
  make_val1 (op man x.val0 y0) x.env
let arrayop op man tx =
  if tx=[||] then raise (Invalid_argument "The array should of size at least 1");
  for i=1 to pred (Array.length tx) do
    check_val1 tx.(0) tx.(i)
  done;
  make_val1 (op man (Array.map (fun x -> x.val0) tx)) tx.(0).env

let meet man x y = binop Abstract0.meet man x y
let meet_array man tx = arrayop Abstract0.meet_array man tx
let meet_lincons_array man x y = binop_add Abstract0.meet_lincons_array lincons0_array_add_dimensions man x y
let meet_tcons_array man x y = binop_add Abstract0.meet_tcons_array Tcons0.array_add_dimensions man x y
let join man x y = binop Abstract0.join man x y
let join_array man tx = arrayop Abstract0.join_array man tx
let add_ray_array man x y = binop_add Abstract0.add_ray_array lingen0_array_add_dimensions man x y

let binop_with op man x y =
  check_val1 x y;
  op man x.val0 y.val0
let binop_add_with op add man x y =
  let y0 = check_val1_add x y add in
  op man x.val0 y0

let meet_with man x y = binop_with Abstract0.meet_with man x y
let meet_lincons_array_with man x y = binop_add_with Abstract0.meet_lincons_array_with lincons0_array_add_dimensions man x y
let meet_tcons_array_with man x y = binop_add_with Abstract0.meet_tcons_array_with Tcons0.array_add_dimensions man x y
let join_with man x y = binop_with Abstract0.join_with man x y
let add_ray_array_with man x y = binop_add_with Abstract0.add_ray_array_with lingen0_array_add_dimensions man x y

(* ============================================================ *)
(** {3 Assignements and Substitutions} *)
(* ============================================================ *)

let transop op add man x tvar y optz =
  let y0 = check_val1_add x y add in
  let val0 =
    op man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) y0
      (match optz with None -> None | Some z -> check_val1 x z; Some z.val0)
  in
  make_val1 val0 x.env

let assign_linexpr_array man x tvar y optz = transop Abstract0.assign_linexpr_array linexpr0_array_add_dimensions man x tvar y optz
let substitute_linexpr_array man x tvar y optz = transop Abstract0.substitute_linexpr_array linexpr0_array_add_dimensions man x tvar y optz
let assign_texpr_array man x tvar y optz = transop Abstract0.assign_texpr_array Texpr0.array_add_dimensions man x tvar y optz
let substitute_texpr_array man x tvar y optz = transop Abstract0.substitute_texpr_array Texpr0.array_add_dimensions man x tvar y optz

let transop_with op add man x tvar y optz =
  let y0 = check_val1_add x y add in
  op man x.val0 (Array.map (Environment.dim_of_var x.env) tvar) y0
    (match optz with None -> None | Some z -> check_val1 x z; Some z.val0)

let assign_linexpr_array_with man x tvar y optz = transop_with Abstract0.assign_linexpr_array_with linexpr0_array_add_dimensions man x tvar y optz
let substitute_linexpr_array_with man x tvar y optz = transop_with Abstract0.substitute_linexpr_array_with linexpr0_array_add_dimensions man x tvar y optz
let assign_texpr_array_with man x tvar y optz = transop_with Abstract0.assign_texpr_array_with Texpr0.array_add_dimensions man x tvar y optz
let substitute_texpr_array_with man x tvar y optz = transop_with Abstract0.substitute_texpr_array_with Texpr0.array_add_dimensions man x tvar y optz

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
    let nenv = Environment.remove x.env tvar in
    change_environment man x nenv true
  end

let rename_array man x tvar1 tvar2 =
  let (nenv,perm) = Environment.rename_perm x.env tvar1 tvar2 in
  make_val1 (Abstract0.permute_dimensions man x.val0 perm) nenv

let change_environment_with man x env b =
  let change2 = Environment.dimchange2 x.env env in
  Abstract0.apply_dimchange2_with man x.val0 change2 b;
  x.env <- env

let minimize_environment_with man x =
  let tvar = minimize_environment_aux man x in
  if tvar<>[||] then begin
    let nenv = Environment.remove x.env tvar in
    change_environment_with man x nenv true
  end
let rename_array_with man x tvar1 tvar2 =
  let (nenv,perm) = Environment.rename_perm x.env tvar1 tvar2 in
  Abstract0.permute_dimensions_with man x.val0 perm;
  x.env <- nenv

(* ============================================================ *)
(** {3 Expansion and folding of dimensions} *)
(* ============================================================ *)

let expand man x var tvar =
  let dimension = Environment.dimension x.env in
  let dim = Environment.dim_of_var x.env var in
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
  let dim = Environment.dim_of_var x.env var in
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
  Array.sort (-) tdim;
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
      let dimperm = Array.init (dimension.Dim.intd+dimension.Dim.reald) (fun i -> i) in
      for rank=tdim.(0) to dim - !index do
	dimperm.(rank) <- rank+1;
      done;
      dimperm.(dim - !index) <- tdim.(0);
      Some(dimperm)
    end
  in
  (nenv,tdim,odimperm)

let fold man x tvar =
  let (nenv,tdim,odimperm) = fold_aux x.env tvar in
  let val0 = Abstract0.fold man x.val0 tdim in
  begin match odimperm with
  | Some dimperm -> Abstract0.permute_dimensions_with man val0 dimperm
  | None -> ()
  end;
  make_val1 val0 nenv

let fold_with man x tvar =
  let (nenv,tdim,odimperm) = fold_aux x.env tvar in
  Abstract0.fold_with man x.val0 tdim;
  begin match odimperm with
  | Some dimperm -> Abstract0.permute_dimensions_with man x.val0 dimperm
  | None -> ()
  end;
  x.env <- nenv

(* ============================================================ *)
(** {3 Widening} *)
(* ============================================================ *)

let widening man x y = binop Abstract0.widening man x y
let widening_threshold man x y z =
  check_val1 x y;
  let z0 = check_val1_add x z lincons0_array_add_dimensions in
  make_val1 (Abstract0.widening_threshold man x.val0 y.val0 z0) x.env

(* ============================================================ *)
(** {3 Closure operation} *)
(* ============================================================ *)

let closure man x = make_val1 (Abstract0.closure man x.val0) x.env
let closure_with man x = Abstract0.closure_with man x.val0

(* ********************************************************************** *)
(** {2 Additional operations} *)
(* ********************************************************************** *)

let of_lincons_array man env x =
  let res = top man env in
  meet_lincons_array_with man res x;
  res
let of_tcons_array man env x =
  let res = top man env in
  meet_tcons_array_with man res x;
  res

(* ============================================================ *)
(** {3 Assignements and Substitutions by single expressions} *)
(* ============================================================ *)

let transop op add man x var y optz =
  let y0 = check_val1_add x y add in
  let val0 =
    op man x.val0 (Environment.dim_of_var x.env var) y0
      (match optz with None -> None | Some z -> check_val1 x z; Some z.val0)
  in
  make_val1 val0 x.env

let transop_with op add man x var y optz =
  let y0 = check_val1_add x y add in
  op man x.val0 (Environment.dim_of_var x.env var) y0
    (match optz with None -> None | Some z -> check_val1 x z; Some z.val0)

let assign_linexpr man x var y optz = transop Abstract0.assign_linexpr linexpr0_add_dimensions man x var y optz
let substitute_linexpr man x var y optz = transop Abstract0.substitute_linexpr linexpr0_add_dimensions man x var y optz
let assign_texpr man x var y optz = transop Abstract0.assign_texpr Texpr0.add_dimensions man x var y optz
let substitute_texpr man x var y optz = transop Abstract0.substitute_texpr Texpr0.add_dimensions man x var y optz

let assign_linexpr_with man x var y optz = transop_with Abstract0.assign_linexpr_with linexpr0_add_dimensions man x var y optz
let substitute_linexpr_with man x var y optz = transop_with Abstract0.substitute_linexpr_with linexpr0_add_dimensions man x var y optz
let assign_texpr_with man x var y optz = transop_with Abstract0.assign_texpr_with Texpr0.add_dimensions man x var y optz
let substitute_texpr_with man x var y optz = transop_with Abstract0.substitute_texpr_with Texpr0.add_dimensions man x var y optz
