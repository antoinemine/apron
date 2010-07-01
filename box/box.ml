(* File generated from box.idl *)


(** Intervals abstract domain *)

(** Type of boxes.

    Boxes constrains each dimension/variable [x_i] to belong to an interval
    [I_i].

    Abstract values which are boxes have the type [t Apron.AbstractX.t].

    Managers allocated for boxes have the type [t Apron.manager.t].
*)
type t

external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_box_box_manager_alloc"


let manager_is_box man =
  let str = Apron.Manager.get_library man in
  (String.compare str "box")==0
let manager_of_box (man:t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_box (man:'a Apron.Manager.t) : t Apron.Manager.t =
  if manager_is_box man then
    Obj.magic man
  else
    failwith "Box.manager_to_box: the argument manager is not a Box manager"
module Abstract0 = struct
  let is_box abs =
    manager_is_box (Apron.Abstract0.manager abs)
  let of_box (abs: t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let to_box (abs:'a Apron.Abstract0.t) : t Apron.Abstract0.t =
  if is_box abs then
    Obj.magic abs
  else
    failwith "Box.Abstract0.to_box: the argument value is not a Box value"
end
module Abstract1 = struct
  let is_box abs =
    manager_is_box (Apron.Abstract1.manager abs)
  let of_box (abs: t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let to_box (abs:'a Apron.Abstract1.t) : t Apron.Abstract1.t =
  if is_box abs then
    Obj.magic abs
  else
    failwith "Box.Abstract1.to_box: the argument value is not a Box value"
end

