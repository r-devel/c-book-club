#include <stdio.h>
#include <stdlib.h>

//void find_two_largest(int a[], int n, int * largest, int *second_largest)
void find_two_largest(
	size_t n, const int a[n], int * largest, int *second_largest
) {
	if (n == 0) return;
	// make sure the variables are initialised
	*largest = *second_largest = *a;
	for (size_t i = 1; i < n; ++i) {
		if (a[i] > *largest) {
			*second_largest = *largest;
			*largest = a[i];
		} else if (a[i] > *second_largest) {
			*second_largest = a[i];
		}
	}
}


enum { SIZE = 32 };
int main(void) {
	int a[SIZE];
	for (size_t i = 0; i < SIZE; ++i) {
		a[i] = rand() % 100;
		printf("%s%d", i ? ", " : "", a[i]);
	}
	puts("");

	int l1, l2;

	find_two_largest(SIZE, a, &l1, &l2);
	printf("Largest = %d; second largest = %d\n", l1, l2);
}
