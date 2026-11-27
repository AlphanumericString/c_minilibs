#include "intrusive_linked_lists.h"

/**
 * \brief Calculates the length of a circular list.
 * \param head The last node of the list (the list handle), or NULL.
 * \return The number of nodes in the list.
 * \ingroup ill
**/
size_t ill_circ_length(const t_cm_ill *head) {
	const t_cm_ill *start;
	const t_cm_ill *curr;
	size_t          len = 0;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (0);
	}
	start = head->next;
	curr  = start;
	do {
		len++;
		curr = curr->next;
	} while (curr != start);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (len);
}

/**
 * \brief Retrieves the first node of a circular list.
 * \param head The last node of the list (the list handle).
 * \return A pointer to the first node, or NULL if the list is empty.
 * \ingroup ill
**/
t_cm_ill *ill_circ_get_head_node(const t_cm_ill *head) {
	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ((t_cm_ill *)head->next);
}

/**
 * \brief Retrieves the first element of a circular list.
 * \param head_elem The last element of the list (the list handle).
 * \param offset Offset of the node in the element.
 * \return A pointer to the first element, or NULL if the list is empty.
 * \ingroup ill
**/
void *ill_circ_get_head(const void *head_elem, size_t offset) {
	const t_cm_ill *ret;

	ret = ill_circ_get_head_node(ill_eton(head_elem, offset));
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Retrieves the last node of a circular list.
 * \param head The last node of the list (the list handle).
 * \return A pointer to the last node, or NULL if the list is empty.
 * \ingroup ill
**/
t_cm_ill *ill_circ_get_tail_node(const t_cm_ill *head) {
	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ((t_cm_ill *)head);
}

/**
 * \brief Retrieves the last element of a circular list.
 * \param head_elem The last element of the list (the list handle).
 * \param offset Offset of the node in the element.
 * \return A pointer to the last element, or NULL if the list is empty.
 * \ingroup ill
**/
void *ill_circ_get_tail(const void *head_elem, size_t offset) {
	const t_cm_ill *ret;

	ret = ill_circ_get_tail_node(ill_eton(head_elem, offset));
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Inserts a node at the front of a circular list.
 * \param head Pointer to the last node of the list.
 * \param node The node to insert.
 * \return The list handle (the last node), or NULL on failure.
 * \ingroup ill
**/
t_cm_ill *ill_circ_push_node(t_cm_ill **head, t_cm_ill *node) {
	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->next = node;
		*head      = node;
	} else {
		node->next    = (*head)->next;
		(*head)->next = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief Inserts an element at the front of a circular list.
 * \param head Pointer to the last node of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node in the element.
 * \return The new first element, or NULL on failure.
 * \ingroup ill
**/
void *ill_circ_push(t_cm_ill **head, const void *elem, size_t offset) {
	const t_cm_ill *ret;

	ret = ill_circ_push_node(head, ill_eton(elem, offset));
	return (ret ? ill_ntoe(ret->next) : NULL);
}

/**
 * \brief Inserts a node at the back of a circular list.
 * \param head Pointer to the last node of the list.
 * \param node The node to insert.
 * \return The new list handle (the new last node), or NULL on failure.
 * \ingroup ill
**/
t_cm_ill *ill_circ_push_back_node(t_cm_ill **head, t_cm_ill *node) {
	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->next = node;
		*head      = node;
	} else {
		node->next    = (*head)->next;
		(*head)->next = node;
		*head         = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief Inserts an element at the back of a circular list.
 * \param head Pointer to the last node of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node in the element.
 * \return The new first element, or NULL on failure.
 * \ingroup ill
**/
void *ill_circ_push_back(t_cm_ill **head, const void *elem, size_t offset) {
	const t_cm_ill *ret;

	ret = ill_circ_push_back_node(head, ill_eton(elem, offset));
	return (ret ? ill_ntoe(ret->next) : NULL);
}

/**
 * \brief Removes and returns the first node of a circular list.
 * \param head Pointer to the last node of the list.
 * \return The removed node, or NULL if the list was empty.
 * \ingroup ill
**/
t_cm_ill *ill_circ_pop_node(t_cm_ill **head) {
	t_cm_ill *ret;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if ((*head)->next == *head) {
		ret   = *head;
		*head = NULL;
	} else {
		ret           = (*head)->next;
		(*head)->next = ret->next;
	}
	ret->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief Removes and returns the first element of a circular list.
 * \param head Pointer to the last node of the list.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup ill
**/
void *ill_circ_pop(t_cm_ill **head) {
	const t_cm_ill *ret;

	ret = ill_circ_pop_node(head);
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes and returns the last node of a circular list.
 * \param head Pointer to the last node of the list.
 * \return The removed node, or NULL if the list was empty.
 * \ingroup ill
**/
t_cm_ill *ill_circ_pop_back_node(t_cm_ill **head) {
	t_cm_ill *prev;
	t_cm_ill *ret;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if ((*head)->next == *head) {
		ret       = *head;
		*head     = NULL;
		ret->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (ret);
	}
	prev = (*head)->next;
	while (prev->next != *head)
		prev = prev->next;
	ret        = *head;
	prev->next = ret->next;
	*head      = prev;
	ret->next  = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief Removes and returns the last element of a circular list.
 * \param head Pointer to the last node of the list.
 * \return The removed element, or NULL if the list was empty.
 * \ingroup ill
**/
void *ill_circ_pop_back(t_cm_ill **head) {
	const t_cm_ill *ret;

	ret = ill_circ_pop_back_node(head);
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Concatenates two circular lists.
 * \param list1 The last node of the first list.
 * \param list2 The last node of the second list.
 * \return The last node of the concatenated list.
 * \note no allocation is performed. list2 is spliced as is; the caller must
 * stop using its old handle as a separate list.
 * \ingroup ill
**/
t_cm_ill *ill_circ_concat_node(t_cm_ill *list1, t_cm_ill *list2) {
	t_cm_ill *first1;
	t_cm_ill *first2;

	if (!list1) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (list2);
	}
	if (!list2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (list1);
	}
	first1      = list1->next;
	first2      = list2->next;
	list1->next = first2;
	list2->next = first1;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (list2);
}

/**
 * \brief Concatenates two circular element lists.
 * \param list1 The last element of the first list.
 * \param list2 The last element of the second list.
 * \param offset Offset of the node in both element types.
 * \return The last element of the concatenated list, or NULL.
 * \ingroup ill
**/
void *ill_circ_concat(const void *list1, const void *list2, size_t offset) {
	const t_cm_ill *ret;

	ret = ill_circ_concat_node(list1 ? ill_eton(list1, offset) : NULL,
	                           list2 ? ill_eton(list2, offset) : NULL);
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes a specific node from a circular list.
 * \param head Pointer to the last node of the list.
 * \param target The node to remove.
 * \return true if the node was removed, false otherwise.
 * \ingroup ill
**/
bool ill_circ_remove_node(t_cm_ill **head, t_cm_ill *target) {
	t_cm_ill *curr;

	if (!head || !*head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	if ((*head)->next == *head) {
		if (*head != target) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
			return (false);
		}
		*head        = NULL;
		target->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (true);
	}
	if (*head == target) {
		curr = (*head)->next;
		while (curr->next != *head)
			curr = curr->next;
		curr->next   = target->next;
		*head        = curr;
		target->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (true);
	}
	curr = *head;
	do {
		if (curr->next == target) {
			curr->next   = target->next;
			target->next = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
			return (true);
		}
		curr = curr->next;
	} while (curr != *head);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
	return (false);
}

/**
 * \brief Removes a specific element from a circular list.
 * \param head Pointer to the last node of the list.
 * \param elem Pointer to the element to remove.
 * \param offset Offset of the node in the element.
 * \return true if the element was removed, false otherwise.
 * \ingroup ill
**/
bool ill_circ_remove(t_cm_ill **head, const void *elem, size_t offset) {
	if (!elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	return (ill_circ_remove_node(head, ill_eton(elem, offset)));
}

/**
 * \brief Removes every node from a circular list.
 * \param head Pointer to the last node of the list.
 * \param dtor Optional destructor called on every element.
 * \note every node is unlinked before *head is reset to NULL.
 * \ingroup ill
**/
void ill_circ_clear(t_cm_ill **head, t_cm_dtor dtor) {
	t_cm_ill *start;
	t_cm_ill *curr;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return;
	}
	if (!*head) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return;
	}
	start = (*head)->next;
	curr  = start;
	do {
		t_cm_ill *nxt = curr->next;

		curr->next = NULL;
		if (dtor) dtor(ill_ntoe(curr));
		curr = nxt;
	} while (curr != start);
	*head = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}

/**
 * \brief Applies the callback to every element of a circular list.
 * \param head The last node of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \note stops early if the callback returns false.
 * \ingroup ill
**/
void *ill_circ_foreach(const t_cm_ill *head, t_cm_callback c, void *u_data) {
	const t_cm_ill *start;
	const t_cm_ill *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (u_data);
	}
	start = head->next;
	curr  = start;
	do {
		const t_cm_ill *nxt = curr->next;

		if (!c(ill_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__ILL_LOG_ECB)
			return (u_data);
		}
		curr = nxt;
	} while (curr != start);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (u_data);
}

/**
 * \brief Pull iterator over the elements of a circular list.
 * \param head The last node of the list.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \param offset Offset of the node in the current element.
 * \return true while an element is available, false once the cycle is
 * exhausted or on invalid arguments.
 * \ingroup ill
**/
bool ill_circ_iter(const t_cm_ill *head, void **iter, size_t offset) {
	const t_cm_ill *node;

	if (!head || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	if (!*iter) {
		*iter = ill_ntoe(head->next);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (true);
	}
	node = ill_eton(*iter, offset);
	if (node == head) {
		*iter = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (false);
	}
	*iter = ill_ntoe(node->next);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (true);
}
