#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/*
 * Simple program to demonstrate:
 * a) Initializing an array of a fixed size by looping through it
 * b) how one might implement a simple append() function
 *    with behavior roughly that of the append built-in in golang
 */

#define       NUMARGS(...)  (sizeof((int[]){0, ##__VA_ARGS__})/sizeof(int)-1)
#define       APPEND(s, ...)  append(s, NUMARGS(__VA_ARGS__), ##__VA_ARGS__)
#define       ELAPSED(f, s) (((double)(f - s))/CLOCKS_PER_SEC)
#define       EXPAND(a) a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],\
                        a[10],a[11],a[12],a[13],a[14],a[15],a[16],a[17],a[18],a[19],\
                        a[20],a[21],a[22],a[23],a[24],a[25],a[26],a[27],a[28],a[29],\
                        a[30],a[31],a[32],a[33],a[34],a[35],a[36],a[37],a[38],a[39],\
                        a[40],a[41],a[42],a[43],a[44],a[45],a[46],a[47],a[48],a[49],\
                        a[50],a[51],a[52],a[53],a[54],a[55],a[56],a[57],a[58],a[59],\
                        a[60],a[61],a[62],a[63],a[64],a[65],a[66],a[67],a[68],a[69],\
                        a[70],a[71],a[72],a[73],a[74],a[75],a[76],a[77],a[78],a[79],\
                        a[80],a[81],a[82],a[83],a[84],a[85],a[86],a[87],a[88],a[89],\
                        a[90],a[91],a[92],a[93],a[94],a[95],a[96],a[97],a[98],a[99]

#define VALS_PER_CHUNK 100

struct go_slice {
  int *array;
  int len;
  int cap;
};
typedef struct go_slice go_slice_t;


int *make_array(int size);
int append(go_slice_t *sp, int argc, ...);

int main(int argc, char *argv[]) {
  int initial_size;
  clock_t start_time, finish_time;
  double el_ptr, el_array, el_1x1, el_chunk;
  int *int_array;
  int *tmp_array;
  go_slice_t slice;

  if (argc < 2) {
    fputs("You must specify the size of the initial array\n", stderr);
    return(-1);
  }

  initial_size = atoi(argv[1]);
  if ((initial_size < VALS_PER_CHUNK) || (initial_size % VALS_PER_CHUNK != 0)) {
    fprintf(stderr, "Size is invalid - must be a positive multiple of %d\n", VALS_PER_CHUNK);
    return(-1);
  }

  tmp_array = int_array = make_array(initial_size);
  if (int_array == NULL) {
    fputs("Unable to allocate initial array\n", stderr);
    return(-1);
  }

  /*
   * Initialize array via pointer arithmetic
   */
  start_time = clock();
  for (int i = 0; i < initial_size; i++) {
    *tmp_array = i;
    tmp_array++;
  }
  finish_time = clock();

  el_ptr = ELAPSED(finish_time, start_time);
  printf("Initializing the array via pointer arithmetic took approximately %f seconds\n", el_ptr);

  free(int_array);

  /*
   * And now by indexing into the array
   */
  int_array = make_array(initial_size);
  if (int_array == NULL) {
   fputs("Unable to allocate initial array\n", stderr);
   return(-1);
  }

  start_time = clock();
  for (int i = 0; i < initial_size; i++) {
    int_array[i] = i;
  }
  finish_time = clock();

  el_array = ELAPSED(finish_time, start_time);
  printf("Initializing the array via indexing took approximately %f seconds\n", el_array);

  free(int_array);

  /*
   * Now let's simulate the behavior of go's append built-in.
   */

  int_array = make_array(1);
  slice.cap = 1;
  slice.len = 0;
  slice.array = int_array;

  /* First add elements one by one */
  start_time = clock();
  for (int i = 0; i < initial_size; i++) {
    APPEND(&slice, i);
  }
  finish_time = clock();

  el_1x1 = ELAPSED(finish_time, start_time);
  printf("Populating slice with %d elements (one at a time) took approximately %f seconds.  Capacity is now %d, last element is %d\n",
    slice.len, el_1x1, slice.cap, slice.array[slice.len - 1]);

  /*
   * And now let's chunk the elements (add them in batches to take advantage of variadicity)
   */

   int_array = make_array(1);
   slice.cap = 1;
   slice.len = 0;
   slice.array = int_array;

   /* Build a source array to make the batch appending workable */
   int chunk_count = initial_size/VALS_PER_CHUNK;
   printf("We will be using %d chunk(s)\n", chunk_count);
   int *src[chunk_count];
   for (int i = 0; i < chunk_count; i++) {
     src[i] = make_array(VALS_PER_CHUNK);
     for (int v = 0; v < VALS_PER_CHUNK; v++) {
       src[i][v] = (i * VALS_PER_CHUNK) + v;
     }
   }

   /* First add elements one by one */
   start_time = clock();
   for (int i = 0; i < chunk_count; i++) {
     APPEND(&slice, EXPAND(src[i]));
   }
   finish_time = clock();

   el_chunk = ELAPSED(finish_time, start_time);
   printf("Populating slice with %d elements (by chunk) took approximately %f seconds.  Capacity is now %d, last element is %d\n",
   slice.len, el_chunk, slice.cap, slice.array[slice.len - 1]);

   printf("=======================\n");
   printf("pointer: %f\n", el_ptr);
   printf("-----------------------\n");
   printf("array  : %f\n", el_array);
   printf("-----------------------\n");
   printf("1 x 1  : %f\n", el_1x1);
   printf("-----------------------\n");
   printf("chunked: %f\n", el_chunk);
}

/*
 * Returns an empty array with storage allocated for `size` elements
 * of type int
 */
int *make_array(int size) {
  return (int *)calloc(size, sizeof(int));
}

int append(go_slice_t *sp, int argc, ...) {
  /*
   * Do we need to grow?
   */
  int total = sp->len + argc;
  if (total > sp->cap) {
    int *new_array;
    int *tmp_array;
    int new_cap = (total * 3/2) + 1;
    /* Create new array and copy existing elements into it */
    tmp_array = new_array = make_array(new_cap);
    for (int i = 0; i < sp->len; i++) {
      *tmp_array = sp->array[i];
      tmp_array++;
    }

    /* Switch to the new array and free up the old one */
    tmp_array = sp->array;
    sp->array = new_array;
    free(tmp_array);
    sp->cap = new_cap;
  }

  /* And now add our new elements */
  va_list ap;
  va_start(ap, argc);
  for (int i = 0; i < argc; i++) {
    sp->array[sp->len++] = va_arg(ap, int);
  }

  return argc;
}
