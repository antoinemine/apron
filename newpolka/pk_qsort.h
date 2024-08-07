/* ********************************************************************** */
/* pk_qsort.h: generic sort */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef __PK_QSORT_H__
#define __PK_QSORT_H__

#define MAKE_SORT(NAME, TYPE, CMP)                                      \
                                                                        \
  static void NAME(TYPE* data, size_t size, void* param) {              \
                                                                        \
    if (size >= 12) {                                                    \
                                                                        \
      /* choose pivot: median of begining, middle, end */               \
      TYPE* beg = data;                                                 \
      TYPE* mid = data + (size/2);                                      \
      TYPE* end = data + (size-1);                                      \
      TYPE pivot;                                                       \
      if (CMP(param, beg, mid) <= 0) {                                  \
        if (CMP(param, mid, end) <= 0)                                  \
          pivot = *mid; /* beg <= mid <= end */                         \
        else if (CMP(param, beg, end) <= 0)                             \
          pivot = *end; /* beg <= end < mid */                          \
        else                                                            \
          pivot = *beg; /* end < beg < mid */                           \
      }                                                                 \
      else {                                                            \
        if (CMP(param, end, mid) <= 0)                                  \
          pivot = *mid; /* end <= mid < beg */                          \
        else if (CMP(param, end, beg) <= 0)                             \
          pivot = *end; /* mid < end <= beg */                          \
        else                                                            \
          pivot = *beg; /* mid < beg < end */                           \
      }                                                                 \
                                                                        \
      /* partition */                                                   \
      while (1) {                                                       \
        while (CMP(param, beg, &pivot) < 0) beg++;                      \
        while (CMP(param, &pivot, end) < 0) end--;                      \
        if (beg >= end) break;                                          \
        TYPE x = *beg; *beg = *end; *end = x;                           \
        beg++; end--;                                                   \
      }                                                                 \
                                                                        \
      /* recursive calls */                                             \
      if (end+1 > data) NAME(data, end-data+1, param);                  \
      if (data+size > end+1) NAME(end+1, data+size-end-1, param);       \
    }                                                                   \
                                                                        \
    else if (size > 1) {                                                \
      /* simple insertion sort */                                       \
      for (TYPE* p = data+1; p < data+size; p++) {                      \
        for (TYPE* q = p; q > data && CMP(param, q-1, q) > 0; q--) {    \
          TYPE x = *(q-1); *(q-1) = *q; *q = x;                         \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }                                                                     \
                                                                        \
  static void checked_##NAME(TYPE* data, size_t size, void* param) {    \
    NAME(data, size, param);                                            \
    for (size_t i = 0; i + 1 < size; i++) {                             \
      assert(CMP(param, data+i, data+i+1) <= 0);                        \
    }                                                                   \
  }

#endif
