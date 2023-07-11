Challenge: Implement a dynamic memory allocator
===============================================

Implement the following functions:

```c
void dyn_init(void * buffer, size_t size);
```
The `dyn_init` function should take a pointer to a buffer of a given
size and prepare the allocator to work. This function will be called
once per `buffer` prior to the `dyn_alloc()` calls.

(Aside: there should be no need for de-initialisation, but if the
application calls `dyn_init()` again on the same buffer, it will assume
that any pointers obtainer prior to this call are invalid and will
forget them.)

The same `buffer` pointer will be given to the other functions of the
allocator in order to let them access the context. Any relevant data
should be stored in the same buffer, so please don't use any static
state variables. For example, the following should work:

```c
uint8_t buf1[BUFSIZE], buf2[BUFSIZE];
dyn_init(buf1, sizeof buf1);
dyn_init(buf2, sizeof buf2);
void * ptr1 = dyn_alloc(buf1, 100);
void * ptr2 = dyn_alloc(buf2, 100);
```

The function is welcome to assume that the buffer is large enough for
its internal purposes. For example, if the function needs 100 bytes to
set aside to store its state (in addition to user data), it can assume
that `size` will always be larger than that.

```c
void * dyn_alloc(void * context, size_t size);
```
The `dyn_alloc` function takes a `context` pointer to a data area
previously initialised by `dyn_init`. It should return a pointer to
somewhere inside the original buffer, valid for accessing at least
`size` bytes. When called multiple times, the resulting pointers should
not point to memory areas that intersect. The buffer may eventually be
exhausted (previously made allocations taking all available space), in
which case `dyn_alloc` should return `NULL`.

<details><summary>Hint 0: where do we even start?</summary>
One of the simplest ways to implement a dynamic memory allocator is
called sequential allocation: keep a `free` pointer pointing to the
start of the unallocated area in the buffer and an `end` pointer to the
end of the buffer. When a request for more memory arrives, check that
there's enough space in the buffer, then increment the `free` pointer by
the size of the allocation and return its previous value.

How do you free the memory? You can remember the latest pointer you
returned. If you're lucky, you're asked to free the same pointer and you
can just decrement the free pointer back; otherwise there isn't anything
you can do. [In some cases, not performing the deallocations is still
fine, especially if total allocation amount is bounded by a small
constant.](https://devblogs.microsoft.com/oldnewthing/20180228-00/?p=98125)

```
    .-----------------------------------------------------.
    |   .-------------------.                end of buffer|
  .-|---|-------------------v----------------------------.v
  | E | F | allocated area | free area                   |
  '------------------------------------------------------'
                            --------> pointer increment
                         allocation size
```
</details>

Additional task 0
-----------------

On many processors, multi-byte data access should be performed on
addresses that are divisible by a given power of two, typically the size
of the variable. Otherwise the memory access may be slower (on typical
Intel and AMD processors), crash the program (some ARM processors) or
produce other undesirable effects (e.g. ignore the lower bits of the
address, effectively accessing a different variable).

A standard-compliant `malloc()` implementation is guaranteed to return
pointers to memory "suitably aligned for any built-in type", so that the
resulting `void*` pointed can be safely cast to a pointer to a
multi-byte variable (e.g. `double*`) and accessed without causing
undefined behaviour.

Provide a guarantee that the addresses you return to the user are always
appropriately aligned, even for large types. In other words, ensure that
the addresses are divisible by `sizeof(long double)`. Feel free to
assume that `sizeof(long double)` is a power of two and that the buffer
originally supplied to `dyn_init` is already appropriately aligned.

Additional task 1
-----------------

Implement the following function:

```c
void dyn_dealloc(void * context, void ** ptr);
```

The `dyn_dealloc` function takes the following arguments:

 * `context`, the pointer previously initialised by `dyn_init` and
   previously used by `dyn_alloc`.
 * `ptr`, a pointer to a pointer previously returned by `dyn_alloc`.
   (The function is welcome to always assume that `*ptr` came from
   `dyn_alloc(context, size)` with the same `context` argument.)

The `dyn_dealloc` should mark the buffer free for reuse by future calls
to `dyn_alloc` and then set `*ptr` to NULL to prevent the caller from
keeping an invalid pointer.

The intended way to use the function is as follows:

```c
// ...initialise the context...
void * ptr = dyn_alloc(context, size);
// ...use the pointer...
dyn_dealloc(context, &ptr);
// ptr is now NULL
```

<details><summary>Hint 1: two more ways to implement an allocator</summary>
The second simplest way to implement an allocator, the one that can
actually deallocate memory, is called a free-list. Inside the buffer,
there is a number of variously-sized blocks, each containing a pointer
to the next block.  Initially, there's only one block: it spans the
whole buffer (so it doesn't have a next block) and is marked free. When
a memory allocation request arrives, a suitably-sized free block is
found and split into two parts.  The first part satisfies the allocation
size requirements. The second part starts right after the first part and
contains the rest of the block. If the block had a neighbour following
it, the second part should now point to that neighbour. Marking memory
free for future reuse is easy, but at some point adjacent free blocks
may need to be combined by doing a reverse of the splitting operation:
the first block should point to the neighbour of the second block (if
any) and the second block should cease to exist.

```
       next block      next block          next block      end
      .----------.    .---------------.    .---------.    .---> NULL
.-----|---------.v----|--------------.v----|--------.v----|----------.
| A | N | data  | F | N | data       | A | N | data | F | N | data   |
'-^-------------'-^------------------'-^------------'-^--------------'
  '-allocated     '-free               '-allocated    '-free
```

Another approach is to carve out a boolean array (ideally an area where
every bit can be inspected separately) at the beginning of the buffer
and split the rest of the buffer into fixed-size chunks, say, 256 bytes
or 1024 bytes or something. Now every boolean (ideally bit, which is why
this allocation strategy is called bitmap-based) must indicate whether
the corresponding chunk is allocated or freed, and the corresponding
functions must walk the bitmap in order to satisfy or release
allocations. In order to know how many chunks to mark free when a
pointer is deallocated, you will either need to store the sizes
somewhere or use more than one bit per block to mark the allocation
boundaries in a special way, distinct from one-bit "allocated"/"free"
states.

```
 <---------- bitmap -----------> <----------- data blocks ----------->
.-------------------------------.-------------------------------------.
| S A A F F F S A F S S F F F F | | | | | | | | | | | | | | | | | | | |
'-------------------------------'-------------------------------------'
 S: start of allocated area                 (not to scale)
 A: continuing allocated block
 F: free block
```
</details>

Checking your solution
----------------------

Name your solution file `allocator.c`. If you're doing the additional
task 0, put `#define ADDITIONAL_TASK_0` on a separate line somewhere in
your file. For task 1, `#define ADDITIONAL_TASK_1`. Put the
`check_allocator.c` file in the same directory, compile and run it. Your
`allocator.c` will be included automatically. Use AddressSanitizer
and/or Valgrind to ensure that your allocator plays by the rules while
being tested.
