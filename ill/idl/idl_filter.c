#include "intrusive_doubly_linked.h"

/**
 * \brief Filters the list, keeping elements for which cb is true and moving the
 * others into *filtered. *head is updated if the current head is moved out.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the elements that did not match.
 * \return The number of elements moved into *filtered.
 * \ingroup idl
**/
size_t idl_filter(t_cm_idl **head, t_cm_callback cb, void *arg,
                  t_cm_idl **filtered) {
	size_t     count = 0;
	t_cm_idl **link;
	t_cm_idl  *ftail;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (0);
	}
	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link) {
		t_cm_idl *moved;
		if (cb(idl_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		moved = *link;
		*link = moved->next;
		if (*link) (*link)->prev = moved->prev;
		moved->next = NULL;
		moved->prev = ftail;
		if (!*filtered) *filtered = moved;
		else ftail->next = moved;
		ftail = moved;
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Inverse of idl_filter, moving elements for which cb is true into
 * *filtered. *head is updated if the current head is moved out.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means move out).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the matching elements.
 * \return The number of elements moved into *filtered.
 * \ingroup idl
**/
size_t idl_filter_out(t_cm_idl **head, t_cm_callback cb, void *arg,
                      t_cm_idl **filtered) {
	size_t     count = 0;
	t_cm_idl **link;
	t_cm_idl  *ftail;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (0);
	}
	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link) {
		t_cm_idl *moved;
		if (!cb(idl_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		moved = *link;
		*link = moved->next;
		if (*link) (*link)->prev = moved->prev;
		moved->next = NULL;
		moved->prev = ftail;
		if (!*filtered) *filtered = moved;
		else ftail->next = moved;
		ftail = moved;
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Destructive variant of idl_filter: destroys (via dtor) the elements
 * that would have been moved into a filtered list.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param d Optional destructor called on each removed element.
 * \return The number of elements removed.
 * \ingroup idl
**/
size_t idl_rm_if(t_cm_idl **head, t_cm_callback cb, void *arg, t_cm_dtor d) {
	size_t     count = 0;
	t_cm_idl **link;

	if (!head || !cb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (0);
	}
	link = head;
	while (*link) {
		t_cm_idl *rm;
		if (cb(idl_ntoe(*link), arg)) {
			link = &(*link)->next;
			continue;
		}
		rm    = *link;
		*link = rm->next;
		if (*link) (*link)->prev = rm->prev;
		rm->next = NULL;
		rm->prev = NULL;
		if (d) d(idl_ntoe(rm));
		count++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (count);
}
