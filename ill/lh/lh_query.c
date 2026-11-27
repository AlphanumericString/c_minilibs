#include "list_handler.h"

/**
 * \brief Checks the cached head/tail invariants of a handler.
 * \param lh The handler to check.
 * \return true when the handler is usable, false otherwise.
 * \note this is an O(1) check. it does not walk the list.
 * \ingroup lh
**/
bool lh_is_valid(const t_cm_lh *lh) {
	if (!lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	if (lh->type != CM_LH_SINGLY && lh->type != CM_LH_DOUBLY &&
	    lh->type != CM_LH_CIRC_SINGLY && lh->type != CM_LH_CIRC_DOUBLY) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	if (!lh->head || !lh->tail) return (lh->head == NULL && lh->tail == NULL);
	switch (lh->type) {
	case CM_LH_SINGLY: {
		const t_cm_ill *tail = lh->tail;

		return (tail->next == NULL);
	}
	case CM_LH_DOUBLY: {
		const t_cm_idl *head = lh->head;
		const t_cm_idl *tail = lh->tail;

		return (head->prev == NULL && tail->next == NULL);
	}
	case CM_LH_CIRC_SINGLY: {
		const t_cm_ill *tail = lh->tail;

		return (tail->next == lh->head);
	}
	case CM_LH_CIRC_DOUBLY: {
		const t_cm_idl *head = lh->head;
		const t_cm_idl *tail = lh->tail;

		return (head->prev == tail && tail->next == head);
	}
	default:
		return (false);
	}
}

/**
 * \brief Checks if a handler is empty.
 * \param lh The handler to check.
 * \return true if the handler is NULL or holds no node.
 * \ingroup lh
**/
bool lh_is_empty(const t_cm_lh *lh) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
	return (!lh || !lh->head);
}

/**
 * \brief Calculates the length of the handled list.
 * \param lh The handler.
 * \return The number of nodes in the list.
 * \ingroup lh
**/
size_t lh_length(const t_cm_lh *lh) {
	if (!lh || !lh->head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (0);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		return (ill_length(lh->head));
	case CM_LH_DOUBLY:
		return (idl_length(lh->head));
	case CM_LH_CIRC_SINGLY:
		return (ill_circ_length(lh->tail));
	case CM_LH_CIRC_DOUBLY:
		return (idl_circ_length(lh->head));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (0);
	}
}

/**
 * \brief Retrieves the first element of the handled list.
 * \param lh The handler.
 * \return The first element, or NULL if the list is empty.
 * \ingroup lh
**/
void *lh_get_head(const t_cm_lh *lh) {
	if (!lh || !lh->head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
	case CM_LH_CIRC_SINGLY:
		return (ill_ntoe(lh->head));
	case CM_LH_DOUBLY:
	case CM_LH_CIRC_DOUBLY:
		return (idl_ntoe(lh->head));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}

/**
 * \brief Retrieves the last element of the handled list.
 * \param lh The handler.
 * \return The last element, or NULL if the list is empty.
 * \ingroup lh
**/
void *lh_get_tail(const t_cm_lh *lh) {
	if (!lh || !lh->tail) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
	case CM_LH_CIRC_SINGLY:
		return (ill_ntoe(lh->tail));
	case CM_LH_DOUBLY:
	case CM_LH_CIRC_DOUBLY:
		return (idl_ntoe(lh->tail));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}
