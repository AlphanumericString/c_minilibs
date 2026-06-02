#ifndef PRIV_SS
#	include <strings.h>
#	include <stdlib.h>
#	include <stddef.h>
#	include <stdbool.h>

/**
 * Sparse set structure:
 *
 * array of elements using triks such as swap on destroy but keeping
 *  elements position constant using a middle layer of indexes. This allow us
 *  to do stuff such as swap-destroy to keep delete operation at constant time.
 *  This method also allows us to see the next free index in destroy-add cases
 *  as swap-destroy will also swap values in indexes.
**/

/// @brief sparse set structure
/// @param data raw vals storage
/// @param indexes storage for offsets after swaps and pushes
/// @param nb_elem number of elements in the array
/// @param sz_elem size of each elements in the array
/// @param cappacity max number of elements in the sparse set
typedef struct s_sprase_set {
	void  *data;
	uint  *indexes;
	size_t nb_elem;
	size_t sz_elem;
	size_t cappacity;
} t_ss;

// init
/* Allocates and init a new ss for elements of size sz_elem */
t_ss *ss_new(size_t sz_elem);
/* Init a given ss for elements of size sz_elem. Allocates sub arrays */
t_ss *ss_init(t_ss *ret, size_t sz_elem);
/* Init a given ss for n elements of size sz_elem. Allocates sub arrays */
t_ss *ss_ninit(t_ss *ret, size_t n, size_t sz_elem);
/** Init a given ss with sub array given and nb_elem + sz_elem
 * WARNING from array takes ownership of said array.
 * futhermore the array is free'd if theres any need for resizing.
 * if you want a constant-sized array try rb/round_buffers.
 * if you want to copy values try ss_cat_array
**/
t_ss *ss_from_array(t_ss *ss, void *array, uint *index_array, size_t nb_elem,
                    size_t sz_elem);
// destroy
/* Clear ss. Does not free sub arrays. */
void  ss_clr(t_ss *to_clear);
/* Free's sub arrays and set all fields to NULL values. DOES NOT FREE SS. */
void  ss_destroy(t_ss *to_destroy);
// add
/* Append an element elem to the ss. */
bool  ss_add(t_ss *ss, void *elem);
/* Append nb elements from arr to the ss. */
bool  ss_add_array(t_ss *vec, void *arr, size_t nb_e);
// getter
/* Try to set value elem into the array at pos. True if opperation succeed */
bool  ss_set(t_ss *ss, size_t pos, void *elem);
/* Return offset into the array for index pos. */
void *ss_at(t_ss *ss, size_t pos);
/* Return offset into the array for index pos, and copy elem to out pointer */
void *ss_get(t_ss *ss, size_t pos, void *out);
/* Return the underlying array of values. possibly un-ordered. */
void *ss_array(t_ss *ss);
// manip
/* swap values at index pos_a and pos_b together. */
bool  ss_swap(t_ss *ss, size_t pos_a, size_t pos_b);
// rm
/* Remove value at pos. */
void  ss_remove(t_ss *ss, size_t pos);
/* Remove value from start to end (excluding end [start, end[ ). */
void  ss_nremove(t_ss *ss, size_t start, size_t end);
// apply
/* Applies func to all elements (un-ordered). True if opperation succeed. */
bool  ss_apply(t_ss *ss, void (*func)(void *elem));
/* Applies func to all elements (un-ordered). True if opperation succeed. */
bool  ss_apply_it(t_ss *ss, void (*func)(void *elem, size_t pos, bool last));
/* Applies func to all elements with u_val in order. Returns u_val. */
void *ss_fold(t_ss *ss, void *u_val, void (*func)(void *u_val, void *e_val));
// convert
/* copies values to a new array of elem. Array is allocated. */
void *ss_to_array(t_ss *ss);
/* copies values to a given array. Array is expected of holding nb_elem. */
void *ss_to_loc_array(t_ss *ss, void *arr);

#	define PSS_SZ_ELEM_DFLT 1024
#	define PSS_NB_ELEM_DFLT 4

#	define PRIV_SS
#endif // !PRIV_SS
