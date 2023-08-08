#include <stdlib.h>
#include <stdio.h>

static void * my_malloc(size_t size) {
	void * ret = malloc(size);
	if (ret) return ret;
	printf("Failed an allocation of size %zu\n", size);
	exit(255);
}

int main(void) {
	size_t sz = 100;
	void * buf = my_malloc(sz);
	printf("Allocated a buffer of size %zu at %p\n", sz, buf);
	free(buf);

	my_malloc(0xffffffffffff);
	puts("This line will not be reached");
}
