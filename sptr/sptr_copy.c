#include "sptr/smart_pointer.h"

t_sptr sptr_copy(t_sptr *sptr) {
	t_sptr ret;

	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
		ret                = *sptr;
		ret.smart_strength = SPTR_WEAK;
		ret.box->wc++;
		break;
	case SPTR_SHARED:
		ret = *sptr;
		if (ret.smart_strength == SPTR_STRONG) ret.box->sc++;
		else ret.box->wc++;
		break;
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}

t_sptr sptr_weakcopy(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
	case SPTR_SHARED:
		sptr->box->wc++;
		break;
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_sptr){.smart_strength = SPTR_WEAK, .box = sptr->box});
}
t_sptr sptr_strongcopy(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
		LOG(C_MINI__SPTR_LOG_EARG "Illegal operation");
		return ((t_sptr){0});
	case SPTR_SHARED:
		sptr->box->sc++;
		break;
	}
	return ((t_sptr){.smart_strength = SPTR_STRONG, .box = sptr->box});
}
