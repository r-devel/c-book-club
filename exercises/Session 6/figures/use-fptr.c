#include <stdio.h>
#include <stdlib.h>

void f(void) {
	puts("Help! I'm being called as an atexit() handler!");
}

int main(void) {
	atexit(f);
	atexit(&f);
	atexit(&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*f);
	atexit(****************************f);
	puts("About to return from main()");
	return 0;
}
