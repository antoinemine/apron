(* File generated from polkaGrid.idl *)


(** Reduced product of NewPolka polyhedra and PPL grids *)

(** Type of abstract values, where ['a] is [Polka.loose] or [Polka.strict]. *)
type 'a t

external manager_alloc : 'a Polka.t Apron.Manager.t -> Ppl.grid Ppl.t Apron.Manager.t -> 'a t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc"

external manager_decompose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Manager.t * Ppl.grid Ppl.t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_decompose"

external decompose : 'a t Apron.Abstract0.t -> 'a Polka.t Apron.Abstract0.t * Ppl.grid Ppl.t Apron.Abstract0.t
	= "camlidl_polkaGrid_ap_pkgrid_decompose"

external compose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Abstract0.t -> Ppl.grid Ppl.t Apron.Abstract0.t -> 'a t Apron.Abstract0.t
	= "camlidl_polkaGrid_ap_pkgrid_compose"


let manager_is_polkagrid man =
  let str = Apron.Manager.get_library man in
  let str =
    try String.sub str 0 9
    with Invalid_argument _ -> ""
  in
  (String.compare str "polkagrid")==0
let manager_of_polkagrid (man:'a t Apron.Manager.t) : 'b Apron.Manager.t = Obj.magic man
let manager_to_polkagrid (man:'a Apron.Manager.t) : 'b t Apron.Manager.t =
  if manager_is_polkagrid man then
    Obj.magic man
  else
    failwith "PolkaGrid.to_polkagrid: the argument manager is not a polkagrid manager"

module Abstract0 = struct
  let is_polkagrid abs =
    manager_is_polkagrid (Apron.Abstract0.manager abs)
  let of_polkagrid (abs: 'a t Apron.Abstract0.t) : 'b Apron.Abstract0.t = Obj.magic abs
  let to_polkagrid (abs:'a Apron.Abstract0.t) : 'b t Apron.Abstract0.t =
    if is_polkagrid abs then
      Obj.magic abs
    else
      failwith "PolkaGrid.Abstract0.to_polkagrid: the argument value is not a polkagrid value"
end

module Abstract1 = struct
  let is_polkagrid abs =
    manager_is_polkagrid (Apron.Abstract1.manager abs)
  let of_polkagrid (abs: 'a t Apron.Abstract1.t) : 'b Apron.Abstract1.t = Obj.magic abs
  let to_polkagrid (abs:'a Apron.Abstract1.t) : 'b t Apron.Abstract1.t =
    if is_polkagrid abs then
      Obj.magic abs
    else
      failwith "PolkaGrid.Abstract1.to_polkagrid: the argument value is not a polkagrid value"
end

