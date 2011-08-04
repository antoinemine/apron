val copy : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t
val size : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> int
val minimize : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> unit
val canonicalize : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> unit
val hash : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> int
val approximate : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> int -> unit
val fdump : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> unit
val print :
  ?first:(unit, Format.formatter, unit) format ->
  ?sep:(unit, Format.formatter, unit) format ->
  ?last:(unit, Format.formatter, unit) format ->
  (int -> string) -> Format.formatter -> 'a Apron.Abstract0.t -> unit
val bottom :
  'a Apron.Manager.t -> Apron.Dim.dimension -> 'a Apron.Abstract0.t
val top : 'a Apron.Manager.t -> Apron.Dim.dimension -> 'a Apron.Abstract0.t
val of_box :
  'a Apron.Manager.t ->
  Apron.Dim.dimension -> Apron.Linexpr0.t -> 'a Apron.Abstract0.t
val dimension :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> Apron.Dim.dimension
val is_bottom : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> bool
val is_top : 'a Apron.Manager.t -> 'a Apron.Abstract0.t -> bool
val is_leq :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t -> bool
val is_eq :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t -> bool
val is_dimension_unconstrained :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> Apron.Dim.t -> bool
val sat_interval :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.t -> 'a Apron.Coeff.tt -> bool
val sat_lincons :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> Apron.Lincons0.t -> bool
val sat_tcons :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> Apron.Tcons0.t -> bool
val bound_dimension :
  'a Apron.Manager.t ->
  Apron.Coeff.t -> 'a Apron.Abstract0.t -> Apron.Dim.t -> unit
val bound_linexpr :
  'a Apron.Manager.t ->
  Apron.Coeff.t -> 'a Apron.Abstract0.t -> Apron.Linexpr0.t -> unit
val bound_texpr :
  'a Apron.Manager.t ->
  Apron.Coeff.t -> 'a Apron.Abstract0.t -> Apron.Texpr0.t -> unit
val to_box :
  'a Apron.Manager.t -> Apron.Linexpr0.t -> 'a Apron.Abstract0.t -> unit
val to_lincons_array :
  'a Apron.Manager.t -> Apron.Lincons0.earray -> 'a Apron.Abstract0.t -> unit
val to_tcons_array :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> Apron.Tcons0.t array
val to_lingen_array :
  'a Apron.Manager.t -> Apron.Lingen0.earray -> 'a Apron.Abstract0.t -> unit
val meet :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t
val meet_array :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t array -> 'a Apron.Abstract0.t
val meet_lincons_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Lincons0.earray -> 'a Apron.Abstract0.t
val meet_tcons_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Tcons0.t array -> 'a Apron.Abstract0.t
val join :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t
val join_array :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t array -> 'a Apron.Abstract0.t
val add_ray_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Lingen0.earray -> 'a Apron.Abstract0.t
val assign_linexpr_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t ->
  Apron.Dim.t array ->
  Apron.Linexpr0.earray ->
  'a Apron.Abstract0.t option -> 'a Apron.Abstract0.t
val substitute_linexpr_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t ->
  Apron.Dim.t array ->
  Apron.Linexpr0.earray ->
  'a Apron.Abstract0.t option -> 'a Apron.Abstract0.t
val assign_texpr_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t ->
  Apron.Dim.t array ->
  Apron.Texpr0.t array -> 'a Apron.Abstract0.t option -> 'a Apron.Abstract0.t
val substitute_texpr_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t ->
  Apron.Dim.t array ->
  Apron.Texpr0.t array -> 'a Apron.Abstract0.t option -> 'a Apron.Abstract0.t
val add_dimensions :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t ->
  Apron.Dim.change option -> bool -> 'a Apron.Abstract0.t
val remove_dimensions :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.change option -> 'a Apron.Abstract0.t
val permute_dimensions :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.perm -> 'a Apron.Abstract0.t
val forget_array :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.t array -> bool -> 'a Apron.Abstract0.t
val expand :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.t -> int -> 'a Apron.Abstract0.t
val fold :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> Apron.Dim.t array -> 'a Apron.Abstract0.t
val widening :
  'a Apron.Manager.t ->
  'a Apron.Abstract0.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t
val closure :
  'a Apron.Manager.t -> 'a Apron.Abstract0.t -> 'a Apron.Abstract0.t
val manager_alloc : unit -> 
  (Apron.Polka.loose, Apron.Polka.numMPQ) Apron.Polka.t Apron.Manager.t
