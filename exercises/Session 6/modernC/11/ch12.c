/*
** Challenge 12 (text processor).
** For a text processor, can you use a doubly linked list to store text?
** The idea is to represent a “blob” of text through a struct that
** contains a string (for the text) and pointers to preceding and
** following blobs.
** Can you build a function that splits a text blob in two at a given
** point?
** One that joins two consecutive text blobs?
** One that runs through the entire text and puts it in the form of one
** blob per line?
** Can you create a function that prints the entire text or prints until
** the text is cut off due to the screen size?
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An "intrusive" linked list contains the pointers right inside the
// structure that lives inside said list.
typedef struct blob_ {
	char * str;
	struct blob_ *prev, *next;
} blob;

// Handling allocation failures is Session 7 material;
// we just bail out at the first sight of trouble
static void * Malloc(size_t sz) {
	void * ret = malloc(sz);
	if (!ret) exit(-1);
	return ret;
}
static char * Strdup(const char * src) {
	size_t len = strlen(src);
	char * ret = Malloc(len+1); // including the NUL terminator
	strcpy(ret, src);
	return ret;
}
static char * Strcat(const char * a, const char * b) {
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	// NB: lena couldn't be equal to SIZE_MAX because the terminator
	// lives in the same array, which means that it's safe to subtract
	// 1 more
	if (SIZE_MAX - lena - 1 < lenb) exit(-1); // overflow
	char * ret = Malloc(lena + lenb + 1); // include the terminator
	strcpy(ret, a);
	strcpy(ret+lena, b);
	return ret;
}

// If the length of `src->str` exceeds `where`, split off the remaining
// part into the following blob.
void blob_split(blob * src, size_t where) {
	if (where >= strlen(src->str)) return;

	blob * next = Malloc(sizeof(blob));

	// Split the string
	next->str = Strdup(src->str + where);
	src->str[where] = '\0';

	// Establish the new invariant: `next` now sits between `src` and
	// `src->next`. Need to change 1+2+1 = 4 pointers.
	next->prev = src;
	next->next = src->next;
	src->next  = next;
	if (next->next) next->next->prev = next; // used to be src
}

// If a `next` blob exists, join it together with the current one.
static void blob_join(blob * x) {
	blob * next = x->next;
	if (!next) return;

	// Concatenate the strings
	char * newstr = Strcat(x->str, next->str);
	free(x->str);
	// Store the new string pointer in the blob
	x->str = newstr;
	// Establish the new invariant: x is now followed by next->next.
	// Need to update up to two pointers (x->next and maybe
	// x->next->prev).
	x->next = next->next;
	if (x->next) x->next->prev = x;
	// Clean up the redundant blob and its contents
	free(next->str);
	free(next);
}

static void blob_split_newlines(blob * x) {
	for (;;) {
		char * newline = strchr(x->str, '\n');
		// Done if no newline found or newline at the end of the string
		if (!newline || !newline[1]) break;
		// Split after the newline
		blob_split(x, newline - x->str + 1);
		// Keep splitting the rest of the string
		x = x->next;
	}
}

// Print a debugging representation of the blob chain, following the `next` pointers.
static void blob_show(const blob * x) {
	if (x->prev) printf("=>"); // is there a previous blob in the chain?
	for (;x;x = x->next) printf(
		"%s*(blob*)(%p) = {.str = \"%s\"@%p, .prev = %p, .next = %p}%s\n",
		x->prev ? " " : "", (void*)x,
		x->str, (void*)x->str, (void*)x->prev, (void*)x->next,
		x->next ? " =>" : ""
	);
}

// Deallocate all memory taken by the blob chain. Again, this is Session
// 7 material, but we need this (or an equivalent to dynamic memory
// allocation) to be correct.
static void blob_free(blob * x) {
	// Rewind first
	while (x->prev) x = x->prev;
	while (x) {
		blob * next = x->next;
		free(x->str);
		free(x);
		// NB: cannot access x->next at this point
		x = next;
	}
}

int main(void) {
	blob * x = Malloc(sizeof(blob));
	*x = (blob){
		.str = Strdup("1234567890"),
		.prev = NULL, .next = NULL
	};
	puts("Before splitting:");
	blob_show(x);
	enum { SPLIT_AT = 5 };
	blob_split(x, SPLIT_AT);
	printf("After splitting at %d:\n", SPLIT_AT);
	blob_show(x);
	blob_join(x);
	puts("After joining again:");
	blob_show(x);
	blob_free(x);

	const char * poetry =
		"To all\n"
		"whom I like, or have liked in the past,\n"
		"whom, as icons, I keep in the cave of my soul,\n"
		"I raise, like a party guest raises a glass,\n"
		"my skull, with verse filled whole.\n"
	; // tranlsated by Serge Elnitsky, https://www.elnitsky.com/flute2.html
	x = Malloc(sizeof(blob));
	*x = (blob){
		.str = Strdup(poetry),
		.prev = NULL, .next = NULL
	};
	puts("Before splitting:");
	blob_show(x);
	blob_split_newlines(x);
	puts("After splitting at newlines:");
	blob_show(x);
	blob_free(x);
}
