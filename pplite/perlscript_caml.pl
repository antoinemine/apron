while (<>) {
    s/Manager.ap_manager_ptr/'a t Apron.Manager.t/g;
    s/Abstract0.ap_abstract0_ptr/'a t Apron.Abstract0.t/g;
    s/ap_lincons0_t/t/g;
    s/Lincons0.t/Apron.Lincons0.t/g;
    s/ap_lincons0_array_t/t array/g;
    s/Lincons0.t array/Apron.Lincons0.t array/g;
    s/ap_tcons0_t/t/g;
    s/ap_tcons0_array_t/t array/g;
    s/external ap_pplite_/external /g;
    s/external ap_abstract0_pplite_/external /g;
    s/external manager_alloc_loose : unit -> 'a t Apron.Manager.t/external manager_alloc_loose : unit -> loose t Apron.Manager.t/g;
    s/external manager_alloc_strict : unit -> 'a t Apron.Manager.t/external manager_alloc_strict : unit -> strict t Apron.Manager.t/g;
    print;
}
