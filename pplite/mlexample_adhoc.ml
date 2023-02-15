 (* This file is part of the APRON Library,
   released under GPL license (use of PPLite).
   Please read the COPYING file packaged in the distribution. *)

(* Testing a couple of PPLite's ad-hoc functions. *)

open Apron;;
open Format;;

let assoc = function
| 0 -> "x"
| _ -> "y"
;;

let print_array = Abstract0.print_array;;

let print_lincons0 fmt x = Lincons0.print assoc fmt x;;
let print_abstract0 fmt a = Abstract0.print assoc fmt a;;

let var_x = Var.of_string "x";;
let var_y = Var.of_string "y";;
let env = Environment.make [||] [|var_x; var_y|];;


let test_split man integral strict =
  (* Creation of abstract value 0 <= x <= 4, 0 <= y <= 4 *)
  let tab =
    Parser.lincons1_of_lstring
      env
      ["0 <= x";
       "x <= 4";
       "0 <= y";
       "y <= 4"]
  in let abs1 = Abstract1.of_lincons_array man env tab
  in let abs0 = abs1.abstract0
  in let lc1_array = Parser.lincons1_of_lstring env ["2 <= x"]
  in let lc1 = Lincons1.array_get lc1_array 0
  in let lc0 = lc1.lincons0
  in let split_kind integral strict =
    if integral then
      "integral"
    else
      if strict then
        "rational (strict)"
      else
        "rational (non-strict)"
  in
  print_string (split_kind integral strict);
  printf " split on constraint %a@." print_lincons0 lc0;
  printf "  abs0 before split = %a@." print_abstract0 abs0;
  let abs0_else = Pplite.abstract0_split man abs0 lc0 integral strict in
  printf "  abs0 after split = %a@." print_abstract0 abs0;
  printf "  abs0_else = %a@.\n" print_abstract0 abs0_else;
  ()
;;

let test_pset man =
  (* Creation of abstract value 0 <= x <= 4, 0 <= y <= 4 *)
  let tab1 =
    Parser.lincons1_of_lstring
      env
      ["0 <= x";
       "x <= 4";
       "0 <= y";
       "y <= 4"]
  in let abs1_1 = Abstract1.of_lincons_array man env tab1
  in let tab2 =
    Parser.lincons1_of_lstring
      env
      ["3 <= x";
       "x <= 7";
       "3 <= y";
       "y <= 7"]
  in let abs1_2 = Abstract1.of_lincons_array man env tab2
  in let abs1 = Abstract1.join man abs1_1 abs1_2
  in let abs0 = abs1.abstract0
  in let is_disj = Pplite.abstract0_is_disjunctive man abs0
  in let num_disj = Pplite.abstract0_num_disjuncts man abs0
  in let disj0_cs = Pplite.abstract0_disj_to_lincons_array man abs0 0
  in let disj1_cs = Pplite.abstract0_disj_to_lincons_array man abs0 1
  in
  printf "abs1 manager is disjunctive: %B\n" is_disj;
  printf "abs1 num disjuncts: %d" num_disj;
  print_newline ();
  printf "abs1 1st disjunct lincons: %a@."
    (print_array print_lincons0) disj0_cs;
  printf "abs1 2nd disjunct lincons: %a@."
    (print_array print_lincons0) disj1_cs;
  ()
;;


let man_loose = Pplite.manager_alloc_loose ();;
let _ = test_split man_loose true false;;
let _ = test_split man_loose false false;;

let man_strict = Pplite.manager_alloc_strict ();;
let _ = test_split man_strict false true;;

let _ = Pplite.manager_set_kind man_loose "P_Set";;
let _ = test_pset man_loose;;
