#include "intrusive_linked_lists.h"

/**
 * \brief Advances an iterator one node forward and returns the next element.
 * \param e Pointer to the current element (user structure).
 * \param offset Offset of the intrusive node within the element.
 * \return A pointer to the next element, or NULL if there is none.
 * \ingroup ill
**/
void *ill_advance_it(const void *e, size_t offset) {
	const t_cm_ill *node;

	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	node = ill_eton(e, offset);
	node = node->next;
	if (!node) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ill_ntoe(node);
}

/**
 * \brief Advances a node iterator one node forward.
 * \param it The current node.
 * \return The next node, or NULL if there is none.
 * \ingroup ill
**/
t_cm_ill *ill_advance_it_node(const t_cm_ill *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return it->next;
}

/**
 * \brief Pull iterator over the elements of the list.
 * \param head The head of the list.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \return true while an element is available, false once the list is exhausted
 * or on invalid arguments.
 * \note typical usage:
 * \code
 * void *it = NULL;
 * while (ill_iter(head, &it))
 *     do_something(it);
 * \endcode
 * \ingroup ill
**/
bool ill_iter(t_cm_ill *head, void **iter) {
	size_t    offset;
	t_cm_ill *node;

	if (!head || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	offset = head->offset;
	if (!*iter) {
		*iter = ill_ntoe(head);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (true);
	}
	node = ill_eton(*iter, offset);
	node = node->next;
	if (!node) {
		*iter = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (false);
	}
	*iter = ill_ntoe(node);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (true);
}
