# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMINT/1/g;
s/NUM_NUMILL/1/g;
s/NUM_NUM\(RAT\|FLT\|IL\|MPZ\|RL\|RLL\|MPQ\|D\|DL\|MPFR\)/0/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMINT_/_NUMINTILL_/;
s/_BOUND_/_BOUNDILL_/;
#
s/NUM_/NUMILL_/g;
s/numint\(_\|(\|[.]h\)/numintIll\1/g;
s/num\(_\|(\|2(\|[.]h\)/numIll\1/g;
s/numIll_numxxx[.]h/numIll_numint.h/;
s/bound\(_\|(\|[.]h\)/boundIll\1/g;
