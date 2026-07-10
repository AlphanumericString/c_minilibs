#ifndef C_MINI__SS
#define C_MINI__SS

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef C_MINI__TEST
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

#else // || C_MINI_TEST

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TEST

/**
 * \brief sparse set structure
 * \param data raw vals storage
 * \param indexes storage for offsets after swaps and pushes
 * \param nb_elem number of elements in the array
 * \param sz_elem size of each elements in the array
 * \param cappacity max number of elements in the sparse set
**/
typedef struct s_sprase_set {
	void  *data;
	uint  *indexes;
	size_t nb_elem;
	size_t sz_elem;
	size_t cappacity;
} t_ss;

// init
t_ss *ss_new(size_t sz_elem);
t_ss *ss_init(t_ss *ret, size_t sz_elem);
t_ss *ss_ninit(t_ss *ret, size_t n, size_t sz_elem);
t_ss *ss_from_array(t_ss *ss, void *array, uint *index_array, size_t nb_elem,
                    size_t sz_elem);
// destroy
void  ss_clr(t_ss *to_clear);
void  ss_destroy(t_ss *to_destroy);
// add
bool  ss_add(t_ss *ss, void *elem);
bool  ss_add_array(t_ss *vec, void *arr, size_t nb_e);
// getter
bool  ss_set(t_ss *ss, size_t pos, void *elem);
void *ss_at(t_ss *ss, size_t pos);
void *ss_get(t_ss *ss, size_t pos, void *out);
void *ss_array(t_ss *ss);
// manip
bool  ss_swap(t_ss *ss, size_t pos_a, size_t pos_b);
// rm
void  ss_remove(t_ss *ss, size_t pos);
void  ss_nremove(t_ss *ss, size_t start, size_t end);
// apply
bool  ss_apply(t_ss *ss, void (*func)(void *elem));
bool  ss_apply_it(t_ss *ss, void (*func)(void *elem, size_t pos, bool last));
void *ss_fold(t_ss *ss, void *u_val, void (*func)(void *u_val, void *e_val));
// convert
void *ss_to_array(t_ss *ss);
void *ss_to_loc_array(t_ss *ss, void *arr);

#define C_MINI_SS_SZ_ELEM_DFLT 1024
#define C_MINI_SS_NB_ELEM_DFLT 4

#endif // !C_MINI__SS
