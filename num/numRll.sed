1 i/* GENERATED, DO NOT MODIFY */
# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/0/g;
s/NUM_NUMRAT/1/g;
s/NUM_NUMFLT/0/g;
s/NUM_NUMIL/0/g;
s/NUM_NUMILL/0/g;
s/NUM_NUMMPZ/0/g;
s/NUM_NUMRL/0/g;
s/NUM_NUMRLL/1/g;
s/NUM_NUMMPQ/0/g;
s/NUM_NUMD/0/g;
s/NUM_NUMDL/0/g;
s/NUM_NUMMPFR/0/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMRAT_/_NUMRATRLL_/;
s/_BOUND_/_BOUNDRLL_/;
#
s/NUMINT_\(MIN\|MAX\|ONE\)/NUMINTILL_\1/g;
s/NUM_/NUMRLL_/g;
s/numint\(_\|(\|2(\|[.]h\)/numintIll\1/g;
s/numrat\(_\|(\|[.]h\)/numratRll\1/g;
s/num\(_\|(\|2(\|[.]h\)/numRll\1/g;
s/numRll_numxxx[.]h/numRll_numrat.h/;
s/bound\(_\|(\|[.]h\)/boundRll\1/g;
