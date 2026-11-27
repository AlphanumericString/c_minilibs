#ifndef C_MINI__LH
#define C_MINI__LH

#include <stdbool.h>
#include <stddef.h>

#include "common_base/fptr.h"
#include "common_base/log.h"
#include "ill/idl/intrusive_doubly_linked.h"
#include "ill/ill/intrusive_linked_lists.h"

#ifdef C_MINI__TESTS

#	define C_MINI__LH_LOG_ECB     C_MINI__LOG_F_PREFIX "Callback failed."
#	define C_MINI__LH_LOG_EARG    C_MINI__LOG_EARG
#	define C_MINI__LH_LOG_EALLOC  C_MINI__LOG_EALLOC
#	define C_MINI__LH_LOG_FAIL    C_MINI__LOG_FAIL
#	define C_MINI__LH_LOG_SUCCESS C_MINI__LOG_SUCCESS

#endif // !C_MINI__TESTS

/**
 * \defgroup lh List Handler
 * \brief a facade over the intrusive singly, doubly and circular list types.
**/

/**
 * \enum e_cm_lh_type
 * \brief the underlying list type selected by a handler.
 * \ingroup lh
**/
typedef enum e_cm_lh_type {
	CM_LH_SINGLY = 0,
	CM_LH_DOUBLY,
	CM_LH_CIRC_SINGLY,
	CM_LH_CIRC_DOUBLY,
} t_cm_lh_type;

/**
 * \typedef t_cm_lh
 * \brief a list handler that caches the first and last nodes of an intrusive
 * list and dispatches to the selected list implementation.
 * \param head the first node of the list, or NULL when empty
 * \param tail the last node of the list, or NULL when empty
 * \param offset offset of the intrusive node inside the user payload
 * \param type underlying list type
 * \note the handler does not allocate nodes: the payloads remain intrusive and
 * must embed a t_cm_ill or t_cm_idl at the same offset.
 * \note the handler is the sole mutator of the list. mutating the nodes behind
 * its back desynchronizes the cached head and tail.
 * \note a future extension may cache a length and/or a user tag; the current
 * version only caches head and tail.
 * \ingroup lh
**/
typedef struct s_cm_lh {
	void        *head;
	void        *tail;
	size_t       offset;
	t_cm_lh_type type;
} t_cm_lh;

t_cm_lh *lh_init(t_cm_lh *lh, t_cm_lh_type type, size_t offset);
t_cm_lh *lh_create(t_cm_lh_type type, size_t offset);
void     lh_destroy(t_cm_lh *lh);
void     lh_free(t_cm_lh **lh);

bool   lh_is_valid(const t_cm_lh *lh);
bool   lh_is_empty(const t_cm_lh *lh);
size_t lh_length(const t_cm_lh *lh);
void  *lh_get_head(const t_cm_lh *lh);
void  *lh_get_tail(const t_cm_lh *lh);

void *lh_push(t_cm_lh *lh, const void *elem);
void *lh_push_back(t_cm_lh *lh, const void *elem);
void *lh_pop(t_cm_lh *lh);
void *lh_pop_back(t_cm_lh *lh);
bool  lh_remove(t_cm_lh *lh, const void *elem);
void  lh_clear(t_cm_lh *lh, t_cm_dtor dtor);
bool  lh_concat(t_cm_lh *dst, t_cm_lh *src);

void *lh_foreach(const t_cm_lh *lh, t_cm_callback c, void *u_data);
bool  lh_iter(const t_cm_lh *lh, void **iter);

#endif // !C_MINI__LH
