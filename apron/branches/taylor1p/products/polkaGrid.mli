(* File generated from polkaGrid.idl *)


(** Reduced product of NewPolka polyhedra and PPL grids *)

(** Type of abstract values, where ['a] is [Polka.loose] or [Polka.strict]. *)
type 'a t

(** Create a PolkaGrid manager from a (loose or strict) polka manager, and a PPL grid manager *)
external manager_alloc : 'a Polka.t Apron.Manager.t -> Ppl.grid Ppl.t Apron.Manager.t -> 'a t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc"

(** Decompose the manager *)
external manager_decompose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Manager.t * Ppl.grid Ppl.t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_decompose"

(** Decompose an abstract value *)
external decompose : 'a t Apron.Abstract0.t -> 'a Polka.t Apron.Abstract0.t * Ppl.grid Ppl.t Apron.Abstract0.t
	= "camlidl_polkaGrid_ap_pkgrid_decompose"

(** Compose an abstract value *)
external compose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Abstract0.t -> Ppl.grid Ppl.t Apron.Abstract0.t -> 'a t Apron.Abstract0.t
	= "camlidl_polkaGrid_ap_pkgrid_compose"

(** {2 Type conversions} *)

val manager_is_polkagrid : 'a Apron.Manager.t -> bool
  (** Return [true] iff the argument manager is a polkagrid manager *)
val manager_of_polkagrid : 'a t Apron.Manager.t -> 'b Apron.Manager.t
  (** Makes a polkagrid manager generic *)
val manager_to_polkagrid : 'a Apron.Manager.t -> 'b t Apron.Manager.t
  (** Instanciate the type of a polkagrid manager. 
      Raises [Failure] if the argument manager is not a polkagrid manager *)

module Abstract0 : sig
  val is_polkagrid : 'a Apron.Abstract0.t -> bool
    (** Return [true] iff the argument manager is a polkagrid value *)
  val of_polkagrid : 'a t Apron.Abstract0.t -> 'b Apron.Abstract0.t
    (** Makes a polkagrid value generic *)
  val to_polkagrid : 'a Apron.Abstract0.t -> 'b t Apron.Abstract0.t
    (** Instanciate the type of a polkagrid value.
	Raises [Failure] if the argument manager is not a polkagrid manager *)
end

module Abstract1 : sig
  val is_polkagrid : 'a Apron.Abstract1.t -> bool
    (** Return [true] iff the argument manager is a polkagrid value *)
  val of_polkagrid : 'a t Apron.Abstract1.t -> 'b Apron.Abstract1.t
    (** Makes a polkagrid value generic *)
  val to_polkagrid : 'a Apron.Abstract1.t -> 'b t Apron.Abstract1.t
    (** Instanciate the type of a polkagrid value.
	Raises [Failure] if the argument manager is not a polkagrid manager *)
end


(**

{2 Compilation information}

See {!Introduction.compilation} for complete explanations.
We just show examples with the file [mlexample.ml].

Do not forget the [-cc "g++"] option: PPL is a C++ library which requires
a C++ linker.

{3 Bytecode compilation}

{[ocamlc -cc "g++"-I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma ppl.cma polkaGrid.cma mlexample.ml]}

{[ocamlc -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -make-runtime -o myrun \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma ppl.cma polkaGrid.cma 

ocamlc -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -use-runtime myrun -o mlexample.byte \
  bigarray.cma gmp.cma apron.cma polkaMPQ.cma ppl.cma polkaGrid.cma mlexample.ml ]}

{3 Native-code compilation}

{[ocamlopt -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa polkaMPQ.cmxa ppl.cmxa polkaGrid.cmxa mlexample.ml ]}

{3 Without auto-linking feature}

{[ocamlopt -cc "g++" -I $MLGMPIDL_PREFIX/lib -I $APRON_PREFIX/lib -noautolink -o mlexample.opt \
  bigarray.cmxa gmp.cmxa apron.cmxa polkaMPQ.cmxa ppl.cmxa polkaGrid.cmxa mlexample.ml \
  -cclib "-L$MLGMPIDL_PREFIX/lib -L$APRON_PREFIX/lib -L$PPL_PREFIX/lib \
          -lpolkaGrid_caml_debug -lap_pkgrid_debug \
          -lpolkaMPQ_caml_debug -lpolkaMPQ_debug \
	  -lap_ppl_caml_debug -lap_ppl_debug -lppl -lgmpxx \
	  -lapron_caml_debug -lapron_debug \
	  -lgmp_caml -L$MPFR_PREFIX/lib -lmpfr -L$GMP_PREFIX/lib -lgmp \
	  -L$CAMLIDL_PREFIX/lib/ocaml -lcamlidl \
	  -lbigarray" ]}
*)
