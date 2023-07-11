#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dyn_init(void * buffer, size_t size);
void * dyn_alloc(void * context, size_t size);
void dyn_dealloc(void * context, void ** ptr);
#include "allocator.c" // should define the functions comprising the allocator

enum { BUFSIZE = 1048576 };
static alignas(long double) uint8_t buf1[BUFSIZE] = {0,}, buf2[BUFSIZE] = {0,};

// Print diagnostics for a test; terminate if test failed
static void ok(bool condition, const char * desc, ...) {
	printf("%s test: ", condition ? "Passed" : "Failed");

	va_list args;
	va_start(args, desc);
	vprintf(desc, args);
	va_end(args);
	puts("");

	if (!condition) exit(1);
}

// Find a zero-terminated string in a buffer
static bool memfind(const void * buf, size_t bufsz, const char * substr) {
	size_t slen = strlen(substr) + 1; // find this many bytes
	for (size_t offset = 0; offset + slen < bufsz; ++offset)
		if (!memcmp((const char*)buf + offset, substr, slen)) return true;
	return false;
}

int main() {
	{
		dyn_init(buf1, sizeof buf1);
		enum { SZ_DIV=3 };
		void * ptr = dyn_alloc(buf1, sizeof buf1 / SZ_DIV);
		ok(
			ptr,
			"must be able to allocate 1/%d of the original buffer",
			SZ_DIV
		);
		memset(ptr, 0xaa, sizeof buf1 / SZ_DIV);
		ok(true, "must be able to fill the allocated buffer without crashing");
		dyn_alloc(buf1, sizeof buf1 / SZ_DIV);
		dyn_alloc(buf1, sizeof buf1 / SZ_DIV);
		ok(
			!dyn_alloc(buf1, sizeof buf1 / SZ_DIV),
			"must return NULL when the space runs out"
		);
	}

	{
		dyn_init(buf1, sizeof buf1);
		dyn_init(buf2, sizeof buf2);
		ok(true, "must initialise two separate buffers without crashing");

		const char *src1 = "Hello \x55 \xaa", *src2 = "world \xaa \x55";
		char * str1 = dyn_alloc(buf1, strlen(src1)+1);
		char * str2 = dyn_alloc(buf2, strlen(src2)+1);
		strncpy(str1, src1, strlen(src1)+1);
		strncpy(str2, src2, strlen(src2)+1);
		ok(
			memfind(buf1, sizeof buf1, src1),
			"string #1 must be found in buffer #1"
		);
		ok(
			memfind(buf2, sizeof buf2, src2),
			"string #2 must be found in buffer #2"
		);
		ok(
			!memfind(buf1, sizeof buf1, src2),
			"string #2 must not be found in buffer #1"
		);
		ok(
			!memfind(buf2, sizeof buf2, src1),
			"string #1 must not be found in buffer #2"
		);
	}

	#ifdef ADDITIONAL_TASK_0
	{
		dyn_init(buf1, sizeof buf1);
		const size_t alignment = sizeof(long double);
		// Presumably, prime-sized allocations would be more likely to trip up
		// alignment-related problems
		const size_t sizes = {0, 1, 3, 5, 7, 11, 13, 17, 19};
		for (size_t i = 0; i < sizeof sizes / sizeof *sizes; ++i) ok(
			((uintptr_t)dyn_alloc(buf1, sizes[i]) % alignment) == 0,
			"allocation of size %zu must result in a pointer divisible "
			"by sizeof(long double) == %zu",
			sizes[i], alignment
		);
		long double * p = dyn_alloc(buf1, sizeof *p);
		*p = 0.42L;
		ok(
			true,
			"must be able to write a long double "
			"into a pointer returned by the allocator "
			"without crashing"
		);
	}
	#endif

	#ifdef ADDITIONAL_TASK_1
	{
		dyn_init(buf1, sizeof buf1);
		enum { N_POINTERS = 5 };
		void ptrs[N_POINTERS];
		unsigned int allocated = 0;
		for (size_t i = 0; i < N_POINTERS; ++i) {
			ptrs[i] = dyn_alloc(buf1, sizeof buf1 / N_POINTERS);
			if (ptrs[i]) {
				++allocated;
				memset(
					ptrs[i],
					i%2 ? 0x55 : 0xaa,
					sizeof buf1 / N_POINTERS
				);
			}
		}
		ok(allocated >= 2, "at least 2 out of %d allocations must succeed", N_POINTERS);
		// some of the allocations will fail, this is fine
		for (size_t i = 0; i < N_POINTERS; ++i)
			if (ptrs[i]) dyn_dealloc(buf1, &ptrs[i]);
		ok(true, "must deallocate previously-made allocations without crashing");
		ok(
			dyn_alloc(buf1, sizeof buf1 / (N_POINTERS+1)),
			"must be able to allocate again after freeing all previously-made allocations"
		);
	}
	#endif
}
