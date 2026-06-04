#ifndef PRIV_SS
#	include <stdlib.h>
#	include <stddef.h>
#	include <stdbool.h>

/**
 * \brief sparse set structure
 * \param data raw vals storage
 * \param indexes storage for offsets after swaps and pushes
 * \param nb_elem number of elements in the array
 * \param sz_elem size of each elements in the array
 * \param cappacity max number of elements in the sparse set
**/
typedef struct s_sprase_set {
	void  *data;
	uint  *indexes;
	size_t nb_elem;
	size_t sz_elem;
	size_t cappacity;
} t_ss;

// init
t_ss *ss_new(size_t sz_elem);
t_ss *ss_init(t_ss *ret, size_t sz_elem);
t_ss *ss_ninit(t_ss *ret, size_t n, size_t sz_elem);
t_ss *ss_from_array(t_ss *ss, void *array, uint *index_array, size_t nb_elem,
                    size_t sz_elem);
// destroy
void  ss_clr(t_ss *to_clear);
void  ss_destroy(t_ss *to_destroy);
// add
bool  ss_add(t_ss *ss, void *elem);
bool  ss_add_array(t_ss *vec, void *arr, size_t nb_e);
// getter
bool  ss_set(t_ss *ss, size_t pos, void *elem);
void *ss_at(t_ss *ss, size_t pos);
void *ss_get(t_ss *ss, size_t pos, void *out);
void *ss_array(t_ss *ss);
// manip
bool  ss_swap(t_ss *ss, size_t pos_a, size_t pos_b);
// rm
void  ss_remove(t_ss *ss, size_t pos);
void  ss_nremove(t_ss *ss, size_t start, size_t end);
// apply
bool  ss_apply(t_ss *ss, void (*func)(void *elem));
bool  ss_apply_it(t_ss *ss, void (*func)(void *elem, size_t pos, bool last));
void *ss_fold(t_ss *ss, void *u_val, void (*func)(void *u_val, void *e_val));
// convert
void *ss_to_array(t_ss *ss);
void *ss_to_loc_array(t_ss *ss, void *arr);

#	define PSS_SZ_ELEM_DFLT 1024
#	define PSS_NB_ELEM_DFLT 4

#	define PRIV_SS
#endif // !PRIV_SS
