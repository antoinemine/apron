open Apron

type t = (Polka.loose, Polka.numMPQ) Polka.t Abstract0.t

let man = Polka.manager_alloc_loose_numMPQ ()

let copy = Abstract0.copy
let size = Abstract0.size
let minimize = Abstract0.minimize
let canonicalize = Abstract0.canonicalize
let hash = Abstract0.hash
let approximate = Abstract0.approximate
let fdump = Abstract0.fdump
let print = Abstract0.print 
let bottom = Abstract0.bottom
let top = Abstract0.top
let of_box = Abstract0.of_box
let dimension = Abstract0.dimension
let is_bottom = Abstract0.is_bottom
let is_top = Abstract0.is_top
let is_leq = Abstract0.is_leq
let is_eq = Abstract0.is_eq
let is_dimension_unconstrained = Abstract0.is_dimension_unconstrained
let sat_interval = Abstract0.sat_interval
let sat_lincons = Abstract0.sat_lincons
let sat_tcons = Abstract0.sat_tcons
let bound_dimension = Abstract0.bound_dimension
let bound_linexpr = Abstract0.bound_linexpr
let bound_texpr = Abstract0.bound_texpr
let to_box = Abstract0.to_box
let to_lincons_array = Abstract0.to_lincons_array
let to_tcons_array = Abstract0.to_tcons_array
let to_lingen_array = Abstract0.to_lingen_array
let meet = Abstract0.meet
let meet_array = Abstract0.meet_array
let meet_lincons_array = Abstract0.meet_lincons_array
let meet_tcons_array = Abstract0.meet_tcons_array
let join = Abstract0.join
let join_array = Abstract0.join_array
let add_ray_array = Abstract0.add_ray_array
let assign_linexpr_array = Abstract0.assign_linexpr_array
let substitute_linexpr_array = Abstract0.substitute_linexpr_array
let assign_texpr_array = Abstract0.assign_texpr_array
let substitute_texpr_array = Abstract0.substitute_texpr_array
let add_dimensions = Abstract0.add_dimensions
let remove_dimensions = Abstract0.remove_dimensions
let permute_dimensions = Abstract0.permute_dimensions
let forget_array = Abstract0.forget_array
let expand = Abstract0.expand
let fold = Abstract0.fold
let widening = Abstract0.widening
let closure = Abstract0.closure

let _ = 
  Callback.register "copy" copy;
  Callback.register "size" size;
  Callback.register "minimize" minimize;
  Callback.register "canonicalize" canonicalize;
  Callback.register "hash" hash;
  Callback.register "approximate" approximate;
  Callback.register "fdump" fdump;
  Callback.register "print" print;
  Callback.register "bottom" bottom;
  Callback.register "top" top;
  Callback.register "of_box" of_box;
  Callback.register "dimension" dimension;
  Callback.register "is_bottom" is_bottom;
  Callback.register "is_top" is_top;
  Callback.register "is_leq" is_leq;
  Callback.register "is_eq" is_eq;
  Callback.register "is_dimension_unconstrained" is_dimension_unconstrained;
  Callback.register "sat_interval" sat_interval;
  Callback.register "sat_lincons" sat_lincons;
  Callback.register "sat_tcons" sat_tcons;
  Callback.register "bound_dimension" bound_dimension;
  Callback.register "bound_linexpr" bound_linexpr;
  Callback.register "bound_texpr" bound_texpr;
  Callback.register "to_box" to_box;
  Callback.register "to_lincons_array" to_lincons_array;
  Callback.register "to_tcons_array" to_tcons_array;
  Callback.register "to_lingen_array" to_lingen_array;
  Callback.register "meet" meet;
  Callback.register "meet_array" meet_array;
  Callback.register "meet_lincons_array" meet_lincons_array;
  Callback.register "meet_tcons_array" meet_tcons_array;
  Callback.register "join" join;
  Callback.register "join_array" join_array;
  Callback.register "add_ray_array" add_ray_array;
  Callback.register "assign_linexpr_array" assign_linexpr_array;
  Callback.register "substitute_linexpr_array" substitute_linexpr_array;
  Callback.register "assign_texpr_array" assign_texpr_array;
  Callback.register "substitute_texpr_array" substitute_texpr_array;
  Callback.register "add_dimensions" add_dimensions;
  Callback.register "remove_dimensions" remove_dimensions;
  Callback.register "permute_dimensions" permute_dimensions;
  Callback.register "forget_array" forget_array;
  Callback.register "expand" expand;
  Callback.register "fold" fold;
  Callback.register "widening" widening;
  Callback.register "closure" closure
