#include <assert.h>
#include <stddef.h>
#include <stdint.h>

struct header {
	size_t n_blocks;
	uint8_t * userdata;
	uint8_t bitmap[];
};

enum {
	// using 1 byte of metadata per this much user data
	BLKSZ = 512,
	// these codes denote the state of each block
	STATE_FREE = 0,
	STATE_START = 1,
	STATE_USED = 2,
	// resulting pointers must be aligned like this
	ALIGNMENT = sizeof(long double)
};

void dyn_init(void * buffer, size_t size) {
	struct header * ctx = buffer;
	// size = sizeof(header) + n_blocks + alignment_offset + BLKSZ * n_blocks
	ctx->n_blocks = (size - sizeof(*ctx)) / (BLKSZ + 1);
	ctx->userdata = ctx->bitmap + ctx->n_blocks;
	// we must also make sure that the user data starts on an aligned
	// address, which may mean leaving some blocks unused
	size_t remainder = (uintptr_t)ctx->userdata % ALIGNMENT;
	if (remainder) {
		ctx->userdata += ALIGNMENT - remainder;
		ctx->n_blocks -= ALIGNMENT/BLKSZ + 1 * ((ALIGNMENT % BLKSZ) > 0);
	}
	for (size_t i = 0; i < ctx->n_blocks; ++i)
		ctx->bitmap[i] = STATE_FREE;
}

// Look up the bitmap contents corresponding to a user pointer
static inline uint8_t * lookup_bitmap(
	struct header * ctx, const uint8_t * ptr
) {
	uint8_t * ret = ctx->bitmap + (ptr - ctx->userdata) / BLKSZ;
	// user pointers should always point to the start of a block
	assert(*ret == STATE_START);
	return ret;
}

// Return the offset of the _next_ block marked free, or NULL if none
static uint8_t * next_free_block(
	struct header * ctx, uint8_t * bit
) {
	const uint8_t * end = ctx->bitmap + ctx->n_blocks;
	for (; bit < end; ++bit) if (*bit == STATE_FREE) break;
	return bit < end ? bit : NULL;
}

// Return the size of the current block in blocks
static size_t block_size(
	const struct header * ctx, const uint8_t * start
) {
	// Free blocks run until STATE_FREE ends.
	// Allocated blocks run while STATE_USED is in effect.
	uint8_t lookfor = *start == STATE_FREE ? STATE_FREE : STATE_USED;

	const uint8_t * block = start+1;
	for (
		// start from the second block, but make sure not to overrun the
		// buffer
		const uint8_t * end = ctx->bitmap + ctx->n_blocks;
		block < end && *block == lookfor;
		++block
	);
	return block - start;
}

// We get this automatically by using a multiplier of sizeof(long
// double) as BLKSZ
#define ADDITIONAL_TASK_0

static inline size_t size_to_blocks(size_t size) {
	return size / BLKSZ + (size % BLKSZ > 0);
}

void * dyn_alloc(void * context, size_t size) {
	if (!size) return NULL;
	struct header * ctx = context;
	uint8_t *block = next_free_block(ctx, ctx->bitmap);
	const uint8_t * end = ctx->bitmap + ctx->n_blocks;
	for (
		;
		block && block < end;
		block = next_free_block(ctx, block)
	) {
		size_t sz = block_size(ctx, block);
		// Does the current free block satisfy the requirement?
		if (sz * BLKSZ >= size) break;
		// No, skip it
		block += sz;
	}
	if (!block || block == end) return NULL;
	// Must mark the block as used
	size_t nblocks = size_to_blocks(size);
	*block = STATE_START;
	for (size_t i = 1; i < nblocks; ++i) block[i] = STATE_USED;
	// Return the corresponding pointer
	return ctx->userdata + BLKSZ * (block - ctx->bitmap);
}

#define ADDITIONAL_TASK_1
void dyn_dealloc(void * context, void ** ptr) {
	// avoid crashing on dealloc(NULL)
	if (!*ptr) return;

	struct header * ctx = context;
	// find out the bitmap offset and start marking the block free
	uint8_t *bit = lookup_bitmap(ctx, *ptr);
	*bit = STATE_FREE;
	// mark the rest of the block free until we reach the start of a new
	// block or a free block
	for (++bit; *bit == STATE_USED; ++bit)
		*bit = STATE_FREE;

	*ptr = NULL;
}

#define ADDITIONAL_TASK_2
bool dyn_realloc(void * context, void ** ptr, size_t newsize) {
	if (!newsize) { // deallocating?
		dyn_dealloc(context, ptr);
		return true;
	}
	if (!*ptr) { // allocating anew?
		void * p = dyn_alloc(context, newsize);
		if (ptr) { *ptr = p; return true; }
		return false;
	}
	// trying to change the allocation in place
	struct header * ctx = context;
	uint8_t * block = lookup_bitmap(ctx, *ptr);
	size_t nblocks = block_size(ctx, block);
	size_t newblocks = size_to_blocks(newsize);
	if (newsize < nblocks * BLKSZ) {
		// shrinking is always possible in place
		for (size_t i = newblocks+1; i < nblocks; ++i)
			block[i] = STATE_FREE;
		return true;
	}
	uint8_t * nextblock = block + nblocks;
	if (
		(nextblock < ctx->bitmap + ctx->n_blocks)
		&& *nextblock == STATE_FREE
		&& (block_size(ctx, nextblock) + nblocks) * BLKSZ >= newsize
	) {
		// can enlarge in place!
		for (size_t i = nblocks; i < newblocks; ++i)
			block[i] = STATE_USED;
		return true;
	}
	// Otherwise have to move the allocation
	void * newblock = dyn_alloc(context, newsize);
	if (!newblock) return false;
	memmove(newblock, *ptr, nblocks * BLKSZ);
	dyn_dealloc(context, ptr);
	*ptr = newblock;
	return true;
}
