#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

/***********/
/* is_uniq */
/***********/

/**
 * \brief check if a strong pointer is unique (strong count == 1)
 * \param ptr strong pointer to check
 * \return true if the strong pointer is unique, false otherwise
 * \ingroup sptr
**/
bool sptr_is_uniqs(const t_cm_strong_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) == 1);
}
/**
 * \brief check if a weak pointer is unique (strong count == 1)
 * \param ptr weak pointer to check
 * \return true if the weak pointer is unique, false otherwise
 * \ingroup sptr
**/
bool sptr_is_uniqw(const t_cm_weak_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) == 1);
}
/**
 * \brief check if a owned pointer is unique (strong count == 1)
 * \param ptr owned pointer to check
 * \return true if the owned pointer is unique, false otherwise
 * \ingroup sptr
**/
bool sptr_is_uniqo(const t_cm_owned_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) == 1);
}
/**
 * \brief check if a uniq pointer is uniq (strong count == 1)
 * \param u uniq pointer to check
 * \return true if the strong count is 1, false otherwise
 * \ingroup sptr
**/
bool sptr_is_uniqu(const t_cm_uniq_ptr *u) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(u) == 1);
}

/*************/
/* is_shared */
/*************/

/**
 * \brief check whether a shared (strong) pointer is shared
 * \param ptr the shared (strong) pointer to check
 * \return true if the shared (strong) pointer is shared, false otherwise
 * \ingroup sptr
**/
bool sptr_is_shareds(const t_cm_strong_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) >= 1);
}
/**
 * \brief check whether a shared (weak) pointer is shared
 * \param ptr the shared (weak) pointer to check
 * \return true if the shared (weak) pointer is shared, false otherwise
 * \ingroup sptr
**/
bool sptr_is_sharedw(const t_cm_weak_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) >= 1);
}
/**
 * \brief check whether an owned pointer is shared
 * \param ptr owned pointer to check
 * \return true if the owned pointer is shared, false otherwise
 * \ingroup sptr
**/
bool sptr_is_sharedo(const t_cm_owned_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptr_get_rc(ptr) >= 1);
}
/**
 * \brief checks whether a uniq pointer is shared
 * \param u the uniq pointer to check
 * \return true if the uniq pointer is shared, otherwise false
 * \note as uniq is "uniq" it always return false
 * \ingroup sptr
**/
bool sptr_is_sharedu(const t_cm_uniq_ptr *u) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	(void)u;
	return (false);
}
