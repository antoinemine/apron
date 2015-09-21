print "\n/* This file is part of the APRON Library, released under LGPL license  with an exception allowing the redistribution of statically linked executables.\n  Please read the COPYING file packaged in the distribution  */";
while (<>) {
    s/extern void camlidl_apron_lincons0_ml2c\(value, ap_lincons0_t \*\)/extern void camlidl_apron_lincons0_ml2c\(value, ap_lincons0_t \*, camlidl_ctx\)/g;
    s/\#define camlidl_ml2c_lincons0_ap_lincons0_t\(v,c,ctx\) camlidl_apron_lincons0_ml2c\(v,c\)/\#define camlidl_ml2c_lincons0_ap_lincons0_t\(v,c,ctx\) camlidl_apron_lincons0_ml2c\(v,c,ctx\)/g;
    s/extern void camlidl_apron_tcons0_ml2c\(value, ap_tcons0_t \*\)/extern void camlidl_apron_tcons0_ml2c\(value, ap_tcons0_t \*, camlidl_ctx\)/g;
    s/\#define camlidl_ml2c_tcons0_ap_tcons0_t\(v,c,ctx\) camlidl_apron_tcons0_ml2c\(v,c\)/\#define camlidl_ml2c_tcons0_ap_tcons0_t\(v,c,ctx\) camlidl_apron_tcons0_ml2c\(v,c,ctx\)/g;
    s/struct ap_texpr_op_t/ap_texpr_op_t/g;
    s/struct ap_texpr_rtype_t/ap_texpr_rtype_t/g;
    s/struct ap_texpr_rdir_t/ap_texpr_rdir_t/g;
    print;
}
