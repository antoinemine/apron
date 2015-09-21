(* This file is part of the APRON Library, released under GPL license. Please read the COPYING file packaged in the distribution. *)

(*
with default setting:

apronppltop -I $MLGMPIDL_INSTALL/lib -I $APRON_INSTALL/lib

#load "bigarray.cma";;
#load "gmp.cma";;
#load "apron.cma";;
#load "boxMPQ.cma";;
#load "octMPQ.cma";;
#load "polkaMPQ.cma";;
#load "ppl.cma";;
#load "polkaGrid.cma";;

#install_printer Apron.Linexpr1.print;;
#install_printer Apron.Lincons1.print;;
#install_printer Apron.Generator1.print;;
#install_printer Apron.Abstract1.print;;

let environment_print fmt x = Apron.Environment.print fmt x;;
let lincons1_array_print fmt x = Apron.Lincons1.array_print fmt x;;
let generator1_array_print fmt x = Apron.Generator1.array_print fmt x;;

#install_printer Apron.Var.print;;
#install_printer environment_print;;
#install_printer lincons1_array_print;;
#install_printer generator1_array_print;;

*)

open Apron;;
open Mpqf;;
open Format;;

let print_array = Abstract0.print_array;;
let lincons1_array_print fmt x =
  Lincons1.array_print fmt x
;;
let generator1_array_print fmt x =
  Generator1.array_print fmt x
;;

let manpk = Polka.manager_alloc_strict();;
let manbox = Box.manager_alloc ();;
let manoct = Oct.manager_alloc ();;
let manppl = Ppl.manager_alloc_strict();;
let mangrid = Ppl.manager_alloc_grid ();;
let maneq = Polka.manager_alloc_equalities ();;
let manpkgrid = PolkaGrid.manager_alloc manpk mangrid;;

let var_x = Var.of_string "x";;
let var_y = Var.of_string "y";;
let var_z = Var.of_string "z";;
let var_w = Var.of_string "w";;
let var_u = Var.of_string "u";;
let var_v = Var.of_string "v";;
let var_a = Var.of_string "a";;
let var_b = Var.of_string "b";;


let ex1 (man:'a Manager.t) : 'a Abstract1.t =
  printf "Using Library: %s, version %s@." (Manager.get_library man) (Manager.get_version man);

  let env = Environment.make
    [|var_x; var_y; var_z; var_w|]
    [|var_u; var_v; var_a; var_b|]
  in
  let env2 = Environment.make [|var_x; var_y; var_z; var_w|] [||]
  in
  printf "env=%a@.env2=%a@."
    (fun x -> Environment.print x) env
    (fun x -> Environment.print x) env2
  ;
  (* Creation of abstract value
     1/2x+2/3y=1, [1,2]<=z+2w<=4, 0<=u<=5 *)
  let tab = Lincons1.array_make env 5 in

  let expr = Linexpr1.make env in
  Linexpr1.set_array expr
    [|
      (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 1 2)), var_x);
      (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_frac 2 3)), var_y)
    |]
    (Some (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (1)))))
    ;
  let cons = Lincons1.make expr Lincons1.EQ in
  Lincons1.array_set tab 0 cons;

  let expr = Linexpr1.make env in
  Linexpr1.set_array expr
    [|
      (Coeff.Scalar (Scalar.Float (-1.0)), var_z);
      (Coeff.Scalar (Scalar.Float (-2.0)), var_w)
    |]
    (Some (Coeff.Scalar (Scalar.Float (4.0))))
  ;
  Lincons1.array_set tab 1 (Lincons1.make expr Lincons1.SUPEQ);

  let expr = Linexpr1.make env2 in
  Linexpr1.set_array expr
    [|
      (Coeff.Scalar (Scalar.Float 1.0), var_z);
      (Coeff.Scalar (Scalar.Float 2.0), var_w)
    |]
    (Some
      (Coeff.Interval
	(Interval.of_infsup
	  (Scalar.Float (-2.0))
	  (Scalar.Float (-1.0)))))
    ;
  Linexpr1.extend_environment_with expr env;
  Lincons1.array_set tab 2 (Lincons1.make expr Lincons1.SUPEQ);

  let cons = Lincons1.make (Linexpr1.make env) Lincons1.SUPEQ in
  Lincons1.set_array cons
    [|
      (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 1)), var_u)
    |]
    None
  ;
  Lincons1.array_set tab 3 cons;
  let cons = Lincons1.make (Linexpr1.make env) Lincons1.SUPEQ in
  Lincons1.set_array cons
    [|
      (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int (-1))), var_u)
    |]
    (Some (Coeff.Scalar (Scalar.Mpqf (Mpqf.of_int 5))))
  ;
  Lincons1.array_set tab 4 cons;

  printf "tab = %a@." lincons1_array_print tab;

  let abs = Abstract1.of_lincons_array man env tab in
  printf "abs=%a@." Abstract1.print abs;
  let array = Abstract1.to_generator_array man abs in
  printf "gen=%a@." generator1_array_print array;
  let array = Abstract1.to_generator_array man abs in
  printf "gen=%a@." generator1_array_print array;

  (* Extraction (we first extract values for existing constraints, then for
     dimensions) *)
  let box = Abstract1.to_box man abs in
  printf "box=%a@." (print_array Interval.print) box.Abstract1.interval_array;
  for i=0 to 4 do
    let expr = Lincons1.get_linexpr1 (Lincons1.array_get tab i) in
    let box = Abstract1.bound_linexpr man abs expr in
    printf "Bound of %a = %a@."
      Linexpr1.print expr
      Interval.print box;
  done;
  (* 2. dimensions *)
  (* 3. of box *)
  let abs2 = Abstract1.of_box man env [|var_x; var_y; var_z; var_w; var_u; var_v; var_a; var_b|]
    box.Abstract1.interval_array
  in
  printf "abs2=%a@." Abstract1.print abs2;
  (* 4. Tests top and bottom *)
  let abs3 = Abstract1.bottom man env in
  printf "abs3=%a@.is_bottom(abs3)=%b@."
    Abstract1.print abs3
    (Abstract1.is_bottom man abs3);

  printf "abs=%a@."
    Abstract1.print abs;
  let p2 = Abstract1.expand man abs (var_y) [|Var.of_string "y1"; Var.of_string "y2"|] in
  printf "p2=expand(abs,y,[y1,y2]))=%a@."
    Abstract1.print p2;
  let p2 = Abstract1.expand man abs (var_u) [|Var.of_string "u1"; Var.of_string "u2"|] in
  printf "p2=expand(abs,u,[u1,u2]))=%a@."
    Abstract1.print p2;
  abs
;;

let abs1 = ex1 manpk;;
let abs2 = ex1 manppl;;
let abs3 = ex1 manoct;;
let abs4 = ex1 manbox;;
let abs5 = ex1 maneq;;
let abs6 = ex1 mangrid;;
