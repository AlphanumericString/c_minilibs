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
 * \typedef t_cm_hp
 * \brief The heap structure is a representation of a heap, min or max depends
 * on the user's usage of said heap.
 * \param nb the number of elements currently present in the heap
 * \param capacity max number of elements in the heap
 * \param sz_e size of one element (the generic part of the storage)
 * \param elems reference to the elements as a raw byte buffer
 * \param cmp comparator function
 * \ingroup hp
**/
typedef struct s_cm_heap {
	size_t          nb;
	size_t          capacity;
	size_t          sz_e;
	void           *elems;
	t_cm_comparator cmp;
} t_cm_hp;

t_cm_hp *hp_create(size_t nb_elems, size_t sz_elem, t_cm_comparator cmp);
t_cm_hp *hp_init(t_cm_hp *hp, size_t nb_elems, size_t sz_elem,
                 t_cm_comparator cmp);

t_cm_hp *hp_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                           t_cm_comparator cmp);
t_cm_hp *hp_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                            size_t sz_elem, t_cm_comparator cmp);

t_cm_hp *hp_min_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                               t_cm_comparator cmp);
t_cm_hp *hp_max_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                               t_cm_comparator cmp);
t_cm_hp *hp_min_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                                size_t sz_elem, t_cm_comparator cmp);
t_cm_hp *hp_max_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                                size_t sz_elem, t_cm_comparator cmp);

t_cm_hp *hp_dup(const t_cm_hp *src);
t_cm_hp *hp_ndup(const t_cm_hp *src, size_t n);

bool hp_delete(t_cm_hp *hp);
bool hp_free(t_cm_hp **hp);
bool hp_clear(t_cm_hp *hp);

bool hp_is_empty(const t_cm_hp *hp);
bool hp_is_full(const t_cm_hp *hp);
bool hp_is_valid(const t_cm_hp *hp);
bool hp_min_is_valid(const t_cm_hp *hp);
bool hp_max_is_valid(const t_cm_hp *hp);

t_cm_comparator hp_get_cmp(const t_cm_hp *hp);
size_t          hp_get_cap(const t_cm_hp *hp);
size_t          hp_get_current_size(const t_cm_hp *hp);
size_t          hp_get_elem_size(const t_cm_hp *hp);
void           *hp_get_elems_array(const t_cm_hp *hp);
void           *hp_at(const t_cm_hp *hp, size_t pos);

bool hp_min_push(t_cm_hp *h, const void *e);
bool hp_max_push(t_cm_hp *h, const void *e);

bool hp_min_pop(t_cm_hp *h, void *out);
bool hp_max_pop(t_cm_hp *h, void *out);

bool hp_min_rm(t_cm_hp *h, size_t n, void *out);
bool hp_max_rm(t_cm_hp *h, size_t n, void *out);

bool hp_min_pushpop(t_cm_hp *h, const void *e, void *out);
bool hp_max_pushpop(t_cm_hp *h, const void *e, void *out);
bool hp_min_replace(t_cm_hp *h, const void *e, void *out);
bool hp_max_replace(t_cm_hp *h, const void *e, void *out);

bool hp_foreach(const t_cm_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_it(const t_cm_hp *h, t_cm_callback_it cb, void *u_arg);
bool hp_foreach_in(const t_cm_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_pre(const t_cm_hp *h, t_cm_callback cb, void *u_arg);
bool hp_foreach_post(const t_cm_hp *h, t_cm_callback cb, void *u_arg);

void *hp_peek(const t_cm_hp *h);
void *hp_find(const t_cm_hp *h, t_cm_comparator c, void *u_data);
bool  hp_find_get(const t_cm_hp *h, t_cm_comparator c, void *u_data,
                  void *out_buffer);
void *hp_fold(const t_cm_hp *h, t_cm_merger m, void *start_val);
void *hp_reduce(const t_cm_hp *h, t_cm_merger m);

void **hp_map(const t_cm_hp *h, t_cm_mapper fn);

bool hp_min_filter(t_cm_hp *h, t_cm_callback keep, void *u_data,
                   t_cm_callback deletor);
bool hp_max_filter(t_cm_hp *h, t_cm_callback keep, void *u_data,
                   t_cm_callback deletor);

bool hp_min_heapify(t_cm_hp *h);
bool hp_max_heapify(t_cm_hp *h);

bool  hp_iter(const t_cm_hp *h, size_t start, size_t last, void **it_val);
bool  hp_iter_rev(const t_cm_hp *h, size_t start, size_t last, void **it_val);
void *hp_it_advance(const t_cm_hp *h, void **it);
void *hp_it_back(const t_cm_hp *h, void **it);

#ifdef C_MINI__HP_INTERNAL

void *hpi_elem_ptr(const t_cm_hp *h, size_t idx);

void hpi_swap(const t_cm_hp *h, size_t pos1, size_t pos2);

void hpi_sift_up(t_cm_hp *h, size_t idx, bool max);
void hpi_sift_down(t_cm_hp *h, size_t idx, bool max);
void hpi_heapify(t_cm_hp *h, bool max);

void hpi_replace_root(t_cm_hp *h, const void *e, void *out, bool max);

#endif // !C_MINI__HP_INTERNAL

#endif // !C_MINI__HP
