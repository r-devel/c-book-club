all: \
	examples/stack_overflow \
	examples/large_malloc \
	CPAMA/17/ex01 \
	CPAMA/17/ex02 \
	CPAMA/17/ex03 \
	modernC/ex31 \
	modernC/ex33 \
	modernC/ex38 modernC/ex38.s \
	modernC/ex40 modernC/ex40.s \
	CPAMA/17/ex04 \
	CPAMA/17/ex05 \
	CPAMA/17/ex15 \
	CPAMA/17/ex17 \
	CPAMA/16/inventory \
	CPAMA/18/ex04 \

CFLAGS = -g -Og -Wall -Wextra -pedantic --std=c11

.SUFFIXES: .c .R .svg .png .s
.PHONY: all

R = R
.R.svg:
	$(R)script $^ $@
.R.png:
	$(R)script $^ $@

.c.s:
	$(CC) -Wall -Wextra -Wpedantic -O3 -S -o $@ $^

CPAMA/16/inventory: CPAMA/16/inventory.o CPAMA/16/readline.o
