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
t_strong_ptr *sptr_make_shared_from_raw(void *raw_ptr, t_dtor d) {
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
	return ((t_strong_ptr *)complete_box);
}
/**
 * \brief create a weak pointer from a weak pointer
 * \param weak_ptr weak pointer to copy
 * \param d destructor to use when the object will be deallocated
 * \return the weak pointer on success, NULL on failure
 * \ingroup sptr
 **/
t_weak_ptr *sptr_make_shared_from_weak(t_weak_ptr *weak_ptr, t_dtor d) {
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
t_weak_ptr *sptr_make_shared_from_strong(t_strong_ptr *strong_ptr, t_dtor d) {
	t_weak_ptr *ret = (t_weak_ptr *)strong_ptr;

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
t_strong_ptr *sptr_make_shared_from_uniq(t_uniq_ptr *uniq, t_dtor d) {
	t_wcbox *ret = NULL;

	if (!uniq || !uniq->wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (uniq->owner != true || uniq->wcbox->rcb.rc != 0) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (NULL);
	}
	if (d) {
		if (uniq->wcbox->rcb.dtor && uniq->wcbox->rcb.dtor != d) {
			LOG(C_MINI__SPTR_LOG_ILL);
			return (NULL);
		}
		uniq->wcbox->rcb.dtor = d;
	}
	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (NULL);
	}
	ret->rcb        = uniq->wcbox->rcb;
	ret->weak_count = 0;
	ret->rcb.rc     = 1;
	uniq->wcbox->rcb.rc--;
	uniq->wcbox->rcb.data = NULL;
	uniq->wcbox->rcb.dtor = NULL;
	uniq->owner           = false;
	if (!uniq->wcbox->rcb.rc) free(uniq->wcbox);
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return ((t_strong_ptr *)ret);
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
	ret.wcbox = malloc(sizeof(*ret.wcbox));
	if (!ret.wcbox) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (ret);
	}
	ret.owner  = true;
	*ret.wcbox = (t_wcbox){
	    {raw_ptr, d, 1},
        0
    };
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
t_uniq_ptr sptr_make_uniq_from_strong(t_strong_ptr *strong_ptr, t_dtor d) {
	t_uniq_ptr ret      = (t_uniq_ptr){0};
	t_wcbox   *full_box = (t_weak_ptr *)strong_ptr;

	if (!strong_ptr || strong_ptr->rc != 1) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (ret);
	}
	if (d && strong_ptr->dtor && strong_ptr->dtor != d) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (ret);
	}
	ret.wcbox = malloc(sizeof(*ret.wcbox));
	if (!ret.wcbox) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return (ret);
	}
	ret.owner  = true;
	*ret.wcbox = *full_box;
	*full_box  = (t_wcbox){0};
	if (d) ret.wcbox->rcb.dtor = d;
	if (full_box->weak_count == 0) free(strong_ptr);
	LOG(C_MINI__SPTR_LOG_SUCCESS);
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

	if (!uniq.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (ret);
	}
	if (d && uniq.wcbox->rcb.dtor && uniq.wcbox->rcb.dtor != d) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (ret);
	}
	ret.wcbox = uniq.wcbox;
	ret.wcbox->weak_count++;
	ret.owner = false;
	if (d) ret.wcbox->rcb.dtor = d;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ret);
}
