(* File generated from lincons0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = {
  mutable linexpr0 : Linexpr0.t;
  mutable typ : typ;
}


and typ =
  | EQ
  | SUPEQ
  | SUP
  | DISEQ
  | EQMOD of Scalar.t


let string_of_typ = function
| EQ | EQMOD _ -> "="
| SUPEQ -> ">="
| SUP -> ">"
| DISEQ -> "<>"

let print assoc fmt cons = 
Linexpr0.print assoc fmt cons.linexpr0;
Format.fprintf fmt "%s0" (string_of_typ cons.typ);
begin match cons.typ with
| EQMOD x -> Format.fprintf fmt " mod %a" Scalar.print x;
| _ -> ()
end;
()
let make expr typ = {
linexpr0 = expr; typ = typ 
}
let copy cons = {
linexpr0 = Linexpr0.copy cons.linexpr0; typ = cons.typ
}

