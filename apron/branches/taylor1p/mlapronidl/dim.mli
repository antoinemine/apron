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

(**

{ul
{- [t=int] is the type of dimensions.}
{- The semantics of an object [(change:change)] is the following one:
{ul
{- [change.intdim] and [change.realdim] indicate the number of integer and
   real dimensions to add or to remove}
{- In case of the addition of dimensions,

   [change.dim[i]=k] means: add one dimension at dimension k and shift the
   already existing dimensions greater than or equal to k one step on the
   right (or increment them).

   if k is equal to the size of the vector, then it means: add a dimension at
   the end.

   Repetition are allowed, and means that one inserts more than one dimensions.

   Example:
   [add_dimensions [i0 i1 r0 r1] { dim=[0 1 2 2 4]; intdim=3; realdim=1 }]
   returns [0 i0 0 i1 0 0 r0 r1 0], considered as a vector with 6 integer
   dimensions and 3 real dimensions.}
{- In case of the removal of dimensions,

   dimchange.dim[i]=k means: remove the dimension k and shift the dimensions
   greater than k one step on the left (or decrement them).

   Repetitions are meaningless (and are not correct specification)

   Example: [remove_dimensions [i0 i1 i2 r0 r1 r2] { dim=[0 2 4]; intdim=2;
   realdim=1 }] returns [i1 r0 r2], considered as a vector with 1 integer
   dimensions and 2 real dimensions.
}}}
{- The semantics of an object [(change2:change2)] is the combination of the
two following transformations:
{ul
{- [change2.add] indicates an optional addition of dimensions.}
{- [change2.remove] indicates an optional removal of dimensions.}
}}
{- [perm] defines a permutation.}
{- [dimension] defines the dimensionality of an abstract value (number of
integer and real dimensions).
}}
*)


(** Assuming a transformation for add_dimensions, invert it in-place to
    obtain the inverse transformation using remove_dimensions *)
val change_add_invert : change -> unit

(** [perm_compose perm1 perm2] composes the 2 permutations perm1 and perm2
  (in this order). The sizes of permutations are supposed to be equal. *)
val perm_compose : perm -> perm -> perm

(** Invert a permutation *)
val perm_invert : perm -> perm

