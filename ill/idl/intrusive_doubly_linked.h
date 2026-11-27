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

#	define C_MINI__IDL_CHECK_STRICT

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
/* nodes passed to insert helpers must be isolated (idl_is_isolated()).       */
/* inserting an already linked node, or an anchor equal to the inserted       */
/* node, is not detected and can create cycles.                               */
/*                                                                            */
/* idl_find_prev()/idl_get_prev() return NULL both when the target is the     */
/* head (no predecessor) and when it is not found; use idl_find() first to    */
/* distinguish the two cases.                                                 */
/*                                                                            */
/* idl_iter() keeps the current element in *iter and uses the offset of the   */
/* current element to step forward. idl_advance_it() is the stateless         */
/* equivalent of "it++": it takes the current element and returns the next    */
/* one. mixing the two is just a manual skip of one element.                  */
/*                                                                            */
/* idl_iter_range() and idl_apply_range() take an exclusive end element; a    */
/* NULL end means the tail. idl_iter_rev() walks from the tail through prev   */
/* links. idl_move() splices an inclusive [first, last] range to the tail of  */
/* the destination list. idl_concat() appends a second list to the first,     */
/* while idl_merge() aliases idl_sorted_merge().                              */
/*                                                                            */
/* mixed node offsets are supported by the iterators. validate does not       */
/* reject them: it checks head/tail isolation, prev/next agreement and that   */
/* the chain terminates.                                                      */
/*                                                                            */
/* the circular list api is exposed under the idl_circ_ prefix. a circular    */
/* list has no NULL terminator: head is the first node, head->prev is the     */
/* last node and the last node's next is head. empty lists are NULL.          */
/* circular functions assume a valid ring and do not run O(n) validation;     */
/* do not pass a circular list to the non-circular api.                       */
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

void  idl_init(const void *elem, size_t offset);
void  idl_init_node(t_cm_idl *node, size_t offset);
void *idl_copy(const t_cm_idl *head, t_cm_duplicator d, t_cm_dtor dtor);

void     *idl_ntoe(const t_cm_idl *node);
t_cm_idl *idl_eton(const void *e, size_t offset);

bool idl_is_isolated(const void *elem, size_t offset);
bool idl_is_isolated_node(const t_cm_idl *node);
bool idl_is_circular(const t_cm_idl *head);
bool idl_is_sorted(const t_cm_idl *head, t_cm_comparator comparator);
bool idl_is_empty(const t_cm_idl *head);
bool idl_has(const t_cm_idl *head, t_cm_callback matcher, void *arg);
bool idl_validate(const t_cm_idl *head);

void     *idl_push(t_cm_idl **head, const void *elem, size_t offset);
t_cm_idl *idl_push_node(t_cm_idl **head, t_cm_idl *node);
void     *idl_npush(t_cm_idl **head, const void *elems, size_t offset);
t_cm_idl *idl_npush_node(t_cm_idl **head, t_cm_idl *nodes);
void     *idl_push_back(t_cm_idl **head, const void *elem, size_t offset);
t_cm_idl *idl_push_back_node(t_cm_idl **head, t_cm_idl *node);

void idl_append(const void *anchor_elem, const void *elem, size_t offset);
void idl_append_node(t_cm_idl *anchor, t_cm_idl *node);
void idl_nappend(const void *anchor_elem, const void *elems, size_t offset);
void idl_nappend_node(t_cm_idl *anchor, t_cm_idl *nodes);

void idl_prepend(const void *anchor_elem, const void *elem, size_t offset);
void idl_prepend_node(t_cm_idl *anchor, t_cm_idl *node);
void idl_nprepend(const void *anchor_elem, const void *elems, size_t offset);
void idl_nprepend_node(t_cm_idl *anchor, t_cm_idl *nodes);

bool idl_move(t_cm_idl **from, t_cm_idl **to, const void *first,
              const void *last, size_t offset);
bool idl_move_node(t_cm_idl **from, t_cm_idl **to, t_cm_idl *first,
                   t_cm_idl *last);

void     *idl_pop(t_cm_idl **head);
t_cm_idl *idl_pop_node(t_cm_idl **head);
void     *idl_pop_back(t_cm_idl **head);
t_cm_idl *idl_pop_back_node(t_cm_idl **head);

void     *idl_remove_after(const void *prev_elem, size_t offset);
t_cm_idl *idl_remove_after_node(t_cm_idl *prev);
bool      idl_remove(t_cm_idl **head, const void *elem, size_t offset);
bool      idl_remove_node(t_cm_idl **head, t_cm_idl *target);
void      idl_clear(t_cm_idl **head, t_cm_dtor dtor);

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
void     *idl_get_tail(const void *elem, size_t offset);
t_cm_idl *idl_get_tail_node(const t_cm_idl *node);
void     *idl_get_head(const void *elem, size_t offset);
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

void *idl_apply(const t_cm_idl *head, t_cm_callback c, void *u_data);
void *idl_napply(const t_cm_idl *head, size_t n, t_cm_callback c, void *u_data);
void *idl_apply_it(const t_cm_idl *head, t_cm_callback_it func, void *u_data);
void *idl_napply_it(const t_cm_idl *head, size_t n, t_cm_callback_it func,
                    void *u_data);
void *idl_apply_range(const void *head_elem, const void *end_elem,
                      t_cm_callback c, void *u_data, size_t offset);

void *idl_fold(const t_cm_idl *head, t_cm_merger m, void *u_start);
void *idl_nfold(const t_cm_idl *head, size_t n, t_cm_merger m, void *u_start);
void *idl_reduce(const t_cm_idl *head, t_cm_merger m, size_t elem_size);
void *idl_nreduce(const t_cm_idl *head, size_t n, t_cm_merger m,
                  size_t elem_size);

bool  idl_iter(const t_cm_idl *head, void **iter, size_t offset);
bool  idl_iter_range(const void *head_elem, const void *end_elem, void **iter,
                     size_t offset);
bool  idl_iter_rev(const void *head_elem, void **iter, size_t offset);
void *idl_advance_it(const void *e, size_t offset);
t_cm_idl *idl_advance_it_node(const t_cm_idl *it);
void     *idl_recede_it(const void *e, size_t offset);
t_cm_idl *idl_recede_it_node(const t_cm_idl *it);

void      idl_reverse(t_cm_idl **head);
t_cm_idl *idl_sorted_merge(t_cm_idl *list1, t_cm_idl *list2,
                           t_cm_comparator comp);
t_cm_idl *idl_merge(t_cm_idl *list1, t_cm_idl *list2, t_cm_comparator comp);
void      idl_sort(t_cm_idl **head, t_cm_comparator comp);

void     *idl_concat(const void *list1, const void *list2, size_t offset);
t_cm_idl *idl_concat_node(t_cm_idl *list1, t_cm_idl *list2);

size_t idl_filter(t_cm_idl **head, t_cm_callback cb, void *arg,
                  t_cm_idl **filtered);
size_t idl_filter_out(t_cm_idl **head, t_cm_callback cb, void *arg,
                      t_cm_idl **filtered);
size_t idl_nfilter(t_cm_idl **head, size_t n, t_cm_callback cb, void *arg,
                   t_cm_idl **filtered);
size_t idl_nfilter_out(t_cm_idl **head, size_t n, t_cm_callback cb, void *arg,
                       t_cm_idl **filtered);
size_t idl_filter_dt(t_cm_idl **head, t_cm_callback cb, void *arg, t_cm_dtor d);
void   idl_unique(t_cm_idl *head, t_cm_comparator c, t_cm_dtor d);

/*
 * circular api: head is the first node, head->prev is the last node.
 * the handle is not a sentinel; it is a real payload node.
 */
size_t    idl_circ_length(const t_cm_idl *head);
t_cm_idl *idl_circ_get_head_node(const t_cm_idl *head);
void     *idl_circ_get_head(const void *head_elem, size_t offset);
t_cm_idl *idl_circ_get_tail_node(const t_cm_idl *head);
void     *idl_circ_get_tail(const void *head_elem, size_t offset);

t_cm_idl *idl_circ_push_node(t_cm_idl **head, t_cm_idl *node);
void     *idl_circ_push(t_cm_idl **head, const void *elem, size_t offset);
t_cm_idl *idl_circ_push_back_node(t_cm_idl **head, t_cm_idl *node);
void     *idl_circ_push_back(t_cm_idl **head, const void *elem, size_t offset);
t_cm_idl *idl_circ_pop_node(t_cm_idl **head);
void     *idl_circ_pop(t_cm_idl **head);
t_cm_idl *idl_circ_pop_back_node(t_cm_idl **head);
void     *idl_circ_pop_back(t_cm_idl **head);

t_cm_idl *idl_circ_concat_node(t_cm_idl *list1, t_cm_idl *list2);
void     *idl_circ_concat(const void *list1, const void *list2, size_t offset);

bool idl_circ_remove_node(t_cm_idl **head, t_cm_idl *target);
bool idl_circ_remove(t_cm_idl **head, const void *elem, size_t offset);
void idl_circ_clear(t_cm_idl **head, t_cm_dtor dtor);

void *idl_circ_foreach(const t_cm_idl *head, t_cm_callback c, void *u_data);
void *idl_circ_foreach_rev(const t_cm_idl *head, t_cm_callback c, void *u_data);
bool  idl_circ_iter(const t_cm_idl *head, void **iter, size_t offset);
bool  idl_circ_iter_rev(const t_cm_idl *head, void **iter, size_t offset);

#ifdef C_MINI__IDL_INTERNAL

t_cm_idl *idli_push_back_node(t_cm_idl **head, t_cm_idl **tail, t_cm_idl *node);
t_cm_idl *idli_pop_back_node(t_cm_idl **head, t_cm_idl **tail);
t_cm_idl *idli_concat_node(t_cm_idl **h1, t_cm_idl **t1, t_cm_idl **h2,
                           t_cm_idl **t2);
t_cm_idl *idli_remove_node(t_cm_idl **head, t_cm_idl **tail, t_cm_idl *target);

#endif // !C_MINI__IDL_INTERNAL

#endif // !C_MINI__IDL
