while (<>) {
    s/Manager.ap_manager_ptr/t Apron.Manager.t/g;
    s/internal_ptr/internal/g;
    s/external avo_/external /g;
    s/Generator0.ap_generator0_array_t/Apron.Generator0.t array/g;
    s/Scalar.ap_scalar_ptr/Apron.Scalar.t/g;
    s/Scalar.ap_scalar_array_t/Apron.Scalar.t array/g;
    s/Abstract0.ap_abstract0_ptr/t Apron.Abstract0.t/g;
    s/external ap_abstract0_avo_/external /g;
    print;
}
