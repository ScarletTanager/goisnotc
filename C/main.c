#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "append.h"
#include "benchmark.h"

/*
 * Simple program to demonstrate:
 * a) Initializing an array of a fixed size by looping through it
 * b) how one might implement a simple append() function
 *    with behavior roughly that of the append built-in in golang
 */

void do_append(int count, go_slice_t *s, int **src);
void do_append_1x1(int count, go_slice_t *s, int **src);
void do_insert_by_index(int count, go_slice_t *s, int **src);
void do_insert_by_pointer_math(int count, go_slice_t *s, int **src);

int main(int argc, char *argv[]) {
  int initial_size;
  double el_ptr, el_array, el_1x1, el_chunk;
  go_slice_t slice;
  int *arrs[4];

  if (argc < 2) {
    fputs("You must specify the size of the initial array\n", stderr);
    return(-1);
  }

  initial_size = atoi(argv[1]);
  if ((initial_size < VALS_PER_CHUNK) || (initial_size % VALS_PER_CHUNK != 0)) {
    fprintf(stderr, "Size is invalid - must be a positive multiple of %d\n", VALS_PER_CHUNK);
    return(-1);
  }

  /* Build a source array - two-dimensional so we can append in chunks */
  int chunk_count = initial_size/VALS_PER_CHUNK;
  printf("We will be using %d chunk(s)\n", chunk_count);
  int *src[chunk_count];
  for (int i = 0; i < chunk_count; i++) {
    src[i] = make_array(VALS_PER_CHUNK);
    for (int v = 0; v < VALS_PER_CHUNK; v++) {
      src[i][v] = (i * VALS_PER_CHUNK) + v;
    }
  }

  /*
   * Initialize array via pointer arithmetic
   */

  arrs[0] = slice.array = make_array(initial_size);
  if (slice.array != NULL) {
    el_ptr = benchmark(do_insert_by_pointer_math, chunk_count, &slice, src);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate array for pointer insertion\n");
  }

  /*
   * And now by indexing into the array
   */

  arrs[1] = slice.array = make_array(initial_size);
  if (slice.array != NULL) {
    el_array = benchmark(do_insert_by_index, chunk_count, &slice, src);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate array for index insertion\n");
  }

  /*
   * Now let's simulate the behavior of go's append built-in.
   */

  slice.array = make_array(1);
  slice.len = 0;
  slice.cap = 1;
  if (slice.array != NULL) {
    el_1x1 = benchmark(do_append_1x1, chunk_count, &slice, src);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate array for 1x1 appending\n");
  }
  arrs[2] = slice.array;

  /*
   * And now let's chunk the elements (add them in batches to take advantage of variadicity)
   */

   slice.array = make_array(1);
   slice.len = 0;
   slice.cap = 1;
   if (slice.array != NULL) {
     el_chunk = benchmark(do_append, chunk_count, &slice, src);
   } else {
     fprintf(stderr, "ERROR: Unable to allocate slice for chunked appending\n");
   }
   arrs[3] = slice.array;

   printf("  count     by pointer      by index      append 1x1    append chunked\n");
   printf("----------|--------------|--------------|--------------|--------------\n");
   printf("%10d,%15.7f,%15.7f,%15.7f,%15.7f\n", initial_size, el_ptr, el_array, el_1x1, el_chunk);

   for (int i = 0; i < 4; i++) {
     free(arrs[i]);
   }
}

void do_append(int count, go_slice_t *s, int **src) {
  for (int i = 0; i < count; i++) {
    APPEND(s, EXPAND(src[i]));
  }
}

void do_append_1x1(int count, go_slice_t *s, int **src) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < VALS_PER_CHUNK; j++) {
      APPEND(s, src[i][j]);
    }
  }
}

void do_insert_by_index(int count, go_slice_t *s, int **src) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < VALS_PER_CHUNK; j++) {
      s->array[(i * VALS_PER_CHUNK) + j] = src[i][j];
    }
  }
}

void do_insert_by_pointer_math(int count, go_slice_t *s, int **src) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < VALS_PER_CHUNK; j++) {
      *(s->array) = src[i][j];
      s->array++;
    }
  }
}
