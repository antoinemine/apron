while (<>) {
    s/Manager.ap_manager_ptr/'a t Apron.Manager.t/g; 
    s/internal_ptr/internal/g;  
    s/external pk_/external /g;  
    s/external manager_alloc_loose : unit -> 'a t Apron.Manager.t/external manager_alloc_loose : unit -> loose t Apron.Manager.t/g;
    s/external manager_alloc_strict : unit -> 'a t Apron.Manager.t/external manager_alloc_strict : unit -> strict t Apron.Manager.t/g;
    s/external manager_alloc_equalities : unit -> 'a t Apron.Manager.t/external manager_alloc_equalities : unit -> equalities t Apron.Manager.t/g;
    print;
}
