#include <stdio.h>

union dptr {
	double * ptr;
	unsigned char bytes[sizeof(double*)];
};

int main(void) {
	double numbers[2] = {3.14, 0.15926};

	for (size_t i = 0; i < 2; ++i) {
		union dptr val = { .ptr = &numbers[i] };
		printf("&numbers[%zu] is %p = ", i, (void*)val.ptr);
		for (size_t j = 0; j < sizeof val.bytes; ++j)
			printf("%s0x%02x", j ? ", " : "", val.bytes[j]);
		puts("");
	}
}
