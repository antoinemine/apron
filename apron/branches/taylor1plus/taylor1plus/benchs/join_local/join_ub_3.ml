open Apron;;
open Mpqf;;
open Format;;
let man = T1p.manager_alloc ();;
let var_t1 = Var.of_string "t1";;
let var_t2 = Var.of_string "t2";;
let var_x = Var.of_string "x";;
let env = Environment.make
[||]
[|var_t1;var_t2;var_x|];;
let abs = Abstract1.of_box man env [|var_t1;var_t2|]
[|
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
|];;
let exp1 = Parser.texpr1_of_string env "-0.561192 ";;
let abs1 = Abstract1.assign_texpr man abs var_x exp1 None;;
let exp2 = Parser.texpr1_of_string env "-0.484997 + 0.120536*t1";;
let abs2 = Abstract1.assign_texpr man abs var_x exp2 None;;
let tab = Parser.tcons1_of_lstring env ["t1 <= 0.674122"; "t1 >= -0.262437";"t2 <= 0.062000"; "t2 >= -0.129787";];;
let abs3 = Abstract1.meet_tcons_array man abs1 tab;;
let tab1 = Parser.tcons1_of_lstring env ["t1 <= 0.529718"; "t1 >= 0.361832";"t2 <= 0.745132"; "t2 >= -0.761573";];;
let abs4 = Abstract1.meet_tcons_array man abs2 tab1;;
let abs5 = Abstract1.join man abs4 abs3;;
let res1 = Abstract1.is_leq man abs3 abs5;;
let res2 = Abstract1.is_leq man abs4 abs5;;

printf "
%B and %B = %B
" res1 res2 (res1 && res2);;
