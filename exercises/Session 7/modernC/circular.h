#ifndef CIRCULAR_H
# define CIRCULAR_H 1

# include <stddef.h>
# include <stdio.h>

/** @brief an opaque type for a circular buffer for @c double values
 **
 ** This data structure allows to add @c double values in rear and to
 ** take them out in front. Each such structure has a maximal amount
 ** of elements that can be stored in it.
 **
 **/
typedef struct circular circular;

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize a circular buffer @a c with maximally @a
 ** max_len elements.
 **
 ** Only use this function on an uninitialized buffer.
 **
 ** Each buffer that is initialized with this function must be
 ** destroyed with a call to ::circular_destroy.
 **/
circular* circular_init(circular* c, size_t max_len);

/** @brief Destroy circular buffer @a c
 **
 ** @a c must have been initialized with a call to ::circular_init
 **/
void circular_destroy(circular* c);

/** @brief Allocate and initialize a circular buffer with maximally
 ** @a len elements.
 **
 ** Each buffer that is allocated with this function must be deleted
 ** with a call to ::circular_delete.
 **/
circular* circular_new(size_t len);

/** @brief Delete circular buffer @a c
 **
 ** @a c must have been allocated with a call to ::circular_new
 **/
void circular_delete(circular* c);
/**
 ** @}
 **/

/**
 ** @name Using elements of a circular
 ** @{
 **/

/** @brief Append a new element with value @a value to the buffer @a c
 **
 ** @return c if the new element could be appended, @c 0 otherwise.
 **/
circular* circular_append(circular* c, double value);

/** @brief Remove the oldest element from @a c and return its value
 **
 ** @return the removed element if it exists, @c 0.0 otherwise.
 **/

double circular_pop(circular* c);

/** @brief Return a pointer to position @a pos in buffer @a c
 **
 ** @return a pointer to element @a pos of the buffer, @c 0 otherwise.
 **/
double* circular_element(circular* c, size_t pos);


/**
 ** @}
 **/

/**
 ** @name Maintain a circular
 ** @{
 **/

/** @brief Return the number of elements stored. */
size_t circular_getlength(circular* c);

/** @brief Resize to capacity @a max_len. */
circular* circular_resize(circular* c, size_t max_len);

/** @brief Print the buffer values to stream @a s. */
void circular_fput(circular* c, FILE* s);

/**
 ** @}
 **/



#endif
