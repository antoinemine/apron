/* ********************************************************************** */
/* pk_bit.h: operations on bitstrings */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* This header file define operations on \emph{bitstrings} and
   \emph{bitindices}, to be used to access and modify bitstrings. */

/* The type \verb-bitstring_t- is simply an integer, which is an element
   of an array. 

   An structured index of a bit in a bitfield is a pair $(w,b)$ where $w$
   reference the considered integer and $b$ is a mask selecting the right
   bit.
*/

#ifndef __PK_BIT_H__
#define __PK_BIT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int bitstring_t;
typedef struct bitindex_t {
  size_t index;
  size_t word;
  bitstring_t bit;
} bitindex_t;

#define bitstring_size (sizeof(bitstring_t)*8)
#define bitstring_msb (1U<<(bitstring_size-1))

/* Operations on \verb-bitindex_t- */
void bitindex_print(bitindex_t* bi);
void bitindex_fprint(FILE* stream, bitindex_t* bi);
bitindex_t bitindex_init(size_t col);
void bitindex_inc(bitindex_t*);
void bitindex_dec(bitindex_t*);
size_t bitindex_size(size_t n);

/* Operations on \verb-bitstring_t- */
bitstring_t* bitstring_alloc(size_t n);
bitstring_t* bitstring_realloc(bitstring_t* b, size_t n);
void bitstring_free(bitstring_t* b);
void bitstring_clear(bitstring_t* b, size_t size);
void bitstring_copy(bitstring_t* b2, bitstring_t* b1, size_t size);
int bitstring_cmp(bitstring_t* r1, bitstring_t* r2, size_t size);

void bitstring_print(bitstring_t* b, size_t size);
void bitstring_fprint(FILE* stream, bitstring_t* b, size_t size);

int bitstring_get(bitstring_t* b, bitindex_t ix);
void bitstring_set(bitstring_t* b, bitindex_t ix);
void bitstring_clr(bitstring_t* b, bitindex_t ix);

#ifdef __cplusplus
}
#endif

#endif
