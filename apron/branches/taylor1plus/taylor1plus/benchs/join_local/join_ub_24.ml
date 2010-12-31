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
let var_t13 = Var.of_string "t13";;
let var_t14 = Var.of_string "t14";;
let var_t15 = Var.of_string "t15";;
let var_t16 = Var.of_string "t16";;
let var_t17 = Var.of_string "t17";;
let var_t18 = Var.of_string "t18";;
let var_t19 = Var.of_string "t19";;
let var_t20 = Var.of_string "t20";;
let var_t21 = Var.of_string "t21";;
let var_t22 = Var.of_string "t22";;
let var_t23 = Var.of_string "t23";;
let var_x = Var.of_string "x";;
let env = Environment.make
[||]
[|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11;var_t12;var_t13;var_t14;var_t15;var_t16;var_t17;var_t18;var_t19;var_t20;var_t21;var_t22;var_t23;var_x|];;
let abs = Abstract1.of_box man env [|var_t1;var_t2;var_t3;var_t4;var_t5;var_t6;var_t7;var_t8;var_t9;var_t10;var_t11;var_t12;var_t13;var_t14;var_t15;var_t16;var_t17;var_t18;var_t19;var_t20;var_t21;var_t22;var_t23|]
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
let exp1 = Parser.texpr1_of_string env "0.339310 + 0.646886*t13+ 0.864646*t18+ 0.482143*t5+ 0.801062*t5+ -0.540665*t10+ -0.684635*t8+ -0.465644*t21+ -0.733111*t19+ 0.914469*t3+ -0.694183*t13+ 0.086817*t11+ -0.344999*t1+ 0.449354*t22+ -0.972933*t6+ 0.071439*t3+ 0.592415*t2+ 0.004071*t10+ 0.242329*t18+ -0.285679*t6+ -0.162907*t20+ -0.939482*t13+ 0.939252*t16";;
let abs1 = Abstract1.assign_texpr man abs var_x exp1 None;;
let exp2 = Parser.texpr1_of_string env "0.482960 + 0.632998*t5+ -0.871643*t5+ -0.984281*t10+ -0.881573*t1+ 0.513666*t7+ -0.502399*t9+ -0.791507*t4+ -0.357138*t12+ 0.424172*t12+ 0.226353*t7+ -0.549220*t10+ 0.886612*t12";;
let abs2 = Abstract1.assign_texpr man abs var_x exp2 None;;
let tab = Parser.tcons1_of_lstring env ["t1 <= 0.200516"; "t1 >= -0.926766";"t2 <= 0.296764"; "t2 >= -0.609861";"t3 <= 0.864301"; "t3 >= -0.190570";"t4 <= 0.792239"; "t4 >= 0.139993";"t5 <= 0.735400"; "t5 >= -0.711684";"t6 <= 0.187149"; "t6 >= -0.956596";"t7 <= -0.071199"; "t7 >= -0.879969";"t8 <= -0.733404"; "t8 >= -0.839308";"t9 <= 0.102494"; "t9 >= -0.554728";"t10 <= -0.065102"; "t10 >= -0.331272";"t11 <= -0.256082"; "t11 >= -0.418550";"t12 <= 0.906400"; "t12 >= 0.359082";"t13 <= 0.255186"; "t13 >= 0.077031";"t14 <= 0.484042"; "t14 >= -0.063747";"t15 <= 0.527450"; "t15 >= -0.497388";"t16 <= 0.666235"; "t16 >= -0.471939";"t17 <= 0.158073"; "t17 >= -0.630667";"t18 <= -0.389878"; "t18 >= -0.699319";"t19 <= 0.054747"; "t19 >= -0.656478";"t20 <= 0.180756"; "t20 >= -0.337107";"t21 <= 0.125275"; "t21 >= -0.196926";"t22 <= -0.159840"; "t22 >= -0.242739";"t23 <= 0.061663"; "t23 >= -0.764085";];;
let abs3 = Abstract1.meet_tcons_array man abs1 tab;;
let tab1 = Parser.tcons1_of_lstring env ["t1 <= 0.869710"; "t1 >= 0.120446";"t2 <= 0.124884"; "t2 >= -0.948331";"t3 <= -0.280316"; "t3 >= -0.897446";"t4 <= -0.805338"; "t4 >= -0.900873";"t5 <= 0.506603"; "t5 >= 0.440554";"t6 <= -0.102784"; "t6 >= -0.663915";"t7 <= -0.063718"; "t7 >= -0.981731";"t8 <= 0.457845"; "t8 >= -0.684486";"t9 <= 0.532614"; "t9 >= -0.214682";"t10 <= 0.419848"; "t10 >= -0.046753";"t11 <= 0.743730"; "t11 >= -0.647599";"t12 <= 0.070077"; "t12 >= -0.041022";"t13 <= 0.857803"; "t13 >= 0.831942";"t14 <= 0.955849"; "t14 >= 0.373698";"t15 <= -0.435028"; "t15 >= -0.490520";"t16 <= -0.754946"; "t16 >= -0.962072";"t17 <= 0.419690"; "t17 >= -0.996237";"t18 <= -0.226260"; "t18 >= -0.716000";"t19 <= 0.664083"; "t19 >= -0.003929";"t20 <= 0.047129"; "t20 >= -0.190554";"t21 <= -0.042244"; "t21 >= -0.139352";"t22 <= 0.142026"; "t22 >= -0.803333";"t23 <= 0.844450"; "t23 >= -0.761250";];;
let abs4 = Abstract1.meet_tcons_array man abs2 tab1;;
let abs5 = Abstract1.join man abs4 abs3;;
let res1 = Abstract1.is_leq man abs3 abs5;;
let res2 = Abstract1.is_leq man abs4 abs5;;

printf "
%B and %B = %B
" res1 res2 (res1 && res2);;
