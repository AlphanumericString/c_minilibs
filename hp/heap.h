#ifndef C_MINI__HP
#define C_MINI__HP

#include <stdbool.h>
#include <stddef.h>

#ifdef C_MINI__TESTS
#	include "common_base/log.h"

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

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TESTS

#ifndef C_MINI__T_COMPARATOR
#	define C_MINI__T_COMPARATOR

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_comparator)(const void *elem, const void *arg);

#endif

#ifndef C_MINI__T_CALLBACK
#	define C_MINI__T_CALLBACK

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_callback)(void *elem, void *arg);

#endif
/**
 * \defgroup hp Heap
**/

/**
 * \brief heap structure
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
