struct {
	union {
		char a, b;
		int c;
	} d;
	int e[5];
} f = {0,}, *p = &f;

int main(void) {
	// p->b = ' '; // b is a member of p->d
	p->e[3] = 10;
	(*p).d.a = '*';
	// p->d->c = 20; // d is not a pointer
	return 0;
}
