#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void leak(size_t sz) {
	void * ptr = malloc(sz);
	if (ptr) printf("Allocated %f GB at %p\n", sz/1e9, ptr);
	else printf("Failed to allocate %f GB\n", sz/1e9);
}

int main(void) {
	size_t bounds[2] = { 0, SIZE_MAX };
	while (bounds[0] < bounds[1] - 1) {
		size_t try = (bounds[0] + bounds[1]) / 2;
		void * ptr = malloc(try);
		bounds[!ptr] = try;
		free(ptr);
	}

	printf(
		"We could allocate up to %f GB of memory.\n",
		bounds[0]/1e9
	);

	leak(bounds[0]);
	leak(bounds[0]);
	leak(bounds[0]/2);
	leak(bounds[0]/2);
	leak(bounds[0]/2);
	leak(bounds[0]/2);
}
