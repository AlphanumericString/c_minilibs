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

/* ************************************************************************* */
/* api shape                                                                 */
/*                                                                           */
/* description:                                                              */
/* the default api is element centric: functions take and return the user    */
/* payload (the struct that embeds a t_cm_ill) plus the offset of the node   */
/* inside that payload. the low level api keeps working on nodes and is      */
/* exposed under the _node suffix, it is the escape hatch for mixed payload  */
/* types or manual node management.                                          */
/*                                                                           */
/* the head pointer is an opaque list handle: read only traversals take a    */
/* const head, structural mutations that may replace the first node take     */
/* a t_cm_ill **head. the node version of a function is named _node, so      */
/* ill_push() takes an element while ill_push_node() takes a node.           */
/*                                                                           */
/* element helpers that build a list from several payloads assume every      */
/* payload in the chain uses the same offset. use the _node variants when    */
/* offsets differ.                                                           */
/*                                                                           */
/* nodes passed to insert helpers must be isolated (ill_is_isolated()).      */
/* inserting an already linked node, or an anchor equal to the inserted      */
/* node, is not detected and can create cycles.                              */
/*                                                                           */
/* ill_find_prev()/ill_get_prev() return NULL both when the target is the    */
/* head (no predecessor) and when it is not found; use ill_find() first to   */
/* distinguish the two cases.                                                */
/*                                                                           */
/* ill_iter() keeps the current element in *iter and uses the offset of the  */
/* current element to step forward. ill_advance_it() is the stateless        */
/* equivalent of "it++": it takes the current element and returns the next   */
/* one. mixing the two is just a manual skip of one element.                 */
/*                                                                           */
/* ill_iter_range() and ill_apply_range() take an exclusive end element; a   */
/* NULL end means the tail. ill_move() splices an inclusive [first, last]    */
/* range to the tail of the destination list. ill_concat() appends a second  */
/* list to the first, while ill_merge() aliases ill_sorted_merge().          */
/*                                                                           */
/* mixed node offsets are supported by the iterators. validate does not      */
/* reject them: it only checks that the chain terminates.                    */
/*                                                                           */
/* the circular list api is exposed under the ill_circ_ prefix. a circular   */
/* list has no NULL terminator: the head argument is the last node and       */
/* head->next is the first node, so push at both ends and pop from the front */
/* are O(1); pop_back/remove need the predecessor and are O(n). empty lists  */
/* are NULL. circular functions assume a valid ring and do not run O(n)      */
/* validation; do not pass a circular list to the non-circular api.          */
/* ************************************************************************* */

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

void  ill_init(const void *elem, size_t offset);
void  ill_init_node(t_cm_ill *node, size_t offset);
void *ill_copy(const t_cm_ill *head, t_cm_duplicator d, t_cm_dtor dtor);

void     *ill_ntoe(const t_cm_ill *node);
t_cm_ill *ill_eton(const void *e, size_t offset);

bool ill_is_isolated(const void *elem, size_t offset);
bool ill_is_isolated_node(const t_cm_ill *node);
bool ill_is_circular(const t_cm_ill *head);
bool ill_is_sorted(const t_cm_ill *head, t_cm_comparator comparator);
bool ill_is_empty(const t_cm_ill *head);
bool ill_has(const t_cm_ill *head, t_cm_callback matcher, void *arg);
bool ill_validate(const t_cm_ill *head);

void     *ill_push(t_cm_ill **head, const void *elem, size_t offset);
t_cm_ill *ill_push_node(t_cm_ill **head, t_cm_ill *node);
void     *ill_npush(t_cm_ill **head, const void *elems, size_t offset);
t_cm_ill *ill_npush_node(t_cm_ill **head, t_cm_ill *nodes);
void     *ill_push_back(t_cm_ill **head, const void *elem, size_t offset);
t_cm_ill *ill_push_back_node(t_cm_ill **head, t_cm_ill *node);

void ill_append(const void *anchor_elem, const void *elem, size_t offset);
void ill_append_node(t_cm_ill *anchor, t_cm_ill *node);
void ill_nappend(const void *anchor_elem, const void *elems, size_t offset);
void ill_nappend_node(t_cm_ill *anchor, t_cm_ill *nodes);

bool ill_move(t_cm_ill **from, t_cm_ill **to, const void *first,
              const void *last, size_t offset);
bool ill_move_node(t_cm_ill **from, t_cm_ill **to, t_cm_ill *first,
                   t_cm_ill *last);

void     *ill_pop(t_cm_ill **head);
t_cm_ill *ill_pop_node(t_cm_ill **head);
void     *ill_pop_back(t_cm_ill **head);
t_cm_ill *ill_pop_back_node(t_cm_ill **head);

void     *ill_remove_after(const void *prev_elem, size_t offset);
t_cm_ill *ill_remove_after_node(t_cm_ill *prev);
bool      ill_remove(t_cm_ill **head, const void *elem, size_t offset);
bool      ill_remove_node(t_cm_ill **head, t_cm_ill *target);
void      ill_clear(t_cm_ill **head, t_cm_dtor dtor);

void     *ill_find_prev(const t_cm_ill *head, const void *elem, size_t offset);
t_cm_ill *ill_find_prev_node(const t_cm_ill *head, const t_cm_ill *target);
void     *ill_find(const t_cm_ill *head, const void *elem, size_t offset);
t_cm_ill *ill_find_node(const t_cm_ill *head, const t_cm_ill *target);
void *ill_get_prev(const t_cm_ill *head, t_cm_comparator c, const void *arg);
t_cm_ill *ill_get_prev_node(const t_cm_ill *head, t_cm_comparator c,
                            const void *arg);
void     *ill_get(const t_cm_ill *head, t_cm_comparator c, const void *arg);
t_cm_ill *ill_get_node(const t_cm_ill *head, t_cm_comparator c,
                       const void *arg);
void     *ill_get_tail(const void *elem, size_t offset);
t_cm_ill *ill_get_tail_node(const t_cm_ill *head);

void     *ill_get_min(const t_cm_ill *head, t_cm_comparator comp);
t_cm_ill *ill_get_min_node(const t_cm_ill *head, t_cm_comparator comp);
void     *ill_get_max(const t_cm_ill *head, t_cm_comparator comp);
t_cm_ill *ill_get_max_node(const t_cm_ill *head, t_cm_comparator comp);
void     *ill_get_kth(const t_cm_ill *head, size_t k);
t_cm_ill *ill_get_kth_node(const t_cm_ill *head, size_t k);

size_t ill_length(const t_cm_ill *head);
size_t ill_nlength(const t_cm_ill *head, size_t n);
size_t ill_match_nb(const t_cm_ill *head, t_cm_callback matcher, void *arg);

void *ill_apply(const t_cm_ill *head, t_cm_callback c, void *u_data);
void *ill_napply(const t_cm_ill *head, size_t n, t_cm_callback c, void *u_data);
void *ill_apply_it(const t_cm_ill *head, t_cm_callback_it func, void *u_data);
void *ill_napply_it(const t_cm_ill *head, size_t n, t_cm_callback_it func,
                    void *u_data);
void *ill_apply_range(const void *head_elem, const void *end_elem,
                      t_cm_callback c, void *u_data, size_t offset);

void *ill_fold(const t_cm_ill *head, t_cm_merger m, void *u_start);
void *ill_nfold(const t_cm_ill *head, size_t n, t_cm_merger m, void *u_start);
void *ill_reduce(const t_cm_ill *head, t_cm_merger m, size_t elem_size);
void *ill_nreduce(const t_cm_ill *head, size_t n, t_cm_merger m,
                  size_t elem_size);

bool  ill_iter(const t_cm_ill *head, void **iter, size_t offset);
bool  ill_iter_range(const void *head_elem, const void *end_elem, void **iter,
                     size_t offset);
void *ill_advance_it(const void *e, size_t offset);
t_cm_ill *ill_advance_it_node(const t_cm_ill *it);

void      ill_reverse(t_cm_ill **head);
t_cm_ill *ill_sorted_merge(t_cm_ill *list1, t_cm_ill *list2,
                           t_cm_comparator comp);
t_cm_ill *ill_merge(t_cm_ill *list1, t_cm_ill *list2, t_cm_comparator comp);
void      ill_sort(t_cm_ill **head, t_cm_comparator comp);

void     *ill_concat(const void *list1, const void *list2, size_t offset);
t_cm_ill *ill_concat_node(t_cm_ill *list1, t_cm_ill *list2);

size_t ill_filter(t_cm_ill **head, t_cm_callback cb, void *arg,
                  t_cm_ill **filtered);
size_t ill_filter_out(t_cm_ill **head, t_cm_callback cb, void *arg,
                      t_cm_ill **filtered);
size_t ill_nfilter(t_cm_ill **head, size_t n, t_cm_callback cb, void *arg,
                   t_cm_ill **filtered);
size_t ill_nfilter_out(t_cm_ill **head, size_t n, t_cm_callback cb, void *arg,
                       t_cm_ill **filtered);
size_t ill_filter_dt(t_cm_ill **head, t_cm_callback cb, void *arg, t_cm_dtor d);
void   ill_unique(t_cm_ill *head, t_cm_comparator c, t_cm_dtor d);

/*
 * circular api: the list handle is the last node, head->next is the first.
 * the handle is not a sentinel; it is a real payload node.
 */
size_t    ill_circ_length(const t_cm_ill *head);
t_cm_ill *ill_circ_get_head_node(const t_cm_ill *head);
void     *ill_circ_get_head(const void *head_elem, size_t offset);
t_cm_ill *ill_circ_get_tail_node(const t_cm_ill *head);
void     *ill_circ_get_tail(const void *head_elem, size_t offset);

t_cm_ill *ill_circ_push_node(t_cm_ill **head, t_cm_ill *node);
void     *ill_circ_push(t_cm_ill **head, const void *elem, size_t offset);
t_cm_ill *ill_circ_push_back_node(t_cm_ill **head, t_cm_ill *node);
void     *ill_circ_push_back(t_cm_ill **head, const void *elem, size_t offset);
t_cm_ill *ill_circ_pop_node(t_cm_ill **head);
void     *ill_circ_pop(t_cm_ill **head);
t_cm_ill *ill_circ_pop_back_node(t_cm_ill **head);
void     *ill_circ_pop_back(t_cm_ill **head);

t_cm_ill *ill_circ_concat_node(t_cm_ill *list1, t_cm_ill *list2);
void     *ill_circ_concat(const void *list1, const void *list2, size_t offset);

bool ill_circ_remove_node(t_cm_ill **head, t_cm_ill *target);
bool ill_circ_remove(t_cm_ill **head, const void *elem, size_t offset);
void ill_circ_clear(t_cm_ill **head, t_cm_dtor dtor);

void *ill_circ_foreach(const t_cm_ill *head, t_cm_callback c, void *u_data);
bool  ill_circ_iter(const t_cm_ill *head, void **iter, size_t offset);

#ifdef C_MINI__ILL_INTERNAL

t_cm_ill *illi_push_back_node(t_cm_ill **head, t_cm_ill **tail, t_cm_ill *node);
t_cm_ill *illi_pop_back_node(t_cm_ill **head, t_cm_ill **tail);
t_cm_ill *illi_concat_node(t_cm_ill **h1, t_cm_ill **t1, t_cm_ill **h2,
                           t_cm_ill **t2);
t_cm_ill *illi_remove_node(t_cm_ill **head, t_cm_ill **tail, t_cm_ill *target);

#endif // !C_MINI__ILL_INTERNAL

#endif // !C_MINI__ILL
