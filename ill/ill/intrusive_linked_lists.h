#ifndef C_MINI__ILL
#define C_MINI__ILL

#include <stdbool.h>
#include <stddef.h>

#include "common_base/log.h"
#ifdef C_MINI__TESTS

#	define C_MINI__ILL_LOG_ECB    C_MINI__LOG_F_PREFIX "Callback failed."
#	define C_MINI__ILL_LOG_EARG   C_MINI__LOG_EARG
#	define C_MINI__ILL_LOG_EALLOC C_MINI__LOG_EALLOC
#	define C_MINI__ILL_LOG_FAIL   C_MINI__LOG_FAIL

#	define C_MINI__ILL_LOG_SUCCESS C_MINI__LOG_SUCCESS

#endif // !C_MINI__TESTS

#include "common_base/fptr.h"

/**
 * \defgroup ill Intrusive Linked List
**/

/**
 * \typedef t_ill
 * \brief the t_ill struct represents an intrusive singly linked list node
 * \param offset offset of the node in the element
 * \param next pointer to the next node
 * \ingroup ill
**/
typedef struct s_ill {
	size_t        offset;
	struct s_ill *next;
} t_ill;

void  ill_init(t_ill *node, size_t offset);
void *ill_copy(const t_ill *head, t_cm_duplicator d, t_cm_dtor dtor);

void  *ill_ntoe(const t_ill *node);
t_ill *ill_eton(void *e, size_t offset);

void  *ill_advance_it(void *e, size_t offset);
t_ill *ill_advance_it_node(t_ill *it);

bool ill_is_isolated(t_ill *node);
bool ill_is_circular(t_ill *head);
bool ill_is_sorted(t_ill *head, t_cm_comparator comparator);
bool ill_has(t_ill *head, t_cm_callback matcher, void *arg);

void ill_append(t_ill *anchor, t_ill *node);
void ill_nappend(t_ill *anchor, t_ill *nodes);

t_ill *ill_npush(t_ill **head, t_ill *nodes);
t_ill *ill_push(t_ill **head, t_ill *node);
t_ill *ill_push_back(t_ill **head, t_ill *node);

t_ill *ill_pop(t_ill **head);
t_ill *ill_pop_back(t_ill **head);

t_ill *ill_remove_after(t_ill *prev);
bool   ill_remove_node(t_ill **head, t_ill *target);

t_ill *ill_find_prev(t_ill *head, t_ill *target);
t_ill *ill_find(t_ill *head, t_ill *target);
t_ill *ill_get_prev(t_ill *head, t_cm_comparator c, void *arg);
t_ill *ill_get(t_ill *head, t_cm_comparator c, void *arg);

t_ill *ill_get_tail(t_ill *head);

void *ill_get_min(t_ill *head, t_cm_comparator comp);
void *ill_get_max(t_ill *head, t_cm_comparator comp);
void *ill_get_kth(t_ill *head, size_t k);

size_t ill_length(t_ill *head);
size_t ill_nlength(t_ill *head, size_t n);
size_t ill_match_nb(t_ill *head, t_cm_callback matcher, void *arg);

bool   ill_for(t_ill *head, size_t n, t_cm_callback c, void *u_data);
bool   ill_foreach(t_ill *head, t_cm_callback c, void *u_data);
t_ill *ill_reverse(t_ill *head);

// move // !
// if !end_cut cut to the end of list included otherwise stop before end_cut
// void   ill_move(t_ill *dest_anchor, t_ill *start_cut, t_ill *end_cut); // !
// Merges two sorted lists into one (classic merge sort building block).
t_ill *ill_sorted_merge(t_ill *list1, t_ill *list2, t_cm_comparator comp);
// Sorts the list in place (classic merge sort, O(n log n)).
void   ill_sort(t_ill **head, t_cm_comparator comp);
// filter
// keeps elements for which cb is true, moves the others into *filtered.
// *head is updated if the current head is moved out.
// returns the number of elements moved into *filtered.
size_t ill_filter(t_ill **head, t_cm_callback cb, void *arg, t_ill **filtered);
// inverse of ill_filter: moves elements for which cb is true into *filtered.
size_t ill_filter_out(t_ill **head, t_cm_callback cb, void *arg,
                      t_ill **filtered);
// destructive variant of ill_filter: destroys (dtor) the moved-out elements.
size_t ill_rm_if(t_ill **head, t_cm_callback cb, void *arg, t_cm_dtor d);
// removes consecutive duplicates according to the comparator, dtor is optional.
void   ill_unique(t_ill *head, t_cm_comparator c, t_cm_dtor d);

#endif // !C_MINI__ILL
