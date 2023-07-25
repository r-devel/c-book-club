int sum_array(const int a[], int n) {
	int i, sum;

	sum = 0;
	for (i = 0; i < n; i++)
		sum += a[i];
	return sum;
}

int sum_array2(size_t n, const int a[n]) {
	int sum = 0, *end = a+n;
	for (; a < end; ++a)
		sum += *a;
	return sum;
}
