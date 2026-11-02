#ifndef C_MINI__RB
#define C_MINI__RB

#include <stdbool.h>
#include <stddef.h>

#ifndef C_MINI__RB_ATOMIC
#	define C_MINI__RB_ATOMIC
#endif

#ifdef C_MINI__RB_ATOMIC
#	include <stdatomic.h>
#endif

#include "common_base/log.h"

#ifdef C_MINI__TESTS
#	define C_MINI__RB_LOG_EARG   C_MINI__LOG_F_PREFIX "Bad arguments."
#	define C_MINI__RB_LOG_ESPACE C_MINI__LOG_F_PREFIX "Not enough space."
#	define C_MINI__RB_LOG_EALLOC C_MINI__LOG_F_PREFIX "Allocation Fail."
#	define C_MINI__RB_LOG_ECB    C_MINI__LOG_F_PREFIX "Callback failed."

#	define C_MINI__RB_LOG_FAIL    C_MINI__LOG_FAIL
#	define C_MINI__RB_LOG_SUCCESS C_MINI__LOG_SUCCESS
#endif

#include "common_base/fptr.h"

#ifndef C_MINI__RB_NBE_DFLT
#	ifdef C_MINI__TESTS
#		define C_MINI__RB_NBE_DFLT 4
#	else // ||!C_MINI__TESTS
#		define C_MINI__RB_NBE_DFLT 64
#	endif
#endif // !C_MINI__RB_NBE_DFLT

/**
 * \defgroup rb Round Buffer
**/

/**
 * \typedef t_rb
 * \brief t_rb represents a round buffer.
 * \param array the raw buffer pointer
 * \param nb_max number max of elements in the array
 * \param sz_e size of elements in the array
 * \param count number of elements currently in the buffer
 * \param i_pos input position (input head)
 * \param o_pos output position (output head)
 * \ingroup rb
**/
typedef struct s_roundbuffer_array {
	void  *array;
	size_t nb_max;
	size_t sz_e;
#ifdef C_MINI__RB_ATOMIC
	atomic_size_t count;

	atomic_size_t i_pos;
	atomic_size_t o_pos;
#else
	size_t count;

	size_t i_pos;
	size_t o_pos;
#endif
} t_rb;

t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e);
t_rb *rb_init(t_rb *rb, size_t sz_elem);
t_rb *rb_from_array(t_rb *rb, void *array, size_t arr_cap, size_t sz_elem);
t_rb *rb_new(size_t sz_elem);
void  rb_destroy(t_rb *rb);
void  rb_clr(t_rb *rb);
void  rb_free(t_rb **rb);

bool   rb_full(t_rb rb);
bool   rb_empty(t_rb rb);
size_t rb_size(t_rb rb);
size_t rb_capacity(t_rb rb);
size_t rb_remaining_space(t_rb rb);
bool   rb_drop(t_rb *rb, size_t n);
void  *rb_pop(t_rb *rb, void *elem_out);
void  *rb_npop(t_rb *rb, size_t n, void *elem_out);
void  *rb_peek(t_rb rb, void *elem_out);
void  *rb_npeek(t_rb rb, size_t n, void *elem_out);
void   rb_push(t_rb *rb, const void *data);
void   rb_npush(t_rb *rb, const void *data, size_t nb_elem);
bool   rb_push_safe(t_rb *rb, const void *data);
bool   rb_npush_safe(t_rb *rb, const void *data, size_t nb_elem);
bool   rb_apply(t_rb *rb, t_cm_callback cb, const void *arg);
bool   rb_napply(t_rb *rb, size_t n, t_cm_callback cb, const void *arg);
bool   rb_filter(t_rb *rb, t_cm_callback cb, const void *arg,
                 void (*destroyer)(void *elem));
void  *rb_at(t_rb rb, size_t idx);
void  *rb_nat(t_rb rb, size_t idx, size_t n, void *buff);
void  *rb_get(const t_rb *rb, t_cm_comparator comp, const void *arg,
              void *buffer);
void  *rb_nget(const t_rb *rb, size_t n, t_cm_comparator comp, const void *arg,
               void *buffer);
void  *rb_getn(const t_rb *rb, size_t n, t_cm_comparator comp, const void *arg,
               void *buffer);
void  *rb_find(const t_rb *rb, t_cm_comparator c, const void *a);
bool   rb_map(t_rb *rb, t_cm_duplicator dup, void **out_buff);
bool   rb_nmap(t_rb *rb, size_t n, t_cm_duplicator dup, void **out_buff);
t_rb  *rb_grow(t_rb *rb, size_t nb_elem_added);

#ifdef C_MINI__RB_INTERNAL

/**
 * The rb positions (i_pos, o_pos) and count are atomic in multi-threaded
 * builds. Every read/write goes through the accessors below so the memory
 * orders are declared once and can be changed here without touching any call
 * site.
 * \ingroup rb_internals
**/

#	ifdef C_MINI__RB_ATOMIC

#		define C_MINI__RB_LOAD_ORDER  memory_order_acquire
#		define C_MINI__RB_STORE_ORDER memory_order_release
#		define C_MINI__RB_RMW_ORDER   memory_order_acq_rel

static inline size_t rbi_ipos_get(const t_rb *rb) {
	return atomic_load_explicit(&rb->i_pos, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_ipos_set(t_rb *rb, size_t pos) {
	atomic_store_explicit(&rb->i_pos, pos, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_opos_get(const t_rb *rb) {
	return atomic_load_explicit(&rb->o_pos, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_opos_set(t_rb *rb, size_t pos) {
	atomic_store_explicit(&rb->o_pos, pos, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_count_get(const t_rb *rb) {
	return atomic_load_explicit(&rb->count, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_count_set(t_rb *rb, size_t n) {
	atomic_store_explicit(&rb->count, n, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_count_inc(t_rb *rb) {
	return atomic_fetch_add_explicit(&rb->count, 1, C_MINI__RB_RMW_ORDER);
}
static inline size_t rbi_count_dec(t_rb *rb) {
	return atomic_fetch_sub_explicit(&rb->count, 1, C_MINI__RB_RMW_ORDER);
}

#	else // ||!C_MINI__RB_ATOMIC

static inline size_t rbi_ipos_get(const t_rb *rb) {
	return rb->i_pos;
}
static inline void rbi_ipos_set(t_rb *rb, size_t pos) {
	rb->i_pos = pos;
}
static inline size_t rbi_opos_get(const t_rb *rb) {
	return rb->o_pos;
}
static inline void rbi_opos_set(t_rb *rb, size_t pos) {
	rb->o_pos = pos;
}
static inline size_t rbi_count_get(const t_rb *rb) {
	return rb->count;
}
static inline void rbi_count_set(t_rb *rb, size_t n) {
	rb->count = n;
}
static inline size_t rbi_count_inc(t_rb *rb) {
	return rb->count++;
}
static inline size_t rbi_count_dec(t_rb *rb) {
	return rb->count--;
}

#	endif // !C_MINI__RB_ATOMIC

#endif // !C_MINI__RB_INTERNAL

#endif // !C_MINI__RB
