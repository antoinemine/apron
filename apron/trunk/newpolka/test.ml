
(* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution. *)

(*
#load "camllib.cma";;

polkatopg -I $MLGMPIDL_INSTALL/lib -I $MLAPRONIDL_INSTALL/lib -I $CAMLLIB_INSTALL/lib

#load "gmp.cma";;
#load "apron.cma";;
#load "polka.cma";;
*)

open Apron;;
open Mpqf
open Format
;;

let assoc = function
| 0 -> "x"
| 1 -> "y"
| 2 -> "z"
| 3 -> "w"
| 4 -> "u"
| 5 -> "v"
| 6 -> "a"
| 7 -> "b"
;;

let print_array = Abstract0.print_array;;

let man = Polka.manager_alloc true;;
let maneq = Polkaeq.manager_alloc ();;


let print_linexpr0 fmt x = Linexpr0.print assoc fmt x;;
let print_lincons0 fmt x = Lincons0.print assoc fmt x;;
let print_generator0 = Generator0.print assoc;;
let print_abstract0 fmt a = Abstract0.print assoc fmt a;;
(*
  if Abstract0.is_bottom man a = Manager.True then
    Format.pp_print_string fmt "bottom"
  else if Abstract0.is_top man a = Manager.True then
    Format.pp_print_string fmt "top"
  else begin
    let tab = Abstract0.to_lincons_array man a in
    print_array (Lincons0.print assoc) fmt tab;
  end
*)

(*

#install_printer print_linexpr0;;
#install_printer print_lincons0;;
#install_printer print_generator0;;
#install_printer print_abstract0;;

*)

let mine1 man = 
  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 0 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-3))));
  Linexpr0.set_coeff expr 1 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-3))));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 14)));
  let tab = Array.make 1 (Lincons0.make expr Lincons0.SUPEQ) in
  let poly = Abstract0.of_lincons_array man 0 2 tab in
  printf "poly=%a@." print_abstract0 poly;
  let poly2 = Abstract0.expand man poly 1 2 in
  printf "poly=%a@." print_abstract0 poly2;
  ()
;;

let _ = mine1 man
;;

let mine2 man =
  let fopt = Manager.get_funopt man Manager.Funid_join in
  let fopt2  = { fopt with Manager.flag_exact_wanted = true; Manager.flag_best_wanted = true } in
  Manager.set_funopt man Manager.Funid_join fopt2;
  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 1 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (4))));
  Linexpr0.set_coeff expr 2 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (4))));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 17)));
  let tab = Array.make 1 (Lincons0.make expr Lincons0.SUPEQ) in
  let poly = Abstract0.of_lincons_array man 0 3 tab in
  printf "poly=%a@." print_abstract0 poly;
  let polye = Abstract0.bottom man 0 3 in
  let poly2 = Abstract0.join man polye poly in
  printf "poly2=%a@." print_abstract0 poly2;
  ()
;;
let _ = mine2 man
;;

let mine3 man =
  let tab = Array.make 2 (Lincons0.make (Linexpr0.make None) Lincons0.SUPEQ) in
  let expr = Linexpr0.make None in
  Linexpr0.set_list expr 
    [((Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-1)))),0)]
    (Some (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 1))));
  tab.(0) <- Lincons0.make expr Lincons0.SUPEQ;
  let expr = Linexpr0.make None in
  Linexpr0.set_list expr 
    [((Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (1)))),0)]
    (Some (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 1 4))));
  tab.(1) <- Lincons0.make expr Lincons0.SUPEQ;
  let poly = Abstract0.of_lincons_array man 0 2 tab in
  Abstract0.canonicalize man poly;
  printf "poly=%a@." print_abstract0 poly;
  let expr = Linexpr0.make None in
  Linexpr0.set_list expr 
    [((Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 5 2))),0)]
    (Some (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-1)))))
  ;
  let poly2 = Abstract0.assign_linexpr man poly 0 expr None in
  printf "poly2=%a@." print_abstract0 poly2;
  ()
;;
let _ = mine3 man
;;

let poly1 man =
  (* Creation du polyèdre
     1/2x+2/3y=1, [1,2]<=z+2w<=4, -2<=1/3z-w<=3,
     u non contraint *)
  let tab = Array.make 5 (Lincons0.make (Linexpr0.make None) Lincons0.EQ) in

  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 0 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 1 2)));
  Linexpr0.set_coeff expr 1 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 2 3)));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (1))));
  tab.(0) <- Lincons0.make expr Lincons0.EQ;

  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 2 (Coeff.Scalar (Scalar.Float 1.0));
  Linexpr0.set_coeff expr 3 (Coeff.Scalar (Scalar.Float 2.0));
  Linexpr0.set_cst expr (Coeff.Interval (
    Interval.of_infsup
    (Scalar.Float (-2.0))
    (Scalar.Float (-1.0))));
  tab.(1) <- Lincons0.make expr Lincons0.SUPEQ;

  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 2 (Coeff.Scalar (Scalar.Float (-1.0)));
  Linexpr0.set_coeff expr 3 (Coeff.Scalar (Scalar.Float (-2.0)));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Float (4.0)));
  tab.(2) <- Lincons0.make expr Lincons0.SUPEQ;

  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 2 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 1 3)));
  Linexpr0.set_coeff expr 3 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-1))));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Float (2.0)));
  tab.(3) <- Lincons0.make expr Lincons0.SUPEQ;

  let expr = Linexpr0.make None in
  Linexpr0.set_coeff expr 2 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac (-1) 3)));
  Linexpr0.set_coeff expr 3 (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 1)));
  Linexpr0.set_cst expr (Coeff.Scalar (Scalar.Float (3.0)));
  tab.(4) <- Lincons0.make expr Lincons0.SUPEQ;

  printf "tab = %a@." 
    (print_array print_lincons0) tab;


  let poly = Abstract0.of_lincons_array man 0 6 tab in
  printf "poly=%a@." print_abstract0 poly;
  let array = Abstract0.to_generator_array man poly in
  printf "gen=%a@." (print_array print_generator0) array;
  Abstract0.canonicalize man poly;
  printf "poly=%a@." print_abstract0 poly;
  let array = Abstract0.to_generator_array man poly in
  printf "gen=%a@." (print_array print_generator0) array;

  (* Extraction (we first extract values for existing constraints, then for
     dimensions) *)
  let titv = Abstract0.to_box man poly in
  printf "titv=%a@." (print_array Interval.print) titv;
  for i=0 to 4 do
    let itv = Abstract0.bound_linexpr man poly tab.(i).Lincons0.linexpr0 in
    printf "Bound of %a = %a@."
      print_linexpr0 tab.(i).Lincons0.linexpr0
      Interval.print itv;
  done;
  (* 2. dimensions *)
  (* 3. of box *)
  let poly2 = Abstract0.of_box man 0 6 titv in
  printf "poly2=%a@." print_abstract0 poly2;
  (* 4. Tests top and bottom *)
  let poly3 = Abstract0.bottom man 2 3 in
  printf "poly3=%a@.is_bottom(poly3)=%a@."
    print_abstract0 poly3 
    Manager.print_tbool (Abstract0.is_bottom man poly3);

  let p2 = Abstract0.expand man poly 2 2 in
  printf "p2=(expand(poly)=%a@."
    print_abstract0 p2;
  poly
;;

let p1 = poly1 man;;
