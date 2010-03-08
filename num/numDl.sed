1 i/* GENERATED, DO NOT MODIFY */
s/NUM_\(NATIVE\|HAS_MAX\)/1/g;
s/NUM_NUMFLT/1/g;
s/NUM_NUM\(INT\|RAT\)/0/g;
s/NUM_EXACT/0/g;
s/_NUMFLT_/_NUMFLTDL_/;
s/NUM_/NUMDL_/g;
s/_BOUND_/_BOUNDDL_/;
s/numflt\(_\|(\|2\|[.]h\)/numfltDl\1/g;
s/num\(_\|(\|2(\|[.]h\)/numDl\1/g;
s/numDl_numxxx[.]h/numDl_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundDl\1/g;
