/* ********************************************************************** */
/* pk_bit.c: operations on bitstrings */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pk_bit.h"

/* ********************************************************************** */
/* I. Bitindices */
/* ********************************************************************** */

void bitindex_fprint(FILE* stream, bitindex_t* bi)
{
  int k;
  bitstring_t m = bi->bit;
  assert (m!=0);
  k=(-1);
  do {
    k++;
    m >>= 1;
  } while (m!=0);
  fprintf(stream,"index=%lu, word=%lu, bit=%d\n",
	  (unsigned long)bi->index,(unsigned long)bi->word,k);
}
void bitindex_print(bitindex_t* bi)
{
  bitindex_fprint(stdout,bi);
}


/* \verb-bitindex_init()- takes as parameter a \emph{flat} index of a bit and
   returns the corresponding structured index.  \verb-bitindex_inc()- and
   \verb-bitindex_dec()- allow to increment and decrement an index.
   \verb-bitindex_size(n)- returns the size of an array of \verb-bitstring_t-
   containing \verb-n- bits. */

bitindex_t bitindex_init(const size_t col)
{
  bitindex_t res;
  res.index = col;
  res.word = col / bitstring_size;
  res.bit = bitstring_msb >> (col % bitstring_size);
  return res;
}

void bitindex_inc(bitindex_t* const bi){
  bi->index++;
  bi->bit >>= 1;
  if (bi->bit==0){
    bi->bit = bitstring_msb;
    bi->word++;
  }
}
void bitindex_dec(bitindex_t* const bi){
  bi->index--;
  if (bi->bit != bitstring_msb){
     bi->bit <<= 1;
  }
  else {
    bi->bit = 1;
    bi->word--;
  }
}

size_t bitindex_size(const size_t n){
  size_t size = n / bitstring_size;
  if (n % bitstring_size) size++;
  return size;
}

/* ********************************************************************** */
/* II. Bitstrings */
/* ********************************************************************** */

/*
  \verb-bitstring_alloc- allocates a new bitstring and
  \verb-bitstring_free()- frees the bitstring.

  \verb-bitstring_clear- sets to \verb-0- the bits, \verb-bitstring_cmp-
  compares two bitfields; be careful, it takes also in account unused bits of
  the last word. Last, \verb-bitstring_print()- writes the bits of a
  bitstring. 
*/
  
bitstring_t* bitstring_alloc(size_t n){
  return (bitstring_t*)malloc(n*sizeof(bitstring_t));
}
bitstring_t* bitstring_realloc(bitstring_t* b, size_t n){
  return (bitstring_t*)realloc(b, n*sizeof(bitstring_t));
}

void bitstring_free(bitstring_t* b){
  free(b);
}

void bitstring_clear(bitstring_t* b, size_t size){
  size_t i;
  for (i=0; i<size; i++) b[i]=0;
}

void bitstring_copy(bitstring_t* b2, bitstring_t* b1, size_t size){
  size_t i;
  for (i=0; i<size; i++) b2[i]=b1[i];
}

void bitstring_fprint(FILE* stream, bitstring_t* b, size_t size)
{
  size_t j,k;
  bitstring_t m;

  for (j=0; j<size; j++){
    m = bitstring_msb; k = 1;
    while (m!=0) {
      if (b[j] & m) fprintf(stream,"1"); else fprintf(stream,"0");
      if (k % 8 == 0) fprintf(stream," ");
      else if (k % 4 == 0) fprintf(stream,",");
      m >>= 1; k++;
    }
  }
}
void bitstring_print(bitstring_t* b, size_t size)
{
  bitstring_fprint(stdout,b,size);
}

int bitstring_cmp(bitstring_t* const r1, bitstring_t* const r2, size_t size){
  size_t i;
  int res=0;
  for (i=0; i<size; i++){
    if (r1[i] < r2[i]){ res=-1; break; }
    else if (r1[i] > r2[i]){ res=1; break; }
  }
  return res;
}

/* These functions allow to read, set or clear individual bits of a bitstring, 
   referenced by a bitindex. */

int bitstring_get(bitstring_t* const b, bitindex_t ix) { 
  return b[ix.word] & ix.bit; 
}

void bitstring_set(bitstring_t* const b, bitindex_t ix){
  b[ix.word] |= ix.bit; 
}

void bitstring_clr(bitstring_t* const b, bitindex_t ix){ 
  b[ix.word] &= ~ix.bit; 
}


