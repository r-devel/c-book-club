#include <stdio.h>

static void cycle(double * x1, double * x2, double * x3) {
	// x1, x2, x3 -> x3, x1, x2
	double x3tmp = *x3;
	*x3 = *x2;
	*x2 = *x1;
	*x1 = x3tmp;
}

int main(void) {
	double x1 = 1, x2 = 2, x3 = 3;
	printf("Before: x1 = %g, x2 = %g, x3 = %g\n", x1, x2, x3);
	cycle(&x1, &x2, &x3);
	printf("After:  x1 = %g, x2 = %g, x3 = %g\n", x1, x2, x3);
	return 0;
}
