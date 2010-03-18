# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUM\(INT\|FLT\|IL\|ILL\|MPZ\|RLL\|MPQ\|D\|DL\|MPFR\)/0/g;
s/NUM_NUMRAT/1/g;
s/NUM_NUMRL/1/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/1/g;
# For including only once an include file
s/_NUMRAT_/_NUMRATRL_/;
s/_BOUND_/_BOUNDRL_/;
#
s/NUMINT_\(MIN\|MAX\|ONE\)/NUMINTIL_\1/g;
s/NUM_/NUMRL_/g;
s/numint\(_\|(\|2(\|[.]h\)/numintIl\1/g;
s/numrat\(_\|(\|[.]h\)/numratRl\1/g;
s/num\(_\|(\|2(\|[.]h\)/numRl\1/g;
s/numRl_numxxx[.]h/numRl_numrat.h/;
s/bound\(_\|(\|[.]h\)/boundRl\1/g;
