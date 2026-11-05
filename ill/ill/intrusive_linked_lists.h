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
 * \typedef t_cm_ill
 * \brief the t_cm_ill struct represents an intrusive singly linked list node
 * \param offset offset of the node in the element
 * \param next pointer to the next node
 * \ingroup ill
**/
typedef struct s_cm_ill {
	size_t           offset;
	struct s_cm_ill *next;
} t_cm_ill;

void  ill_init(t_cm_ill *node, size_t offset);
void *ill_copy(const t_cm_ill *head, t_cm_duplicator d, t_cm_dtor dtor);

void     *ill_ntoe(const t_cm_ill *node);
t_cm_ill *ill_eton(void *e, size_t offset);

void     *ill_advance_it(void *e, size_t offset);
t_cm_ill *ill_advance_it_node(const t_cm_ill *it);

bool ill_is_isolated(const t_cm_ill *node);
bool ill_is_circular(const t_cm_ill *head);
bool ill_is_sorted(const t_cm_ill *head, t_cm_comparator comparator);
bool ill_has(const t_cm_ill *head, t_cm_callback matcher, void *arg);

void ill_append(t_cm_ill *anchor, t_cm_ill *node);
void ill_nappend(t_cm_ill *anchor, t_cm_ill *nodes);

t_cm_ill *ill_npush(t_cm_ill **head, t_cm_ill *nodes);
t_cm_ill *ill_push(t_cm_ill **head, t_cm_ill *node);
t_cm_ill *ill_push_back(t_cm_ill **head, t_cm_ill *node);

t_cm_ill *ill_pop(t_cm_ill **head);
t_cm_ill *ill_pop_back(t_cm_ill **head);

t_cm_ill *ill_remove_after(t_cm_ill *prev);
bool      ill_remove_node(t_cm_ill **head, t_cm_ill *target);

t_cm_ill *ill_find_prev(const t_cm_ill *head, const t_cm_ill *target);
t_cm_ill *ill_find(const t_cm_ill *head, const t_cm_ill *target);
t_cm_ill *ill_get_prev(const t_cm_ill *head, t_cm_comparator c,
                       const void *arg);
t_cm_ill *ill_get(const t_cm_ill *head, t_cm_comparator c, const void *arg);

t_cm_ill *ill_get_tail(t_cm_ill *head);

void *ill_get_min(const t_cm_ill *head, t_cm_comparator comp);
void *ill_get_max(const t_cm_ill *head, t_cm_comparator comp);
void *ill_get_kth(const t_cm_ill *head, size_t k);

size_t ill_length(const t_cm_ill *head);
size_t ill_nlength(const t_cm_ill *head, size_t n);
size_t ill_match_nb(const t_cm_ill *head, t_cm_callback matcher, void *arg);

bool      ill_for(t_cm_ill *head, size_t n, t_cm_callback c, void *u_data);
bool      ill_foreach(t_cm_ill *head, t_cm_callback c, void *u_data);
t_cm_ill *ill_reverse(t_cm_ill *head);

// move // !
// if !end_cut cut to the end of list included otherwise stop before end_cut
// void   ill_move(t_cm_ill *dest_anchor, t_cm_ill *start_cut, t_cm_ill
// *end_cut); // ! Merges two sorted lists into one (classic merge sort building
// block).
t_cm_ill *ill_sorted_merge(t_cm_ill *list1, t_cm_ill *list2,
                           t_cm_comparator comp);
// Sorts the list in place (classic merge sort, O(n log n)).
void      ill_sort(t_cm_ill **head, t_cm_comparator comp);
// filter
// keeps elements for which cb is true, moves the others into *filtered.
// *head is updated if the current head is moved out.
// returns the number of elements moved into *filtered.
size_t    ill_filter(t_cm_ill **head, t_cm_callback cb, void *arg,
                     t_cm_ill **filtered);
// inverse of ill_filter: moves elements for which cb is true into *filtered.
size_t    ill_filter_out(t_cm_ill **head, t_cm_callback cb, void *arg,
                         t_cm_ill **filtered);
// destructive variant of ill_filter: destroys (dtor) the moved-out elements.
size_t    ill_rm_if(t_cm_ill **head, t_cm_callback cb, void *arg, t_cm_dtor d);
// removes consecutive duplicates according to the comparator, dtor is optional.
void      ill_unique(t_cm_ill *head, t_cm_comparator c, t_cm_dtor d);

#endif // !C_MINI__ILL
