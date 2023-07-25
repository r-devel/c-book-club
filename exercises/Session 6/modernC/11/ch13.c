#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Calculate the first derivative of `f` at `x` using the central difference method:
// Starting with a Taylor approximation:
// f(x + step) = f(x) + step * f'(x)
// f(x - step) = f(x) - step * f'(x)
// we get:
// f'(x) = (f(x + step) - f(x - step)) / step / 2
static double deriv(double (*f)(double x), double x, double step) {
	return (f(x + step) - f(x - step)) / step / 2;
}
// Same thing, but we use a Taylor series in two dimensions, Re x and
// Im x:
// f(Re(x + step), Im(x + step)) = f(x) +
//                                + d f / d Re x * Re(step)
//                                + d f / d Im x * Im(step)
// f(Re(x - step), Im(x - step)) = f(x) +
//                                - d f / d Re x * Re(step)
//                                - d f / d Im x * Im(step)
// Which gives the same result in the end.
static double complex cderiv(
	double complex (*f)(double complex x),
	double complex x, double complex step
) {
	return (f(x + step) - f(x - step)) / step / 2;
}
// It's possible to optimise the step value for a given function at a
// given point, but that's outside the scope of the challenge.
#define STEP (1e-6)

// Find a root of the equation f(x) = 0 using the Newton's method with
// x0 as the starting point. Assume convergence when relative change in
// `x` is less than `relstop`.
static double findroot(double (*f)(double x), double x0, double relstop) {
	double x;
	for (;;) {
		x = x0 - f(x0)/deriv(f, x0, STEP);
		if (fabs((x - x0)/x0) < relstop) return x;
		x0 = x;
	}
}
// Newton's method is described as working equally well for complex
// numbers, but the step used in the derivative calculation must be
// complex too.
static double complex cfindroot(
	double complex (*f)(double complex x), double complex x0,
	double relstop
) {
	double complex x;
	for (;;) {
		x = x0 - f(x0)/cderiv(f, x0, STEP+STEP*I);
		if (cabs((x - x0)/x0) < relstop) return x;
		x0 = x;
	}
}

static double golden(double phi) {
	return phi*phi - phi - 1;
}

static double complex cpoly(double complex x) {
	return (x - 1) * (x * x + x + 1);
}

int main(void) {
	double reltol = 1e-6;

	{
		double x0 = 1.5;
		puts("Trying to find the golden ratio by solving phi^2 = phi + 1...");
		printf("It's around %g, right?\n", x0);
		printf("It's actually %g, with relative tolerance of %g.\n", findroot(golden, x0, reltol), reltol);
	}

	puts("");

	{
		puts("Trying a complex polynomial, (x - 1) * (x^2 + x + 1).");
		printf(
			"Its roots should be 1 and also -0.5 +/- sqrt(3/4),"
			"which is around %g.\n",
			sqrt(3./4)
		);
		const double complex x0[] = {
			2, -1. + I, -1. - I
		};
		for (size_t i = 0; i < sizeof x0/sizeof *x0; ++i) {
			double complex x = x0[i];
			printf("Starting with %g+%gi, we get ", creal(x), cimag(x));
			x = cfindroot(cpoly, x, reltol);
			printf("%g+%gi.\n", creal(x), cimag(x));
		}
	}
}
