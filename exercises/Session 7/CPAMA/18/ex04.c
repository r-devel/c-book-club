int f(int i) {
	static int j = 0;
	return i * j++;
}

#include <stdio.h>

int main(void) {
	printf("Calling f(10) for the first time: %d\n", f(10));
	for (int i = 0; i < 4; ++i) f(0);
	printf("Calling f(10) after five previous calls: %d\n", f(10));
	return 0;
}
