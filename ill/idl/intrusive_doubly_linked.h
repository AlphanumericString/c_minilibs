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
 * \typedef t_idl
 * \brief t_idl struct represents an intrusive doubly linked list node
 * \param offset offset of the node in the element
 * \param next pointer to the next node
 * \param prev pointer to the previous node
 * \ingroup idl
**/
typedef struct s_idl {
	struct s_idl *prev;
	struct s_idl *next;
	size_t        offset;
} t_idl;

void   idl_init(t_idl *node, size_t offset);
void  *idl_copy(const t_idl *head, t_cm_duplicator d);
void  *idl_ntoe(const t_idl *node);
t_idl *idl_eton(void *e, size_t offset);
bool   idl_is_isolated(t_idl *node);
bool   idl_is_circular(t_idl *head);
bool   idl_is_sorted(t_idl *head, t_cm_comparator comparator);
bool   idl_has(t_idl *head, t_cm_callback matcher, void *arg);
void   idl_append(t_idl *anchor, t_idl *node);
void   idl_nappend(t_idl *anchor, t_idl *nodes);
void   idl_prepend(t_idl *anchor, t_idl *node);
void   idl_nprepend(t_idl *anchor, t_idl *nodes);
t_idl *idl_push(t_idl **head, t_idl *node);
t_idl *idl_npush(t_idl **head, t_idl *nodes);
t_idl *idl_push_back(t_idl **head, t_idl *node);
t_idl *idl_pop(t_idl **head);
t_idl *idl_pop_back(t_idl **head);
t_idl *idl_remove_after(t_idl *prev);
bool   idl_remove_node(t_idl **head, t_idl *target);
t_idl *idl_find_prev(t_idl *head, t_idl *target);
t_idl *idl_find(t_idl *head, t_idl *target);
t_idl *idl_get_prev(t_idl *head, t_cm_comparator c, void *arg);
t_idl *idl_get(t_idl *head, t_cm_comparator c, void *arg);
t_idl *idl_get_tail(t_idl *node);
t_idl *idl_get_head(t_idl *node);
void  *idl_get_min(t_idl *head, t_cm_comparator c);
void  *idl_get_max(t_idl *head, t_cm_comparator c);
void  *idl_get_kth(t_idl *head, size_t k);
size_t idl_length(t_idl *head);
size_t idl_nlength(t_idl *head, size_t n);
size_t idl_match_nb(t_idl *head, t_cm_callback matcher, void *arg);
bool   idl_for(t_idl *head, size_t n, t_cm_callback c, void *u_data);
bool   idl_foreach(t_idl *head, t_cm_callback c, void *u_data);
t_idl *idl_reverse(t_idl *head);

#endif // !C_MINI__IDL
