(* mltest.ml
 * 
 * A very simple example
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014
 *)


(* compile with:

ocamlc -I ${INSTALL_DIR}/lib \
   gmp.cma apron.cma avo.cma mltest.ml \
   -cclib -lavoQg -cclib -lapron

you can change:
- INSTALL_DIR to where APRON is installed,
- -lavoQg to the desired underlying numeric type,
- ocamlc to ocamlopt, and replace each .cma to the corresponding .cmxa

*)

open Apron

let m = Avo.manager_alloc ()
let _ = 
  Printf.printf "name: %s\nversion: %s\n" 
    (Manager.get_library m) (Manager.get_version m)

(* TO BE CONTINUED *)
