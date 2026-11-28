#define C_MINI__IDL_INTERNAL
#define C_MINI__ILL_INTERNAL
#include "list_handler.h"

/**
 * \brief Inserts an element at the front of the handled list.
 * \param lh The handler.
 * \param elem Pointer to the element to insert.
 * \return The new first element, or NULL on failure.
 * \ingroup lh
**/
void *lh_push(t_cm_lh *lh, const void *elem) {
	t_cm_ill *ill;
	t_cm_idl *idl;
	t_cm_ill *ihead;
	t_cm_idl *dhead;

	if (!lh || !elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		ihead = lh->head;
		ill   = ill_eton(elem, lh->offset);
		ill_push_node(&ihead, ill);
		lh->head = ihead;
		if (!lh->tail) lh->tail = ill;
		return (ill_ntoe(lh->head));
	case CM_LH_DOUBLY:
		dhead = lh->head;
		idl   = idl_eton(elem, lh->offset);
		idl_push_node(&dhead, idl);
		lh->head = dhead;
		if (!lh->tail) lh->tail = idl;
		return (idl_ntoe(lh->head));
	case CM_LH_CIRC_SINGLY:
		ihead = lh->tail;
		ill   = ill_eton(elem, lh->offset);
		ill_circ_push_node(&ihead, ill);
		lh->tail = ihead;
		lh->head = ihead->next;
		return (ill_ntoe(lh->head));
	case CM_LH_CIRC_DOUBLY:
		dhead = lh->head;
		idl   = idl_eton(elem, lh->offset);
		idl_circ_push_node(&dhead, idl);
		lh->head = dhead;
		lh->tail = idl_circ_get_tail_node(dhead);
		return (idl_ntoe(lh->head));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}

/**
 * \brief Inserts an element at the back of the handled list.
 * \param lh The handler.
 * \param elem Pointer to the element to insert.
 * \return The first element of the list, or NULL on failure.
 * \ingroup lh
**/
void *lh_push_back(t_cm_lh *lh, const void *elem) {
	t_cm_ill *ill;
	t_cm_idl *idl;
	t_cm_ill *ihead;
	t_cm_ill *itail;
	t_cm_idl *dhead;
	t_cm_idl *dtail;

	if (!lh || !elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		ihead = lh->head;
		itail = lh->tail;
		ill   = ill_eton(elem, lh->offset);
		illi_push_back_node(&ihead, &itail, ill);
		lh->head = ihead;
		lh->tail = itail;
		return (ill_ntoe(lh->head));
	case CM_LH_DOUBLY:
		dhead = lh->head;
		dtail = lh->tail;
		idl   = idl_eton(elem, lh->offset);
		idli_push_back_node(&dhead, &dtail, idl);
		lh->head = dhead;
		lh->tail = dtail;
		return (idl_ntoe(lh->head));
	case CM_LH_CIRC_SINGLY:
		ihead = lh->tail;
		ill   = ill_eton(elem, lh->offset);
		ill_circ_push_back_node(&ihead, ill);
		lh->tail = ihead;
		lh->head = ihead->next;
		return (ill_ntoe(lh->head));
	case CM_LH_CIRC_DOUBLY:
		dhead = lh->head;
		idl   = idl_eton(elem, lh->offset);
		idl_circ_push_back_node(&dhead, idl);
		lh->head = dhead;
		lh->tail = idl_circ_get_tail_node(dhead);
		return (idl_ntoe(lh->head));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}

/**
 * \brief Removes and returns the first element of the handled list.
 * \param lh The handler.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup lh
**/
void *lh_pop(t_cm_lh *lh) {
	const t_cm_ill *ill;
	const t_cm_idl *idl;
	t_cm_ill       *ihead;
	t_cm_idl       *dhead;

	if (!lh || !lh->head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		ihead    = lh->head;
		ill      = ill_pop_node(&ihead);
		lh->head = ihead;
		if (!ihead) lh->tail = NULL;
		return (ill_ntoe(ill));
	case CM_LH_DOUBLY:
		dhead    = lh->head;
		idl      = idl_pop_node(&dhead);
		lh->head = dhead;
		if (!dhead) lh->tail = NULL;
		return (idl_ntoe(idl));
	case CM_LH_CIRC_SINGLY:
		ihead    = lh->tail;
		ill      = ill_circ_pop_node(&ihead);
		lh->tail = ihead;
		lh->head = ihead ? ihead->next : NULL;
		return (ill_ntoe(ill));
	case CM_LH_CIRC_DOUBLY:
		dhead    = lh->head;
		idl      = idl_circ_pop_node(&dhead);
		lh->head = dhead;
		lh->tail = idl_circ_get_tail_node(dhead);
		return (idl_ntoe(idl));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}

/**
 * \brief Removes and returns the last element of the handled list.
 * \param lh The handler.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup lh
**/
void *lh_pop_back(t_cm_lh *lh) {
	const t_cm_ill *ill;
	const t_cm_idl *idl;
	t_cm_ill       *ihead;
	t_cm_ill       *itail;
	t_cm_idl       *dhead;
	t_cm_idl       *dtail;

	if (!lh || !lh->tail) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		ihead    = lh->head;
		itail    = lh->tail;
		ill      = illi_pop_back_node(&ihead, &itail);
		lh->head = ihead;
		lh->tail = itail;
		return (ill_ntoe(ill));
	case CM_LH_DOUBLY:
		dhead    = lh->head;
		dtail    = lh->tail;
		idl      = idli_pop_back_node(&dhead, &dtail);
		lh->head = dhead;
		lh->tail = dtail;
		return (idl_ntoe(idl));
	case CM_LH_CIRC_SINGLY:
		ihead    = lh->tail;
		ill      = ill_circ_pop_back_node(&ihead);
		lh->tail = ihead;
		lh->head = ihead ? ihead->next : NULL;
		return (ill_ntoe(ill));
	case CM_LH_CIRC_DOUBLY:
		dhead    = lh->head;
		idl      = idl_circ_pop_back_node(&dhead);
		lh->head = dhead;
		lh->tail = idl_circ_get_tail_node(dhead);
		return (idl_ntoe(idl));
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
}

/**
 * \brief Removes a specific element from the handled list.
 * \param lh The handler.
 * \param elem Pointer to the element to remove.
 * \return true if the element was removed, false otherwise.
 * \ingroup lh
**/
bool lh_remove(t_cm_lh *lh, const void *elem) {
	t_cm_ill *ill;
	t_cm_idl *idl;

	if (!lh || !elem || !lh->head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;
		t_cm_ill *tail = lh->tail;

		ill = ill_eton(elem, lh->offset);
		if (!illi_remove_node(&head, &tail, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		lh->head = head;
		lh->tail = tail;
		return (true);
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;
		t_cm_idl *tail = lh->tail;

		idl = idl_eton(elem, lh->offset);
		if (!idli_remove_node(&head, &tail, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		lh->head = head;
		lh->tail = tail;
		return (true);
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill = ill_eton(elem, lh->offset);
		if (!ill_circ_remove_node(&raw, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		lh->tail = raw;
		lh->head = raw ? raw->next : NULL;
		return (true);
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl = idl_eton(elem, lh->offset);
		if (!idl_circ_remove_node(&head, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		lh->head = head;
		lh->tail = idl_circ_get_tail_node(head);
		return (true);
	}
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
}

/**
 * \brief Removes every node from the handled list.
 * \param lh The handler.
 * \param dtor Optional destructor called on every element.
 * \ingroup lh
**/
void lh_clear(t_cm_lh *lh, t_cm_dtor dtor) {
	t_cm_ill *ihead;
	t_cm_idl *dhead;

	if (!lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return;
	}
	switch (lh->type) {
	case CM_LH_SINGLY:
		ihead = lh->head;
		ill_clear(&ihead, dtor);
		break;
	case CM_LH_DOUBLY:
		dhead = lh->head;
		idl_clear(&dhead, dtor);
		break;
	case CM_LH_CIRC_SINGLY:
		ihead = lh->tail;
		ill_circ_clear(&ihead, dtor);
		break;
	case CM_LH_CIRC_DOUBLY:
		dhead = lh->head;
		idl_circ_clear(&dhead, dtor);
		break;
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return;
	}
	lh->head = NULL;
	lh->tail = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
}

/**
 * \brief Concatenates two handlers of the same type and offset.
 * \param dst The destination handler.
 * \param src The source handler; it is emptied on success.
 * \return true if the lists were concatenated, false otherwise.
 * \ingroup lh
**/
bool lh_concat(t_cm_lh *dst, t_cm_lh *src) {
	t_cm_ill *ih1;
	t_cm_ill *it1;
	t_cm_ill *ih2;
	t_cm_ill *it2;
	t_cm_idl *dh1;
	t_cm_idl *dt1;
	t_cm_idl *dh2;
	t_cm_idl *dt2;

	if (!dst || !src || dst == src) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	if (dst->type != src->type || dst->offset != src->offset) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	if (!src->head) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
		return (true);
	}
	switch (dst->type) {
	case CM_LH_SINGLY:
		ih1 = dst->head;
		it1 = dst->tail;
		ih2 = src->head;
		it2 = src->tail;
		illi_concat_node(&ih1, &it1, &ih2, &it2);
		dst->head = ih1;
		dst->tail = it1;
		break;
	case CM_LH_DOUBLY:
		dh1 = dst->head;
		dt1 = dst->tail;
		dh2 = src->head;
		dt2 = src->tail;
		idli_concat_node(&dh1, &dt1, &dh2, &dt2);
		dst->head = dh1;
		dst->tail = dt1;
		break;
	case CM_LH_CIRC_SINGLY:
		ih1       = ill_circ_concat_node(dst->tail, src->tail);
		dst->tail = ih1;
		dst->head = ih1->next;
		break;
	case CM_LH_CIRC_DOUBLY:
		dh1       = idl_circ_concat_node(dst->head, src->head);
		dst->head = dh1;
		dst->tail = idl_circ_get_tail_node(dh1);
		break;
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	src->head = NULL;
	src->tail = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
	return (true);
}
