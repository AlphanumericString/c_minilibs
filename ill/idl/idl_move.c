#include "intrusive_doubly_linked.h"

/**
 * \brief Moves an inclusive range of nodes to the tail of another list.
 * \param from Pointer to the head of the source list.
 * \param to Pointer to the head of the destination list.
 * \param first The first node of the range.
 * \param last The last node of the range, reachable from first.
 * \return true if the range was moved, false on invalid arguments or when the
 * range does not belong to the source list.
 * \note both lists must be distinct. The range is spliced as is and remains in
 * the same order.
 * \ingroup idl
**/
bool idl_move_node(t_cm_idl **from, t_cm_idl **to, t_cm_idl *first,
                   t_cm_idl *last) {
	t_cm_idl      **link;
	t_cm_idl       *prev;
	t_cm_idl       *after;
	const t_cm_idl *scan;
	t_cm_idl       *tail;

	if (!from || !to || !*from || !first || !last || from == to) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	link = from;
	prev = NULL;
	while (*link && *link != first) {
		prev = *link;
		link = &(*link)->next;
	}
	if (!*link) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (false);
	}
	for (scan = first; scan && scan != last; scan = scan->next)
		;
	if (!scan) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (false);
	}
	after = last->next;
	if (prev) prev->next = after;
	else *from = after;
	if (after) after->prev = prev;
	last->next  = NULL;
	first->prev = NULL;
	if (!*to) {
		*to = first;
	} else {
		tail        = idl_get_tail_node(*to);
		tail->next  = first;
		first->prev = tail;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Moves an inclusive element range to the tail of another list.
 * \param from Pointer to the head of the source list.
 * \param to Pointer to the head of the destination list.
 * \param first The first element of the range.
 * \param last The last element of the range, reachable from first.
 * \param offset Offset of the node in both element types.
 * \return true if the range was moved, false on invalid arguments or when the
 * range does not belong to the source list.
 * \note both lists must be distinct. The range is spliced as is and remains in
 * the same order.
 * \ingroup idl
**/
bool idl_move(t_cm_idl **from, t_cm_idl **to, const void *first,
              const void *last, size_t offset) {
	if (!first || !last) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	return (idl_move_node(from, to, idl_eton(first, offset),
	                      idl_eton(last, offset)));
}
