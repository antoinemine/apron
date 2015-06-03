foreach (@ARGV) {
    open(F,"<",$_) || next;
    while (<F>) {
        s/I0_CHECK_EXC\(([^)]*)\)/if \($1->result.exn!=AP_EXC_NONE\) camlidl_apron_manager_check_exception\($1,_ctx\);/g;
        print;
    }
}

