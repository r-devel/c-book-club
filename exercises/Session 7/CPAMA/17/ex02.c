#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char * duplicate(const char * src) {
	size_t len = strlen(src);

	char * ret = malloc(len+1);
	if (!ret) return ret; // or NULL

	memcpy(ret, src, len+1);
	return ret;
}

int main(void) {
	const char * src = "Hello world!";
	printf("Source string \"%s\" at %p\n", src, src);

	char * dest = duplicate(src);
	printf("Copy \"%s\" at %p\n", dest, dest);
}
