(* File generated from box.idl *)


(** Intervals abstract domain *)

(** Type of boxes.

    Boxes constrains each dimension/variable [x_i] to belong to an interval
    [I_i].

    Abstract values which are boxes have the type [t Apron.AbstractX.t].

    Managers allocated for boxes have the type [t Apron.manager.t].
*)
type t


(** Create a Box manager. *)
external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_box_box_manager_alloc"


(** {2 Type conversions} *)

val manager_is_box : 'a Apron.Manager.t -> bool
  (** Return [true] iff the argument manager is a box manager *)
val manager_of_box : t Apron.Manager.t -> 'a Apron.Manager.t
  (** Make a box manager generic *)
val manager_to_box : 'a Apron.Manager.t -> t Apron.Manager.t
  (** Instanciate the type of a box manager.
      Raises [Failure] if the argument manager is not a box manager *)
module Abstract0 : sig
  val is_box : 'a Apron.Abstract0.t -> bool
    (** Return [true] iff the argument value is a box value *)
  val of_box : t Apron.Abstract0.t -> 'a Apron.Abstract0.t
    (** Make a box value generic *)
  val to_box : 'a Apron.Abstract0.t -> t Apron.Abstract0.t
    (** Instanciate the type of a box value.
	Raises [Failure] if the argument value is not a box value *)
end
module Abstract1 : sig
  val is_box : 'a Apron.Abstract1.t -> bool
    (** Return [true] iff the argument value is a box value *)
  val of_box : t Apron.Abstract1.t -> 'a Apron.Abstract1.t
    (** Make a box value generic *)
  val to_box : 'a Apron.Abstract1.t -> t Apron.Abstract1.t
    (** Instanciate the type of a box value.
	Raises [Failure] if the argument value is not a box value *)
end


(**
{2 Compilation information}

See {!Introduction.compilation} for complete explanations. 
We just show examples with the file [mlexample.ml].

{3 Bytecode compilation}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma boxMPQ.cma mlexample.ml]}

{[ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -make-runtime -o myrun \
  bigarray.cma gmp.cma apron.cma boxMPQ.cma

ocamlc -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -use-runtime myrun -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma boxMPQ.cma mlexample.ml ]}

{3 Native-code compilation}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa boxMPQ.cmxa mlexample.ml ]}

{3 Without auto-linking feature}

{[ocamlopt -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -noautolink -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa boxMPQ.cmxa mlexample.ml \
  -cclib "-L$MLGMPIDL_PREFIX/lib -L$APRON_PREFIX/lib \
	  -lboxMPQ_caml_debug -lboxMPQ_debug \
	  -lapron_caml_debug -lapron_debug \
	  -lgmp_caml -L$MPFR_PREFIX/lib -lmpfr -L$GMP/lib_PREFIX/lib -lgmp \
	  -L$CAMLIDL_PREFIX/lib/ocaml -lcamlidl \
	  -lbigarray" ]}

*)
