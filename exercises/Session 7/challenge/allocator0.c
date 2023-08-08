#define ADDITIONAL_TASK_0

struct context {
	unsigned char *free, *end;
};

void dyn_init(void * buffer, size_t size) {
	struct context * ctx = buffer;
	ctx->free = (void*)(ctx + 1); // immediately following the header
	ctx->end = (unsigned char*)buffer + size; // past the end of the buffer
}

void * dyn_alloc(void * context, size_t size) {
	struct context * ctx = context;
	// bump up the size so that the next allocation would be aligned too
	size_t remainder = size % sizeof(long double);
	if (remainder) size += sizeof(long double) - remainder;
	if (size > ctx->end - ctx->free) return NULL;
	void * ret = ctx->free;
	ctx->free += size;
	return ret;
}
