#ifndef PRIV_DA

#	include <stdbool.h>
#	include <stddef.h>
#	include <stdlib.h>
#	include <string.h>

/**
 * \brief da struct
 * \param array pointer holder for the memory
 * \param nb_e number of elements currently present in the array
 * \param cap_e number of elements the memory can hold
 * \param sz_e size of 1 elements (the generic part of the array)
**/
typedef struct s_dynamic_memory_array {
	void  *array;
	size_t nb_e;
	size_t cap_e;
	size_t sz_e;
} t_da;

// init
/* init a given da with the default number of elements(1024) of size sz_elem */
t_da *da_init(t_da *ret, size_t sz_elem);
/* init a given da with the number n of elements of size sz_elem */
t_da *da_ninit(t_da *ret, size_t n, size_t sz_elem);
/* init a given da with the array given and taking:
 * the max nb of e from nb_elem.
 * the size of e from sz_elem.
 * the array from array.
 *
 * WARNING 'from_array' takes ownership so:
 * DO NOT USE NON-ALLOCATED array if there may be a need for resizing.
 * if you want a constant-sized array try rb/round_buffers.
 * if you want to copy values try da_cat_array
*/
t_da *da_from_array(t_da *da, void *array, size_t nb_elem, size_t sz_elem);
/* create a new da struct and allocate a buffer for elem of size sz_elem */
t_da *da_new(size_t sz_elem);
// destroy
/* clear - memory is reusable and only set 0 */
void  da_clr(t_da *ret);
/* destroy the da and the memory in it */
void  da_destroy(t_da *ret);

// add
/* add an element, if needed grow the da */
bool  da_add(t_da *da, void *elem);
/* add nb_e elements of size sz_e from the array arr, if needed grows the da */
bool  da_cat_array(t_da *vec, void *arr, size_t nb_e, size_t sz_e);

// getter
/* get element at offset pos respecting the size of elements */
void *da_at(t_da *da, size_t pos);
/* set element at offset pos respecting the size of elements */
bool  da_set(t_da *da, size_t pos, void *elem);
/* get element at offset pos respecting the size of elements and copies it at elem */
bool  da_get(t_da *da, size_t pos, void *elem);
/* get the raw inner array of the da */
void *da_array(t_da *da);

// manip
/* swap the values in the da at 2 offsets */
bool  da_swap(t_da *da, size_t pos_a, size_t pos_b);
// rm
/* rm a single element while respecting the order of the da */
bool  da_rm_order(t_da *da, size_t pos);
/* rm elements [start, end[ while respecting the order of the da */
bool  da_rms_order(t_da *da, size_t start, size_t end);
/* rm a single element (very fast) no garantee on order */
bool  da_rm(t_da *da, size_t pos);
// BUG: da_rms has a gap problem - check tests
/* rm elements (very fast) no garantee on order */
bool  da_rms(t_da *da, size_t start, size_t end);

// convert
/* convert to array (doesn't free da) */
void *da_to_array(t_da *da);
/* convert to a given array (doesn't allocate array) (doesn't free da) */
void *da_to_loc_array(t_da *da, void *arr);

// apply
/* apply a function to each elements */
bool  da_apply(t_da *da, void (*func)(void *elem));
/* apply a function to each elements and gives position and boolean if last */
bool  da_apply_it(t_da *da, void (*func)(void *elem, size_t pos, bool last));
/* apply a function (func) to a da with a user argument(u_val) */
void *da_apply_with(t_da *da, void *u_val, void (*func)(void *u_val, void *e_val));

// TODO: create
// dup		+n

// TODO: rm
// fold		+n
// filter	+n
// shrink	+n
// advance	+n

// TODO: manip
// reverse	+n
// sort		+n
// nswap

// TODO: apply
// napply_with
// napply_it

// TODO: getter
// find
// nfind_get
// nget

#	define PVEC_SZ_ELEM_DFLT 1024
#	define PVEC_NB_ELEM_DFLT 4

#	define PRIV_DA
#endif // !PRIV_DA
