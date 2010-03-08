1 i/* GENERATED, DO NOT MODIFY */
s/NUM_\(NATIVE\|HAS_MAX\)/0/g;
s/NUM_NUMRAT/1/g;
s/NUM_NUM\(INT\|FLT\)/0/g;
s/NUM_EXACT/1/g;
s/_NUMRAT_/_NUMRATMPQ_/;
s/NUM_/NUMMPQ_/g;
s/_BOUND_/_BOUNDMPQ_/;
s/numint\(_\|(\|2(\|[.]h\)/numintMPZ\1/g;
s/numrat\(_\|(\|[.]h\)/numratMPQ\1/g;
s/num\(_\|(\|2(\|[.]h\)/numMPQ\1/g;
s/numMPQ_numxxx[.]h/numMPQ_numrat.h/;
s/bound\(_\|(\|[.]h\)/boundMPQ\1/g;
