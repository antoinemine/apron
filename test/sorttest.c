#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "../newpolka/pk_qsort.h"

int cmp_int(void* unused, int* a, int* b) {
  return (*a > *b) ? 1 : (*a < *b) ? -1 : 0;
}

MAKE_SORT(sort_int, int, cmp_int)

int sum(int* a, size_t size) {
  int x = 0;
  for (size_t i = 0; i < size; i++) x += a[i];
  return x;
}

void check_cst_array(size_t size) {
  int x = lrand48();
  int* a = malloc(sizeof(int)*size);
  for (size_t i = 0; i < size; i++)
    a[i] = x;
  int s = sum(a,size);
  checked_sort_int(a, size, NULL);
  assert(s == sum(a,size));
  free(a);
}
  
void check_inc_array(size_t size) {
  int* a = malloc(sizeof(int)*size);
  for (size_t i = 0; i < size; i++)
    a[i] = i;
  int s = sum(a,size);
  checked_sort_int(a, size, NULL);
  assert(s == sum(a,size));
  free(a);
}
  
void check_dec_array(size_t size) {
  int* a = malloc(sizeof(int)*size);
  for (size_t i = 0; i < size; i++)
    a[i] = size-i;
  int s = sum(a,size);
  checked_sort_int(a, size, NULL);
  assert(s == sum(a,size));
  free(a);
}

void check_rnd_array(size_t size, int max_rand) {
  int* a = malloc(sizeof(int)*size);
  for (size_t i = 0; i < size; i++)
    a[i] = lrand48() & max_rand;
  int s = sum(a,size);
  checked_sort_int(a, size, NULL);
  assert(s == sum(a,size));
  free(a);
}

  
int main() {
  for (int i = 0; i < 1000; i++) {
    printf("check constant array of size %i\n",i);
    check_cst_array(i);
  }
  for (int i = 1; i < 1000; i++) {
    printf("check increasing array of size %i\n",i);
    check_inc_array(i);
  }
  for (int i = 1; i < 1000; i++) {
    printf("check decreasing array of size %i\n",i);
    check_dec_array(i);
  }
  for (int i = 1; i < 1000; i++) {
    printf("check random array of size %i\n",i);
    for (int j = i/2; j < i*2; j += 2) {
      check_rnd_array(i, j);
    }
    for (int j = 0; j < 1000; j++) {
      check_rnd_array(i, INT_MAX);
    }
  }
 for (int i = 1000; i < 1000000; i = i*4/3) {
    printf("check random array of size %i\n",i);
    for (int j = 0; j < 100; j++) {
      check_rnd_array(i, INT_MAX);
    }
  }
  return 0;
}
