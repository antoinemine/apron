1 i/* GENERATED, DO NOT MODIFY */
s/NUM_\(NATIVE\|HAS_MAX\)/1/g;
s/NUM_NUMINT/1/g;
s/NUM_NUM\(RAT\|FLT\)/0/g;
s/NUM_EXACT/1/g;
s/_NUMINT_/_NUMINTILL_/;
s/NUM_/NUMILL_/g;
s/_BOUND_/_BOUNDILL_/;
s/numint\(_\|(\|[.]h\)/numintIll\1/g;
s/num\(_\|(\|2(\|[.]h\)/numIll\1/g;
s/numIll_numxxx[.]h/numIll_numint.h/;
s/bound\(_\|(\|[.]h\)/boundIll\1/g;
