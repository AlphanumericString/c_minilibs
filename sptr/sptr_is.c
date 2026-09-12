#include "sptr/smart_pointer.h"

/***********/
/* is_uniq */
/***********/

/**
 * \brief check if a strong pointer is unique (strong count)
 * \param ptr strong pointer to check
 * \return true if the strong pointer is unique, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_uniqs(const t_strong_ptr *ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rc == 1);
}
/**
 * \brief check if a weak pointer is unique (by strong count)
 * \param ptr weak pointer to check
 * \return true if the weak pointer is unique, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_uniqw(const t_weak_ptr *ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rcb.rc == 1);
}
/**
 * \brief check if a uniq pointer is solely owned by a uniq ptr
 * \param ptr uniq pointer to check
 * \return true if the uniq pointer is unique, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_uniqu(const t_uniq_ptr ptr) {
	if (!ptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.wcbox->rcb.rc == 1);
}

/*************/
/* is_shared */
/*************/

/**
 * \brief check if a strong pointer is shared (strong count > 1)
 * \param ptr strong pointer to check
 * \return true if the strong pointer is shared, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_shareds(const t_strong_ptr *ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rc > 1);
}
/**
 * \brief check if a weak pointer is shared (strong count)
 * \param ptr weak pointer to check
 * \return true if the weak pointer is shared, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_sharedw(const t_weak_ptr *ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rcb.rc > 1);
}
/**
 * \brief check if a uniq pointer is shared
 * \param ptr uniq pointer to check
 * \return true if the uniq pointer is shared, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_sharedu(const t_uniq_ptr ptr) {
	if (!ptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.wcbox->rcb.rc == 1 && ptr.owner);
}

/**********/
/* is_own */
/**********/

/**
 * \brief check if a strong pointer is the sole owner of the data
 * \param ptr strong pointer to check
 * \return true if the strong pointer is the owner, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_owns(const t_strong_ptr *ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rc == 1);
}
/**
 * \brief check if a weak pointer is the sole owner of the data
 * \param ptr weak pointer to check
 * \return true if the weak pointer is the owner, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_ownw(const t_weak_ptr *ptr) {
	(void)ptr;
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	if (!ptr->rcb.rc && ptr->weak_count == 1) return (true);
	return (false);
}
/**
 * \brief check if a uniq pointer is the owner of the data
 * \param ptr uniq pointer to check
 * \return true if the uniq pointer is the owner, false otherwise
 * \ingroup sptr
 **/
bool sptr_is_ownu(const t_uniq_ptr ptr) {
	if (!ptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.owner);
}
