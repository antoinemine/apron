(* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution. 
*)

(*
with default setting (if dynamic libraries):
ocaml -I $MLGMPIDL_INSTALL/lib -I $APRON_INSTALL/lib

#load "bigarray.cma";;
#load "gmp.cma";;
#load "apron.cma";;
#load "octD.cma";;

#install_printer Apron.Linexpr1.print;;
#install_printer Apron.Texpr1.print;;
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

let man = Oct.manager_alloc ();;

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
  let tab =
    Parser.lincons1_of_lstring
      env
      ["1/2x+2/3y=1";
      "[1;2]<=z+2w";"z+2w<=4";
      "0<=u";"u<=5"]
  in
  printf "tab = %a@." lincons1_array_print tab;

  let abs = Abstract1.of_lincons_array man env tab in
  printf "abs=%a@." Abstract1.print abs;

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
  let abs2 =
    Abstract1.of_box man env
      [|var_x; var_y; var_z; var_w; var_u; var_v; var_a; var_b|]
      box.Abstract1.interval_array
  in
  printf "abs2=%a@." Abstract1.print abs2;
  (* 4. Tests top and bottom *)
  let abs3 = Abstract1.bottom man env in
  printf "abs3=%a@.is_bottom(abs3)=%b@."
    Abstract1.print abs3
    (Abstract1.is_bottom man abs3);

  printf "abs=%a@." Abstract1.print abs;
  let p2 = Abstract1.expand man abs
    var_y [|Var.of_string "y1"; Var.of_string "y2"|]
  in
  printf "p2=expand(abs,y,[y1,y2]))=%a@." Abstract1.print p2;
  let p2 = Abstract1.expand man abs
    var_u [|Var.of_string "u1"; Var.of_string "u2"|]
  in
  printf "p2=expand(abs,u,[u1,u2]))=%a@." Abstract1.print p2;

  (* Tree expressions *)
  let texpr = Parser.texpr1_of_string env "a + (x*y*y/sqrt(b))" in
  let abs2 = Abstract1.assign_texpr man abs var_u texpr None in
  printf "abs2=%a@." Abstract1.print abs2;
  abs
;;

let ex2 (man:'a Manager.t) =
  let env = Environment.make
    [||]
    [|var_x; var_y; var_z|]
  in
  (* Creation of abstract value
     5<=x<=14, 4<=y<=12, z=0 *)
  let abs1 = Abstract1.of_box man env [|var_x;var_y;var_z|]
    [|
      Interval.of_int 5 14;
      Interval.of_int 4 12;
      Interval.of_int 0 0;
    |]
  in
  let abs2 = Abstract1.of_box man env [|var_x;var_y;var_z|]
    [|
      Interval.of_int 3 12;
      Interval.of_int 5 13;
      Interval.of_int 1 1;
    |]
  in
  let abs3 = Abstract1.join man abs1 abs2 in
  abs3
;;

(* Comparing join of two different assignements and assignement by the "join"
   of expressions *)
let ex3 (man:'a Manager.t) =
  let env = Environment.make
    [||]
    [|var_x; var_y; var_z|]
  in
  (* Creation of abstract value
     -3<=x<=-2, 10<=y<=12, -1<=z<=1 *)
  let abs = Abstract1.of_box man env [|var_x;var_y;var_z|]
    [|
      Interval.of_int (-3) (-2);
      Interval.of_int 10 12;
      Interval.of_int (-1) (1)
    |]
  in
  (* Creation of linear expressions *)
  let linexpr1 = Parser.linexpr1_of_string env "z+x+2y" in
  let linexpr2 = Parser.linexpr1_of_string env "z+2x+y" in

  let abs1 = Abstract1.assign_linexpr man abs var_z linexpr1 None in
  let abs2 = Abstract1.assign_linexpr man abs var_z linexpr2 None in
  let res1 = Abstract1.join man abs1 abs2 in
  printf "abs=%a@.abs1=%a@.abs2=%a@.res1=%a@."
    Abstract1.print abs
    Abstract1.print abs1
    Abstract1.print abs2
    Abstract1.print res1;
  (* Creation of linear expression [1,2]y and [1,2]z *)
  let linexpr = Parser.linexpr1_of_string env "z + [1;2]x + [1;2]y" in
  let res2 = Abstract1.assign_linexpr man abs var_z linexpr None in
  printf "res2=%a@."
    Abstract1.print res2
  ;
  let abs1 = Abstract1.substitute_linexpr man res1 var_z linexpr1 None in
  let abs2 = Abstract1.substitute_linexpr man res1 var_z linexpr2 None in
  let res1 = Abstract1.join man abs1 abs2 in
  printf "abs1=%a@.abs2=%a@.res1=%a@."
    Abstract1.print abs1
    Abstract1.print abs2
    Abstract1.print res1
  ;
  let res2 = Abstract1.substitute_linexpr man res2 var_z linexpr None in
  printf "res2=%a@."
    Abstract1.print res2
  ;
  res1
;;

let abs1 = ex1 man;;
let abs2 = ex2 man;;
let abs3 = ex3 man;;
