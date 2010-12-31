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
let var_t12 = Var.of_string "t12";;
let var_x = Var.of_string "x";;
let env = Environment.make
[||]
[|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11;var_t12;var_x|];;
let abs = Abstract1.of_box man env [|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11;var_t12|]
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
Interval.of_int (-1) 1;
|];;
let exp1 = Parser.texpr1_of_string env "0.214620 + -0.887327*t1";;
let abs1 = Abstract1.assign_texpr man abs var_x exp1 None;;
let exp2 = Parser.texpr1_of_string env "-0.047244 + -0.265803*t3+ -0.972181*t7+ -0.942775*t10+ -0.537087*t7+ 0.893353*t6+ -0.547085*t5+ -0.817740*t9+ 0.218732*t2+ -0.253781*t2+ 0.723017*t3+ -0.005596*t10";;
let abs2 = Abstract1.assign_texpr man abs var_x exp2 None;;
let tab = Parser.tcons1_of_lstring env ["t1 <= 0.333346"; "t1 >= -0.593355";"t2 <= -0.770815"; "t2 >= -0.805407";"t3 <= -0.236040"; "t3 >= -0.800411";"t4 <= 0.926560"; "t4 >= -0.626785";"t5 <= 0.169205"; "t5 >= -0.701022";"t6 <= -0.456014"; "t6 >= -0.529175";"t7 <= 0.007954"; "t7 >= -0.424946";"t8 <= 0.830472"; "t8 >= -0.609702";"t9 <= 0.243493"; "t9 >= -0.189901";"t10 <= 0.643824"; "t10 >= -0.885823";"t11 <= 0.566530"; "t11 >= 0.478807";"t12 <= 0.799435"; "t12 >= 0.303204";];;
let abs3 = Abstract1.meet_tcons_array man abs1 tab;;
let tab1 = Parser.tcons1_of_lstring env ["t1 <= 0.018652"; "t1 >= -0.410678";"t2 <= -0.388827"; "t2 >= -0.532902";"t3 <= 0.894339"; "t3 >= -0.656619";"t4 <= 0.577380"; "t4 >= -0.795285";"t5 <= -0.129264"; "t5 >= -0.336203";"t6 <= 0.782129"; "t6 >= 0.184123";"t7 <= -0.160725"; "t7 >= -0.820524";"t8 <= -0.538533"; "t8 >= -0.961738";"t9 <= 0.866055"; "t9 >= -0.807792";"t10 <= 0.480150"; "t10 >= -0.630215";"t11 <= 0.985777"; "t11 >= -0.971324";"t12 <= -0.483117"; "t12 >= -0.942316";];;
let abs4 = Abstract1.meet_tcons_array man abs2 tab1;;
let abs5 = Abstract1.join man abs4 abs3;;
let res1 = Abstract1.is_leq man abs3 abs5;;
let res2 = Abstract1.is_leq man abs4 abs5;;

printf "
%B and %B = %B
" res1 res2 (res1 && res2);;
