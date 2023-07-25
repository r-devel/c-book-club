#include <stdio.h>

#include "rationals.c"

char const * rat_print (size_t len, char tmp[len], rat const * x) {
	snprintf(tmp, len, "%c%zu/%zu", x->sign ? '-' : '+', x->num, x->denom);
	return tmp;
}

int main(void) {
	rat x = rat_get(22LL, 7ULL);
	// FIXME: what's the lifetime of the compound literal here?
	char const * str = rat_print(1+20+20+1, (char[1+20+20+1]){0,}, &x);
	puts(str);
}
