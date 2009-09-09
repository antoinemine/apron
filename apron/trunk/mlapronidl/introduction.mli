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
{- {!module: Ppl}:  PPL convex polyhedra and linear congruences domains}
{- {!module: PolkaGrid}: reduced product of convex polyhedra and PPL linear congruences}
}
}
{- {b Level 1 of the interface (user-level)}: manipulation of generic datatypes (expressions, constraints, ..., and generic abstract domain interface)}
{- {b Level 0 of the interface (lower-level)}: manipulation of generic datatypes (expressions, constraints, ..., and generic abstract domain interface)}
}

The package also requires the [MLGMPIDL] wrapper to GMP and
MPFR libraries.

{2 Requirements}

{ul
{- APRON library}
{- GMP library version 4.2 or up (tested with version 4.2.1 and 4.3.1)}
{- MPFR library version 2.2 or up (tested with version 2.2.1 and 2.3.1)}
{- OCaml 3.09 or up}
{- Camlidl (tested with 1.05)}
}

For compiling from repository (strongly recommended):
{ul
{- GNU M4 preprocessor}
{- GNU sed}
}
It is important to have the GNU versions !

{2 Installation}

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

{2:compilation Compilation and Linking}

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
