#include "sptr/smart_pointer.h"

/**
 * \brief return the raw pointer
 * \param sptr the smart pointer to get the raw pointer from
 * \return the raw pointer
 * \ingroup sptr
 **/
void *sptr_get(const t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->box->raw);
}
/**
 * \brief get the weak count
 * \param sptr the smart pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_wc(const t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->box->wc);
}
/**
 * \brief get the strong count
 * \param sptr the smart pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_sc(const t_sptr *sptr) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->box->sc);
}
/**
 * \brief get the dtor
 * \param sptr the smart pointer to get the dtor from
 * \return the dtor
 * \ingroup sptr
 **/
void (*sptr_get_dtor(const t_sptr *sptr))(void *) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->box->dtor);
}
