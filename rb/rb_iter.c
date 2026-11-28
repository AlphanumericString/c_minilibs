#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \internal
 * \brief true when rb is usable as an iteration source
 * \param rb the round buffer
 * \return true when the backing array, the element size and the capacity are
 * all set
 * \ingroup rb_internals
**/
static bool rbi_iter_valid(const t_cm_rb *rb) {
	return (rb && rb->array && rb->sz_e && rb->nb_max);
}

/**
 * \internal
 * \brief logical index of an element pointer, relative to a given output head
 * \param rb the round buffer
 * \param base output head the pointer must be resolved against
 * \param p an element pointer previously yielded by an rb iterator
 * \return the index in [0, nb_max); a stale pointer maps past the live window
 * (idx >= rb_size) and is rejected by the callers
 * \note on a magic buffer the pointer can live in the second window, it is
 * folded back before the distance from base is computed
 * \ingroup rb_internals
**/
static size_t rbi_idx_from_ptr(const t_cm_rb *rb, size_t base, const void *p) {
	size_t raw;

	raw =
	    (size_t)((const unsigned char *)p - (const unsigned char *)rb->array) /
	    rb->sz_e;
	if (raw >= rb->nb_max) raw -= rb->nb_max;
	if (raw >= base) return (raw - base);
	return (raw + rb->nb_max - base);
}

/**
 * \brief pull iterator over the elements in [start, last[
 * \param rb pointer to the round buffer (the iterator handle)
 * \param start index of the first element to yield, counted from o_pos
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate up to and including the last element
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is exhausted
 * or on invalid arguments
 * \note the index is re-anchored against the current o_pos on every call, so a
 * head move mid-walk does not dangle the iterator; if the element is no longer
 * live the walk stops. This is not a consistent snapshot: mutating the buffer
 * from the same side during a walk is not supported.
 * \note typical usage:
 * \code
 * void  *it_val = NULL;
 * size_t start  = 0;
 * while (rb_iter(&rb, start, (size_t)-1, &it_val))
 *     do_something(*(int *)it_val);
 * \endcode
 * \ingroup rb
**/
bool rb_iter(const t_cm_rb *rb, size_t start, size_t last, void **it_val) {
	size_t base;
	size_t count;
	size_t idx;

	if (!rbi_iter_valid(rb) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	count = rbi_count_get(rb);
	base  = rbi_opos_get(rb);
	if (!count) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (false);
	}
	if (last == (size_t)-1 || last > count) last = count;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = rbi_get_pos(rb, rbi_off_at(rb, base, start));
	} else {
		idx = rbi_idx_from_ptr(rb, base, *it_val);
		if (idx < start || idx >= count) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
			return (false);
		}
		idx++;
		if (idx >= last) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
			return (false);
		}
		*it_val = rbi_get_pos(rb, rbi_off_at(rb, base, idx));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}

/**
 * \brief reverse pull iterator over the elements in [start, last[
 * \param rb pointer to the round buffer (the iterator handle)
 * \param start index of the first element of the range, counted from o_pos. It
 * is the last element yielded by the iterator
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate from the last element of the rb
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is exhausted
 * or on invalid arguments
 * \note the elements are yielded from last - 1 down to start included
 * \note the index is re-anchored against the current o_pos on every call, see
 * rb_iter for the mid-walk caveats
 * \ingroup rb
**/
bool rb_iter_rev(const t_cm_rb *rb, size_t start, size_t last, void **it_val) {
	size_t base;
	size_t count;
	size_t idx;

	if (!rbi_iter_valid(rb) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	count = rbi_count_get(rb);
	base  = rbi_opos_get(rb);
	if (!count) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (false);
	}
	if (last == (size_t)-1 || last > count) last = count;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = rbi_get_pos(rb, rbi_off_at(rb, base, last - 1));
	} else {
		idx = rbi_idx_from_ptr(rb, base, *it_val);
		if (idx >= count || idx <= start) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
			return (false);
		}
		idx--;
		*it_val = rbi_get_pos(rb, rbi_off_at(rb, base, idx));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}

/**
 * \brief visit the elements in [start, last[ with a positional callback
 * \param rb pointer to the round buffer
 * \param start index of the first element to visit, counted from o_pos
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to visit up to and including the last element
 * \param cb callback receiving the element, the user argument, the logical
 * position and a boolean set on the last visited element
 * \param arg user argument passed to the callback
 * \return true when the whole range was visited or when the range is empty,
 * false on invalid arguments or when the callback stops the walk
 * \note the position is the logical index from o_pos, matching rb_at
 * \note rb_iter already covers ranged pull iteration; this is the callback
 * counterpart of da_apply_range
 * \ingroup rb
**/
bool rb_iter_range(const t_cm_rb *rb, size_t start, size_t last,
                   t_cm_callback_it cb, void *arg) {
	size_t base;
	size_t count;
	size_t i;

	if (!rbi_iter_valid(rb) || !cb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	count = rbi_count_get(rb);
	base  = rbi_opos_get(rb);
	if (last == (size_t)-1 || last > count) last = count;
	if (start >= last) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (true);
	}
	for (i = start; i < last; i++) {
		if (!cb(rbi_get_pos(rb, rbi_off_at(rb, base, i)), arg, i,
		        i + 1 == last)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_ECB)
			return (false);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}

/**
 * \brief advance an element pointer to the next element of the round buffer
 * \param rb pointer to the round buffer
 * \param it current element pointer. Initialize it to NULL to yield the first
 * element
 * \return the next element address, or NULL when the end is reached or on
 * invalid arguments
 * \note the current pointer is resolved against the current o_pos, so a head
 * move mid-walk does not dangle the iterator; a slot that fell out of the live
 * window ends the walk
 * \ingroup rb
**/
void *rb_advance_it(const t_cm_rb *rb, const void *it) {
	size_t base;
	size_t count;
	size_t idx;

	if (!rbi_iter_valid(rb)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	count = rbi_count_get(rb);
	base  = rbi_opos_get(rb);
	if (!count) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (NULL);
	}
	if (!it) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (rbi_get_pos(rb, rbi_off_at(rb, base, 0)));
	}
	idx = rbi_idx_from_ptr(rb, base, it);
	if (idx >= count) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
		return (NULL);
	}
	idx++;
	if (idx >= count) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rbi_get_pos(rb, rbi_off_at(rb, base, idx)));
}

/**
 * \brief move an element pointer to the previous element of the round buffer
 * \param rb pointer to the round buffer
 * \param it current element pointer. Initialize it to NULL to yield the last
 * element
 * \return the previous element address, or NULL when the start is reached or on
 * invalid arguments
 * \note the current pointer is resolved against the current o_pos, so a head
 * move mid-walk does not dangle the iterator; a slot that fell out of the live
 * window ends the walk
 * \ingroup rb
**/
void *rb_recede_it(const t_cm_rb *rb, const void *it) {
	size_t base;
	size_t count;
	size_t idx;

	if (!rbi_iter_valid(rb)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	count = rbi_count_get(rb);
	base  = rbi_opos_get(rb);
	if (!count) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (NULL);
	}
	if (!it) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (rbi_get_pos(rb, rbi_off_at(rb, base, count - 1)));
	}
	idx = rbi_idx_from_ptr(rb, base, it);
	if (idx >= count) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
		return (NULL);
	}
	if (idx == 0) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (NULL);
	}
	idx--;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rbi_get_pos(rb, rbi_off_at(rb, base, idx)));
}
