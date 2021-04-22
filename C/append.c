#include "append.h"
#include <stdio.h>

/*
 * Returns an empty array with storage allocated for `size` elements
 * of type int
 */
int *make_array(int size) {
  return (int *)malloc((size_t)(size * sizeof(int)));
}

int append(go_slice_t *sp, int argc, ...) {
  if (!sp->array) {
    return(-1);
  }
  /*
   * Do we need to grow?
   */
  int new_cap = sp->cap;
  int requested = sp->len + argc;
  int doublecap = sp->cap + sp->cap;
  if (requested > doublecap) {
    new_cap = requested;
  } else {
    if (sp->cap < 1024) {
      new_cap = doublecap;
    } else {
      while((new_cap > 0) && (new_cap < requested)) {
        new_cap += (new_cap / 4);
      }
      if (new_cap <= 0) {
        new_cap = requested;
      }
    }
  }

  if (new_cap > sp->cap) {
    int *new_array;
    int *tmp_array;

    /* First attempt just to resize */
    new_array = realloc(sp->array, (new_cap * sizeof(int)));
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
      } else {
        free(sp->array);
      }
      sp->array = new_array;
    } else {
      sp->array = new_array;
    }
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
