open Apron;;
open Mpqf;;
open Format;;

let man = T1p.manager_alloc ();;

let var_t1 = Var.of_string "t1";;
let var_t2 = Var.of_string "t2";;
let var_t3 = Var.of_string "t3";;
let var_x = Var.of_string "x";;

let env = Environment.make
    [|var_t1; var_t2; var_t3; var_x|]
    [||]
;;
let abs = Abstract1.of_box man env [|var_t1;var_t2;var_t3|]
[|
    Interval.of_int (-1) 1;
    Interval.of_int (-1) 1;
    Interval.of_int (-1) 1;
|]
;;

let exp1 = Parser.linexpr1_of_string env "t1+t3+t2";;
let abs1 = Abstract1.assign_linexpr man abs var_x exp1 None
;;

let exp2 = Parser.linexpr1_of_string env "t1+t3-t2";;
let abs2 = Abstract1.assign_linexpr man abs var_x exp2 None
;;

let tab = Parser.lincons1_of_lstring env ["1/2t1+2/3t2<=0"];;
let abs3 = Abstract1.meet_lincons_array man abs1 tab
;;

let tab1 = Parser.lincons1_of_lstring env ["-1/2t1-t3<=0"];;
let abs4 = Abstract1.meet_lincons_array man abs2 tab1
;;

let abs5 = Abstract1.join man abs3 abs4
in
printf "join=%a@." Abstract1.print abs5
;;

