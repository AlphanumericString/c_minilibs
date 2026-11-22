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
/* the circular list api is reserved under the ill_circ_ prefix and is not   */
/* implemented yet.                                                          */
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

void  ill_init(void *elem, size_t offset);
void  ill_init_node(t_cm_ill *node, size_t offset);
void *ill_copy(const t_cm_ill *head, t_cm_duplicator d, t_cm_dtor dtor);

void     *ill_ntoe(const t_cm_ill *node);
t_cm_ill *ill_eton(const void *e, size_t offset);

bool ill_is_isolated(const void *elem, size_t offset);
bool ill_is_isolated_node(const t_cm_ill *node);
bool ill_is_circular(const t_cm_ill *head);
bool ill_is_sorted(const t_cm_ill *head, t_cm_comparator comparator);
bool ill_has(const t_cm_ill *head, t_cm_callback matcher, void *arg);

void     *ill_push(t_cm_ill **head, void *elem, size_t offset);
t_cm_ill *ill_push_node(t_cm_ill **head, t_cm_ill *node);
void     *ill_npush(t_cm_ill **head, void *elems, size_t offset);
t_cm_ill *ill_npush_node(t_cm_ill **head, t_cm_ill *nodes);
void     *ill_push_back(t_cm_ill **head, void *elem, size_t offset);
t_cm_ill *ill_push_back_node(t_cm_ill **head, t_cm_ill *node);
void      ill_append(void *anchor_elem, void *elem, size_t offset);
void      ill_append_node(t_cm_ill *anchor, t_cm_ill *node);
void      ill_nappend(void *anchor_elem, void *elems, size_t offset);
void      ill_nappend_node(t_cm_ill *anchor, t_cm_ill *nodes);

void     *ill_pop(t_cm_ill **head);
t_cm_ill *ill_pop_node(t_cm_ill **head);
void     *ill_pop_back(t_cm_ill **head);
t_cm_ill *ill_pop_back_node(t_cm_ill **head);
void     *ill_remove_after(void *prev_elem, size_t offset);
t_cm_ill *ill_remove_after_node(t_cm_ill *prev);
bool      ill_remove(t_cm_ill **head, const void *elem, size_t offset);
bool      ill_remove_node(t_cm_ill **head, t_cm_ill *target);

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
void     *ill_get_tail(const t_cm_ill *head);
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

void     *ill_advance_it(const void *e, size_t offset);
t_cm_ill *ill_advance_it_node(const t_cm_ill *it);
bool ill_for(const t_cm_ill *head, size_t n, t_cm_callback c, void *u_data);
bool ill_foreach(const t_cm_ill *head, t_cm_callback c, void *u_data);

void      ill_reverse(t_cm_ill **head);
t_cm_ill *ill_sorted_merge(t_cm_ill *list1, t_cm_ill *list2,
                           t_cm_comparator comp);
void      ill_sort(t_cm_ill **head, t_cm_comparator comp);

size_t ill_filter(t_cm_ill **head, t_cm_callback cb, void *arg,
                  t_cm_ill **filtered);
size_t ill_filter_out(t_cm_ill **head, t_cm_callback cb, void *arg,
                      t_cm_ill **filtered);
size_t ill_rm_if(t_cm_ill **head, t_cm_callback cb, void *arg, t_cm_dtor d);
void   ill_unique(t_cm_ill *head, t_cm_comparator c, t_cm_dtor d);

#endif // !C_MINI__ILL
