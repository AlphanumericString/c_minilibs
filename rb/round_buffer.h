#ifndef C_MINI__RB
#define C_MINI__RB

/**
 * The magic buffer backend needs POSIX 2008 (shm_open, ftruncate, mmap).
 * Defined before any system header so -std=c11/c17 builds and the amalgamated
 * single header keep the declarations. A user that already pulled a system
 * header before this one has to define a feature macro itself.
**/
#if !defined(_POSIX_C_SOURCE) && !defined(_GNU_SOURCE) &&                      \
    !defined(_DEFAULT_SOURCE) && !defined(_XOPEN_SOURCE)
#	define _POSIX_C_SOURCE 200809L
#endif

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
 * \typedef t_cm_rb
 * \brief t_cm_rb represents a round buffer.
 * \param array the raw buffer pointer
 * \param nb_max number max of elements in the array
 * \param sz_e size of elements in the array
 * \param map_size byte size of one mirrored window (0 when not mirrored)
 * \param mirrored true when array is mapped twice back to back (magic buffer)
 * \param count number of elements currently in the buffer
 * \param i_pos input position (input head)
 * \param o_pos output position (output head)
 * \ingroup rb
**/
typedef struct s_cm_roundbuffer_array {
	void  *array;
	size_t nb_max;
	size_t sz_e;
	size_t map_size;
	bool   mirrored;
#ifdef C_MINI__RB_ATOMIC
	atomic_size_t count;

	atomic_size_t i_pos;
	atomic_size_t o_pos;
#else
	size_t count;

	size_t i_pos;
	size_t o_pos;
#endif
} t_cm_rb;

t_cm_rb *rb_create(t_cm_rb *rb, size_t sz_elem, size_t nb_e);
t_cm_rb *rb_create_magic(t_cm_rb *rb, size_t sz_elem, size_t nb_e);
t_cm_rb *rb_init(t_cm_rb *rb, size_t sz_elem);
t_cm_rb *rb_init_magic(t_cm_rb *rb, size_t sz_elem);
t_cm_rb *rb_from_array(t_cm_rb *rb, void *array, size_t arr_cap,
                       size_t sz_elem);
t_cm_rb *rb_new(size_t sz_elem);
t_cm_rb *rb_new_magic(size_t sz_elem);
void     rb_destroy(t_cm_rb *rb);
void     rb_clr(t_cm_rb *rb);
void     rb_free(t_cm_rb **rb);

bool   rb_is_magic(t_cm_rb rb);
bool   rb_full(t_cm_rb rb);
bool   rb_empty(t_cm_rb rb);
size_t rb_size(t_cm_rb rb);
size_t rb_capacity(t_cm_rb rb);
size_t rb_remaining_space(t_cm_rb rb);
bool   rb_drop(t_cm_rb *rb, size_t n);
void  *rb_pop(t_cm_rb *rb, void *elem_out);
void  *rb_npop(t_cm_rb *rb, size_t n, void *elem_out);
void  *rb_peek(t_cm_rb rb, void *elem_out);
void  *rb_npeek(t_cm_rb rb, size_t n, void *elem_out);
void   rb_push(t_cm_rb *rb, const void *data);
void   rb_npush(t_cm_rb *rb, const void *data, size_t nb_elem);
bool   rb_push_safe(t_cm_rb *rb, const void *data);
bool   rb_npush_safe(t_cm_rb *rb, const void *data, size_t nb_elem);
bool   rb_apply(const t_cm_rb *rb, t_cm_callback cb, const void *arg);
bool  rb_napply(const t_cm_rb *rb, size_t n, t_cm_callback cb, const void *arg);
bool  rb_filter(t_cm_rb *rb, t_cm_callback cb, const void *arg,
                void (*destroyer)(void *elem));
void *rb_at(t_cm_rb rb, size_t idx);
void *rb_nat(t_cm_rb rb, size_t idx, size_t n, void *buff);
void *rb_get(const t_cm_rb *rb, t_cm_comparator comp, const void *arg,
             void *buffer);
void *rb_nget(const t_cm_rb *rb, size_t n, t_cm_comparator comp,
              const void *arg, void *buffer);
void *rb_getn(const t_cm_rb *rb, size_t n, t_cm_comparator comp,
              const void *arg, void *buffer);
void *rb_find(const t_cm_rb *rb, t_cm_comparator c, const void *a);
bool  rb_map(const t_cm_rb *rb, t_cm_duplicator dup, void **out_buff);
bool rb_nmap(const t_cm_rb *rb, size_t n, t_cm_duplicator dup, void **out_buff);
t_cm_rb *rb_grow(const t_cm_rb *rb, size_t nb_elem_added);

#ifdef C_MINI__RB_INTERNAL

void *rbi_get_pos(const t_cm_rb *rb, size_t pos);

/**
 * \brief map nb_bytes (rounded up to a page) twice, back to back, so indices
 * can run past nb_max without wrapping.
 * \param nb_bytes requested byte capacity
 * \param out_array receives the mapping base
 * \param out_size receives the page-rounded byte size of one window
 * \return true on success, false on any failure (nothing is leaked)
 * \ingroup rb_internals
**/
bool rbi_map_ring(size_t nb_bytes, void **out_array, size_t *out_size);
/**
 * \brief release a mapping produced by rbi_map_ring
 * \param array mapping base
 * \param map_size one window byte size
 * \ingroup rb_internals
**/
void rbi_unmap_ring(void *array, size_t map_size);
/**
 * \brief copy n elements starting at pos out of the buffer into dst. Single
 * memcpy on a magic buffer, one or two chunks otherwise.
 * \param rb the round buffer
 * \param pos start position, must be < nb_max when rb is not mirrored
 * \param n number of elements
 * \param dst destination buffer, assumed correctly sized
 * \ingroup rb_internals
**/
void rbi_copy_out(const t_cm_rb *rb, size_t pos, size_t n, void *dst);
/**
 * \brief copy n elements from src into the buffer at pos. Single memcpy on a
 * magic buffer, one or two chunks otherwise.
 * \param rb the round buffer
 * \param pos start position, must be < nb_max when rb is not mirrored
 * \param n number of elements
 * \param src source buffer, assumed to hold n elements
 * \ingroup rb_internals
**/
void rbi_copy_in(t_cm_rb *rb, size_t pos, const void *src, size_t n);

/**
 * The rb positions (i_pos, o_pos) and count are atomic in multi-threaded
 * builds. Every read/write goes through the accessors below so the memory
 * orders are declared once and can be changed here without touching any call
 * site.
 * \ingroup rb_internals
 * \note static inline so function are posted here
**/

#	ifdef C_MINI__RB_ATOMIC

#		define C_MINI__RB_LOAD_ORDER  memory_order_acquire
#		define C_MINI__RB_STORE_ORDER memory_order_release
#		define C_MINI__RB_RMW_ORDER   memory_order_acq_rel

static inline size_t rbi_ipos_get(const t_cm_rb *rb) {
	return atomic_load_explicit(&rb->i_pos, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_ipos_set(t_cm_rb *rb, size_t pos) {
	atomic_store_explicit(&rb->i_pos, pos, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_opos_get(const t_cm_rb *rb) {
	return atomic_load_explicit(&rb->o_pos, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_opos_set(t_cm_rb *rb, size_t pos) {
	atomic_store_explicit(&rb->o_pos, pos, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_count_get(const t_cm_rb *rb) {
	return atomic_load_explicit(&rb->count, C_MINI__RB_LOAD_ORDER);
}
static inline void rbi_count_set(t_cm_rb *rb, size_t n) {
	atomic_store_explicit(&rb->count, n, C_MINI__RB_STORE_ORDER);
}
static inline size_t rbi_count_inc(t_cm_rb *rb) {
	return atomic_fetch_add_explicit(&rb->count, 1, C_MINI__RB_RMW_ORDER);
}
static inline size_t rbi_count_dec(t_cm_rb *rb) {
	return atomic_fetch_sub_explicit(&rb->count, 1, C_MINI__RB_RMW_ORDER);
}

#	else // ||!C_MINI__RB_ATOMIC

static inline size_t rbi_ipos_get(const t_cm_rb *rb) {
	return rb->i_pos;
}
static inline void rbi_ipos_set(t_cm_rb *rb, size_t pos) {
	rb->i_pos = pos;
}
static inline size_t rbi_opos_get(const t_cm_rb *rb) {
	return rb->o_pos;
}
static inline void rbi_opos_set(t_cm_rb *rb, size_t pos) {
	rb->o_pos = pos;
}
static inline size_t rbi_count_get(const t_cm_rb *rb) {
	return rb->count;
}
static inline void rbi_count_set(t_cm_rb *rb, size_t n) {
	rb->count = n;
}
static inline size_t rbi_count_inc(t_cm_rb *rb) {
	return rb->count++;
}
static inline size_t rbi_count_dec(t_cm_rb *rb) {
	return rb->count--;
}

#	endif // !C_MINI__RB_ATOMIC

/**
 * Wrap a bounded position without a division. Precondition: pos < 2 * nb_max.
 * \ingroup rb_internals
**/
static inline size_t rbi_wrap(const t_cm_rb *rb, size_t pos) {
	return (pos >= rb->nb_max) ? pos - rb->nb_max : pos;
}
/**
 * Position of the off-th element counted from base. On a magic buffer the
 * second window makes the address valid without any wrap.
 * \ingroup rb_internals
**/
static inline size_t rbi_off_at(const t_cm_rb *rb, size_t base, size_t off) {
	size_t pos = base + off;

	return rb->mirrored ? pos : rbi_wrap(rb, pos);
}
/**
 * Position of the off-th element counted from the output head.
 * \ingroup rb_internals
**/
static inline size_t rbi_head_off(const t_cm_rb *rb, size_t off) {
	return rbi_off_at(rb, rbi_opos_get(rb), off);
}

#endif // !C_MINI__RB_INTERNAL

#endif // !C_MINI__RB
