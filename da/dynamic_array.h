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

// create
t_da *da_init(t_da *ret, size_t sz_elem);
t_da *da_ninit(t_da *ret, size_t n, size_t sz_elem);
t_da *da_from_array(t_da *da, void *array, size_t nb_elem, size_t sz_elem);
t_da *da_new(size_t sz_elem);

// clear
void  da_clr(t_da *ret);

// destroy
void  da_destroy(t_da *ret);

// add
bool  da_add(t_da *da, void *elem);
bool  da_cat_array(t_da *vec, void *arr, size_t nb_e, size_t sz_e);

// getter
void *da_at(t_da *da, size_t pos);
bool  da_set(t_da *da, size_t pos, void *elem);
bool  da_get(t_da *da, size_t pos, void *elem);
void *da_array(t_da *da);

// swap
bool  da_swap(t_da *da, size_t pos_a, size_t pos_b);

// rm
bool  da_rm_order(t_da *da, size_t pos);
bool  da_rms_order(t_da *da, size_t start, size_t end);
bool  da_rm(t_da *da, size_t pos);
bool  da_rms(t_da *da, size_t start, size_t end);

// convert
void *da_to_array(t_da *da);
void *da_to_loc_array(t_da *da, void *arr);

// apply
bool  da_apply(t_da *da, void (*func)(void *elem));
bool  da_apply_it(t_da *da, void (*func)(void *elem, size_t pos, bool last));
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

// TODO: BUGS:
//	fix bug on rms (gap problem)

#	define PVEC_SZ_ELEM_DFLT 1024
#	define PVEC_NB_ELEM_DFLT 4

#	define PRIV_DA
#endif // !PRIV_DA