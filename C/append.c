#include "append.h"
#include <stdio.h>

/*
 * Returns an empty array with storage allocated for `size` elements
 * of type int
 */
int *make_array(int size) {
  return (int *)calloc(size, sizeof(int));
}

int append(go_slice_t *sp, int argc, ...) {
  if (!sp->array) {
    return(-1);
  }
  /*
   * Do we need to grow?
   */
  int total = sp->len + argc;
  if (total > sp->cap) {
    int *new_array;
    int *tmp_array;
    int new_cap = (total * 3/2) + 1;

    /* First attempt just to resize */
    fprintf(stderr, "DEBUG: Will attempt to resize\n");
    /*new_array = realloc(sp->array, (new_cap * sizeof(int)));
    if (new_array == NULL) {
      fprintf(stderr, "DEBUG: Unable to resize, creating new array\n");
      /* Nope, so create new array and copy existing elements into it */
      tmp_array = new_array = make_array(new_cap);
      for (int i = 0; i < sp->len; i++) {
        *tmp_array = sp->array[i];
        tmp_array++;
      }

      /* Switch to the new array and free up the old one */
      /* tmp_array = sp->array;
      sp->array = new_array;
      free(tmp_array); */
      if (sp->array == NULL) {
        fprintf(stderr, "ERROR: Why in the hell is the array NULL??\n");
      }
      free(sp->array);
      sp->array = new_array;
    /*} else {
      sp->array = new_array;
    } */
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
