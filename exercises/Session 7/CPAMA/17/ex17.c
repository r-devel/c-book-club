#include <stdio.h>
#include <stdlib.h>

int a[100];

static int cmp(const void * a, const void * b) {
	int aa = *(int*)a, bb = *(int*)b;
	return aa < bb ? -1
		: aa > bb ? +1
		: 0;
}

int main(void) {
	srand(0);
	for (size_t i = 0; i < sizeof(a)/sizeof(*a); ++i) a[i] = rand() % 100;

	qsort(a + 50, 50, sizeof *a, cmp);
	qsort(&a[50], 50, sizeof *a, cmp);
	return 0;
}
