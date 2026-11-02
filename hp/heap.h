#ifndef C_MINI__HP
#define C_MINI__HP

#include <stdbool.h>
#include <stddef.h>

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__HP_LOG_SUCCESS C_MINI__LOG_SUCCESS
#	define C_MINI__HP_LOG_CLR     "Clear Heap"
#	define C_MINI__HP_LOG_STATUS  "Heap(%p), {nb:%zu, cap:%zu, sz:%zu}"
#	define C_MINI__HP_LOG_POP     "Pop (out:%p)\t" C_MINI__HP_LOG_STATUS
#	define C_MINI__HP_LOG_PUSH    "Push (elem:%p)\t" C_MINI__HP_LOG_STATUS
#	define C_MINI__HP_LOG_SWAP    "Swap (%zu, %zu)"

#	define C_MINI__HP_LOG_CB_FAIL                                             \
		C_MINI__LOG_F_PREFIX "Callback failed. (element %zu)"
#	define C_MINI__HP_LOG_EARG   C_MINI__LOG_EARG
#	define C_MINI__HP_LOG_EALLOC C_MINI__LOG_EALLOC

#endif // !C_MINI__TESTS

/**
 * \defgroup hp Heap
**/

/**
 * \typedef t_hp
 * \brief The heap structure is a representation of a heap, min or max depends
 * on the user's usage of said heap.
 * \param nb the number of elements currently present in the heap
 * \param capacity max number of elements in the heap
 * \param sz_e size of one element (the generic part of the storage)
 * \param elems reference to the elements as a raw byte buffer
 * \param cmp comparator function
 * \ingroup hp
**/
typedef struct s_heap {
	size_t          nb;
	size_t          capacity;
	size_t          sz_e;
	void           *elems;
	t_cm_comparator cmp;
} t_hp;

t_hp *hp_create(size_t nb_elems, size_t sz_elem, t_cm_comparator cmp);
t_hp *hp_init(t_hp *hp, size_t nb_elems, size_t sz_elem, t_cm_comparator cmp);

t_hp *hp_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                        t_cm_comparator cmp);
t_hp *hp_init_from_array(t_hp *hp, void *arr, size_t nb_elems, size_t sz_elem,
                         t_cm_comparator cmp);

bool hp_delete(t_hp *hp);
bool hp_free(t_hp **hp);
bool hp_clear(t_hp *hp);

bool hp_is_empty(const t_hp *hp);
bool hp_is_full(const t_hp *hp);
bool hp_is_valid(const t_hp *hp);

t_cm_comparator hp_get_cmp(const t_hp *hp);
size_t          hp_get_cap(const t_hp *hp);
size_t          hp_get_current_size(const t_hp *hp);
size_t          hp_get_elem_size(const t_hp *hp);
void           *hp_get_elems_array(const t_hp *hp);
void           *hp_at(const t_hp *hp, size_t pos);

bool hp_min_push(t_hp *h, const void *e);
bool hp_max_push(t_hp *h, const void *e);

bool hp_min_pop(t_hp *h, void *out);
bool hp_max_pop(t_hp *h, void *out);

bool hp_min_rm(t_hp *h, size_t n, void *out);
bool hp_max_rm(t_hp *h, size_t n, void *out);

bool hp_foreach(const t_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_in(t_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_pre(t_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_post(t_hp *h, t_cm_callback cb, void *u_arg);

#endif // !C_MINI__HP
