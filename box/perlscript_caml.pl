while (<>) {
    s/Manager.ap_manager_ptr/t Apron.Manager.t/g; 
    s/external box_/external /g;  
    s/Policy.ap_policy_manager_ptr/t Apron.Policy.man/g;
    print;
}
