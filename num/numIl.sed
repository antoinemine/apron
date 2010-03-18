# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/1/g;
s/NUM_NUMRAT/0/g;
s/NUM_NUMFLT/0/g;
s/NUM_NUMILL/0/g;
s/NUM_NUMIL/1/g;
s/NUM_NUMMPZ/0/g;
s/NUM_NUMRLL/0/g;
s/NUM_NUMRL/0/g;
s/NUM_NUMMPQ/0/g;
s/NUM_NUMDL/0/g;
s/NUM_NUMD/0/g;
s/NUM_NUMMPFR/0/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMINT_/_NUMINTIL_/;
s/_BOUND_/_BOUNDIL_/;
#
s/NUM_/NUMIL_/g;
s/numint\(_\|(\|[.]h\)/numintIl\1/g;
s/num\(_\|(\|2(\|[.]h\)/numIl\1/g;
s/numIl_numxxx[.]h/numIl_numint.h/;
s/bound\(_\|(\|[.]h\)/boundIl\1/g;
