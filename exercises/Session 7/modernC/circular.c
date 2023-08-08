#include <stdlib.h>
#include <string.h>
#include "circular.h"

/** @brief the hidden implementation of the circular buffer type */
struct circular {
  size_t start;    /**< Position of element 0 */
  size_t len;      /**< Number of elements stored */
  size_t max_len;  /**< Maximum capacity */
  double* tab;     /**< Array holding the data */
};

circular* circular_init(circular* c, size_t max_len) {
  if (c) {
    if (max_len) {
      *c = (circular){
        .max_len = max_len,
        .tab = malloc(sizeof(double[max_len])),
      };
        // Allocation failed.
      if (!c->tab) c->max_len = 0;
    } else {
      *c = (circular){ 0 };
    }
  }
  return c;
}

void circular_destroy(circular* c) {
  if (c) {
    free(c->tab);
    circular_init(c, 0);
  }
}

static size_t circular_getpos(circular* c, size_t pos) {
  pos += c->start;
  pos %= c->max_len;
  return pos;
}

#if 0
circular* circular_new(size_t len) {
  return circular_init(malloc(sizeof(circular)), len);
}

void circular_delete(circular* c) {
  circular_destroy(c);
  free(c);
}


size_t circular_getlength(circular* c) {
  return c ? c->len : 0;
}

circular* circular_append(circular* c, double value) {
  if (c) {
    double* where = circular_element(c, c->len);
    if (where) {
      *where = value;
      ++c->len;
      return c;
    }
  }
  return 0;
}

double circular_pop(circular* c) {
  double ret = 0.0;
  if (c && c->len) {
    double* p = circular_element(c, 0);
    if (p) ret = *p;
    ++c->start;
    --c->len;
  }
  return ret;
}
#endif

double* circular_element(circular* c, size_t pos) {
  double* ret = 0;
  if (c) {
    if (pos < c->max_len) {
      pos = circular_getpos(c, pos);
      ret = &c->tab[pos];
    }
  }
  return ret;
}



#if 0
circular* circular_resize(circular* c, size_t nlen) {
  if (c) {
    size_t len = c->len;
    if (len > nlen) return 0;
    size_t olen = c->max_len;
    if (nlen != olen) {
      size_t ostart = circular_getpos(c, 0);
      size_t nstart = ostart;
      double* otab = c->tab;
      double* ntab;
      if (nlen > olen) {
        ntab = realloc(c->tab, sizeof(double[nlen]));
        if (!ntab) return 0;
        // Two separate chunks
        if (ostart+len > olen) {
          size_t ulen = olen - ostart;
          size_t llen = len - ulen;
          if (llen <= (nlen - olen)) {
            /* Copy the lower one up after the old end. */ /*@\label{cpy-lowchunk}*/
            memcpy(ntab + olen, ntab,
                   llen*sizeof(double));
          } else {
            /* Move the upper one up to the new end. */ /*@\label{cpy-hichunk}*/
            nstart = nlen - ulen;
            memmove(ntab + nstart, ntab + ostart,
                    ulen*sizeof(double));
          }
        }
      } else {
        if (ostart+len > olen) {
          // Two separate chunks; mv the upper one down to the new end.
          size_t ulen = olen - ostart;
          nstart = nlen - ulen;
          memmove(otab + nstart, otab + ostart, ulen*sizeof(double));
        } else {
          // A single chunk
          if (ostart + len > nlen) {
            // Reallocation cuts the existing chunk in two.
            memmove(otab, otab + ostart, len*sizeof(double));
            nstart = 0;
          }
        }
        // Now all data is saved in the conserved part of the array.
        ntab = realloc(c->tab, sizeof(double[nlen]));
        // If realloc fails in this case (would be weird), just overrule it.
        if (!ntab) ntab = otab;
      }
      *c = (circular){
        .max_len = nlen,
        .start = nstart,
        .len = len,
        .tab = ntab,
      };
    }
  }
  return c;
}
#endif

void circular_fput(circular* c, FILE* s) {
  if (c) {
    size_t len = circular_getlength(c);
    double* tab = c->tab;
    if (tab) {
      fprintf(s, "%p+%zu (%zu+%zu):", (void*)tab, c->max_len, c->start, len);
      for (size_t i = 0; i < len; ++i) {
        fprintf(s, "\t%g", *circular_element(c, i));
      }
      fputc('\n', s);
      return;
    }
  }
  fputs("invalid circular\n", s);
}
