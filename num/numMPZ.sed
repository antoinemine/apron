1 i/* GENERATED, DO NOT MODIFY */
s/NUM_\(NATIVE\|HAS_MAX\)/0/g;
s/NUM_NUMINT/1/g;
s/NUM_NUM\(RAT\|FLT\)/0/g;
s/NUM_EXACT/1/g;
s/_NUMINT_/_NUMINTMPZ_/;
s/NUM_/NUMMPZ_/g;
s/_BOUND_/_BOUNDMPZ_/;
s/numint\(_\|(\|[.]h\)/numintMPZ\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPZ\1/g;
s/numMPZ_numxxx[.]h/numMPZ_numint.h/;
s/bound\(_\|(\|[.]h\)/boundMPZ\1/g;
