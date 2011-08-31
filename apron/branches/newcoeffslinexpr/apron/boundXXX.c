/* ********************************************************************** */
/* boundXXX.c: bounds */
/* ********************************************************************** */

#include "boundXXX.h"

void boundXXX_fprint(FILE* stream, const boundXXX_t a)
{
  if (boundXXX_infty(a)) fprintf(stream,"%coo",boundXXX_sgn(a)>0 ? '+' : '-');
  else numXXX_fprint(stream,boundXXX_numref(a));
}
void boundXXX_print(const boundXXX_t a)
{
  boundXXX_fprint(stdout,a); fflush(stdout);
}
int boundXXX_snprint(char* s, int size, const boundXXX_t a)
{
  if (boundXXX_infty(a)) return ap_snprintf(s,size,"%coo",boundXXX_sgn(a)>0 ? '+' : '-');
  else return numXXX_snprint(s,size,boundXXX_numref(a));
}
