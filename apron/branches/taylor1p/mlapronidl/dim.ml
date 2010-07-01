(* File generated from dim.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = int
and change = {
  dim : int array;
  intdim : int;
  realdim : int;
}
and change2 = {
  add: change option;
  remove: change option;
}
and perm = int array
and dimension = {
  intd: int;
  reald: int;
}

(** APRON Dimensions and related types *)

let change_add_invert change =
  let dim = change.dim in
  for i=0 to (Array.length dim)-1 do
    dim.(i) <- dim.(i) + i;
  done

let perm_compose perm1 perm2 =
  let length = Array.length perm1 in
  assert(length==(Array.length perm2));
  Array.init length (fun i -> perm2.(perm1.(i)))

let perm_invert perm =
  let length = Array.length perm in
  let res = Array.make length 0 in
  for i=0 to length-1 do
    res.(perm.(i)) <- i;
  done;
  res

