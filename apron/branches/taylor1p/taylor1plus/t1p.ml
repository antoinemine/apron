(* File generated from t1p.idl *)


(** Taylor1+ abstract domain *)

(** Type of Taylor1+ forms.

Each dimension/variable [x_i] has the affine form: alpha_0 + Sum(alpha_i*eps_i), where eps_i are the noise symbols, and alpha_i their associated coefficients.

Abstract values which are Taylor1+ forms (affine forms) have the type [t Apron.AbstractX.t].

Managers allocated for Taylor1+ abstract values have the type [t Apron.manager.t].
*)
type t

external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_t1p_t1p_manager_alloc"

