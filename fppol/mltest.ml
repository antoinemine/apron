(* mltest.ml
 * 
 * A very simple example
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008
 *)


(* compile with:

ocamlc -I ${INSTALL_DIR}/lib \
   gmp.cma apron.cma fpp.cma mltest.ml \
   -cclib -lfppQg -cclib -lapron

you can change:
- INSTALL_DIR to where APRON is installed,
- -lfppQg to the desired underlying numeric type,
- ocamlc to ocamlopt, and replace each .cma to the corresponding .cmxa

*)

open Apron

let m = Fpp.manager_alloc ()
let _ = 
  Printf.printf "name: %s\nversion: %s\n" 
    (Manager.get_library m) (Manager.get_version m)

(* TO BE CONTINUED *)
