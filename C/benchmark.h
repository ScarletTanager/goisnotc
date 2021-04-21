#include "append.h"

#ifndef __BENCHMARK_H
#define __BENCHMARK_H
/* Compute elapsed time in milliseconds */
#define       ELAPSED(f, s) (((double)(f - s))/CLOCKS_PER_SEC * 1000)

double benchmark(void (*f)(int, go_slice_t *, int **), int count, go_slice_t *s, int **src);
#endif
