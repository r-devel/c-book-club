#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// static int * create_array(int n, int initial_value)

static int * create_array(size_t n, int initial_value) {
	// n * sizeof(int) may overflow
	if (SIZE_MAX / sizeof(int) < n) return NULL;

	int * ret = malloc(sizeof(int[n])); // or: sizeof(int) * n
	if (!ret) return ret;

	for (size_t i = 0; i < n; ++i) ret[i] = initial_value;

	return ret;
}

int main(void) {
	volatile size_t n = 10;
	int initial_value = 42;
	int * array = create_array(n, initial_value);
	if (!array) return -1;
	printf(
		"Got an array of %zu elements filled with %d at %p:\n",
		n, initial_value, (void*)array
	);
	for (size_t i = 0; i < n; ++i)
		printf(i ? ", %d" : "%d", array[i]);
	puts("");
}
