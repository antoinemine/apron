(* File generated from environment.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type typvar =
  | INT
  | REAL
and t

(** APRON Environments binding dimensions to names *)
external make : Var.t array -> Var.t array -> t
	= "camlidl_environment_ap_environment_make"

external add : t -> Var.t array -> Var.t array -> t
	= "camlidl_environment_ap_environment_add"

external remove : t -> Var.t array -> t
	= "camlidl_environment_ap_environment_remove"

external rename : t -> Var.t array -> Var.t array -> t
	= "camlidl_environment_ap_environment_rename"

external rename_perm : t -> Var.t array -> Var.t array -> t * Dim.perm
	= "camlidl_environment_ap_environment_rename_perm"

external lce : t -> t -> t
	= "camlidl_environment_ap_environment_lce"

external lce_change : t -> t -> t * Dim.change option * Dim.change option
	= "camlidl_environment_ap_environment_lce_change"

external dimchange : t -> t -> Dim.change
	= "camlidl_environment_ap_environment_dimchange"

external dimchange2 : t -> t -> Dim.change2
	= "camlidl_environment_ap_environment_dimchange2"

external equal : t -> t -> bool
	= "camlidl_environment_ap_environment_equal"

external compare : t -> t -> int
	= "camlidl_environment_ap_environment_compare"

external hash : t -> int
	= "camlidl_environment_ap_environment_hash"

external dimension : t -> Dim.dimension
	= "camlidl_environment_ap_environment_dimension"

external size : t -> int
	= "camlidl_environment_ap_environment_size"

external mem_var : t -> Var.t -> bool
	= "camlidl_environment_ap_environment_mem_var"

external typ_of_var : t -> Var.t -> typvar
	= "camlidl_environment_ap_environment_typ_of_var"

external vars : t -> Var.t array * Var.t array
	= "camlidl_environment_ap_environment_vars"

external var_of_dim : t -> Dim.t -> Var.t
	= "camlidl_environment_ap_environment_var_of_dim"

external dim_of_var : t -> Var.t -> Dim.t
	= "camlidl_environment_ap_environment_dim_of_var"


(** Printing *)

let print
?(first=("[|@[":(unit,Format.formatter,unit) format))
?(sep = (";@ ":(unit,Format.formatter,unit) format))
?(last = ("@]|]":(unit,Format.formatter,unit) format))
(fmt:Format.formatter)
(env: t)
: unit
=
let dim = dimension env in
if dim.Dim.intd+dim.Dim.reald=0 then begin
Format.fprintf fmt first;
Format.fprintf fmt last;
end
else begin
Format.fprintf fmt first;
let first = ref true in
for i=0 to pred dim.Dim.intd do
let var = var_of_dim env i in
if !first then first := false else Format.fprintf fmt sep;
Format.fprintf fmt "%i> %s:int"
i (Var.to_string var)
done;
for i=dim.Dim.intd to dim.Dim.intd + (pred dim.Dim.reald) do
let var = var_of_dim env i in
if !first then first := false else Format.fprintf fmt sep;
Format.fprintf fmt "%i> %s:real"
i (Var.to_string var)
done;
Format.fprintf fmt last;
end

