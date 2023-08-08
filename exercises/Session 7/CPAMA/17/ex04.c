#include <stdio.h>
#include <stdlib.h>

struct point { int x, y; };
struct rectangle {
	struct point upper_left, lower_right;
} * p;

int main(void) {
	p = malloc(sizeof(*p));
	//p->upper_left.x  = 10;
	//p->upper_left.y  = 25;
	//p->lower_right.x = 20;
	//p->lower_right.y = 15;

	*p = (struct rectangle){
		.upper_left  = { .x = 10, .y = 25 },
		.lower_right = { .x = 20, .y = 15 },
	};
	printf(
		"Rectangle((%d, %d), (%d, %d))\n",
		p->upper_left.x,
		p->upper_left.y,
		p->lower_right.x,
		p->lower_right.y
	);
}
