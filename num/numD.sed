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
s/NUM_NUMD/1/g;
s/NUM_NUMMPFR/0/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/0/g;
# For including only once an include file
s/_NUMFLT_/_NUMFLTD_/;
s/_BOUND_/_BOUNDD_/;
#
s/NUM_/NUMD_/g;
s/numflt\(_\|(\|2\|[.]h\)/numfltD\1/g;
s/num\(_\|(\|2(\|[.]h\)/numD\1/g;
s/numD_numxxx[.]h/numD_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundD\1/g;
