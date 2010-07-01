(* File generated from oct.idl *)

type internal

(*
 This file is part of the APRON Library, released under LGPL license.
 Please read the COPYING file packaged in the distribution.
*)

 
(** Octagon abstract domain. *)
 


 
type t
(** Type of octagons.

Octagons are defined by conjunctions of inequalities of the form
[+/-x_i +/- x_j >= 0].

Abstract values which are octagons have the type [t Apron.AbstractX.t].

Managers allocated for octagons have the type [t Apron.manager.t].
*)

 
(** Allocate a new manager to manipulate octagons. *)
external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_oct_oct_manager_alloc"

(** No internal parameters for now... *)
external manager_get_internal : t Apron.Manager.t -> internal
	= "camlidl_oct_manager_get_internal"

(** Approximate a set of generators to an abstract value, with best precision. *)
external of_generator_array : t Apron.Manager.t -> int -> int -> Apron.Generator0.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_of_generator_array"

(** Widening with scalar thresholds. *)
external widening_thresholds : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_widening_thresholds"

(** Standard narrowing. *)
external narrowing : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_narrowing"

(** Perturbation. *)
external add_epsilon : t Apron.Manager.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon"

(** Perturbation. *)
external add_epsilon_bin : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon_bin"

(** Algorithms. *)
val pre_widening : int

(** {2 Type conversions} *)

val manager_is_oct : 'a Apron.Manager.t -> bool
  (** Return [true] iff the argument manager is an octagon manager *)
val manager_of_oct : t Apron.Manager.t -> 'a Apron.Manager.t
  (** Make an octagon manager generic *)
val manager_to_oct : 'a Apron.Manager.t -> t Apron.Manager.t
  (** Instanciate the type of an octagon manager.
      Raises [Failure] if the argument manager is not an octagon manager *)
module Abstract0 : sig
  val is_oct : 'a Apron.Abstract0.t -> bool
    (** Return [true] iff the argument value is an oct value *)
  val of_oct : t Apron.Abstract0.t -> 'a Apron.Abstract0.t
    (** Make an oct value generic *)
  val to_oct : 'a Apron.Abstract0.t -> t Apron.Abstract0.t
    (** Instanciate the type of an oct value. 
        Raises [Failure] if the argument value is not an oct value *)
end
module Abstract1 : sig
  val is_oct : 'a Apron.Abstract1.t -> bool
    (** Return [true] iff the argument value is an oct value *)
  val of_oct : t Apron.Abstract1.t -> 'a Apron.Abstract1.t
    (** Make an oct value generic *)
  val to_oct : 'a Apron.Abstract1.t -> t Apron.Abstract1.t
    (** Instanciate the type of an oct value. 
        Raises [Failure] if the argument value is not an oct value *)
end


(**
{2 Compilation information}

See {!Introduction.compilation} for complete explanations. 
We just show examples with the file [mlexample.ml].

{3 Bytecode compilation}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma octD.cma mlexample.ml]}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -make-runtime -o myrun \
  bigarray.cma gmp.cma apron.cma octD.cma

ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -use-runtime myrun -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma octD.cma mlexample.ml ]}

{3 Native-code compilation}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa octD.cmxa mlexample.ml ]}

{3 Without auto-linking feature}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -noautolink -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa octD.cmxa mlexample.ml \
  -cclib "-L$MLGMPIDL_PREFIX/lib -L$APRON_PREFIX/lib \
	  -loctD_caml_debug -loctD_debug \
	  -lapron_caml_debug -lapron_debug \
	  -lgmp_caml -L$MPFR_PREFIX/lib -lmpfr -L$GMP_PREFIX/lib -lgmp \
	  -L$CAMLIDL_PREFIX/lib/ocaml -lcamlidl \
	  -lbigarray" ]}

*)
