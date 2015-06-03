while (<>) {
    s/Manager.ap_manager_ptr/t Apron.Manager.t/g; 
    s/external t1p_/external /g;  
    print;
}
