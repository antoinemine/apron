# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/0/g;
s/NUM_NUMRAT/1/g;
s/NUM_NUMFLT/0/g;
s/NUM_NUMILL/0/g;
s/NUM_NUMIL/0/g;
s/NUM_NUMMPZ/0/g;
s/NUM_NUMRLL/0/g;
s/NUM_NUMRL/0/g;
s/NUM_NUMMPQ/1/g;
s/NUM_NUMDL/0/g;
s/NUM_NUMD/0/g;
s/NUM_NUMMPFR/0/g;
s/NUM_NATIVE/0/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMRAT_/_NUMRATMPQ_/;
s/_BOUND_/_BOUNDMPQ_/;
#
s/NUM_/NUMMPQ_/g;
s/numint\(_\|(\|2(\|[.]h\)/numintMPZ\1/g;
s/numrat\(_\|(\|[.]h\)/numratMPQ\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPQ\1/g;
s/numMPQ_numxxx[.]h/numMPQ_numrat.h/;
s/bound\(_\|(\|[.]h\)/boundMPQ\1/g;
