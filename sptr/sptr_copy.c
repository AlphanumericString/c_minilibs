#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

/**
 * \brief copy a strong pointer by incrementing its strong reference count
 * \param strong_ptr strong pointer to copy
 * \return the strong pointer on success, NULL on failure
 * \note fails if strong_ptr is NULL or its strong count is 0
 * \ingroup sptr
**/
t_strong_ptr *sptr_copys(t_strong_ptr *strong_ptr) {
	if (!strong_ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	if (!sptr_get_rc(strong_ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA)
		return (NULL);
	}
	sptri_rc_inc(strong_ptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (strong_ptr);
}
/**
 * \brief copy a weak pointer by incrementing its weak reference count
 * \param weak_ptr weak pointer to copy
 * \return the weak pointer on success, NULL on failure
 * \note fails if argument is NULL or its weak count is 0
 * \ingroup sptr
**/
t_weak_ptr *sptr_copyw(t_weak_ptr *weak_ptr) {
	if (!weak_ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	if (!sptr_get_wc(weak_ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA)
		return (NULL);
	}
	sptri_wc_inc(weak_ptr->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (weak_ptr);
}
/**
 * \brief create a borrowed (non-owning) copy of a owned pointer
 * \param owned_ptr owned pointer to copy
 * \return new non-owning owned pointer with incremented weak count, or
 * zero-filled owned pointer structure on failure
 * \note the returned owned pointer has owner set to false and increments
 * weak count; fails if argument is NULL or strong count is 0
 * \ingroup sptr
**/
t_owned_ptr sptr_copyo(t_owned_ptr *owned_ptr) {
	t_owned_ptr ret;

	if (!owned_ptr || !owned_ptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_owned_ptr){0});
	}
	if (!sptr_get_rc(owned_ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILLDATA)
		return ((t_owned_ptr){0});
	}
	sptri_wc_inc(owned_ptr->meta);
	ret       = *owned_ptr;
	ret.owner = false;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief copies a uniq pointer to a returned value.
 * \param ptr the uniq pointer to use as a source
 * \return a new uniq pointer with values from the source uniq
 * \note the uniq source pointer given is zeroed out
 * \ingroup sptr
**/
t_uniq_ptr sptr_copyu(t_uniq_ptr *ptr) {
	t_uniq_ptr ret;

	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	ret  = (t_uniq_ptr){.data = ptr->data, .dtor = ptr->dtor};
	*ptr = (t_uniq_ptr){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
