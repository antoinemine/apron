1 i/* GENERATED, DO NOT MODIFY */
s/NUM_NATIVE/0/g;
s/NUM_HAS_MAX/1/g;
s/NUM_NUMFLT/1/g;
s/NUM_NUM\(INT\|RAT\)/0/g;
s/NUM_EXACT/0/g;
s/_NUMFLT_/_NUMFLTMPFR_/;
s/NUMFLT_\(NATIVE\|MAX\)/NUMFLTMPFR_\1/g;
s/NUM_/NUMMPFR_/g;
s/_BOUND_/_BOUNDMPFR_/;
s/numflt\(_\|(\|2\|[.]h\)/numfltMPFR\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPFR\1/g;
s/numMPFR_numxxx[.]h/numMPFR_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundMPFR\1/g;
