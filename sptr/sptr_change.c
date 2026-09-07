#include <assert.h>
#include <stdbool.h>

#include "sptr/smart_pointer.h"

/**
 * \brief weaken smart ptr
 * \param sptr the smart pointer to weaken
 * \return true if operation was successful false otherwise
 * \ingroup sptr
 **/
bool sptr_weaken(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_W) {
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (true);
	}
	switch (sptr->box->type) {
	case (SPTR_UNIQ):
		LOG(C_MINI__SPTR_LOG_EARG " Illegal operation(uniq)");
		return (false);
	case (SPTR_SHRD):
		assert(sptr->box->sc > 0);
		sptr->box->sc--;
		if (!sptr->box->sc) {
			if (sptr->box->dtor) sptr->box->dtor(sptr->box->raw);
			sptr->box->raw = NULL;
		}
		sptr->box->wc++;
		sptr->smart_strength = SPTR_W;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (true);
	}
}
/**
 * \brief strengthen smart ptr
 * \param sptr the smart pointer to strengthen
 * \return true if operation was successful false otherwise
 * \ingroup sptr
 **/
bool sptr_strengthen(t_sptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_S) {
		LOG(C_MINI__SPTR_LOG_SUCCESS "(already strong)");
		return (true);
	}
	if (!sptr->box || sptr->box->type == SPTR_UNIQ ||
	    !sptr->box->sc) { // catch error
		if (!sptr->box) {
			LOG(C_MINI__SPTR_LOG_EARG);
		} else if (sptr->box->type == SPTR_UNIQ) {
			LOG(C_MINI__SPTR_LOG_ILLUNIQ)
		} else if (!sptr->box->sc) {
			LOG(C_MINI__SPTR_LOG_ILLDATA);
		}
		return (false);
	}
	assert(sptr->box->type == SPTR_SHRD); // should be the only type left
	sptr->box->sc++;
	sptr->box->wc--;
	sptr->smart_strength = SPTR_S;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
/**
 * \brief move the ownership of the pointed value from one sptr to another
 * \param to sptr to move ownership to
 * \param from sptr to move ownership from
 * \return true if operation was successful false otherwise
 * \ingroup sptr
 **/
bool sptr_steal(t_sptr *to, t_sptr *from) {
	if (!from || !to || !from->box || !from->smart_strength ||
	    to->box != from->box || from == to) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (from->box->type == SPTR_SHRD) return (sptr_strengthen(to));
	if (from->box->type != SPTR_UNIQ || from->smart_strength != SPTR_S) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	from->smart_strength = SPTR_W;
	to->smart_strength   = SPTR_S;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
