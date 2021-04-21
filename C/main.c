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

go_slice_t *make_slice(int size, int cap, int len);
int destroy_slice(go_slice_t *s);
void do_append(int count, go_slice_t *s, int **src);
void do_append_1x1(int count, go_slice_t *s, int **src);
void do_insert_by_index(int count, go_slice_t *s, int **src);
void do_insert_by_pointer_math(int count, go_slice_t *s, int **src);

int main(int argc, char *argv[]) {
  int initial_size;
  double el_ptr, el_array, el_1x1, el_chunk;
  go_slice_t *slice1, *slice2, *slice3, *slice4;

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

  slice1 = make_slice(initial_size, 0, 0);
  if (slice1 != NULL) {
    el_ptr = benchmark(do_insert_by_pointer_math, chunk_count, slice1, src);
    destroy_slice(slice1);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate slice for pointer insertion\n");
  }

  /*
   * And now by indexing into the array
   */

  slice2 = make_slice(initial_size, 0, 0);
  if (slice2 != NULL) {
    el_array = benchmark(do_insert_by_index, chunk_count, slice2, src);
    destroy_slice(slice2);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate slice for index insertion\n");
  }

  /*
   * Now let's simulate the behavior of go's append built-in.
   */

  slice3 = make_slice(1, 0, 1);
  if (slice3 != NULL) {
    el_1x1 = benchmark(do_append_1x1, chunk_count, slice3, src);
    destroy_slice(slice3);
  } else {
    fprintf(stderr, "ERROR: Unable to allocate slice for 1x1 appending\n");
  }

  /*
   * And now let's chunk the elements (add them in batches to take advantage of variadicity)
   */

   slice4 = make_slice(1, 0, 1);
   if (slice4 != NULL) {
     el_chunk = benchmark(do_append, chunk_count, slice4, src);
     destroy_slice(slice4);
   } else {
     fprintf(stderr, "ERROR: Unable to allocate slice for chunked appending\n");
   }

   printf("  count     by pointer      by index      append 1x1    append chunked\n");
   printf("----------|--------------|--------------|--------------|--------------\n");
   printf("%10d,%15.7f,%15.7f,%15.7f,%15.7f\n", initial_size, el_ptr, el_array, el_1x1, el_chunk);
}

go_slice_t *make_slice(int size, int len, int cap) {
  go_slice_t *s = (go_slice_t *)malloc(sizeof(go_slice_t));
  if(s != NULL) {
    s->array = make_array(size);
    if (s->array == NULL) {
      free(s);
      return(NULL);
    }

    s->len = len;
    s->cap = cap;
  }

  return(s);
}

int destroy_slice(go_slice_t *s) {
  int *tmp_array = s->array;
  if (tmp_array != NULL) {
    free(tmp_array);
  }
  free(s);
  return(0);
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
