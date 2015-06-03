while (<>) {
    s/Manager.ap_manager_ptr/'a t Apron.Manager.t/g; 
    s/Abstract0.ap_abstract0_ptr/'a t Apron.Abstract0.t/g; 
    s/ap_pkgrid_manager_alloc : 'a t Apron.Manager.t -> 'a t Apron.Manager.t -> 'a t Apron.Manager.t/manager_alloc : 'a Polka.t Apron.Manager.t -> Ppl.grid Ppl.t Apron.Manager.t -> 'a t Apron.Manager.t/g;
    s/ap_pkgrid_manager_decompose : 'a t Apron.Manager.t -> 'a t Apron.Manager.t \* 'a t Apron.Manager.t/manager_decompose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Manager.t \* Ppl.grid Ppl.t Apron.Manager.t/g;
    s/ap_pkgrid_decompose : 'a t Apron.Abstract0.t -> 'a t Apron.Abstract0.t \* 'a t Apron.Abstract0.t/decompose : 'a t Apron.Abstract0.t -> 'a Polka.t Apron.Abstract0.t \* Ppl.grid Ppl.t Apron.Abstract0.t/g;
    s/ap_pkgrid_compose : 'a t Apron.Manager.t -> 'a t Apron.Abstract0.t -> 'a t Apron.Abstract0.t -> 'a t Apron.Abstract0.t/compose : 'a t Apron.Manager.t -> 'a Polka.t Apron.Abstract0.t -> Ppl.grid Ppl.t Apron.Abstract0.t -> 'a t Apron.Abstract0.t/g;
    print;
}

