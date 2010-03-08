1 i/* GENERATED, DO NOT MODIFY */
s/NUM_\(NATIVE\|HAS_MAX\)/1/g;
s/NUM_NUMFLT/1/g;
s/NUM_NUM\(INT\|RAT\)/0/g;
s/NUM_EXACT/0/g;
s/_NUMFLT_/_NUMFLTD_/;
s/NUM_/NUMD_/g;
s/_BOUND_/_BOUNDD_/;
s/numflt\(_\|(\|2\|[.]h\)/numfltD\1/g;
s/num\(_\|(\|2(\|[.]h\)/numD\1/g;
s/numD_numxxx[.]h/numD_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundD\1/g;
