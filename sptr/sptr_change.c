#include <assert.h>
#include <stdbool.h>

#include "sptr/smart_pointer.h"

bool sptr_weaken(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_WEAK) {
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (true);
	}
	switch (sptr->box->type) {
	case (SPTR_UNIQ):
		LOG(C_MINI__SPTR_LOG_EARG " Illegal operation(uniq)");
		return (false);
	case (SPTR_SHARED):
		assert(sptr->box->sc > 0);
		sptr->box->sc--;
		if (!sptr->box->sc) {
			if (sptr->box->closure) sptr->box->closure(sptr->box->raw);
			sptr->box->raw = NULL;
		}
		sptr->box->wc++;
		sptr->smart_strength = SPTR_WEAK;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (true);
	}
}
bool sptr_strengthen(t_sptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_STRONG) {
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
	assert(sptr->box->type == SPTR_SHARED); // should be the only type left
	sptr->box->sc++;
	sptr->box->wc--;
	sptr->smart_strength = SPTR_STRONG;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
bool sptr_steal(t_sptr *to, t_sptr *from) {
	if (!from || !to || !from->box || !from->smart_strength ||
	    to->box != from->box || from == to) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (from->box->type == SPTR_SHARED) return (sptr_strengthen(to));
	if (from->box->type != SPTR_UNIQ || from->smart_strength != SPTR_STRONG) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	from->smart_strength = SPTR_WEAK;
	to->smart_strength   = SPTR_STRONG;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
