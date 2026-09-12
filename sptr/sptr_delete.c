#include <stdlib.h>

#include "sptr/smart_pointer.h"

/**
 * \brief delete a strong pointer
 * \param ptr strong pointer to delete
 * \return true if the strong pointer was deleted, false otherwise
 * \ingroup sptr
 **/
bool sptr_deletes(t_strong_ptr *ptr) {
	if (!ptr || !ptr->rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	ptr->rc--;
	if (!ptr->rc) {
		if (ptr->dtor) ptr->dtor(ptr->data);
		ptr->data = NULL;
	}
	if (!ptr->rc && !((t_wcbox *)ptr)->weak_count) free(ptr);
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
/**
 * \brief delete a weak pointer
 * \param ptr weak pointer to delete
 * \return true if the weak pointer was deleted, false otherwise
 * \ingroup sptr
 **/
bool sptr_deletew(t_weak_ptr *ptr) {
	if (!ptr || !ptr->rcb.rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	ptr->weak_count--;
	if (!ptr->weak_count && !ptr->rcb.rc) free(ptr);
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
/**
 * \brief delete a uniq pointer
 * \param ptr uniq pointer to delete
 * \return true if the uniq pointer was deleted, false otherwise
 * \ingroup sptr
 **/
bool sptr_deleteu(t_uniq_ptr ptr) {
	if (!ptr.wcbox || !ptr.wcbox->rcb.rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (ptr.owner == true ^ ptr.wcbox->rcb.rc == 1) {
		LOG(C_MINI__SPTR_LOG_ILL);
		return (false);
	}
	if (ptr.owner) {
		if (ptr.wcbox->rcb.dtor) ptr.wcbox->rcb.dtor(ptr.wcbox->rcb.data);
		ptr.wcbox->rcb.data = NULL;
		ptr.wcbox->rcb.rc--;
	}
	ptr.wcbox->weak_count--;
	if (ptr.wcbox->weak_count == 0) {
		free(ptr.wcbox);
		ptr.wcbox = NULL;
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
