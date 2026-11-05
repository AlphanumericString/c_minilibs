#include "intrusive_linked_lists.h"

/**
 * \brief Filters the list, keeping elements for which cb is true and moving the
 * others into *filtered. *head is updated if the current head is moved out.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the elements that did not match.
 * \return The number of elements moved into *filtered.
 * \ingroup ill
**/
size_t ill_filter(t_cm_ill **head, t_cm_callback cb, void *arg,
                  t_cm_ill **filtered) {
	size_t     count = 0;
	t_cm_ill **link;
	t_cm_ill  *ftail;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link) {
		t_cm_ill *moved;
		if (cb(ill_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		moved       = *link;
		*link       = moved->next;
		moved->next = NULL;
		if (!*filtered) *filtered = moved;
		else ftail->next = moved;
		ftail = moved;
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Inverse of ill_filter, moving elements for which cb is true into
 * *filtered. *head is updated if the current head is moved out.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means move out).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the matching elements.
 * \return The number of elements moved into *filtered.
 * \ingroup ill
**/
size_t ill_filter_out(t_cm_ill **head, t_cm_callback cb, void *arg,
                      t_cm_ill **filtered) {
	size_t     count = 0;
	t_cm_ill **link;
	t_cm_ill  *ftail;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link) {
		t_cm_ill *moved;
		if (!cb(ill_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		moved       = *link;
		*link       = moved->next;
		moved->next = NULL;
		if (!*filtered) *filtered = moved;
		else ftail->next = moved;
		ftail = moved;
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Destructive variant of ill_filter: destroys (via dtor) the elements
 * that would have been moved into a filtered list.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param d Optional destructor called on each removed element.
 * \return The number of elements removed.
 * \ingroup ill
**/
size_t ill_rm_if(t_cm_ill **head, t_cm_callback cb, void *arg, t_cm_dtor d) {
	size_t     count = 0;
	t_cm_ill **link;

	if (!head || !cb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	link = head;
	while (*link) {
		t_cm_ill *rm;
		if (cb(ill_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		rm       = *link;
		*link    = rm->next;
		rm->next = NULL;
		if (d) d(ill_ntoe(rm));
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}
