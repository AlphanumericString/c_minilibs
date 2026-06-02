#ifndef PRIV_RB
#	include <stdlib.h>
#	include <stddef.h>
#	include <stdbool.h>

/**
 * Round Buffer Array:
 *
 * A way to handle an array allowing to keep reusing the same space
**/

/// @brief round buffer struct, rb
/// @param array the raw buffer pointer
/// @param nb_max number max of elements in the array
/// @param i_pos input position
/// @param o_pos output position
/// @param sz_e size of elements in the array
typedef struct s_roundbuffer_array {
	void  *array;
	size_t nb_max;

	size_t i_pos;
	size_t o_pos;

	size_t sz_e;
} t_rb;

// init
/* populate rb's field and allocate array from sz_e and nb_e */
t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e);
/* populate rb's field using the default nb_e and given sz_e. allocate array */
t_rb *rb_init(t_rb *rb, size_t sz_elem);
/* populate rb from given values, does not allocate array */
t_rb *rb_from_array(t_rb *rb, void *array, size_t nb_elem, size_t sz_elem);
/* allocate a new rb and populate it with default nb_e and given sz_e */
t_rb *rb_new(size_t sz_elem);

// delete
/* clears inner buffer and reset to 0 all inner fields. does not free rb */
void rb_destroy(t_rb *rb);
/* reset i_pos and o_pos, effectively setting all buffer as ok o push to */
void rb_clr(t_rb *rb);

// getters
// BUG: check math, i_pos+1 is not wrapped in % nb_max so it might lead to BS.
/* true if rb is full, false otherwise */
bool   rb_full(t_rb rb);
/* true if rb is empty, false otherwise */
bool   rb_empty(t_rb rb);
/* return number of entries in rb not yet read/poped */
size_t rb_size(t_rb rb);
/* return max number of elements rb can hold */
size_t rb_cappacity(t_rb rb);
/* return number of elements rb left as disponible to hold */
size_t rb_free(t_rb rb);

// remove
/* true if removes n elements, false otherwise */
bool  rb_drop(t_rb *rb, size_t n);
/* returns removed element position, if elem_out copy removed elem to it */
void *rb_pop(t_rb *rb, void *elem_out);
/* returns first removed element position, if elem_out copy values to it */
void *rb_npop(t_rb *rb, size_t n, void *elem_out);
/* return position in array for next elem, if elem_out copies value to it */
void *rb_peek(t_rb rb, void *elem_out);
/* return position in array for next elems, if elem_out copies values to it */
void *rb_npeek(t_rb rb, size_t n, void *elem_out);

// add
/* add value to rb, if overwrite happens, silently resolve and contiues */
void rb_push(t_rb *rb, const void *data);
/* add values to rb, if overwrite happens, silently resolve and contiues */
void rb_npush(t_rb *rb, const void *data, size_t nb_elem);
/* add value to rb, fails on overwrite if it would happen */
bool rb_push_safe(t_rb *rb, const void *data);
/* add values to rb, fails on overwrite if it would happen */
bool rb_npush_safe(t_rb *rb, const void *data, size_t nb_elem);

#	define PRB_NB_ELEM_DFLT 4
#	define PRB_SZ_ELEM_DFLT 1024

#	define PRIV_RB
#endif // !PRIV_RB
