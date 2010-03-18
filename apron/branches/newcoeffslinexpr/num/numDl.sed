# Macros defined for any type, to be used with #if #else #endif
s/NUM_NUMFLT/1/g;
s/NUM_NUMDL/1/g;
s/NUM_NUM\(INT\|RAT\|IL\|ILL\|MPZ\|RL\|RLL\|MPQ\|D\|MPFR\)/0/g;
s/NUM_NATIVE/1/g;
s/NUM_EXACT/0/g;
# For including only once an include file
s/_NUMFLT_/_NUMFLTDL_/;
s/_BOUND_/_BOUNDDL_/;
#
s/NUM_/NUMDL_/g;
s/numflt\(_\|(\|2\|[.]h\)/numfltDl\1/g;
s/num\(_\|(\|2(\|[.]h\)/numDl\1/g;
s/numDl_numxxx[.]h/numDl_numflt.h/;
s/bound\(_\|(\|[.]h\)/boundDl\1/g;
