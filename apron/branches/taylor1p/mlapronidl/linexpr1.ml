(* File generated from linexpr1.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable linexpr0: Linexpr0.t;
  mutable env: Environment.t;
}

external get_coeff : t -> Var.t -> Coeff.t
	= "camlidl_linexpr1_ap_linexpr1_get_coeff"

external set_coeff : t -> Var.t -> Coeff.t -> unit
	= "camlidl_linexpr1_ap_linexpr1_set_coeff"

external extend_environment : t -> Environment.t -> t
	= "camlidl_linexpr1_ap_linexpr1_extend_environment"

external extend_environment_with : t -> Environment.t -> unit
	= "camlidl_linexpr1_ap_linexpr1_extend_environment_with"

external is_integer : t -> bool
	= "camlidl_linexpr1_ap_linexpr1_is_integer"

external is_real : t -> bool
	= "camlidl_linexpr1_ap_linexpr1_is_real"


let make ?(sparse=true) env = {
linexpr0 = Linexpr0.make
(if sparse
then None
else Some (Environment.size env));
env = env;
}
let minimize e = Linexpr0.minimize e.linexpr0
let copy e = {
linexpr0 = Linexpr0.copy e.linexpr0;
env = e.env;
}
let get_cst expr =
Linexpr0.get_cst expr.linexpr0
let get_linexpr0 expr = expr.linexpr0
let get_env expr = expr.env
let set_cst expr cst =
Linexpr0.set_cst expr.linexpr0 cst
let set_list expr list ocst = 
List.iter
(fun (coeff,var) -> set_coeff expr var coeff )
list;
begin match ocst with
| Some cst -> set_cst expr cst
| None -> ()
end;
()
let set_array expr tab ocst = 
Array.iter
(fun (coeff,var) -> set_coeff expr var coeff )
tab;
begin match ocst with
| Some cst -> set_cst expr cst
| None -> ()
end;
()

let iter f expr =
Linexpr0.iter
(begin fun coeff dim ->
f coeff (Environment.var_of_dim expr.env dim)
end)
expr.linexpr0
let print fmt expr =
Linexpr0.print
(fun dim -> Var.to_string (Environment.var_of_dim expr.env dim))
fmt expr.linexpr0
 
