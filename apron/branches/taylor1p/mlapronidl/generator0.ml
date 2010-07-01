(* File generated from generator0.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type typ =
  | LINE
  | RAY
  | VERTEX
  | LINEMOD
  | RAYMOD
and t = {
  mutable linexpr0: Linexpr0.t;
  mutable typ: typ;
}



let string_of_typ = function
| LINE -> "LIN"
| RAY -> "RAY"
| VERTEX -> "VTX"
| LINEMOD -> "LINMOD"
| RAYMOD -> "RAYMOD"

let print assoc fmt gen = 
Format.fprintf fmt "%s:" (string_of_typ gen.typ);
Linexpr0.print assoc fmt gen.linexpr0;
()
let make expr typ = {
linexpr0 = expr; typ = typ;
}
let copy gen = {
linexpr0 = Linexpr0.copy gen.linexpr0; typ = gen.typ;
}

