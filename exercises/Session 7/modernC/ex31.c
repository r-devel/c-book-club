#include "circular.c"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

circular* circular_new(size_t len) {
	return circular_init(malloc(sizeof(circular)), len);
}
void circular_delete(circular* c) {
	if (!c) return;
	free(c->tab);
	free(c);
}

size_t circular_getlength(circular* c) {
	return c->len;
}

circular* circular_append(circular* c, double value) {
	// the element at c->len is just past the end
	double * new = circular_element(c, c->len);
	if (!new) return 0;

	*new = value;
	++c->len;
	return c;
}

double circular_pop(circular* c) {
	// NB: no good way to signal absence
	double ret = NAN;
	if (!c->len) return ret;

	ret = *circular_element(c, 0);
	c->start += 1;
	// start < max_len is an invariant
	c->start %= c->max_len;
	--c->len;

	return ret;
}

circular* circular_resize(circular* c, size_t max_len) {
	// can't keep the values
	if (c->len > max_len) return 0;
	// nothing to do?
	if (max_len == c->max_len) return c;

	bool pre_wrapping = false;
	size_t saved_start = 0;
	if (
		max_len < c->max_len // shrinking?
		&& c->len // and have data?
		&& circular_getpos(c, c->len-1) < c->start // and wrapped around?
	) {
		// must wrap around the upper part of the block first
		pre_wrapping = true;
		saved_start = c->start;
		memmove(
			c->tab + c->start - (c->max_len - max_len),
			c->tab + c->start,
			sizeof(double[c->max_len - c->start])
		);
		c->start = max_len - (c->max_len - c->start);
	}

	double *newtab = realloc(c->tab, sizeof(double[max_len]));
	if (!newtab) {
		// allocation failed for whatever reason
		if (pre_wrapping) {
			// must undo pre-wrapping
			// (yes, shrinking an allocation *may* fail too)
			memmove(
				c->tab + max_len, c->tab,
				sizeof(double[c->start + c->len - max_len])
			);
			c->start = saved_start;
		}
		return 0;
	}
	c->tab = newtab;

	if (
		max_len > c->max_len // enlarging?
		&& c->len // and have data?
		&& circular_getpos(c, c->len-1) < c->start // and wrap around?
	) {
		// must move the upper block towards the end of the new buffer
		// NB: they may intersect if enlarging just a bit
		memmove(
			c->tab + max_len - (c->max_len - c->start),
			c->tab + c->start,
			sizeof(double[c->max_len - c->start])
		);
		c->start += max_len - c->max_len;
	}
	c->max_len = max_len;
	return c;
}

int main() {
	circular * c = circular_new(11);
	int i = 0;
	for (; i < 10; ++i) {
		printf("circular_append(%p, %d)\n", (void*)c, i);
		if (!circular_append(c, i)) exit(-1);
	}
	for (i = 0; i < 7; ++i)
		printf("circular_pop(%p) -> %g\n", (void*)c, circular_pop(c));
	for (i = 10; i < 16; ++i) {
		printf("circular_append(%p, %d)\n", (void*)c, i);
		if (!circular_append(c, i)) exit(-1);
	}
	circular_fput(c, stdout);
	if (!circular_resize(c, 19)) exit(-2);
	circular_fput(c, stdout);
	if (!circular_resize(c, 10)) exit(-2);
	circular_fput(c, stdout);
	circular_delete(c);
	return 0;
}
