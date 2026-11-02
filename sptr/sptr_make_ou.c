#include <stdbool.h>
#include <stdlib.h>

#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

// *-----*
// |owned|
// *-----*

/**
 * \brief create a owned pointer from a raw pointer
 * \param raw_ptr pointer to the object to wrap
 * \param d destructor to use when the object will be deallocated
 * \return owned pointer with owner set to true on success, or
 * zero-filled structure on failure
 * \ingroup sptr
**/
t_owned_ptr sptr_make_owned_from_raw(void *raw_ptr, t_cm_dtor d) {
	t_owned_ptr ret = (t_owned_ptr){0};
	t_fbox     *f;

	if (!raw_ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (ret);
	}
	f = malloc(sizeof(*f));
	if (!f) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return (ret);
	}
	*f  = (t_fbox){.data = raw_ptr, .dtor = d, .rc = 1, .wc = 0};
	ret = (t_owned_ptr){.owner = true, .meta = f, .data = raw_ptr};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief create a owned pointer from a strong pointer
 * \param sp the strong pointer to convert
 * \param d destructor to use when the object will be deallocated
 * \return owned pointer with owner set to true on success, or
 * zero-filled structure on failure
 * \note requires strong count == 1; if weak count == 0 frees or clears
 * strong_ptr
 * \ingroup sptr
**/
t_owned_ptr sptr_make_owned_from_strong(t_strong_ptr *sp, t_cm_dtor d) {
	t_owned_ptr ret = (t_owned_ptr){0};

	if (!sp || sptr_get_rc(sp) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (ret);
	}
	if (d && sptr_get_dtor(sp) && sptr_get_dtor(sp) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return (ret);
	}
	ret = (t_owned_ptr){.owner = true, .meta = sp->meta, .data = sp->data};
	if (d) ret.meta->dtor = d;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*sp = (t_strong_ptr){0};
	return (ret);
}
/**
 * \brief create a borrowed (non-owning) owned pointer from an existing
 * owned pointer
 * \param owned owned pointer to borrow from
 * \param d destructor to use when the object will be deallocated
 * \return borrowed owned pointer with owner set to false on success,
 * or zero-filled structure on failure
 * \note increments weak count; the returned owned pointer has owner set to
 * false
 * \ingroup sptr
**/
t_owned_ptr sptr_make_owned_from_owned(t_owned_ptr *owned, t_cm_dtor d) {
	t_owned_ptr ret = (t_owned_ptr){0};

	if (!owned || !owned->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (ret);
	}
	if (d && sptr_get_dtor(owned) && sptr_get_dtor(owned) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return (ret);
	}
	sptri_wc_inc(owned->meta);
	ret       = *owned;
	ret.owner = false;
	if (d) ret.meta->dtor = d;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief create an owned pointer from an existing uniq pointer
 * \param uptr unique pointer to transform
 * \param d dtor to set/change to. (if uptr has a dtor and doesn't match d,
 * fails)
 * \return the owned pointer structure (ownership set to true) or a zeroed out
 * owned structure
 * \note clears out uptr internal values
 * \ingroup sptr
**/
t_owned_ptr sptr_make_owned_from_uniq(t_uniq_ptr *uptr, t_cm_dtor d) {
	t_owned_ptr ret = (t_owned_ptr){0};
	t_fbox     *f;

	if (!uptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (ret);
	}
	if (d && uptr->dtor && uptr->dtor != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return (ret);
	}
	f = malloc(sizeof(*f));
	if (!f) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EALLOC)
		return (ret);
	}
	*f  = (t_fbox){.data = uptr->data, .dtor = uptr->dtor, 1, 0};
	ret = (t_owned_ptr){.owner = true, .meta = f, .data = f->data};
	if (d) f->dtor = d;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*uptr = (t_uniq_ptr){0};
	return (ret);
}

// *----*
// |uniq|
// *----*

/**
 * \brief create a new uniq pointer from a raw pointer
 * \param ptr the source raw pointer (fails on null).
 * \param d the dtor to set.
 * \return the newly created uniq pointer or a zeroed out uniq pointer
 * \ingroup sptr
**/
t_uniq_ptr sptr_make_uniq_from_raw(void *ptr, t_cm_dtor d) {
	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_uniq_ptr){.data = ptr, .dtor = d});
}
/**
 * \brief create a new uniq pointer from a weak pointer
 * \param ptr the source weak pointer
 * \param _ the dtor to try to set.
 * \return a zeroed out uniq pointer.
 * \warning this is an illegal operation. Implemented purely to warn people.
 * \ingroup sptr
**/
t_uniq_ptr sptr_make_uniq_from_weak(const t_weak_ptr *ptr, t_cm_dtor _) {
	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_ILL)
	return ((t_uniq_ptr){0});
}
/**
 * \brief create a new uniq pointer from a strong pointer
 * \param ptr the source strong pointer
 * \param d the dtor to try to set. (if ptr has a dtor and d mismatch said dtor,
 * fails)
 * \return the newly created uniq pointer or a zeroed out uniq pointer
 * \note zeroes out the fields of ptr for data and dtor.
 * \ingroup sptr
**/
t_uniq_ptr sptr_make_uniq_from_strong(t_strong_ptr *ptr, t_cm_dtor d) {
	t_uniq_ptr ret;

	if (!ptr || sptr_get_rc(ptr) != 1 || !ptr->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	if (d && sptr_get_dtor(ptr) && sptr_get_dtor(ptr) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_uniq_ptr){0});
	}
	ret = (t_uniq_ptr){.data = sptr_get(ptr), .dtor = sptr_get_dtor(ptr)};
	sptri_rc_dec(ptr->meta);
	if (!sptri_wc_get(ptr->meta)) free(ptr->meta);
	if (d) sptr_set_dtor(&ret, d);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*ptr = (t_strong_ptr){0};
	return (ret);
}
/**
 * \brief create a new uniq pointer from an owned pointer
 * \param ptr the source owned pointer
 * \param d the dtor to try to set. (if ptr has a dtor and d doesn't match said
 * dtor, fails)
 * \return the newly created uniq pointer or a zeroed out uniq pointer
 * \note zeroes out the fields of ptr for data and dtor.
 * \ingroup sptr
**/
t_uniq_ptr sptr_make_uniq_from_owned(t_owned_ptr *ptr, t_cm_dtor d) {
	t_uniq_ptr ret;

	if (!ptr || !ptr->owner || sptr_get_rc(ptr) != 1 || !sptr_get(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	if (d && sptr_get_dtor(ptr) && sptr_get_dtor(ptr) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_uniq_ptr){0});
	}
	ret = (t_uniq_ptr){.data = sptr_get(ptr), .dtor = sptr_get_dtor(ptr)};
	sptri_rc_dec(ptr->meta);
	ptr->owner = false;
	if (!sptri_wc_get(ptr->meta)) free(ptr->meta);
	if (d) ret.dtor = d;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*ptr = (t_owned_ptr){0};
	return (ret);
}
/**
 * \brief make a uniq pointer from an other uniq pointer
 * \param ptr the source uniq pointer to replace
 * \param d the dtor to try to set. (if d != ptr.dtor, fails)
 * \return the t_uniq_ptr containing the values of ptr and updated dtor or
 * zeroed uniq pointer
 * \note empties out the source uniq pointer ptr.
 * \ingroup sptr
**/
t_uniq_ptr sptr_make_uniq_from_uniq(t_uniq_ptr *ptr, t_cm_dtor d) {
	t_uniq_ptr ret;

	if (!ptr || !ptr->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_uniq_ptr){0});
	}
	if (d && sptr_get_dtor(ptr) && sptr_get_dtor(ptr) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_uniq_ptr){0});
	}
	ret = (t_uniq_ptr){.data = ptr->data, .dtor = ptr->dtor};
	if (d) ret.dtor = d;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*ptr = (t_uniq_ptr){0};
	return (ret);
}
