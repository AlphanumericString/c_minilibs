#ifndef C_MINI__DA

#	include <stdbool.h>
#	include <stddef.h>

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

// comon funct ptr definitions

#	ifndef CMINI_T_CALLBAKC_DEFINE
#		define CMINI_T_CALLBAKC_DEFINE

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_callback)(void *elem, void *arg);

#	endif

#	ifndef CMINI_T_CALLBAKC_IT_DEFINE
#		define CMINI_T_CALLBAKC_IT_DEFINE

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \param  pos  position of the elem in the sub array backing
 * \param  last boolean on wheter or not this is the last element of the call
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_callback_it)(void *elem, void *arg, size_t pos, bool last);

#	endif // CMINI_T_CALLBAKC_IT

#	ifndef CMINI_T_COMPARATOR_DEFINE
#		define CMINI_T_COMPARATOR_DEFINE

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_comparator)(const void *elem, const void *arg);

#	endif

#	ifndef CMINI_T_DUOLICATOR_DEFINE
#		define CMINI_T_DUOLICATOR_DEFINE

/**
 * \brief duplication function pointer for datas
 * \param elem pointer to the duplication target
 * \return pointer to a valid copy of the data
**/
typedef void *(*t_duplicator)(const void *elem);

#	endif

#	ifndef CMINI_T_MERGER_DEFINE
#		define CMINI_T_MERGER_DEFINE

typedef void *(*t_merger)(void *elem, void *arg);

#	endif // CMINI_T_MERGER_DEFINE

/**
 * \brief da struct
 * \param array pointer holder for the memory
 * \param nb_e number of elements currently present in the array
 * \param cap_e number of elements the memory can hold
 * \param sz_e size of 1 elements (the generic part of the array)
**/
typedef struct s_dynamic_memory_array {
	void  *array;
	size_t nb_e;
	size_t cap_e;
	size_t sz_e;
} t_da;

// create
t_da *da_init(t_da *ret, size_t sz_elem);
t_da *da_ninit(t_da *ret, size_t n, size_t sz_elem);
t_da *da_from_array(t_da *da, void *array, size_t nb_elem, size_t sz_elem);
t_da *da_new(size_t sz_elem);

// dup
t_da  da_dup(const t_da *src);
t_da  da_ndup(const t_da *src, size_t n);
t_da *da_dup_to(t_da *dst, const t_da *src);
t_da *da_ndup_to(t_da *dst, size_t n, const t_da *src);

// clear
void da_clr(t_da *ret);

// destroy
void da_destroy(t_da *ret);

// add
bool da_add(t_da *da, void *elem);
bool da_cat_array(t_da *da, void *arr, size_t nb_e, size_t sz_e);

// grow
bool da_grow(t_da *da);
bool da_grow_for_size(t_da *da, size_t minimal_free_slots);

// shrink
bool da_shrink(t_da *da);
bool da_shrink_min(t_da *da, size_t keep_n);

// get
void *da_at(t_da *da, size_t pos);
bool  da_get(t_da *da, size_t pos, void *elem);
bool  da_nget(t_da *da, size_t n, size_t pos, void *out_buffer);
void *da_array(t_da *da);

// set
bool da_set(t_da *da, size_t pos, void *elem);
bool da_nset(t_da *da, size_t n, size_t pos, void *elems);

// swap
bool da_swap(t_da *da, size_t pos_a, size_t pos_b);
bool da_nswap(t_da *da, size_t n, size_t pos_a, size_t pos_b);

// rm
bool da_rm_order(t_da *da, size_t pos);
bool da_rms_order(t_da *da, size_t start, size_t end);
bool da_rm(t_da *da, size_t pos);
bool da_rms(t_da *da, size_t start, size_t end);

// convert
void *da_to_array(t_da *da);
void *da_to_loc_array(t_da *da, void *arr);

// apply
void *da_apply(t_da *da, t_callback cb, void *u_data);
void *da_napply(t_da *da, size_t n, t_callback c, void *u_data);
void *da_apply_it(t_da *da, t_callback_it func, void *u_val);
void *da_napply_it(t_da *da, size_t n, t_callback_it func, void *u_val);

// fold
void *da_fold(t_da *da, t_merger c, void *u_start_val);
void *da_nfold(t_da *da, size_t n, t_merger c, void *u_start_val);
void *da_reduce(t_da *da, t_merger m);
void *da_nreduce(t_da *da, size_t n, t_merger m);

// filter
bool da_filter(t_da *da, t_callback c, void *u_data, t_callback deletor);
bool da_nfilter(t_da *da, size_t n, t_callback c, void *u_data,
                t_callback deletor);
bool da_filter_out(t_da *da, t_callback c, void *u_data, t_callback deletor);
bool da_nfilter_out(t_da *da, size_t n, t_callback c, void *u_data,
                    t_callback deletor);

// advance
bool da_advance(t_da *da, size_t n, t_callback deletor, void *u_data);

// reverse
void da_reverse(t_da *da);
void da_nreverse(t_da *da, size_t n);
void da_b_nreverse(t_da *da, size_t n);

// find
void *da_find(t_da *da, t_comparator c, void *u_data);
void *da_nfind(t_da *da, size_t n, t_comparator c, void *u_data);
bool  da_find_get(t_da *da, t_comparator c, void *u_data, void *out_buffer);

// sort
bool da_sort(t_da *da, t_comparator c);
bool da_nsort(t_da *da, size_t n, t_comparator c);

// BUG:
// fix bug on rms (gap problem)

#	define C_MINI_DA__SZ_ELEM_DFLT 1024
#	define C_MINI_DA__NB_ELEM_DFLT 4
#	define C_MINI_DA__GROWTH       1.5

#	define C_MINI__DA
#endif // !C_MINI__DA
