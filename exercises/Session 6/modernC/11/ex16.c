#include <stdio.h>
#include "rationals.c"

char const * rat_print(size_t len, char tmp[len], rat const * x) {
	snprintf(tmp, len, "%c%zu/%zu", x->sign ? '-' : '+', x->num, x->denom);
	return tmp;
}

rat* rat_dotproduct(rat rp[static 1], size_t n, rat const A[n], rat const B[n]) {
	rat_init(rp, 0LL, 1ULL);
	for (size_t i = 0; i < n; ++i)
		rat_rma(rp, A[i], B[i]);
	return rp;
}

enum { SERIES_SIZE = 31 };
rat series[SERIES_SIZE];

int main(void) {
	rat sum;
	for (size_t i = 0; i < SERIES_SIZE; ++i) {
		rat_init(&series[i], 1, (1ULL << i));
		printf("%s%s",
			i ? ", " : "",
			rat_print(1+20+20+1, (char[1+20+20+1]){0,}, &series[i])
		);
	}
	putchar('\n');
	rat_dotproduct(&sum, SERIES_SIZE, series, series);
	// FIXME: what's the lifetime of the compound literal here?
	char const * str = rat_print(1+20+20+1, (char[1+20+20+1]){0,}, rat_normalize(&sum));
	printf("%s approx. equal to %g\n", str, (double)sum.num/sum.denom);

}
