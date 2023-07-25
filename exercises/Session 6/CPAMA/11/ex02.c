void foo(void) {
	int i, *p, *q;
	p = i;
	*p = &i;
	&p = q;
	p = &q;
	p = *&q;
	p = q;
	p = *q;
	*p = q;
	*p = *q;
}
