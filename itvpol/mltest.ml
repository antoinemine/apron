(* mltest.ml
 * 
 * A very simple example
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *)


(* compile with:

ocamlc -I ${INSTALL_DIR}/lib \
   gmp.cma apron.cma itvp.cma mltest.ml \
   -cclib -litvpQg -cclib -lapron

you can change:
- INSTALL_DIR to where APRON is installed,
- -litvpQg to the desired underlying numeric type,
- ocamlc to ocamlopt, and replace each .cma to the corresponding .cmxa

*)

open Apron

let m = Itvp.manager_alloc ()
let _ = 
  Printf.printf "name: %s\nversion: %s\n" 
    (Manager.get_library m) (Manager.get_version m)

(* TO BE CONTINUED *)
