#include "list_handler.h"

/**
 * \brief Applies the callback to every element of the handled list.
 * \param lh The handler.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \note stops early if the callback returns false.
 * \ingroup lh
**/
void *lh_foreach(const t_cm_lh *lh, t_cm_callback c, void *u_data) {
	if (!lh || !lh->head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (u_data);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		return (ill_apply(lh->head, c, u_data));
	case CM_LH_DOUBLY:
		return (idl_apply(lh->head, c, u_data));
	case CM_LH_CIRC_SINGLY:
		return (ill_circ_foreach(lh->tail, c, u_data));
	case CM_LH_CIRC_DOUBLY:
		return (idl_circ_foreach(lh->head, c, u_data));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (u_data);
	}
}

/**
 * \brief Pull iterator over the elements of the handled list.
 * \param lh The handler.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \return true while an element is available, false once the list is
 * exhausted or on invalid arguments.
 * \ingroup lh
**/
bool lh_iter(const t_cm_lh *lh, void **iter) {
	if (!lh || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		return (ill_iter(lh->head, iter, lh->offset));
	case CM_LH_DOUBLY:
		return (idl_iter(lh->head, iter, lh->offset));
	case CM_LH_CIRC_SINGLY:
		return (ill_circ_iter(lh->tail, iter, lh->offset));
	case CM_LH_CIRC_DOUBLY:
		return (idl_circ_iter(lh->head, iter, lh->offset));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
}
