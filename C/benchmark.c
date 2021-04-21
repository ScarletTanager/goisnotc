#include <time.h>
#include "append.h"
#include "benchmark.h"

double benchmark(void (*f)(int, go_slice_t *, int **), int count, go_slice_t *s, int **src) {
  clock_t start_time = clock();
  (*f)(count, s, src);
  clock_t finish_time = clock();
  return ELAPSED(finish_time, start_time);
}
