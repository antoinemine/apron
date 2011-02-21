/* ************************************************************************* */
/* pk_bit.h: bitindices and bitstrings */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _PK_BIT_H_
#define _PK_BIT_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* II. Bitstrings */
/* ********************************************************************** */

typedef unsigned int bitstring_t;
typedef struct bitindex_t {
  size_t index;
  size_t word;
  bitstring_t bit;
} bitindex_t;

#define bitstring_size (sizeof(bitstring_t)*8)
#define bitstring_msb (1U<<(bitstring_size-1))

/* Operations on bitindex_t */
void bitindex_print(bitindex_t* bi);
void bitindex_fprint(FILE* stream, bitindex_t* bi);
bitindex_t bitindex_init(size_t col);
void bitindex_inc(bitindex_t*);
void bitindex_dec(bitindex_t*);
size_t bitindex_size(size_t n);
/* bitindex_init() takes as parameter a \emph{flat} index of a bit and
   returns the corresponding structured index.  bitindex_inc() and
   bitindex_dec() allow to increment and decrement an index.
   bitindex_size(n) returns the size of an array of bitstring_t
   containing n bits. */

/* Operations on bitstring_t */
static inline bitstring_t* bitstring_alloc(size_t n);
static inline bitstring_t* bitstring_realloc(bitstring_t* b, size_t n);
static inline void bitstring_free(bitstring_t* b);
void bitstring_clear(bitstring_t* b, size_t size);
void bitstring_copy(bitstring_t* b2, bitstring_t* b1, size_t size);
int bitstring_cmp(bitstring_t* r1, bitstring_t* r2, size_t size);
/* bitstring_alloc allocates a new bitstring and bitstring_free() frees the
   bitstring.
   bitstring_clear sets to 0 the bits, bitstring_cmp compares two bitfields;
   be careful, it takes also in account unused bits of the last word. Last,
   bitstring_print() writes the bits of a bitstring.
*/

void bitstring_print(bitstring_t* b, size_t size);
void bitstring_fprint(FILE* stream, bitstring_t* b, size_t size);

static inline int bitstring_get(bitstring_t* b, bitindex_t ix);
static inline void bitstring_set(bitstring_t* b, bitindex_t ix);
static inline void bitstring_clr(bitstring_t* b, bitindex_t ix);
/* These functions allow to read, set or clear individual bits of a bitstring,
   referenced by a bitindex. */

/* ********************************************************************** */
/* ********************************************************************** */
/* Inline functions definitions */

static inline bitstring_t* bitstring_alloc(size_t n){
  return (bitstring_t*)malloc(n*sizeof(bitstring_t));
}
static inline bitstring_t* bitstring_realloc(bitstring_t* b, size_t n){
  return (bitstring_t*)realloc(b, n*sizeof(bitstring_t));
}

static inline void bitstring_free(bitstring_t* b){
  free(b);
}

static inline int bitstring_get(bitstring_t* b, bitindex_t ix) {
  return b[ix.word] & ix.bit;
}

static inline void bitstring_set(bitstring_t* b, bitindex_t ix){
  b[ix.word] |= ix.bit;
}

static inline void bitstring_clr(bitstring_t* b, bitindex_t ix){
  b[ix.word] &= ~ix.bit;
}

#ifdef __cplusplus
}
#endif

#endif
