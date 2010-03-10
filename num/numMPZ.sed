1 i/* GENERATED, DO NOT MODIFY */
# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/1/g;
s/NUM_NUMRAT/0/g;
s/NUM_NUMFLT/0/g;
s/NUM_NUMIL/0/g;
s/NUM_NUMILL/0/g;
s/NUM_NUMMPZ/1/g;
s/NUM_NUMRL/0/g;
s/NUM_NUMRLL/0/g;
s/NUM_NUMMPQ/0/g;
s/NUM_NUMD/0/g;
s/NUM_NUMDL/0/g;
s/NUM_NUMMPFR/0/g;
s/NUM_NATIVE/0/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMINT_/_NUMINTMPZ_/;
s/_BOUND_/_BOUNDMPZ_/;
#
s/NUM_/NUMMPZ_/g;
s/numint\(_\|(\|[.]h\)/numintMPZ\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPZ\1/g;
s/numMPZ_numxxx[.]h/numMPZ_numint.h/;
s/bound\(_\|(\|[.]h\)/boundMPZ\1/g;
