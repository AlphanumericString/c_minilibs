#include <assert.h>
#include <stdbool.h>

#include "sptr/smart_pointer.h"

/**
 * \brief strengthens a weak pointer to a strong pointer
 * \param sptr weak pointer to strengthen
 * \return strong pointer if successful, NULL otherwise
 * \note if rcb.rc == 0, it will fail
 * \warning THE GIVEN SPTR IS NO LONGER VALID
 * \ingroup sptr
 **/
t_strong_ptr *sptr_strengthen(t_weak_ptr *sptr) {
	if (!sptr || sptr->rcb.rc == 0) {
		LOG(C_MINI__SPTR_LOG_ILLDATA);
		return (NULL);
	}
	sptr->rcb.rc++;
	sptr->weak_count--;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_strong_ptr *)sptr);
}
/**
 * \brief weakens a strong pointer to a weak pointer
 * \param sptr strong pointer to weaken
 * \return weak pointer if successful, NULL otherwise
 * \warning THE GIVEN SPTR IS NO LONGER VALID
 * \ingroup sptr
 **/
t_weak_ptr *sptr_weaken(t_strong_ptr *sptr) {
	t_weak_ptr *ret;
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	ret = (t_weak_ptr *)sptr;
	ret->rcb.rc--;
	if (!ret->rcb.rc) {
		if (ret->rcb.dtor) ret->rcb.dtor(ret->rcb.data);
		ret->rcb.data = NULL;
	}
	ret->weak_count++;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}
/**
 * \brief steals the ownership of a unique pointer
 * \param to unique pointer to steal to
 * \param from unique pointer to steal from
 * \return true if successful, false otherwise
 * \note if to.rcbox != from.rcbox, it will fail
 * \ingroup sptr
 **/
bool sptr_steal(t_uniq_ptr *to, t_uniq_ptr *from) {
	if (!to || !from || !to->wcbox || !from->wcbox || from->owner == false) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (to->wcbox != from->wcbox) {
		LOG(C_MINI__SPTR_LOG_ILLDATA);
		return (false);
	}
	to->owner   = true;
	from->owner = false;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
