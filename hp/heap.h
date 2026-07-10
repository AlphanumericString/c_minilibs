#ifndef C_MINI__HP
#define C_MINI__HP

#include <stdbool.h>
#include <stddef.h>

#	ifdef C_MINI__TEST
#		include <stdio.h>

#		ifndef LOG
#			define LOG(fmt, ...)                                              \
				do {                                                           \
					printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                  \
					       __func__ __VA_OPT__(, ) __VA_ARGS__);               \
					fflush(stdout);                                            \
				} while (0);
#		endif // !LOG

#		ifndef DBG
#			define DBG(fmt, ...)                                              \
				do {                                                           \
					printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                \
					       __func__ __VA_OPT__(, ) __VA_ARGS__);               \
					fflush(stdout);                                            \
				} while (0);
#		endif // !DBG

#		ifndef DBG_POS
#			define DBG_POS                                                    \
				do {                                                           \
					printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__,        \
					       __LINE__, __FUNCTION__);                            \
					fflush(stdout);                                            \
				} while (0);
#		endif // !DBG_POS

// log messages standardization
#	define C_MINI__HP_LOG_CLR     "Clear Heap"
#	define C_MINI__HP_LOG_SUCCESS "Succes"
#	define C_MINI__HP_LOG_STATUS  "Heap(%p), {nb:%zu, cap:%zu}"
#	define C_MINI__HP_LOG_POP     "Pop (return:%p)\t" C_MINI__HP_LOG_STATUS
#	define C_MINI__HP_LOG_PUSH    "Push (elem:%p)\t" C_MINI__HP_LOG_STATUS
#	define C_MINI__HP_LOG_SWAP    "Swap (%zu, %zu)"

#	define C_MINI__HP_LOG_CB_FAIL "Callback failed. (element %zu)"
#	define C_MINI__HP_LOG_EARG    "Bad arguments"
#	define C_MINI__HP_LOG_EALLOC  "Allocation failed"

#	else // || C_MINI_TEST

#		ifndef LOG
#			define LOG(fmt, ...) (void)0;
#		endif // !LOG

#		ifndef DBG
#			define DBG(fmt, ...) (void)0;
#		endif // !DBG

#		ifndef DBG_POS
#			define DBG_POS (void)0;
#		endif // !DBG_POS

#	endif // !C_MINI__TEST

#ifndef CMINI_T_COMPARATOR_DEFINE
#	define CMINI_T_COMPARATOR_DEFINE

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_comparator)(const void *elem, const void *arg);

#endif

#ifndef CMINI_T_CALLBAKC_DEFINE
#	define CMINI_T_CALLBAKC_DEFINE

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_callback)(void *elem, void *arg);

#endif

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

#endif // C_MINI__HP
