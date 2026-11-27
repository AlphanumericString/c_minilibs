#include "intrusive_linked_lists.h"

// internal for filter
static size_t internal_filter_on(t_cm_ill **head, t_cm_callback cb, void *arg,
                                 t_cm_ill **filtered, bool filter_on) {
	size_t     count = 0;
	t_cm_ill **link;
	t_cm_ill  *ftail;

	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link) {
		t_cm_ill *moved;
		if (cb(ill_ntoe(*link), arg) != filter_on) {
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
	return (count);
}

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
	size_t count = 0;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	count = internal_filter_on(head, cb, arg, filtered, false);
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
	size_t count = 0;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	count = internal_filter_on(head, cb, arg, filtered, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}

static size_t internal_nfilter_on(t_cm_ill **head, size_t n, t_cm_callback cb,
                                  void *arg, t_cm_ill **filtered,
                                  bool filter_on) {
	size_t     count = 0;
	t_cm_ill **link;
	t_cm_ill  *ftail;

	*filtered = NULL;
	link      = head;
	ftail     = NULL;
	while (*link && n) {
		t_cm_ill *moved;

		n--;
		if (cb(ill_ntoe(*link), arg) != filter_on) {
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
	return (count);
}

/**
 * \brief Filters at most the first n elements, keeping those for which cb is
 * true and moving the others into *filtered.
 * \param head Pointer to the head of the list.
 * \param n Maximum number of elements to inspect.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the elements that did not match.
 * \return The number of elements moved into *filtered.
 * \note elements beyond n are left untouched in *head.
 * \ingroup ill
**/
size_t ill_nfilter(t_cm_ill **head, size_t n, t_cm_callback cb, void *arg,
                   t_cm_ill **filtered) {
	size_t count;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	count = internal_nfilter_on(head, n, cb, arg, filtered, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Inverse of ill_nfilter over at most the first n elements.
 * \param head Pointer to the head of the list.
 * \param n Maximum number of elements to inspect.
 * \param cb The callback used to test elements (true means move out).
 * \param arg Argument passed to the callback.
 * \param filtered Output list receiving the matching elements.
 * \return The number of elements moved into *filtered.
 * \note elements beyond n are left untouched in *head.
 * \ingroup ill
**/
size_t ill_nfilter_out(t_cm_ill **head, size_t n, t_cm_callback cb, void *arg,
                       t_cm_ill **filtered) {
	size_t count;

	if (!head || !cb || !filtered) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (0);
	}
	count = internal_nfilter_on(head, n, cb, arg, filtered, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (count);
}

/**
 * \brief Destructive variant of ill_filter: keeps elements for which cb is true
 * and destroys (via dtor) the others.
 * \param head Pointer to the head of the list.
 * \param cb The callback used to test elements (true means keep).
 * \param arg Argument passed to the callback.
 * \param d Optional destructor called on each removed element.
 * \return The number of elements removed.
 * \ingroup ill
**/
size_t ill_filter_dt(t_cm_ill **head, t_cm_callback cb, void *arg,
                     t_cm_dtor d) {
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
