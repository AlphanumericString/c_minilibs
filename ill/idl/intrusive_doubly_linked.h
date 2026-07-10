#ifndef C_MINI__IDL

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

#	ifndef CMINI_T_COMPARATOR_DEFINE
#		define CMINI_T_COMPARATOR_DEFINE

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_comparator)(void *elem, void *arg);

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

/**
 * \brief t_idll struct
 * \param offset offset of the node in the element
 * \param next pointer to the next node
 * \param prev pointer to the next node
**/
typedef struct s_idl {
	struct s_idl *prev;
	struct s_idl *next;
	size_t        offset;
} t_idl;

// Initialize node (not in any list)
void  idl_init(t_idl *node, size_t offset);
void *idl_copy(const t_idl *head, t_duplicator d);

// NODE->ELEM and ELEM->NODE
void  *idl_ntoe(const t_idl *node);
t_idl *idl_eton(void *e, size_t offset);

// Checks
bool idl_is_isolated(t_idl *node);
bool idl_is_circular(t_idl *head);
// auto detects if reverse sorted
bool idl_is_sorted(t_idl *head, t_comparator comparator);
bool idl_has(t_idl *head, t_callback matcher, void *arg);

// Insert 'node' after 'anchor' (anchor does not move)
void idl_append(t_idl *anchor, t_idl *node);
void idl_nappend(t_idl *anchor, t_idl *nodes);
// Insert 'node' before 'anchor' (anchor does move)
void idl_prepend(t_idl *anchor, t_idl *node);
void idl_nprepend(t_idl *anchor, t_idl *nodes);

// Insert at front of list (updates head pointer)
t_idl *idl_push(t_idl **head, t_idl *node);
t_idl *idl_npush(t_idl **head, t_idl *nodes);
t_idl *idl_push_back(t_idl **head, t_idl *node);

// Remove head/tail
t_idl *idl_pop(t_idl **head);
t_idl *idl_pop_back(t_idl **head);

// Remove node
t_idl *idl_remove_after(t_idl *prev);
bool   idl_remove_node(t_idl **head, t_idl *target);

// Find node
t_idl *idl_find_prev(t_idl *head, t_idl *target);
t_idl *idl_find(t_idl *head, t_idl *target);
// -get version
t_idl *idl_get_prev(t_idl *head, t_comparator c, void *arg);
t_idl *idl_get(t_idl *head, t_comparator c, void *arg);

// Get tail (last node)
t_idl *idl_get_tail(t_idl *head);

// Get min/max
void *idl_get_min(t_idl *head, t_comparator c);
void *idl_get_max(t_idl *head, t_comparator c);
void *idl_get_kth(t_idl *head, size_t k);

// Get length
size_t idl_length(t_idl *head);
size_t idl_nlength(t_idl *head, size_t n);
size_t idl_match_nb(t_idl *head, t_callback matcher, void *arg);

// Forward iteration macro
bool idl_for(t_idl *head, size_t n, t_callback c, void *u_data);
bool idl_foreach(t_idl *head, t_callback c, void *u_data);

// reverse
t_idl *idl_reverse(t_idl *head);

// move // !
// if !end_cut cut to the end of list included otherwise stop before end_cut
// void   idl_move(t_idl *dest_anchor, t_idl *start_cut, t_idl *end_cut); // !
//- Merges two sorted lists into one.
// void   idl_sort(t_idl **head, t_comparator comp);                       // !
// t_idl *idl_sorted_merge(t_idl *list1, t_idl *list2, t_comparator comp); // !

// filter
// list for tru is returned list of false has sotred in filtered
// t_idl *idl_filter(t_idl *head, t_callback cb, void *arg, t_idl **filtered);
// remove if aka filter in place. if !free_func no free.
// bool   idl_remove_if(t_idl **head, t_callback condition_cb, void *arg,
//                      void (*free_func)(void *elem));
// // (requirement: list must be sorted)
// void   idl_unique(t_idl *head, t_comparator c); // !

// TODO: idl_[n][push|pop][_back]_elem : take elem as node w offset
// annoying for users to have to do the convertions

#	define C_MINI__IDL
#endif // !C_MINI__IDL
