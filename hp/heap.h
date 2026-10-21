#ifndef C_MINI__HP
#define C_MINI__HP

#include <stdbool.h>
#include <stddef.h>

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__HP_LOG_SUCCESS C_MINI__LOG_SUCCESS
#	define C_MINI__HP_LOG_CLR     "Clear Heap"
#	define C_MINI__HP_LOG_STATUS  "Heap(%p), {nb:%zu, cap:%zu}"
#	define C_MINI__HP_LOG_POP     "Pop (return:%p)\t" C_MINI__HP_LOG_STATUS
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

// TODO: switch elems to a voided ptr with size_elements as a struct value
// TODO: shorten t_heap -> t_hp
/**
 * \typedef t_heap
 * \brief The heap structure is a representation of a heap, min or max depends
 * on the user's usage of said heap.
 * \param nb the number of elements currently present in the heap
 * \param capacity max number of elements in the heap
 * \param elems reference to the elements as void pointers
 * \param cmp comparator fuction
 * \ingroup hp
**/
typedef struct s_heap {
	size_t       nb;
	size_t       capacity;
	void       **elems;
	t_comparator cmp;
} t_heap;

t_heap *hp_create(size_t nb_elems, t_comparator cmp);
t_heap *hp_init(t_heap *hp, size_t nb_elems, t_comparator cmp);

void hp_delete(t_heap *hp);
void hp_free(t_heap *hp);
void hp_clear(t_heap *hp);

void hp_min_push(t_heap *h, void *e);
void hp_max_push(t_heap *h, void *e);

void *hp_min_pop(t_heap *h);
void *hp_max_pop(t_heap *h);

void *hp_min_rm(t_heap *h, size_t n);
void *hp_max_rm(t_heap *h, size_t n);

bool hp_foreach(t_heap *h, t_callback cb, void *u_arg);
bool hp_foreach_in(t_heap *h, t_callback cb, void *u_arg);
bool hp_foreach_pre(t_heap *h, t_callback cb, void *u_arg);
bool hp_foreach_post(t_heap *h, t_callback cb, void *u_arg);

// TODO: hp_from_array(arr, nb_elems, cmp) -> t_hp
// TODO: hp_init_from_array(hp, arr, nb_elems, cmp) -> t_hp*
// TODO: switch return to bool everywhere possible + add test checking return
//		values
// TODO: add hp_is_[empty|full|valid](const ..); -> bool
// TODO: add hp_get_[cmp|cap|curent_size|elems_array](...); -> <val type>
// TODO: void* backing with typeagnostic heuristics (retro actively add
//		get_elem_size)

#endif // !C_MINI__HP
