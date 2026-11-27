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

	if (!lh || !elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;

		ill = ill_eton(elem, lh->offset);
		if (!ill_push_node(&head, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		if (!lh->tail) lh->tail = ill;
		return (ill_ntoe(lh->head));
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl = idl_eton(elem, lh->offset);
		if (!idl_push_node(&head, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		if (!lh->tail) lh->tail = idl;
		return (idl_ntoe(lh->head));
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill = ill_eton(elem, lh->offset);
		if (!ill_circ_push_node(&raw, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->tail = raw;
		lh->head = raw->next;
		return (ill_ntoe(lh->head));
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl = idl_eton(elem, lh->offset);
		if (!idl_circ_push_node(&head, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		lh->tail = idl_circ_get_tail_node(head);
		return (idl_ntoe(lh->head));
	}
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

	if (!lh || !elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;
		t_cm_ill *tail = lh->tail;

		ill = ill_eton(elem, lh->offset);
		if (!illi_push_back_node(&head, &tail, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		lh->tail = tail;
		return (ill_ntoe(lh->head));
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;
		t_cm_idl *tail = lh->tail;

		idl = idl_eton(elem, lh->offset);
		if (!idli_push_back_node(&head, &tail, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		lh->tail = tail;
		return (idl_ntoe(lh->head));
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill = ill_eton(elem, lh->offset);
		if (!ill_circ_push_back_node(&raw, ill)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->tail = raw;
		lh->head = raw->next;
		return (ill_ntoe(lh->head));
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl = idl_eton(elem, lh->offset);
		if (!idl_circ_push_back_node(&head, idl)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (NULL);
		}
		lh->head = head;
		lh->tail = idl_circ_get_tail_node(head);
		return (idl_ntoe(lh->head));
	}
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

	if (!lh || !lh->head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;

		ill      = ill_pop_node(&head);
		lh->head = head;
		if (!head) lh->tail = NULL;
		return (ill ? ill_ntoe(ill) : NULL);
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl      = idl_pop_node(&head);
		lh->head = head;
		if (!head) lh->tail = NULL;
		return (idl ? idl_ntoe(idl) : NULL);
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill      = ill_circ_pop_node(&raw);
		lh->tail = raw;
		lh->head = raw ? raw->next : NULL;
		return (ill ? ill_ntoe(ill) : NULL);
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl      = idl_circ_pop_node(&head);
		lh->head = head;
		lh->tail = idl_circ_get_tail_node(head);
		return (idl ? idl_ntoe(idl) : NULL);
	}
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

	if (!lh || !lh->tail) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;
		t_cm_ill *tail = lh->tail;

		ill      = illi_pop_back_node(&head, &tail);
		lh->head = head;
		lh->tail = tail;
		return (ill ? ill_ntoe(ill) : NULL);
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;
		t_cm_idl *tail = lh->tail;

		idl      = idli_pop_back_node(&head, &tail);
		lh->head = head;
		lh->tail = tail;
		return (idl ? idl_ntoe(idl) : NULL);
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill      = ill_circ_pop_back_node(&raw);
		lh->tail = raw;
		lh->head = raw ? raw->next : NULL;
		return (ill ? ill_ntoe(ill) : NULL);
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl      = idl_circ_pop_back_node(&head);
		lh->head = head;
		lh->tail = idl_circ_get_tail_node(head);
		return (idl ? idl_ntoe(idl) : NULL);
	}
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
	if (!lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return;
	}
	switch (lh->type) {
	case CM_LH_SINGLY: {
		t_cm_ill *head = lh->head;

		ill_clear(&head, dtor);
		break;
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl_clear(&head, dtor);
		break;
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = lh->tail;

		ill_circ_clear(&raw, dtor);
		break;
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = lh->head;

		idl_circ_clear(&head, dtor);
		break;
	}
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
	case CM_LH_SINGLY: {
		t_cm_ill *h1 = dst->head;
		t_cm_ill *t1 = dst->tail;
		t_cm_ill *h2 = src->head;
		t_cm_ill *t2 = src->tail;

		if (!illi_concat_node(&h1, &t1, &h2, &t2)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		dst->head = h1;
		dst->tail = t1;
		break;
	}
	case CM_LH_DOUBLY: {
		t_cm_idl *h1 = dst->head;
		t_cm_idl *t1 = dst->tail;
		t_cm_idl *h2 = src->head;
		t_cm_idl *t2 = src->tail;

		if (!idli_concat_node(&h1, &t1, &h2, &t2)) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_FAIL)
			return (false);
		}
		dst->head = h1;
		dst->tail = t1;
		break;
	}
	case CM_LH_CIRC_SINGLY: {
		t_cm_ill *raw = ill_circ_concat_node(dst->tail, src->tail);

		dst->tail = raw;
		dst->head = raw ? raw->next : NULL;
		break;
	}
	case CM_LH_CIRC_DOUBLY: {
		t_cm_idl *head = idl_circ_concat_node(dst->head, src->head);

		dst->head = head;
		dst->tail = idl_circ_get_tail_node(head);
		break;
	}
	default:
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (false);
	}
	src->head = NULL;
	src->tail = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
	return (true);
}
