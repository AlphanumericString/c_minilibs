#ifndef C_MINI__IDL
#define C_MINI__IDL

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "common_base/log.h"
#ifdef C_MINI__TESTS

#	define C_MINI__IDL_LOG_ECB     C_MINI__LOG_F_PREFIX "Callback Fail."
#	define C_MINI__IDL_LOG_EARG    C_MINI__LOG_EARG
#	define C_MINI__IDL_LOG_EALLOC  C_MINI__LOG_EALLOC
#	define C_MINI__IDL_LOG_FAIL    C_MINI__LOG_FAIL
#	define C_MINI__IDL_LOG_SUCCESS C_MINI__LOG_SUCCESS

#endif // !C_MINI__TESTS

#include "common_base/fptr.h"

/**
 * \defgroup idl Intrusive Doubly-linked Lists
**/

/**
 * \typedef t_cm_idl
 * \brief t_cm_idl struct represents an intrusive doubly linked list node
 * \param offset offset of the node in the element
 * \param next pointer to the next node
 * \param prev pointer to the previous node
 * \ingroup idl
**/
typedef struct s_cm_idl {
	struct s_cm_idl *prev;
	struct s_cm_idl *next;
	size_t           offset;
} t_cm_idl;

void      idl_init(t_cm_idl *node, size_t offset);
void     *idl_copy(const t_cm_idl *head, t_cm_duplicator d, t_cm_dtor dtor);
void     *idl_ntoe(const t_cm_idl *node);
t_cm_idl *idl_eton(const void *e, size_t offset);
void     *idl_advance_it(const void *e, size_t offset);
void     *idl_recede_it(const void *e, size_t offset);
t_cm_idl *idl_advance_it_node(const t_cm_idl *it);
t_cm_idl *idl_recede_it_node(const t_cm_idl *it);
bool      idl_is_isolated(const t_cm_idl *node);
bool      idl_is_circular(const t_cm_idl *head);
bool      idl_is_sorted(const t_cm_idl *head, t_cm_comparator comparator);
bool      idl_has(const t_cm_idl *head, t_cm_callback matcher, void *arg);
void      idl_append(t_cm_idl *anchor, t_cm_idl *node);
void      idl_nappend(t_cm_idl *anchor, t_cm_idl *nodes);
void      idl_prepend(t_cm_idl *anchor, t_cm_idl *node);
void      idl_nprepend(t_cm_idl *anchor, t_cm_idl *nodes);
t_cm_idl *idl_push(t_cm_idl **head, t_cm_idl *node);
t_cm_idl *idl_npush(t_cm_idl **head, t_cm_idl *nodes);
t_cm_idl *idl_push_back(t_cm_idl **head, t_cm_idl *node);
t_cm_idl *idl_pop(t_cm_idl **head);
t_cm_idl *idl_pop_back(t_cm_idl **head);
t_cm_idl *idl_remove_after(t_cm_idl *prev);
bool      idl_remove_node(t_cm_idl **head, t_cm_idl *target);
t_cm_idl *idl_find_prev(t_cm_idl *head, const t_cm_idl *target);
t_cm_idl *idl_find(t_cm_idl *head, const t_cm_idl *target);
t_cm_idl *idl_get_prev(t_cm_idl *head, t_cm_comparator c, const void *arg);
t_cm_idl *idl_get(t_cm_idl *head, t_cm_comparator c, const void *arg);
t_cm_idl *idl_get_tail(t_cm_idl *node);
t_cm_idl *idl_get_head(t_cm_idl *node);
void     *idl_get_min(const t_cm_idl *head, t_cm_comparator c);
void     *idl_get_max(const t_cm_idl *head, t_cm_comparator c);
void     *idl_get_kth(const t_cm_idl *head, size_t k);
size_t    idl_length(const t_cm_idl *head);
size_t    idl_nlength(const t_cm_idl *head, size_t n);
size_t    idl_match_nb(const t_cm_idl *head, t_cm_callback matcher, void *arg);
bool      idl_for(t_cm_idl *head, size_t n, t_cm_callback c, void *u_data);
bool      idl_foreach(t_cm_idl *head, t_cm_callback c, void *u_data);
t_cm_idl *idl_reverse(t_cm_idl *head);

// Merges two sorted lists into one (classic merge sort building block).
t_cm_idl *idl_sorted_merge(t_cm_idl *list1, t_cm_idl *list2,
                           t_cm_comparator comp);
// Sorts the list in place (classic merge sort, O(n log n)).
void      idl_sort(t_cm_idl **head, t_cm_comparator comp);
// filter
// keeps elements for which cb is true, moves the others into *filtered.
// *head is updated if the current head is moved out.
// returns the number of elements moved into *filtered.
size_t    idl_filter(t_cm_idl **head, t_cm_callback cb, void *arg,
                     t_cm_idl **filtered);
// inverse of idl_filter: moves elements for which cb is true into *filtered.
size_t    idl_filter_out(t_cm_idl **head, t_cm_callback cb, void *arg,
                         t_cm_idl **filtered);
// destructive variant of idl_filter: destroys (dtor) the moved-out elements.
size_t    idl_rm_if(t_cm_idl **head, t_cm_callback cb, void *arg, t_cm_dtor d);
// removes consecutive duplicates according to the comparator, dtor is optional.
void      idl_unique(t_cm_idl *head, t_cm_comparator c, t_cm_dtor d);

#endif // !C_MINI__IDL
