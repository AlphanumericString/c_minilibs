#include <assert.h>
#include <stdbool.h>

#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

/**
 * \brief strengthens a weak pointer to a strong pointer
 * \param sptr weak pointer to strengthen
 * \return strong pointer on success, NULL otherwise
 * \note converts pointer in-place (increments strong count, decrements
 * weak count); fails if sptr is NULL or weak count is 0
 * \warning the given weak pointer should be treated as replaced by the returned
 * strong pointer
 * \ingroup sptr
**/
t_strong_ptr sptr_into_strong(t_weak_ptr *sptr) {
	if (!sptr || sptr_get_wc(sptr) == 0 || sptr_get_rc(sptr) == 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA)
		return ((t_strong_ptr){0});
	}
	sptri_rc_inc(sptr->meta);
	sptri_wc_dec(sptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_strong_ptr){.meta = sptr->meta, .data = sptr->meta->data});
}
/**
 * \brief weakens a strong pointer to a weak pointer
 * \param sptr strong pointer to weaken
 * \return weak pointer on success, NULL otherwise
 * \note converts pointer in-place; decrements strong count and increments
 * weak count; if strong count reaches 0, calls destructor on data and sets data
 * to NULL
 * \warning the given strong pointer should be treated as replaced by the
 * returned weak pointer
 * \ingroup sptr
**/
t_weak_ptr sptr_into_weak(t_strong_ptr *sptr) {
	if (!sptr || sptr_get_rc(sptr) == 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_weak_ptr){0});
	}
	sptri_rc_dec(sptr->meta);
	if (!sptri_rc_get(sptr->meta)) {
		if (sptr->meta->dtor) sptr->meta->dtor(sptr->meta->data);
		sptr->meta->data = NULL;
	}
	sptri_wc_inc(sptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_weak_ptr){sptr->meta});
}
/**
 * \brief steals the ownership of a owned pointer
 * \param to owned pointer to transfer ownership to
 * \param from owned pointer to transfer ownership from
 * \return true if successful, false otherwise
 * \note transfers ownership between owned pointers referencing the same box;
 * fails if to is already owner, from is not owner, or boxes differ
 * \ingroup sptr
**/
bool sptr_transfer_ownership(t_owned_ptr *to, t_owned_ptr *from) {
	if (!to || !from || !to->meta || !from->meta || from->owner == false ||
	    to->owner == true) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	if (to->meta != from->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA)
		return (false);
	}
	to->owner   = true;
	from->owner = false;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (true);
}
/**
 * \brief move ownership moves the smart pointer of from to the variable to
 * and switches who owns the data. If dest is not empty calls the delete
 * function on the smart pointer before moving the data. The variable from is
 * zeroed out.
 * \param dest the destination of where to place the data. If not zero filed
 * calls delete before moving the data into it.
 * \param from the location to copy the data from. If copy is successfully made
 * the variable is zeroed out.
 * \return true on success, false otherwise.
**/
bool sptr_move_ownership(t_owned_ptr *dest, t_owned_ptr *from) {
	t_owned_ptr r;

	if (!dest || !from || !from->meta || !from->owner ||
	    sptr_get_rc(from) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	if (dest->meta && sptr_deleteo(dest) != true) {
		CM_LOG(C_MINI__LEVEL_WARN, "non-empty container overridden")
		return (false);
	}
	r     = *from;
	*from = (t_owned_ptr){0};
	*dest = r;
	return (true);
}
/**
 * \brief switches data from one pointer to another. Does not swap.
 * \param src source of data to move
 * \param dst destination of where to put the data to.
 * \return return destination on success, null on error
 * \note if destination's data exist calls sptr_delete() on destination
 * beforehand
 * \ingroup sptr
**/
t_uniq_ptr *sptr_uniq_yield(t_uniq_ptr *dst, t_uniq_ptr *src) {
	if (!dst || !src) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	if (dst->data) sptr_delete(dst);
	*dst = *src;
	*src = (t_uniq_ptr){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (dst);
}
/**
 * \brief switches data from both uniq
 * \param sw1 to swap data to and fro
 * \param sw2 to swap data to and fro
 * \return return true on success, false on error
 * \ingroup sptr
**/
bool sptr_uniq_swap(t_uniq_ptr *sw1, t_uniq_ptr *sw2) {
	t_uniq_ptr tmp;

	if (!sw1 || !sw2) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	tmp  = *sw1;
	*sw1 = *sw2;
	*sw2 = tmp;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (true);
}
