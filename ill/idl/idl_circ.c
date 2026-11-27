#include "intrusive_doubly_linked.h"

/**
 * \brief Calculates the length of a circular list.
 * \param head The first node of the list, or NULL.
 * \return The number of nodes in the list.
 * \ingroup idl
**/
size_t idl_circ_length(const t_cm_idl *head) {
	const t_cm_idl *curr;
	size_t          len = 0;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (0);
	}
	curr = head;
	do {
		len++;
		curr = curr->next;
	} while (curr != head);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (len);
}

/**
 * \brief Retrieves the first node of a circular list.
 * \param head The first node of the list.
 * \return A pointer to the first node, or NULL if the list is empty.
 * \ingroup idl
**/
t_cm_idl *idl_circ_get_head_node(const t_cm_idl *head) {
	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return ((t_cm_idl *)head);
}

/**
 * \brief Retrieves the first element of a circular list.
 * \param head_elem The first element of the list.
 * \param offset Offset of the node in the element.
 * \return A pointer to the first element, or NULL if the list is empty.
 * \ingroup idl
**/
void *idl_circ_get_head(const void *head_elem, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_circ_get_head_node(idl_eton(head_elem, offset));
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Retrieves the last node of a circular list.
 * \param head The first node of the list.
 * \return A pointer to the last node, or NULL if the list is empty.
 * \ingroup idl
**/
t_cm_idl *idl_circ_get_tail_node(const t_cm_idl *head) {
	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return ((t_cm_idl *)head->prev);
}

/**
 * \brief Retrieves the last element of a circular list.
 * \param head_elem The first element of the list.
 * \param offset Offset of the node in the element.
 * \return A pointer to the last element, or NULL if the list is empty.
 * \ingroup idl
**/
void *idl_circ_get_tail(const void *head_elem, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_circ_get_tail_node(idl_eton(head_elem, offset));
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Inserts a node at the front of a circular list.
 * \param head Pointer to the first node of the list.
 * \param node The node to insert.
 * \return The new first node, or NULL on failure.
 * \ingroup idl
**/
t_cm_idl *idl_circ_push_node(t_cm_idl **head, t_cm_idl *node) {
	t_cm_idl *tail;

	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->prev = node;
		node->next = node;
		*head      = node;
	} else {
		tail          = (*head)->prev;
		node->next    = *head;
		node->prev    = tail;
		tail->next    = node;
		(*head)->prev = node;
		*head         = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief Inserts an element at the front of a circular list.
 * \param head Pointer to the first node of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node in the element.
 * \return The new first element, or NULL on failure.
 * \ingroup idl
**/
void *idl_circ_push(t_cm_idl **head, const void *elem, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_circ_push_node(head, idl_eton(elem, offset));
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Inserts a node at the back of a circular list.
 * \param head Pointer to the first node of the list.
 * \param node The node to insert.
 * \return The first node of the list, or NULL on failure.
 * \ingroup idl
**/
t_cm_idl *idl_circ_push_back_node(t_cm_idl **head, t_cm_idl *node) {
	t_cm_idl *tail;

	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->prev = node;
		node->next = node;
		*head      = node;
	} else {
		tail          = (*head)->prev;
		node->next    = *head;
		node->prev    = tail;
		tail->next    = node;
		(*head)->prev = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief Inserts an element at the back of a circular list.
 * \param head Pointer to the first node of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node in the element.
 * \return The first element of the list, or NULL on failure.
 * \ingroup idl
**/
void *idl_circ_push_back(t_cm_idl **head, const void *elem, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_circ_push_back_node(head, idl_eton(elem, offset));
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Removes and returns the first node of a circular list.
 * \param head Pointer to the first node of the list.
 * \return The removed node, or NULL if the list was empty.
 * \ingroup idl
**/
t_cm_idl *idl_circ_pop_node(t_cm_idl **head) {
	t_cm_idl *ret;
	t_cm_idl *tail;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	ret = *head;
	if (ret->next == ret) {
		*head = NULL;
	} else {
		tail          = ret->prev;
		*head         = ret->next;
		tail->next    = *head;
		(*head)->prev = tail;
	}
	ret->prev = NULL;
	ret->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief Removes and returns the first element of a circular list.
 * \param head Pointer to the first node of the list.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup idl
**/
void *idl_circ_pop(t_cm_idl **head) {
	const t_cm_idl *ret;

	ret = idl_circ_pop_node(head);
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Removes and returns the last node of a circular list.
 * \param head Pointer to the first node of the list.
 * \return The removed node, or NULL if the list was empty.
 * \ingroup idl
**/
t_cm_idl *idl_circ_pop_back_node(t_cm_idl **head) {
	t_cm_idl *ret;
	t_cm_idl *new_tail;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	ret = (*head)->prev;
	if (ret == *head) {
		*head = NULL;
	} else {
		new_tail       = ret->prev;
		new_tail->next = *head;
		(*head)->prev  = new_tail;
	}
	ret->prev = NULL;
	ret->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief Removes and returns the last element of a circular list.
 * \param head Pointer to the first node of the list.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup idl
**/
void *idl_circ_pop_back(t_cm_idl **head) {
	const t_cm_idl *ret;

	ret = idl_circ_pop_back_node(head);
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Concatenates two circular lists.
 * \param list1 The first node of the first list.
 * \param list2 The first node of the second list.
 * \return The first node of the concatenated list.
 * \note no allocation is performed. list2 is spliced as is; the caller must
 * stop using its old head as a separate list.
 * \ingroup idl
**/
t_cm_idl *idl_circ_concat_node(t_cm_idl *list1, t_cm_idl *list2) {
	t_cm_idl *tail1;
	t_cm_idl *tail2;

	if (!list1) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (list2);
	}
	if (!list2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (list1);
	}
	tail1       = list1->prev;
	tail2       = list2->prev;
	tail1->next = list2;
	list2->prev = tail1;
	tail2->next = list1;
	list1->prev = tail2;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (list1);
}

/**
 * \brief Concatenates two circular element lists.
 * \param list1 The first element of the first list.
 * \param list2 The first element of the second list.
 * \param offset Offset of the node in both element types.
 * \return The first element of the concatenated list, or NULL.
 * \ingroup idl
**/
void *idl_circ_concat(const void *list1, const void *list2, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_circ_concat_node(list1 ? idl_eton(list1, offset) : NULL,
	                           list2 ? idl_eton(list2, offset) : NULL);
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Removes a specific node from a circular list.
 * \param head Pointer to the first node of the list.
 * \param target The node to remove.
 * \return true if the node was removed, false otherwise.
 * \ingroup idl
**/
bool idl_circ_remove_node(t_cm_idl **head, t_cm_idl *target) {
	if (!head || !*head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	if (target->next == target) {
		if (*head != target) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
			return (false);
		}
		*head        = NULL;
		target->prev = NULL;
		target->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (true);
	}
	if (!target->prev || !target->next) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (false);
	}
	target->prev->next = target->next;
	target->next->prev = target->prev;
	if (*head == target) *head = target->next;
	target->prev = NULL;
	target->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Removes a specific element from a circular list.
 * \param head Pointer to the first node of the list.
 * \param elem Pointer to the element to remove.
 * \param offset Offset of the node in the element.
 * \return true if the element was removed, false otherwise.
 * \ingroup idl
**/
bool idl_circ_remove(t_cm_idl **head, const void *elem, size_t offset) {
	if (!elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	return (idl_circ_remove_node(head, idl_eton(elem, offset)));
}

/**
 * \brief Removes every node from a circular list.
 * \param head Pointer to the first node of the list.
 * \param dtor Optional destructor called on every element.
 * \note every node is unlinked on both sides before *head is reset to NULL.
 * \ingroup idl
**/
void idl_circ_clear(t_cm_idl **head, t_cm_dtor dtor) {
	t_cm_idl *start;
	t_cm_idl *curr;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	if (!*head) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return;
	}
	start = *head;
	curr  = start;
	do {
		t_cm_idl *nxt = curr->next;

		curr->prev = NULL;
		curr->next = NULL;
		if (dtor) dtor(idl_ntoe(curr));
		curr = nxt;
	} while (curr != start);
	*head = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}

/**
 * \brief Applies the callback to every element of a circular list.
 * \param head The first node of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \note stops early if the callback returns false.
 * \ingroup idl
**/
void *idl_circ_foreach(const t_cm_idl *head, t_cm_callback c, void *u_data) {
	const t_cm_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (u_data);
	}
	curr = head;
	do {
		const t_cm_idl *nxt = curr->next;

		if (!c(idl_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__IDL_LOG_ECB)
			return (u_data);
		}
		curr = nxt;
	} while (curr != head);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (u_data);
}

/**
 * \brief Applies the callback to every element of a circular list, from the
 * last node down to the first.
 * \param head The first node of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \note stops early if the callback returns false.
 * \ingroup idl
**/
void *idl_circ_foreach_rev(const t_cm_idl *head, t_cm_callback c,
                           void *u_data) {
	const t_cm_idl *start;
	const t_cm_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (u_data);
	}
	start = head->prev;
	curr  = start;
	do {
		const t_cm_idl *prv = curr->prev;

		if (!c(idl_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__IDL_LOG_ECB)
			return (u_data);
		}
		curr = prv;
	} while (curr != start);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (u_data);
}

/**
 * \brief Pull iterator over the elements of a circular list.
 * \param head The first node of the list.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \param offset Offset of the node in the current element.
 * \return true while an element is available, false once the cycle is
 * exhausted or on invalid arguments.
 * \ingroup idl
**/
bool idl_circ_iter(const t_cm_idl *head, void **iter, size_t offset) {
	const t_cm_idl *node;

	if (!head || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	if (!*iter) {
		*iter = idl_ntoe(head);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (true);
	}
	node = idl_eton(*iter, offset);
	if (node->next == head) {
		*iter = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (false);
	}
	*iter = idl_ntoe(node->next);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Reverse pull iterator over the elements of a circular list.
 * \param head The first node of the list.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \param offset Offset of the node in the current element.
 * \return true while an element is available, false once the cycle is
 * exhausted or on invalid arguments.
 * \note the elements are yielded from the last node down to the first.
 * \ingroup idl
**/
bool idl_circ_iter_rev(const t_cm_idl *head, void **iter, size_t offset) {
	const t_cm_idl *node;

	if (!head || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	if (!*iter) {
		*iter = idl_ntoe(head->prev);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (true);
	}
	node = idl_eton(*iter, offset);
	if (node == head) {
		*iter = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (false);
	}
	*iter = idl_ntoe(node->prev);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}
