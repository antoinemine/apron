(** *)

(**

This package is an OCaml interface for the APRON
library/interface.  The interface is accessed via the module
Apron, which is decomposed into 15 submodules, corresponding to C
modules, and which can be organized in 4 groups

{ol
{- {b Coefficients}: scalar numbers, intervals, ...
}
{- {b Managers and Abstract Domains}
{ul
{- {!module: Manager}: managers}
{- {!module: Box}: interval domain}
{- {!module: Oct}: octagon domain}
{- {!module: Polka}: convex polyhedra and linear equalities domains}
{- {!module: T1p}: Taylor1plus abstract domain}
{- {!module: Ppl}:  PPL convex polyhedra and linear congruences domains}
{- {!module: PolkaGrid}: reduced product of convex polyhedra and PPL linear congruences}
}
}
{- {b Level 1 of the interface (user-level)}: manipulation of generic datatypes (expressions, constraints, ..., and generic abstract domain interface)}
{- {b Level 0 of the interface (lower-level)}: manipulation of generic datatypes (expressions, constraints, ..., and generic abstract domain interface)}
}
*)

(*  ********************************************************************** *)
(** {2 Requirements and installation} *)
(*  ********************************************************************** *)

(**
(See README of general APRON distribution for more details)

{ul
{- APRON library}
{- GMP library version 4.2 or up (tested with version 4.2.1 and 4.3.1)}
{- MPFR library version 2.2 or up (tested with version 2.2.1 and 2.3.1)}
{- MLGMPIDL to GMP and MPFR libraries}
{- OCaml 3.09 or up}
{- Camlidl (tested with 1.05)}
}

For compiling from repository (strongly recommended):
{ul
{- GNU M4 preprocessor}
{- GNU sed}
}
It is important to have the GNU versions !
*)

(*  ====================================================================== *)
(** {3 Installation} *)
(*  ====================================================================== *)

(**
{ul
{- {b Library}:

Set the file [../Makefile.config] to your own setting.

type 'make', and then 'make install'

The OCaml part of the library is named [apron.cma] ([.cmxa], [.a]). The C
part of the library, which is automatically referenced by
[apron.cma/apron.cmxa], is named [libapron_caml.a],
[libapron_caml.so], [dllapron_caml.so] (which is a soft link to the
previous library) (debug versions: [libapron_caml_debug.a],
[libapron_caml_debug.so], [dllapron_caml_debug.so])

'make install' installs not only [.mli], [.cmi], but also [.idl] files.
}
{- {b Documentation}:

The documentation is generated with ocamldoc.

'make mlapronidl.pdf'

'make html' (put the HTML files in the html subdirectoy)
}
{- {b Miscellaneous}:

'make clean' and 'make distclean' have the usual behaviour.
}
}
*)

(*  ********************************************************************** *)
(** {2:hints Hints on programming idioms} *)
(*  ********************************************************************** *)

(*  ====================================================================== *)
(** {3 Allocating managers} *)
(*  ====================================================================== *)

(**
The user might have some difficulties to exploit the genericity of
the interface at first glance (it was actually my case).

Assume your main analysis function looks like:
{[let analyze_and_display equations (man : 'a Apron.Manager.t) : unit =
  ... ]}

where [equations] is the equation system, [man] the APRON manager,
and ['a] the effective abstract domain/implementation to be used
in the analysis.

{ol
{- You might want to write code like

{[let manager_alloc option = match option with
  | `Box -> Box.manager_alloc ()
  | `Oct -> Oct.manager_alloc ()
;;
let main option equations =
  let man = manager_alloc opt in
  analyze_and_display man equations
;;]}

but this does not work because [manager_alloc] cannot be typed
(the types of [(Box.manager_alloc ()) : Box.t Apron.Manager.t] and
[(Oct.manager_alloc ()) : Oct.t Apron.Manager.t] cannot be
unified).
}
{- Using continuations does not work either:

{[let manager_alloc_and_continue option (continuation:'a Apron.Manager.t -> 'b) =
  match option with
  | `Box -> continuation (Box.manager_alloc ())
  | `Oct -> continuation (Oct.manager_alloc ())
;;
let main option equations =
  manager_alloc_and_continue option
    (fun apron -> analyze_and_display equations equations apron)
;;]}

because the argument [continuation] is
monomorphic inside the body of [manager_alloc_and_continue] (i.e,
it is not generalized):

{[let manager_alloc_and_continue option (continuation:'a Apron.Manager.t -> 'b) =
    match option with
    | `Box -> continuation (Box.manager_alloc ())
    | `Oct -> continuation (Oct.manager_alloc ())
;;                          ^^^^^^^^^^^^^^^^^^^^
Error: This expression has type Oct.t Apron.Manager.t
       but an expression was expected of type Box.t Apron.Manager.t]}
You can read detailed explanations about this issue on
{{:http://caml.inria.fr/pub/old_caml_site/FAQ/FAQ_EXPERT-eng.html#arguments_polymorphes}OCaml FAQ}.
}
}

I can suggest 3 solutions:
{ol
{- Following {{:http://caml.inria.fr/pub/old_caml_site/FAQ/FAQ_EXPERT-eng.html#arguments_polymorphes}OCaml FAQ}, you can modify attempt 2 above as follows:

{[let manager_alloc_and_continue option equations =
  match option with
  | `Box -> analyze_and_display equations (Box.manager_alloc ())
  | `Oct -> analyze_and_display equations (Oct.manager_alloc ())
;;
let main option equations =
  manager_alloc_and_continue option equations
;;]}

Now this can be type-checked:

{[val manager_alloc_and_continue : [< `Box | `Oct ] -> 'a -> unit = <fun>]}

This is not very elegant: the call to
[analyze_and_display] is hard-coded in
[manager_alloc_and_continue], and one has to pass all its arguments (like [equations]) to
[manager_alloc_and_continue].
}
{- It is possible to not give up with continuations by encapsulating them into a record (resp. an immediate  object), because record fields (resp. methods) may be polymorphic.

{ul
{- Using records:

{[type continuation = {
  f : 'a. 'a Apron.Manager.t -> unit;
};;
let manager_alloc_and_continue option (continuation:continuation) =
  match option with
  | `Box -> continuation.f (Box.manager_alloc ())
  | `Oct -> continuation.f (Oct.manager_alloc ())
;;
let main option equations =
  manager_alloc_and_continue option
    {f = fun apron -> analyze_and_display equations apron}
;;]}

}
{- Using immediate objects:

{[type continuation = < f : 'a. 'a Apron.Manager.t -> unit >;;
let manager_alloc_and_continue option (continuation:continuation) =
  match option with
  | `Box -> continuation#f (Box.manager_alloc ())
  | `Oct -> continuation#f (Oct.manager_alloc ())
;;
let main option equations =
  manager_alloc_and_continue option
    (object method f: 'a .'a Apron.Manager.t -> unit =
       fun apron -> analyze_and_display equations apron
    end)
;;]}

Compared to records, using immediate objects requires to repeat polymorphic type annotations. On the other hand, one does not need to define a new type [continuation]:

{[let manager_alloc_and_continue option (continuation:< f : 'a. 'a Apron.Manager.t -> unit >)
  match option with
  | `Box -> continuation#f (Box.manager_alloc ())
  | `Oct -> continuation#f (Oct.manager_alloc ())
;;
let main option equations =
  manager_alloc_and_continue option
    (object method f: 'a .'a Apron.Manager.t -> unit =
       fun apron -> analyze_and_display equations apron
    end)
;;]}
}}}
{- A last possibility is to use the type conversion functions
provided in {!module: Box} and {!module: Oct} (as well as in the
other domain modules). One can modify attempt 1 as follows:

{[let manager_alloc option = match option with
  | `Box -> Box.manager_of_box (Box.manager_alloc ())
  | `Oct -> Oct.manager_of_oct (Oct.manager_alloc ())
;;
let main option equations =
  let man = manager_alloc opt in
  analyze_and_display man equations
;;]}

{[val manager_alloc : [< `Box | `Oct ] -> 'a Apron.Manager.t = <fun>]}

The purpose of functions {!Box.manager_of_box} and {!Oct.manager_of_oct} is
to generalize the type of their arguments
(this is implemented with the [Obj.magic] function... but this is safe).

This is the most simple and flexible way.
}
}
*)

(*  ====================================================================== *)
(** {3 Breaking (locally) genericity} *)
(*  ====================================================================== *)

(**
Assume that you are inside the body of the same

{[analyze_and_display: equations -> 'a Apron.Manager.t -> unit]}

function and that you want at some point
- either to modify an option of the manager [man], depending on the
  effective underlying domain
  (like {!Polka.set_max_coeff_size});
- or similarly to perform a specific operation on an abstract value.

You can modify the solution 1 above so as to pass a
[modify: 'a Apron.Manager.t -> unit] function to [analyze_and_display]:
{[let analyze_and_display equations
      (man : 'a Apron.Manager.t)
      (modify : 'a Apron.Manager.t -> unit)
  =
  ...
;;
let manager_alloc_and_continue option equations =
  match option with
  | `Box -> analyze_and_display equations (Box.manager_alloc ()) box_modify
  | `Oct -> analyze_and_display equations (Oct.manager_alloc ()) oct_modify
;;
let main option equations =
  manager_alloc_and_continue option equations
;;]}

The most flexible way however is to use the ``dynamic cast''
functions {!Box.manager_to_box}, {!Box.Abstract0.to_box},
{!Oct.manager_to_oct}, {!Oct.Abstract0.to_oct}. These functions
raise a [Failure] exception in case of (dynamic) typing error, but
this can be avoided by the test functions {!Box.manager_is_box}
and {!Oct.manager_is_oct}
*)

(*  ********************************************************************** *)
(** {2:compilation Compiling and linking client programs against APRON} *)
(*  ********************************************************************** *)

(**

To make things clearer, we assume an example file
[mlexample.ml] which uses both NewPolka (convex
polyhedra) and Box (intervals) libraries, in their
versions where rationals are GMP rationals (which is the default).
We assume that C and OCaml interface and library files are located
in directory $APRON/lib.

The native-code compilation command looks like

{[ ocamlopt -I $APRON/lib -o mlexample.opt \
   bigarray.cmxa gmp.cmxa apron.cmxa boxMPFR.cmxa polkaMPQ.cmxa mlexample.ml]}
Comments:
{ol
{- You need at least the libraries [bigarray] (standard
   OCaml distribution), [gmp], and [apron]
   (standard APRON distribution), plus the one implementing an
   effective abstract domains: here, [boxMPFR], and
   [polkaMPQ].}
{- The C libraries associated to those OCaml libraries
   (e.g., [gmp_caml], [boxMPFR_caml], \ldots)
   are automatically looked for, as well as the the libraries
   implementing abstract domains (e.g., [polkaMPQ],
   [boxMPFR]).

   If other versions of abstract domains library are wanted, you
   should use the [-noautolink] option as explained below.}
{- If in [Makefile.config], the [HAS_SHARED]
  variable is set to a non-empty value, dynamic versions of those
  libraires are also available, but makes sure that all the needed
  libraries are in the dynamic search path indicated by
  [$LD_LIBRARY_PATH].}
}

If dynamic libraries are available, the byte-code compilation
process looks like

{[ ocamlc -I $MLGMPIDL/lib -I $APRON/lib -o mlexample.byte \
   bigarray.cma gmp.cma apron.cma boxMPFR.cma polkaMPQ.cma mlexample.ml ]}
Comments:
{ol
{- The ocamlrun bytecode interpreter will automatically load
  the dynamic libraries, using environment variables
  [$LD_LIBRARY_PATH] (and possibly
  [$CAML_LD_LIBRARY_PATH], see OCaml documentation,
  section on OCaml/C interface).}
{- You can very easily use the interactive toplevel
  interpreter: type ['ocaml -I $MLGMPIDL/lib -I $APRON/lib']
  and then enter:

{[    #load "bigarray.cma";;
    #load "gmp.cma";;
    #load "apron.cma";;
    #load "polkaMPQ.cma";;
    ... ]}
}
{- This is also the only way to load and use in the OCaml
  debugger pretty-printers depending on C code, like

{[    #load "bigarray.cma";;
    #load "gmp.cma";;
    #load "apron.cma";;

    #installl_printer Apron.Abstract1.print;; ]}
}
}

If only static libraries are available, you can:
{ol
{- Create a custom runtime and use it as follows:

{[     ocamlc -I $MLGMPIDL/lib -I $APRON/lib -make-runtime -o myrun \
       bigarray.cma gmp.cma apron.cma boxMPFR.cma polkaMPQ.cma

     ocamlc -I $MLGMPIDL/lib -I $APRON/lib -use-runtime myrun -o \mlexample.byte \
       bigarray.cma gmp.cma apron.cma box.cma polka.cma mlexample.ml ]}
  Comments:
  {ol
  {- One first build a custom bytecode interpreter that
    includes the new native-code needed; }
  {- One then compile the [mlexample.ml] file, using the
    generated bytecode interpreter. }
  }
}
{- If you want to use the interactive toplevel interpreter, you
  have to generate a custom toplevel interpreter using the
  [ocamlmktop] command (see OCaml documentation, section on
  OCaml/C interface):

{[     ocamlmktop -I $MLGMPIDL/lib -I $APRON/lib -o mytop \
       bigarray.cma gmp.cma apron.cma boxMPFR.cma polkaMPQ.cma ]}
}
}

The automatic search for C libraries associated to these
OCaml libraries can be disabled by the option
[-noautolink] supported by both [ocamlc] and
[ocamlopt] commands. For instance, the command for
native-code compilation can alternatively looks like:

{[  ocamlopt -I $MLGMPIDL/lib -I $APRON/lib -noautolink -o mlexample.opt \
    bigarray.cmxa gmp.cmxa apron.cmxa boxMPFR.cmxa polkaMPQ.cmxa mlexample.ml \
    -cclib "-L$MLGMPIDL/lib -L$APRON/lib \
	    -lpolkaMPQ_caml_debug -lpolkaMPQ_debug \
	    -lboxMPFR_caml_debug -lboxMPFR_debug \
	    -lapron_caml_debug -lapron_debug \
	    -lgmp_caml -L$MPFR -lmpfr -L$GMP/lib -lgmp \
	    -L$CAMLIDL/lib/ocaml -lcamlidl \
	    -lbigarray" ]}
or more simply, if dynamic libraries are available (because some
dynamic libraries are automatically referenced by others):

{[  ocamlopt -I $MLGMPIDL/lib -I $APRON/lib -noautolink -o mlexample.opt \
    bigarray.cmxa gmp.cmxa apron.cmxa boxMPFR.cmxa polkaMPQ.cmxa mlexample.ml \
    -cclib "-L$MLGMPIDL/lib -L$APRON/lib \
	    -lpolkaMPQ_caml_debug \
	    -lboxMPFR_caml_debug \
	    -lapron_caml_debug \
	    -lgmp_caml \
	    -lbigarray" ]}
This is mandatory if you want to use non-default versions of
libraries (here, debug versions).

The option [-verbose] helps to understand what is happening
in case of problem.

More details are given in the modules implementing a specific
abstract domain.
*)
