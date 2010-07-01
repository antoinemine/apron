(* File generated from polka.idl *)

type internal_ptr

(** Convex Polyhedra and Linear Equalities abstract domains *)

type loose
type strict
  (** Two flavors for convex polyhedra: loose or strict.

      Loose polyhedra cannot have strict inequality constraints like [x>0].
      They are algorithmically more efficient
      (less generators, simpler normalization).

      Convex polyhedra are defined by the conjunction of a set of linear
      constraints of the form [a_0*x_0 + ... + a_n*x_n + b >= 0] or
      [a_0*x_0 + ... + a_n*x_n + b > 0]
      where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.
  *)
type equalities
  (** Linear equalities.

      Linear equalities are conjunctions of linear
      equalities of the form [a_0*x_0 + ... + a_n*x_n + b = 0].
  *)
type 'a t
(** Type of convex polyhedra/linear equalities, where ['a] is [loose], [strict] or [equalities].

    Abstract values which are convex polyhedra have the type
    [(loose t) Apron.Abstract0.t] or [(loose t) Apron.Abstract1.t] or
    [(strict t) Apron.Abstract0.t] or [(strict t) Apron.Abstract1.t].

    Abstract values which are conjunction of linear equalities have the type
    [(equalities t) Apron.Abstract0.t] or [(equalities t) Apron.Abstract1.t].

    Managers allocated by NewPolka have the type ['a t Apron.Manager.t].
*)

(** Create a NewPolka manager for loose convex polyhedra. *)
external pk_manager_alloc_loose : unit -> Manager.ap_manager_ptr
	= "camlidl_polka_pk_manager_alloc_loose"

(** Create a NewPolka manager for strict convex polyhedra. *)
external pk_manager_alloc_strict : unit -> Manager.ap_manager_ptr
	= "camlidl_polka_pk_manager_alloc_strict"

(** Create a NewPolka manager for conjunctions of linear equalities. *)
external pk_manager_alloc_equalities : unit -> Manager.ap_manager_ptr
	= "camlidl_polka_pk_manager_alloc_equalities"

(** Get the internal submanager of a NewPolka manager. *)
external manager_get_internal : Manager.ap_manager_ptr -> internal_ptr
	= "camlidl_polka_manager_get_internal"

(** Various options. See the C documentation *)

external pk_set_max_coeff_size : internal_ptr -> int -> unit
	= "camlidl_polka_pk_set_max_coeff_size"

external pk_set_approximate_max_coeff_size : internal_ptr -> int -> unit
	= "camlidl_polka_pk_set_approximate_max_coeff_size"

external pk_get_max_coeff_size : internal_ptr -> int
	= "camlidl_polka_pk_get_max_coeff_size"

external pk_get_approximate_max_coeff_size : internal_ptr -> int
	= "camlidl_polka_pk_get_approximate_max_coeff_size"

(** {2 Type conversions} *)

val manager_is_polka : 'a Apron.Manager.t -> bool
val manager_is_polka_loose : 'a Apron.Manager.t -> bool
val manager_is_polka_strict : 'a Apron.Manager.t -> bool
val manager_is_polka_equalities : 'a Apron.Manager.t -> bool
  (** Return [true] iff the argument manager is a polka manager *)
val manager_of_polka : 'a t Apron.Manager.t -> 'b Apron.Manager.t
val manager_of_polka_loose : loose t Apron.Manager.t -> 'a Apron.Manager.t
val manager_of_polka_strict : strict t Apron.Manager.t -> 'a Apron.Manager.t
val manager_of_polka_equalities : equalities t Apron.Manager.t -> 'a Apron.Manager.t
  (** Makes a polka manager generic *)
val manager_to_polka : 'a Apron.Manager.t -> 'b t Apron.Manager.t
val manager_to_polka_loose : 'a Apron.Manager.t -> loose t Apron.Manager.t
val manager_to_polka_strict : 'a Apron.Manager.t -> strict t Apron.Manager.t
val manager_to_polka_equalities : 'a Apron.Manager.t -> equalities t Apron.Manager.t
  (** Instanciate the type of a polka manager. 
      Raises [Failure] if the argument manager is not a polka manager *)

module Abstract0 : sig
  val is_polka : 'a Apron.Abstract0.t -> bool
  val is_polka_loose : 'a Apron.Abstract0.t -> bool
  val is_polka_strict : 'a Apron.Abstract0.t -> bool
  val is_polka_equalities : 'a Apron.Abstract0.t -> bool
    (** Return [true] iff the argument manager is a polka value *)
  val of_polka : 'a t Apron.Abstract0.t -> 'b Apron.Abstract0.t
  val of_polka_loose : loose t Apron.Abstract0.t -> 'a Apron.Abstract0.t
  val of_polka_strict : strict t Apron.Abstract0.t -> 'a Apron.Abstract0.t
  val of_polka_equalities : equalities t Apron.Abstract0.t -> 'a Apron.Abstract0.t
    (** Makes a polka value generic *)
  val to_polka : 'a Apron.Abstract0.t -> 'b t Apron.Abstract0.t
  val to_polka_loose : 'a Apron.Abstract0.t -> loose t Apron.Abstract0.t
  val to_polka_strict : 'a Apron.Abstract0.t -> strict t Apron.Abstract0.t
  val to_polka_equalities : 'a Apron.Abstract0.t -> equalities t Apron.Abstract0.t
    (** Instanciate the type of a polka value.
	Raises [Failure] if the argument manager is not a polka manager *)
end

module Abstract1 : sig
  val is_polka : 'a Apron.Abstract1.t -> bool
  val is_polka_loose : 'a Apron.Abstract1.t -> bool
  val is_polka_strict : 'a Apron.Abstract1.t -> bool
  val is_polka_equalities : 'a Apron.Abstract1.t -> bool
    (** Return [true] iff the argument manager is a polka value *)
  val of_polka : 'a t Apron.Abstract1.t -> 'b Apron.Abstract1.t
  val of_polka_loose : loose t Apron.Abstract1.t -> 'a Apron.Abstract1.t
  val of_polka_strict : strict t Apron.Abstract1.t -> 'a Apron.Abstract1.t
  val of_polka_equalities : equalities t Apron.Abstract1.t -> 'a Apron.Abstract1.t
    (** Makes a polka value generic *)
  val to_polka : 'a Apron.Abstract1.t -> 'b t Apron.Abstract1.t
  val to_polka_loose : 'a Apron.Abstract1.t -> loose t Apron.Abstract1.t
  val to_polka_strict : 'a Apron.Abstract1.t -> strict t Apron.Abstract1.t
  val to_polka_equalities : 'a Apron.Abstract1.t -> equalities t Apron.Abstract1.t
    (** Instanciate the type of a polka value.
	Raises [Failure] if the argument manager is not a polka manager *)
end


(**
{2 Compilation information}

See {!Introduction.compilation} for complete explanations. 
We just show examples with the file [mlexample.ml].

{3 Bytecode compilation}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma mlexample.ml]}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -make-runtime -o myrun \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma

ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -use-runtime myrun -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma mlexample.ml ]}

{3 Native-code compilation}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa polkaMPQ.cmxa mlexample.ml ]}

{3 Without auto-linking feature}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -noautolink -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa polkaMPQ.cmxa mlexample.ml \
  -cclib "-L$MLGMPIDL_PREFIX/lib -L$APRON_PREFIX/lib \
	  -lpolkaMPQ_caml_debug -lpolkaMPQ_debug \
	  -lapron_caml_debug -lapron_debug \
	  -lgmp_caml -L$MPFR_PREFIX/lib -lmpfr -L$GMP_PREFIX/lib -lgmp \
	  -L$CAMLIDL_PREFIX/lib/ocaml -lcamlidl \
	  -lbigarray" ]}

*)
