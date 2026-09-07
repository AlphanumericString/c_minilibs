#include "sptr/smart_pointer.h"

/**
 * \brief create a copy of a smart pointer
 * \param sptr the smart pointer to copy
 * \return the copy of the smart pointer or a zero'ed sptr if it fails
 * \ingroup sptr
 **/
t_sptr sptr_copy(t_sptr *sptr) {
	t_sptr ret;

	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
		ret                = *sptr;
		ret.smart_strength = SPTR_W;
		ret.box->wc++;
		break;
	case SPTR_SHRD:
		ret = *sptr;
		if (ret.smart_strength == SPTR_S) ret.box->sc++;
		else ret.box->wc++;
		break;
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}

/**
 * \brief create a weak copy of a smart pointer
 * \param sptr the smart pointer to copy
 * \return the weak copy or a zero'ed sptr if it fails
 * \ingroup sptr
 **/
t_sptr sptr_weakcopy(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
	case SPTR_SHRD:
		sptr->box->wc++;
		break;
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_sptr){.smart_strength = SPTR_W, .box = sptr->box});
}
/**
 * \brief create a strong copy of a smart pointer
 * \param sptr the smart pointer to copy
 * \return the strong copy or a zero'ed sptr if it fails
 * \ingroup sptr
 **/
t_sptr sptr_strongcopy(t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_UNIQ:
		LOG(C_MINI__SPTR_LOG_EARG "Illegal operation");
		return ((t_sptr){0});
	case SPTR_SHRD:
		sptr->box->sc++;
		break;
	}
	return ((t_sptr){.smart_strength = SPTR_S, .box = sptr->box});
}
