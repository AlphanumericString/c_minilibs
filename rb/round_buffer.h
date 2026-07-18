#ifndef C_MINI__RB
#define C_MINI__RB

#include <stdbool.h>
#include <stddef.h>

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	ifndef LOG
#		define LOG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                      \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                    \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS                                                        \
			do {                                                               \
				printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__, __LINE__,  \
				       __FUNCTION__);                                          \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG_POS

#define C_MINI__RB_LOG_EARG		"Bad arguments."
#define C_MINI__RB_LOG_ESPACE	"Not enough space."
#define C_MINI__RB_LOG_EALLOC	"Allocation Fail."
#define C_MINI__RB_LOG_ECB		"Callback failed."
#define C_MINI__RB_LOG_FAIL		"Faillure"
#define C_MINI__RB_LOG_SUCCESS	"Success"

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

/**
 * \defgroup rb Round Buffer
**/

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

#ifndef CMINI_T_COMPARATOR_DEFINE
#	define CMINI_T_COMPARATOR_DEFINE

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_comparator)(void *elem, void *arg);

#endif

#ifndef CMINI_T_DUOLICATOR_DEFINE
#	define CMINI_T_DUOLICATOR_DEFINE

/**
 * \brief duplication function pointer for datas
 * \param elem pointer to the duplication target
 * \return pointer to a valid copy of the data
**/
typedef void *(*t_duplicator)(const void *elem);

#endif

/**
 * \brief round buffer struct, rb
 * \param array the raw buffer pointer
 * \param nb_max number max of elements in the array
 * \param i_pos input position
 * \param o_pos output position
 * \param sz_e size of elements in the array
 * \ingroup rb
**/
typedef struct s_roundbuffer_array {
	void  *array;
	size_t nb_max;
	size_t sz_e;
	size_t count;

	size_t i_pos;
	size_t o_pos;

} t_rb;

// init
t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e);
t_rb *rb_init(t_rb *rb, size_t sz_elem);
t_rb *rb_from_array(t_rb *rb, void *array, size_t nb_elem, size_t sz_elem);
t_rb *rb_new(size_t sz_elem);

// delete
void rb_destroy(t_rb *rb);
void rb_clr(t_rb *rb);

// getters
bool   rb_full(t_rb rb);
bool   rb_empty(t_rb rb);
size_t rb_size(t_rb rb);
size_t rb_cappacity(t_rb rb);
size_t rb_remaining_space(t_rb rb);

// remove
bool  rb_drop(t_rb *rb, size_t n);
void *rb_pop(t_rb *rb, void *elem_out);
void *rb_npop(t_rb *rb, size_t n, void *elem_out);
void *rb_peek(t_rb rb, void *elem_out);
void *rb_npeek(t_rb rb, size_t n, void *elem_out);

// add
void rb_push(t_rb *rb, const void *data);
void rb_npush(t_rb *rb, const void *data, size_t nb_elem);
bool rb_push_safe(t_rb *rb, const void *data);
bool rb_npush_safe(t_rb *rb, const void *data, size_t nb_elem);

// apply
bool rb_apply(t_rb *rb, t_callback cb, const void *arg);
bool rb_naplly(t_rb *rb, size_t n, t_callback cb, const void *arg);

// filter
bool rb_filter(t_rb *rb, t_callback cb, const void *arg,
               void (*destroyer)(void *elem));

// access
void *rb_at(t_rb rb, size_t idx);
void *rb_nat(t_rb rb, size_t idx, size_t n, void *buff);

// get
void *rb_get(const t_rb *rb, t_comparator comp, const void *arg, void *buffer);
void *rb_nget(const t_rb *rb, size_t n, t_comparator comp, const void *arg,
              void *buffer);
// void *rb_getn(const t_rb *rb, size_t n, t_comparator comp, const void *arg,
// void *buffer);

// find
void *rb_find(const t_rb *rb, t_comparator c, const void *a);

// Map
bool rb_map(t_rb *rb, t_duplicator dup, void **out_buff);
bool rb_nmap(t_rb *rb, size_t n, t_duplicator dup, void **out_buff);

// grow
t_rb *rb_grow(
    t_rb  *rb,
    size_t nb_elem_added); // return a new rb. ALWAYS. or null if alloc fails

#define C_MINI_RB_NB_ELEM_DFLT 4
#define C_MINI_RB_SZ_ELEM_DFLT 1024

// TODO:
// rb_nget -> get's the n elements after first match
// rb_getn -> get up to n elements that matched

#endif // !C_MINI__RB
