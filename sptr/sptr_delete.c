#include <stdlib.h>

#define C_MINI__INTERNAL
#include "sptr/smart_pointer.h"

/**
 * \brief decrement strong reference count and delete or destroy data if count
 * reaches 0
 * \param ptr strong pointer to decrement or delete
 * \return true on success, false on failure
 * \note decrements strong count; if strong count reaches 0, calls destructor on
 * data and sets data to NULL; frees strong pointer box if both strong count and
 * weak count reach 0
 * \ingroup sptr
 **/
bool sptr_deletes(t_strong_ptr *ptr) {
	if (!ptr || !ptr->meta || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	sptri_rc_dec(ptr->meta);
	if (!sptr_get_rc(ptr)) {
		if (ptr->meta->dtor) ptr->meta->dtor(ptr->data);
		ptr->meta->dtor = NULL;
		ptr->meta->data = NULL;
		if (!sptri_wc_get(ptr->meta)) free(ptr->meta);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS);
	*ptr = (typeof(*ptr)){0};
	return (true);
}
/**
 * \brief decrement weak reference count and free memory if all counts reach 0
 * \param ptr weak pointer to decrement or delete
 * \return true on success, false on failure
 * \note decrements weak count; frees memory if both weak count and strong count
 * reach 0
 * \ingroup sptr
 **/
bool sptr_deletew(t_weak_ptr *ptr) {
	if (!ptr || !ptr->meta || !sptr_get_wc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	sptri_wc_dec(ptr->meta);
	if (!sptri_wc_get(ptr->meta) && !sptri_rc_get(ptr->meta)) free(ptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS);
	*ptr = (typeof(*ptr)){0};
	return (true);
}
/**
 * \brief delete or release a reference to an owned pointer
 * \param ptr owned pointer to delete or release
 * \return true on success, false on failure
 * \note if smart pointer is the owner, calls destructor on data and decrements
 * strong count; otherwise decrements weak count; frees memory if both strong
 * count and weak count reach 0
 * \ingroup sptr
 **/
bool sptr_deleteo(t_owned_ptr *ptr) {
	if (!ptr || !ptr->meta || (!ptr->owner && !sptri_wc_get(ptr->meta))) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (ptr->owner == true && sptr_get_rc(ptr) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA);
		return (false);
	}
	if (ptr->owner) {
		if (ptr->meta->dtor) ptr->meta->dtor(ptr->data);
		ptr->meta->dtor = NULL;
		ptr->meta->data = NULL;
		sptri_rc_dec(ptr->meta);
	} else {
		sptri_wc_dec(ptr->meta);
	}
	if (!sptri_wc_get(ptr->meta) && !sptri_rc_get(ptr->meta)) free(ptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS);
	*ptr = (typeof(*ptr)){0};
	return (true);
}
/**
 * \brief delete a uniq pointer
 * \param ptr the uniq pointer to delete
 * \return true on success, false on error
 * \note all fields are zeroed out for safety
 * \ingroup sptr
 **/
bool sptr_deleteu(t_uniq_ptr *ptr) {
	if (!ptr || !ptr->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (ptr->dtor) ptr->dtor(ptr->data);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS);
	*ptr = (typeof(*ptr)){0};
	return (true);
}
