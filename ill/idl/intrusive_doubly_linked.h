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

/* ************************************************************************** */
/* api shape                                                                  */
/*                                                                            */
/* description:                                                               */
/* the default api is element centric: functions take and return the user     */
/* payload (the struct that embeds a t_cm_idl) plus the offset of the node    */
/* inside that payload. the low level api keeps working on nodes and is       */
/* exposed under the _node suffix, it is the escape hatch for mixed payload   */
/* types or manual node management.                                           */
/*                                                                            */
/* the head pointer is an opaque list handle: read only traversals take a     */
/* const head, structural mutations that may replace the first node take      */
/* a t_cm_idl **head. the node version of a function is named _node, so       */
/* idl_push() takes an element while idl_push_node() takes a node.            */
/*                                                                            */
/* element helpers that build a list from several payloads assume every       */
/* payload in the chain uses the same offset. use the _node variants when     */
/* offsets differ.                                                            */
/*                                                                            */
/* the circular list api is reserved under the idl_circ_ prefix and is not    */
/* implemented yet.                                                           */
/* ************************************************************************** */

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

void  idl_init(void *elem, size_t offset);
void  idl_init_node(t_cm_idl *node, size_t offset);
void *idl_copy(const t_cm_idl *head, t_cm_duplicator d, t_cm_dtor dtor);

void     *idl_ntoe(const t_cm_idl *node);
t_cm_idl *idl_eton(const void *e, size_t offset);

bool idl_is_isolated(const void *elem, size_t offset);
bool idl_is_isolated_node(const t_cm_idl *node);
bool idl_is_circular(const t_cm_idl *head);
bool idl_is_sorted(const t_cm_idl *head, t_cm_comparator comparator);
bool idl_has(const t_cm_idl *head, t_cm_callback matcher, void *arg);

void     *idl_push(t_cm_idl **head, void *elem, size_t offset);
t_cm_idl *idl_push_node(t_cm_idl **head, t_cm_idl *node);
void     *idl_npush(t_cm_idl **head, void *elems, size_t offset);
t_cm_idl *idl_npush_node(t_cm_idl **head, t_cm_idl *nodes);
void     *idl_push_back(t_cm_idl **head, void *elem, size_t offset);
t_cm_idl *idl_push_back_node(t_cm_idl **head, t_cm_idl *node);
void      idl_append(void *anchor_elem, void *elem, size_t offset);
void      idl_append_node(t_cm_idl *anchor, t_cm_idl *node);
void      idl_nappend(void *anchor_elem, void *elems, size_t offset);
void      idl_nappend_node(t_cm_idl *anchor, t_cm_idl *nodes);
void      idl_prepend(void *anchor_elem, void *elem, size_t offset);
void      idl_prepend_node(t_cm_idl *anchor, t_cm_idl *node);
void      idl_nprepend(void *anchor_elem, void *elems, size_t offset);
void      idl_nprepend_node(t_cm_idl *anchor, t_cm_idl *nodes);

void     *idl_pop(t_cm_idl **head);
t_cm_idl *idl_pop_node(t_cm_idl **head);
void     *idl_pop_back(t_cm_idl **head);
t_cm_idl *idl_pop_back_node(t_cm_idl **head);
void     *idl_remove_after(void *prev_elem, size_t offset);
t_cm_idl *idl_remove_after_node(t_cm_idl *prev);
bool      idl_remove(t_cm_idl **head, const void *elem, size_t offset);
bool      idl_remove_node(t_cm_idl **head, t_cm_idl *target);

void     *idl_find_prev(const t_cm_idl *head, const void *elem, size_t offset);
t_cm_idl *idl_find_prev_node(const t_cm_idl *head, const t_cm_idl *target);
void     *idl_find(const t_cm_idl *head, const void *elem, size_t offset);
t_cm_idl *idl_find_node(const t_cm_idl *head, const t_cm_idl *target);
void *idl_get_prev(const t_cm_idl *head, t_cm_comparator c, const void *arg);
t_cm_idl *idl_get_prev_node(const t_cm_idl *head, t_cm_comparator c,
                            const void *arg);
void     *idl_get(const t_cm_idl *head, t_cm_comparator c, const void *arg);
t_cm_idl *idl_get_node(const t_cm_idl *head, t_cm_comparator c,
                       const void *arg);
void     *idl_get_tail(const t_cm_idl *head);
t_cm_idl *idl_get_tail_node(const t_cm_idl *node);
void     *idl_get_head(const t_cm_idl *head);
t_cm_idl *idl_get_head_node(const t_cm_idl *node);

void     *idl_get_min(const t_cm_idl *head, t_cm_comparator c);
t_cm_idl *idl_get_min_node(const t_cm_idl *head, t_cm_comparator c);
void     *idl_get_max(const t_cm_idl *head, t_cm_comparator c);
t_cm_idl *idl_get_max_node(const t_cm_idl *head, t_cm_comparator c);
void     *idl_get_kth(const t_cm_idl *head, size_t k);
t_cm_idl *idl_get_kth_node(const t_cm_idl *head, size_t k);

size_t idl_length(const t_cm_idl *head);
size_t idl_nlength(const t_cm_idl *head, size_t n);
size_t idl_match_nb(const t_cm_idl *head, t_cm_callback matcher, void *arg);

void     *idl_advance_it(const void *e, size_t offset);
t_cm_idl *idl_advance_it_node(const t_cm_idl *it);
void     *idl_recede_it(const void *e, size_t offset);
t_cm_idl *idl_recede_it_node(const t_cm_idl *it);
bool idl_for(const t_cm_idl *head, size_t n, t_cm_callback c, void *u_data);
bool idl_foreach(const t_cm_idl *head, t_cm_callback c, void *u_data);

void      idl_reverse(t_cm_idl **head);
t_cm_idl *idl_sorted_merge(t_cm_idl *list1, t_cm_idl *list2,
                           t_cm_comparator comp);
void      idl_sort(t_cm_idl **head, t_cm_comparator comp);

size_t idl_filter(t_cm_idl **head, t_cm_callback cb, void *arg,
                  t_cm_idl **filtered);
size_t idl_filter_out(t_cm_idl **head, t_cm_callback cb, void *arg,
                      t_cm_idl **filtered);
size_t idl_rm_if(t_cm_idl **head, t_cm_callback cb, void *arg, t_cm_dtor d);
void   idl_unique(t_cm_idl *head, t_cm_comparator c, t_cm_dtor d);

#endif // !C_MINI__IDL
