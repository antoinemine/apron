(* File generated from oct.idl *)

type internal

(*
 This file is part of the APRON Library, released under LGPL license.
 Please read the COPYING file packaged in the distribution.
*)

 
type t
(** Type of octagons.

Octagons are defined by conjunctions of inequalities of the form
[+/-x_i +/- x_j >= 0].

Abstract values which are octagons have the type [t Apron.AbstractX.t].

Managers allocated for octagons have the type [t Apron.manager.t].
*)

 
external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_oct_oct_manager_alloc"

external manager_get_internal : t Apron.Manager.t -> internal
	= "camlidl_oct_manager_get_internal"

external of_generator_array : t Apron.Manager.t -> int -> int -> Apron.Generator0.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_of_generator_array"

external widening_thresholds : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_widening_thresholds"

external narrowing : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_narrowing"

external add_epsilon : t Apron.Manager.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon"

external add_epsilon_bin : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon_bin"

let pre_widening = 99


let manager_is_oct man =
  let str = Apron.Manager.get_library man in
  (String.compare str "oct")==0
let manager_of_oct (man:t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_oct (man:'a Apron.Manager.t) : t Apron.Manager.t =
  if manager_is_oct man then 
    Obj.magic man
  else
    failwith "Oct.manager_to_oct: the argument manager is not an Oct manager"
module Abstract0 = struct
  let is_oct abs = 
    manager_is_oct (Apron.Abstract0.manager abs)
  let of_oct (abs: t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let to_oct (abs:'a Apron.Abstract0.t) : t Apron.Abstract0.t =
  if is_oct abs then 
    Obj.magic abs
  else
    failwith "Oct.Abstract0.to_oct: the argument value is not an oct value"
end
module Abstract1 = struct
  let is_oct abs = 
    manager_is_oct (Apron.Abstract1.manager abs)
  let of_oct (abs: t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let to_oct (abs:'a Apron.Abstract1.t) : t Apron.Abstract1.t =
  if is_oct abs then 
    Obj.magic abs
  else
    failwith "Oct.Abstract1.to_oct: the argument value is not an oct value"
end

