#ifndef C_MINI__DA
#define C_MINI__DA

#include <stdbool.h>
#include <stddef.h>

#include "common_base/log.h"
#ifdef C_MINI__TESTS

#	define C_MINI__DA_LOG_EARG     C_MINI__LOG_EARG
#	define C_MINI__DA_LOG_EALLOC   C_MINI__LOG_EALLOC
#	define C_MINI__DA_LOG_GETFAIL  C_MINI__LOG_F_PREFIX "get."
#	define C_MINI__DA_LOG_FINDFAIL C_MINI__LOG_F_PREFIX "find."

#	define C_MINI__DA_LOG_SUCCESS C_MINI__LOG_SUCCESS
#	define C_MINI__DA_LOG_SWAP    "Swap (nb:%zu, pos1:%zu, pos2:%zu)"
#	define C_MINI__DA_LOG_GROWTH  "Growth (arr:%p -> %p, cap:%zu -> %zu)"
#	define C_MINI__DA_LOG_RM      "Remove (pos:%zu, nb del:%zu, nb_e:%zu -> %zu)"

#endif // !C_MINI__TESTS

#include "common_base/fptr.h"

/**
 * \defgroup da Dynamic array
**/

/**
 * \typedef t_cm_da
 * \brief da struct represents a dynamic array
 * \param array pointer holder for the memory
 * \param nb_e number of elements currently present in the array
 * \param cap_e number of elements the memory can hold
 * \param sz_e size of one element (the generic part of the array)
 * \ingroup da
**/
typedef struct s_cm_dynamic_memory_array {
	void  *array;
	size_t nb_e;
	size_t cap_e;
	size_t sz_e;
} t_cm_da;

t_cm_da *da_init(t_cm_da *ret, size_t sz_elem);
t_cm_da *da_ninit(t_cm_da *ret, size_t n, size_t sz_elem);
t_cm_da *da_from_array(t_cm_da *da, void *array, size_t nb_elem,
                       size_t sz_elem);
t_cm_da *da_new(size_t sz_elem);

t_cm_da  da_dup(const t_cm_da *src);
t_cm_da  da_ndup(const t_cm_da *src, size_t n);
t_cm_da *da_dup_to(t_cm_da *dst, const t_cm_da *src);
t_cm_da *da_ndup_to(t_cm_da *dst, size_t n, const t_cm_da *src);

void da_clr(t_cm_da *ret);

void da_destroy(t_cm_da *ret);
void da_free(t_cm_da *ret);

bool da_add(t_cm_da *da, const void *elem);
bool da_cat_array(t_cm_da *da, const void *arr, size_t nb_e, size_t sz_e);

bool da_grow(t_cm_da *da);
bool da_grow_for_size(t_cm_da *da, size_t minimal_free_slots);

bool da_shrink(t_cm_da *da);
bool da_shrink_min(t_cm_da *da, size_t keep_n);

void *da_at(const t_cm_da *da, size_t pos);
int   da_get(const t_cm_da *da, size_t pos, void *elem);
int   da_nget(const t_cm_da *da, size_t n, size_t pos, void *out_buffer);
void *da_array(const t_cm_da *da);

bool da_set(const t_cm_da *da, size_t pos, const void *elem);
bool da_nset(const t_cm_da *da, size_t n, size_t pos, const void *elems);

bool da_swap(const t_cm_da *da, size_t pos_a, size_t pos_b);
bool da_nswap(const t_cm_da *da, size_t n, size_t pos_a, size_t pos_b);

bool da_rm_order(t_cm_da *da, size_t pos);
bool da_rms_order(t_cm_da *da, size_t start, size_t end);
bool da_rm(t_cm_da *da, size_t pos);
bool da_rms(t_cm_da *da, size_t start, size_t end);

void *da_to_array(const t_cm_da *da);
void *da_to_loc_array(const t_cm_da *da, void *arr);

void *da_apply(const t_cm_da *da, t_cm_callback cb, void *u_data);
void *da_napply(const t_cm_da *da, size_t n, t_cm_callback c, void *u_data);
void *da_apply_it(const t_cm_da *da, t_cm_callback_it func, void *u_val);
void *da_napply_it(const t_cm_da *da, size_t n, t_cm_callback_it func,
                   void *u_val);

void *da_fold(const t_cm_da *da, t_cm_merger c, void *u_start_val);
void *da_nfold(const t_cm_da *da, size_t n, t_cm_merger c, void *u_start_val);
void *da_reduce(const t_cm_da *da, t_cm_merger m);
void *da_nreduce(const t_cm_da *da, size_t n, t_cm_merger m);

bool da_filter(t_cm_da *da, t_cm_callback c, void *u_data,
               t_cm_callback deletor);
bool da_nfilter(t_cm_da *da, size_t n, t_cm_callback c, void *u_data,
                t_cm_callback deletor);
bool da_filter_out(t_cm_da *da, t_cm_callback c, void *u_data,
                   t_cm_callback deletor);
bool da_nfilter_out(t_cm_da *da, size_t n, t_cm_callback c, void *u_data,
                    t_cm_callback deletor);

bool da_advance(t_cm_da *da, size_t n, t_cm_callback deletor, void *u_data);

void da_reverse(const t_cm_da *da);
void da_nreverse(const t_cm_da *da, size_t n);
void da_b_nreverse(const t_cm_da *da, size_t n);

void *da_find(const t_cm_da *da, t_cm_comparator c, void *u_data);
void *da_nfind(const t_cm_da *da, size_t n, t_cm_comparator c, void *u_data);
bool  da_find_get(const t_cm_da *da, t_cm_comparator c, void *u_data,
                  void *out_buffer);

bool da_sort(t_cm_da *da, t_cm_comparator c);
bool da_nsort(t_cm_da *da, size_t n, t_cm_comparator c);

bool da_is_valid(const t_cm_da *da);
bool da_is_not_null(const t_cm_da *da);
bool da_is_empty(const t_cm_da *da);
bool da_is_full(const t_cm_da *da);

#define C_MINI__DA_SZ_ELEM_DFLT 1024
#define C_MINI__DA_NB_ELEM_DFLT 4
#define C_MINI__DA_GROWTH       1.5

#endif // !C_MINI__DA
