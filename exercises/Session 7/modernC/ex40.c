#include <stdio.h>
void fgoto (unsigned n) {
	unsigned * q = 0;
	unsigned * p = &((unsigned){0,});
	for (unsigned j = 1; j <= n; ++j) {
		printf(
			"%u: p and q are %s, *p is %u\n",
			j, (q == p) ? "equal" : "unequal", *p
		);
		q = p;
		*p = j;
	}
}
int main(void) {
	fgoto(0);
	puts("");
	fgoto(1);
	puts("");
	fgoto(2);
	puts("");
	fgoto(8);
}
