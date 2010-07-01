(* File generated from polka.idl *)

type internal

(** Convex Polyhedra and Linear Equalities abstract domains *)

type loose
type strict
  (** Two flavors for convex polyhedra: loose or strict.

      Loose polyhedra cannot have strict inequality constraints like [x>0].
      They are algorithmically more efficient
      (less generators, simpler normalization).

      Convex polyhedra are defined by the conjunction of a set of linear
      constraints of the form [a_0*x_0 + ... + a_n*x_n + b >= 0] or
      [a_0*x_0 + ... + a_n*x_n + b > 0]
      where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.
  *)
type equalities
  (** Linear equalities.

      Linear equalities are conjunctions of linear
      equalities of the form [a_0*x_0 + ... + a_n*x_n + b = 0].
  *)
type 'a t
(** Type of convex polyhedra/linear equalities, where ['a] is [loose], [strict] or [equalities].

    Abstract values which are convex polyhedra have the type
    [(loose t) Apron.Abstract0.t] or [(loose t) Apron.Abstract1.t] or
    [(strict t) Apron.Abstract0.t] or [(strict t) Apron.Abstract1.t].

    Abstract values which are conjunction of linear equalities have the type
    [(equalities t) Apron.Abstract0.t] or [(equalities t) Apron.Abstract1.t].

    Managers allocated by NewPolka have the type ['a t Apron.Manager.t].
*)

external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_loose"

external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_strict"

external manager_alloc_equalities : unit -> equalities t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_equalities"

external manager_get_internal : 'a t Apron.Manager.t -> internal
	= "camlidl_polka_manager_get_internal"

external set_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_max_coeff_size"

external set_approximate_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_approximate_max_coeff_size"

external get_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_max_coeff_size"

external get_approximate_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_approximate_max_coeff_size"


let manager_is_polka man =
  let str = Apron.Manager.get_library man in
  let str =
    try String.sub str 0 5
    with Invalid_argument _ -> ""
  in
  (String.compare str "polka")==0
let manager_of_polka (man:'a t Apron.Manager.t) : 'b Apron.Manager.t = Obj.magic man
let manager_to_polka (man:'a Apron.Manager.t) : 'b t Apron.Manager.t =
  if manager_is_polka man then
    Obj.magic man
  else
    failwith "Polka.to_polka: the argument manager is not a Polka manager"

let manager_is_polka_loose man =
  let str = Apron.Manager.get_library man in
  (String.compare str "polka, loose mode")==0
let manager_of_polka_loose (man:loose t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_polka_loose (man:'a Apron.Manager.t) : loose t Apron.Manager.t =
  if manager_is_polka_loose man then
    Obj.magic man
  else
    failwith "Polka.to_polka_loose: the argument manager is not a loose Polka manager"

let manager_is_polka_strict man =
  let str = Apron.Manager.get_library man in
  (String.compare str "polka, strict mode")==0
let manager_of_polka_strict (man:strict t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_polka_strict (man:'a Apron.Manager.t) : strict t Apron.Manager.t =
  if manager_is_polka_strict man then
    Obj.magic man
  else
    failwith "Polka.to_polka_strict: the argument manager is not a strict Polka manager"

let manager_is_polka_equalities man =
  let str = Apron.Manager.get_library man in
  (String.compare str "polka, equalities mode")==0
let manager_of_polka_equalities (man:equalities t Apron.Manager.t) : 'a Apron.Manager.t = Obj.magic man
let manager_to_polka_equalities (man:'a Apron.Manager.t) : equalities t Apron.Manager.t =
  if manager_is_polka_equalities man then
    Obj.magic man
  else
    failwith "Polka.to_polka_equalities: the argument manager is not an equalities Polka manager"

module Abstract0 = struct
  let is_polka abs =
    manager_is_polka (Apron.Abstract0.manager abs)
  let is_polka_loose abs =
    manager_is_polka_loose (Apron.Abstract0.manager abs)
  let is_polka_strict abs =
    manager_is_polka (Apron.Abstract0.manager abs)
  let is_polka_equalities abs =
    manager_is_polka_equalities (Apron.Abstract0.manager abs)
  let of_polka (abs: 'a t Apron.Abstract0.t) : 'b Apron.Abstract0.t = Obj.magic abs
  let of_polka_loose (abs: loose t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let of_polka_strict (abs: strict t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let of_polka_equalities (abs: equalities t Apron.Abstract0.t) : 'a Apron.Abstract0.t = Obj.magic abs
  let to_polka (abs:'a Apron.Abstract0.t) : 'b t Apron.Abstract0.t =
    if is_polka abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract0.to_polka: the argument value is not a polka value"
  let to_polka_loose (abs:'a Apron.Abstract0.t) : loose t Apron.Abstract0.t =
    if is_polka_loose abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract0.to_polka_loose: the argument value is not a loose polka value"
  let to_polka_strict (abs:'a Apron.Abstract0.t) : strict t Apron.Abstract0.t =
    if is_polka_strict abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract0.to_polka_strict: the argument value is not a strict polka value"
  let to_polka_equalities (abs:'a Apron.Abstract0.t) : equalities t Apron.Abstract0.t =
    if is_polka_equalities abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract0.to_polka_equalities: the argument value is not an equalities polka value"
end

module Abstract1 = struct
  let is_polka abs =
    manager_is_polka (Apron.Abstract1.manager abs)
  let is_polka_loose abs =
    manager_is_polka_loose (Apron.Abstract1.manager abs)
  let is_polka_strict abs =
    manager_is_polka (Apron.Abstract1.manager abs)
  let is_polka_equalities abs =
    manager_is_polka_equalities (Apron.Abstract1.manager abs)
  let of_polka (abs: 'a t Apron.Abstract1.t) : 'b Apron.Abstract1.t = Obj.magic abs
  let of_polka_loose (abs: loose t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let of_polka_strict (abs: strict t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let of_polka_equalities (abs: equalities t Apron.Abstract1.t) : 'a Apron.Abstract1.t = Obj.magic abs
  let to_polka (abs:'a Apron.Abstract1.t) : 'b t Apron.Abstract1.t =
    if is_polka abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract1.to_polka: the argument value is not a polka value"
  let to_polka_loose (abs:'a Apron.Abstract1.t) : loose t Apron.Abstract1.t =
    if is_polka_loose abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract1.to_polka_loose: the argument value is not a loose polka value"
  let to_polka_strict (abs:'a Apron.Abstract1.t) : strict t Apron.Abstract1.t =
    if is_polka_strict abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract1.to_polka_strict: the argument value is not a strict polka value"
  let to_polka_equalities (abs:'a Apron.Abstract1.t) : equalities t Apron.Abstract1.t =
    if is_polka_equalities abs then
      Obj.magic abs
    else
      failwith "Polka.Abstract1.to_polka_equalities: the argument value is not an equalities polka value"
end

