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

(** Allocate a PPL manager for loose convex polyhedra. *)
external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_loose"

(** Allocate a PPL manager for strict convex polyhedra. *)
external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_strict"

(** Allocate a new manager for linear congruences (grids) *)
external manager_alloc_grid : unit -> grid t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_grid"

(** {2 Type conversions} *)
val manager_is_ppl : 'a Apron.Manager.t -> bool
val manager_is_ppl_loose : 'a Apron.Manager.t -> bool
val manager_is_ppl_strict : 'a Apron.Manager.t -> bool
val manager_is_ppl_grid : 'a Apron.Manager.t -> bool
  (** Return [true] iff the argument manager is a ppl manager *)
val manager_of_ppl : 'a t Apron.Manager.t -> 'b Apron.Manager.t
val manager_of_ppl_loose : loose t Apron.Manager.t -> 'a Apron.Manager.t
val manager_of_ppl_strict : strict t Apron.Manager.t -> 'a Apron.Manager.t
val manager_of_ppl_grid : grid t Apron.Manager.t -> 'a Apron.Manager.t
  (** Make a ppl manager generic *)
val manager_to_ppl : 'a Apron.Manager.t -> 'b t Apron.Manager.t
val manager_to_ppl_loose : 'a Apron.Manager.t -> loose t Apron.Manager.t
val manager_to_ppl_strict : 'a Apron.Manager.t -> strict t Apron.Manager.t
val manager_to_ppl_grid : 'a Apron.Manager.t -> grid t Apron.Manager.t
  (** Instanciate the type of a ppl manager.
      Raises [Failure] if the argument manager is not a ppl manager *)

module Abstract0 : sig
  val is_ppl : 'a Apron.Abstract0.t -> bool
  val is_ppl_loose : 'a Apron.Abstract0.t -> bool
  val is_ppl_strict : 'a Apron.Abstract0.t -> bool
  val is_ppl_grid : 'a Apron.Abstract0.t -> bool
    (** Return [true] iff the argument manager is a ppl value *)
  val of_ppl : 'a t Apron.Abstract0.t -> 'b Apron.Abstract0.t
  val of_ppl_loose : loose t Apron.Abstract0.t -> 'a Apron.Abstract0.t
  val of_ppl_strict : strict t Apron.Abstract0.t -> 'a Apron.Abstract0.t
  val of_ppl_grid : grid t Apron.Abstract0.t -> 'a Apron.Abstract0.t
    (** Make a ppl value generic *)
  val to_ppl : 'a Apron.Abstract0.t -> 'b t Apron.Abstract0.t
  val to_ppl_loose : 'a Apron.Abstract0.t -> loose t Apron.Abstract0.t
  val to_ppl_strict : 'a Apron.Abstract0.t -> strict t Apron.Abstract0.t
  val to_ppl_grid : 'a Apron.Abstract0.t -> grid t Apron.Abstract0.t
    (** Instanciate the type of a ppl value.
	Raises [Failure] if the argument manager is not a ppl manager *)
end

module Abstract1 : sig
  val is_ppl : 'a Apron.Abstract1.t -> bool
  val is_ppl_loose : 'a Apron.Abstract1.t -> bool
  val is_ppl_strict : 'a Apron.Abstract1.t -> bool
  val is_ppl_grid : 'a Apron.Abstract1.t -> bool
    (** Return [true] iff the argument manager is a ppl value *)
  val of_ppl : 'a t Apron.Abstract1.t -> 'b Apron.Abstract1.t
  val of_ppl_loose : loose t Apron.Abstract1.t -> 'a Apron.Abstract1.t
  val of_ppl_strict : strict t Apron.Abstract1.t -> 'a Apron.Abstract1.t
  val of_ppl_grid : grid t Apron.Abstract1.t -> 'a Apron.Abstract1.t
    (** Make a ppl value generic *)
  val to_ppl : 'a Apron.Abstract1.t -> 'b t Apron.Abstract1.t
  val to_ppl_loose : 'a Apron.Abstract1.t -> loose t Apron.Abstract1.t
  val to_ppl_strict : 'a Apron.Abstract1.t -> strict t Apron.Abstract1.t
  val to_ppl_grid : 'a Apron.Abstract1.t -> grid t Apron.Abstract1.t
    (** Instanciate the type of a ppl value.
	Raises [Failure] if the argument manager is not a ppl manager *)
end


(**
{2 Compilation information}

See {!Introduction.compilation} for complete explanations.
We just show examples with the file [mlexample.ml].

Do not forget the [-cc "g++"] option: PPL is a C++ library which requires
a C++ linker.

{3 Bytecode compilation}

{[ocamlc -cc "g++"-I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma ppl.cma mlexample.ml]}

{[ocamlc -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -make-runtime -o myrun \
  bigarray.cma gmp.cma apron.cma ppl.cma

ocamlc -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -use-runtime myrun -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma ppl.cma mlexample.ml ]}

{3 Native-code compilation}

{[ocamlopt -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa mlexample.ml ]}

{3 Without auto-linking feature}

{[ocamlopt -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -noautolink -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa mlexample.ml \
  -cclib "-L$MLGMPIDL_PREFIX/lib -L$APRON_PREFIX/lib -L$PPL_PREFIX/lib\
	  -lap_ppl_caml_debug -lap_ppl_debug -lppl -lgmpxx \
	  -lapron_caml_debug -lapron_debug \
	  -lgmp_caml -L$MPFR_PREFIX/lib -lmpfr -L$GMP_PREFIX/lib -lgmp \
	  -L$CAMLIDL_PREFIX/lib/ocaml -lcamlidl \
	  -lbigarray" ]}

*)
