1 i/* GENERATED, DO NOT MODIFY */
# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/0/g;
s/NUM_NUMRAT/0/g;
s/NUM_NUMFLT/1/g;
s/NUM_NUMILL/0/g;
s/NUM_NUMIL/0/g;
s/NUM_NUMMPZ/0/g;
s/NUM_NUMRLL/0/g;
s/NUM_NUMRL/0/g;
s/NUM_NUMMPQ/0/g;
s/NUM_NUMDL/0/g;
s/NUM_NUMD/0/g;
s/NUM_NUMMPFR/1/g;
s/NUM_NATIVE/0/g;
s/NUM_EXACT/0/g;
# For including only once an include file
s/_NUMFLT_/_NUMFLTMPFR_/;
s/_BOUND_/_BOUNDMPFR_/;
#
s/NUM_/NUMMPFR_/g;
s/numflt\(_\|(\|2\|[.]h\)/numfltMPFR\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPFR\1/g;
s/numMPFR_numxxx[.]h/numMPFR_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundMPFR\1/g;