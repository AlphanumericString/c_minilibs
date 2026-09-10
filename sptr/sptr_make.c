#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "sptr/smart_pointer.h"

/**
 * \brief create a strong pointer from a raw pointer
 * \param raw_ptr pointer to the object to wrap
 * \param d destructor to use when the object will be deallocated
 * \return the strong pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_strong_ptr sptr_make_shared_from_raw(void *raw_ptr, t_dtor d) {
	t_wcbox *complete_box;

	if (!raw_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	complete_box = malloc(sizeof(*complete_box));
	if (!complete_box) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (NULL);
	}
	*complete_box = (t_wcbox){
	    (t_rcbox){raw_ptr, d, 1},
        0
    };
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_strong_ptr)complete_box);
}
/**
 * \brief create a weak pointer from a weak pointer
 * \param weak_ptr weak pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the weak pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_weak_ptr sptr_make_shared_from_weak(t_weak_ptr weak_ptr, t_dtor d) {
	if (!weak_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (d) {
		if (weak_ptr->rcb.dtor && weak_ptr->rcb.dtor != d) {
			LOG(C_MINI__SPTR_LOG_ILL);
			return (NULL);
		}
		weak_ptr->rcb.dtor = d;
	}
	weak_ptr->weak_count++;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (weak_ptr);
}
/**
 * \brief create a weak pointer from a strong pointer
 * \param strong_ptr strong pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the weak pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_weak_ptr sptr_make_shared_from_strong(t_strong_ptr strong_ptr, t_dtor d) {
	t_weak_ptr ret = (t_weak_ptr)strong_ptr;

	if (!strong_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (d) {
		if (strong_ptr->dtor && strong_ptr->dtor != d) {
			LOG(C_MINI__SPTR_LOG_ILL);
			return (NULL);
		}
		strong_ptr->dtor = d;
	}
	ret->weak_count++;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}
/**
 * \brief create a strong pointer from a uniq pointer
 * \param uniq uniq pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the strong pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_strong_ptr sptr_make_shared_from_uniq(t_uniq_ptr *uniq, t_dtor d) {
	t_wcbox *ret = NULL;

	if (!uniq || !uniq->rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (uniq->owner != true) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (NULL);
	}
	if (d) {
		if (uniq->rcbox->dtor && uniq->rcbox->dtor != d) {
			LOG(C_MINI__SPTR_LOG_ILL);
			return (NULL);
		}
		uniq->rcbox->dtor = d;
	}
	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (NULL);
	}
	ret->rcb        = *(uniq->rcbox);
	ret->weak_count = 0;
	ret->rcb.rc     = 1;
	uniq->rcbox->rc--;
	uniq->rcbox->data = NULL;
	uniq->rcbox->dtor = NULL;
	uniq->owner       = false;
	if (!uniq->rcbox->rc) free(uniq->rcbox);
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_strong_ptr)ret);
}

// *----*
// |uniq|
// *----*

/**
 * \brief create a uniq pointer from a raw pointer
 * \param raw_ptr pointer to the object to wrap
 * \param d destructor to use when the object will be deallocated
 * \return the uniq pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_uniq_ptr sptr_make_uniq_from_raw(void *raw_ptr, t_dtor d) {
	t_uniq_ptr ret = (t_uniq_ptr){0};

	if (!raw_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (ret);
	}
	ret.rcbox = malloc(sizeof(*ret.rcbox));
	if (!ret.rcbox) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (ret);
	}
	ret.owner  = true;
	*ret.rcbox = (t_rcbox){raw_ptr, d, 1};
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}
/**
 * \brief create a uniq pointer from a strong pointer
 * \param strong_ptr strong pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the uniq pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_uniq_ptr sptr_make_uniq_from_strong(t_strong_ptr strong_ptr, t_dtor d) {
	t_uniq_ptr ret = (t_uniq_ptr){0};

	if (!strong_ptr || strong_ptr->rc != 1) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (ret);
	}
	if (d && strong_ptr->dtor && strong_ptr->dtor != d) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (ret);
	}
	ret.rcbox = malloc(sizeof(*ret.rcbox));
	if (!ret.rcbox) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (ret);
	}
	ret.owner   = true;
	*ret.rcbox  = *strong_ptr;
	*strong_ptr = (t_rcbox){0};
	if (d) ret.rcbox->dtor = d;
	if (((t_wcbox *)strong_ptr)->weak_count == 0) free(strong_ptr);
	return (ret);
}
/**
 * \brief create a uniq pointer from a uniq pointer
 * \param uniq uniq pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the uniq pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_uniq_ptr sptr_make_uniq_from_uniq(t_uniq_ptr uniq, t_dtor d) {
	t_uniq_ptr ret = (t_uniq_ptr){0};

	if (!uniq.rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (ret);
	}
	if (d && uniq.rcbox->dtor && uniq.rcbox->dtor != d) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (ret);
	}
	ret.rcbox = uniq.rcbox;
	ret.rcbox->rc++;
	ret.owner = false;
	if (d) ret.rcbox->dtor = d;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}
