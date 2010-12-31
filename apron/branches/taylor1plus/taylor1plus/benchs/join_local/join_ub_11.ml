open Apron;;
open Mpqf;;
open Format;;
let man = T1p.manager_alloc ();;
let var_t1 = Var.of_string "t1";;
let var_t2 = Var.of_string "t2";;
let var_t3 = Var.of_string "t3";;
let var_t4 = Var.of_string "t4";;
let var_t5 = Var.of_string "t5";;
let var_t6 = Var.of_string "t6";;
let var_t7 = Var.of_string "t7";;
let var_t8 = Var.of_string "t8";;
let var_t9 = Var.of_string "t9";;
let var_t10 = Var.of_string "t10";;
let var_t11 = Var.of_string "t11";;
let var_x = Var.of_string "x";;
let env = Environment.make
[||]
[|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11;var_x|];;
let abs = Abstract1.of_box man env [|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11|]
[|
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
Interval.of_int (-1) 1;
|];;
let exp1 = Parser.texpr1_of_string env "-0.700746 + -0.998205*t1+ 0.935959*t2+ 0.173514*t3+ 0.358239*t4+ -0.045999*t5+ 0.911521*t6+ 0.971192*t7+ -0.234461*t8+ 0.571085*t9+ 0.404021*t10+ 0.474401*t11";;
let abs1 = Abstract1.assign_texpr man abs var_x exp1 None;;
let exp2 = Parser.texpr1_of_string env "0.714724 + -0.692715*t1+ 0.657338*t2+ -0.515681*t3+ -0.483080*t4+ -0.663297*t5+ -0.976898*t6+ -0.928425*t7+ 0.340808*t8+ 0.485008*t9+ 0.187719*t10+ 0.089559*t11";;
let abs2 = Abstract1.assign_texpr man abs var_x exp2 None;;
let tab = Parser.tcons1_of_lstring env ["t1 <= 0.010974"; "t1 >= -0.077504";"t2 <= 0.053868"; "t2 >= -0.175294";"t3 <= 0.866492"; "t3 >= -0.102415";"t4 <= 0.891719"; "t4 >= -0.554739";"t5 <= -0.426045"; "t5 >= -0.795773";"t6 <= 0.674579"; "t6 >= -0.872441";"t7 <= 0.605412"; "t7 >= -0.840592";"t8 <= 0.191553"; "t8 >= -0.793427";"t9 <= 0.432345"; "t9 >= -0.221407";"t10 <= 0.903367"; "t10 >= -0.111975";"t11 <= -0.148617"; "t11 >= -0.816590";];;
let abs3 = Abstract1.meet_tcons_array man abs1 tab;;
let tab1 = Parser.tcons1_of_lstring env ["t1 <= -0.366272"; "t1 >= -0.498918";"t2 <= 0.988471"; "t2 >= -0.221760";"t3 <= 0.873255"; "t3 >= 0.704450";"t4 <= -0.546121"; "t4 >= -0.767531";"t5 <= 0.598290"; "t5 >= -0.051125";"t6 <= 0.590318"; "t6 >= -0.299862";"t7 <= 0.660471"; "t7 >= 0.417034";"t8 <= -0.440196"; "t8 >= -0.467192";"t9 <= 0.247706"; "t9 >= -0.144295";"t10 <= -0.708024"; "t10 >= -0.926083";"t11 <= 0.553704"; "t11 >= 0.125325";];;
let abs4 = Abstract1.meet_tcons_array man abs2 tab1;;
let abs5 = Abstract1.join man abs4 abs3;;
let res1 = Abstract1.is_leq man abs3 abs5;;
let res2 = Abstract1.is_leq man abs4 abs5;;

printf "
%B and %B = %B
" res1 res2 (res1 && res2);;
