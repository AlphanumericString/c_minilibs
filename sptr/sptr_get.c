#include "sptr/smart_pointer.h"

/**************/
/* const data */
/**************/

/**
 * \brief get the constant raw pointer from a strong pointer
 * \param sptr strong pointer to get the raw pointer from
 * \return the raw pointer
 * \ingroup sptr
 **/
const void *sptr_getsc(const t_strong_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	if (!sptr->rc) return (NULL);
	return (sptr->data);
}
/**
 * \brief get the constant raw pointer from a weak pointer
 * \param sptr weak pointer to get the raw pointer from
 * \return the raw pointer
 * \ingroup sptr
 **/
const void *sptr_getwc(const t_weak_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	if (!sptr->rcb.rc) return (NULL);
	return (sptr->rcb.data);
}

/********/
/* data */
/********/

/**
 * \brief get the raw pointer from a strong pointer
 * \param sptr strong pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_gets(t_strong_ptr *sptr) {
	return ((void *)sptr_getsc(sptr));
}
/**
 * \brief get the raw pointer from a weak pointer
 * \param sptr weak pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_getw(t_weak_ptr *sptr) {
	return ((void *)sptr_getwc(sptr));
}
/**
 * \brief get the raw pointer from a uniq pointer
 * \param sptr uniq pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_getu(t_uniq_ptr sptr) {
	if (!sptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr.wcbox->rcb.data);
}

/*****************/
/* strong counts */
/*****************/

/**
 * \brief get the strong count from a strong pointer
 * \param sptr strong pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_strongcount_s(const t_strong_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->rc);
}
/**
 * \brief get the strong count from a weak pointer
 * \param sptr weak pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_strongcount_w(const t_weak_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->rcb.rc);
}
/**
 * \brief get the strong count from a uniq pointer
 * \param sptr uniq pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_ownercount_u(const t_uniq_ptr sptr) {
	if (!sptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr.wcbox->rcb.rc);
}

/***************/
/* weak counts */
/***************/

/**
 * \brief get the weak count from a strong pointer
 * \param sptr strong pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_weakcount_s(const t_strong_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (((t_wcbox *)sptr)->weak_count);
}
/**
 * \brief get the weak count from a weak pointer
 * \param sptr weak pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_weakcount_w(const t_weak_ptr *sptr) {
	if (!sptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr->weak_count);
}
/**
 * \brief get the weak count from a uniq pointer
 * \param sptr uniq pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_weakcount_u(const t_uniq_ptr sptr) {
	if (!sptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr.wcbox->weak_count);
}

/******************/
/* dtor (get+set) */
/******************/

/**
 * \brief get the dtor from a strong pointer
 * \param ptr uniq pointer to get the strong count from
 * \return the dtor
 * \ingroup sptr
 **/
t_dtor sptr_get_dtors(const t_strong_ptr *ptr) {
	if (!ptr || !ptr->rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->dtor);
}
/**
 * \brief get the dtor from a weak pointer
 * \param ptr uniq pointer to get the weak count from
 * \return the dtor
 * \ingroup sptr
 **/
t_dtor sptr_get_dtorw(const t_weak_ptr *ptr) {
	if (!ptr || !ptr->weak_count || !ptr->rcb.rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rcb.dtor);
}
/**
 * \brief get the dtor from a uniq pointer
 * \param ptr uniq pointer to get the uniq count from
 * \return the dtor
 * \ingroup sptr
 **/
t_dtor sptr_get_dtoru(const t_uniq_ptr ptr) {
	if (!ptr.wcbox || !ptr.wcbox->weak_count || !ptr.wcbox->rcb.rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.wcbox->rcb.dtor);
}

/**
 * \brief get the dtor of a strong pointer
 * \param ptr uniq pointer to get the strong count from
 * \param dtor the new dtor value to set
 * \return true on success, flase otherwise
 * \ingroup sptr
 **/
bool sptr_set_dtors(t_strong_ptr *ptr, t_dtor dtor) {
	if (!ptr || !ptr->rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	ptr->dtor = dtor;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
/**
 * \brief get the dtor of a weak pointer
 * \param ptr uniq pointer to get the weak count from
 * \param dtor the new dtor value to set
 * \return true on success, flase otherwise
 * \note weak ptr cannot change the dtor. they do not own the data.
 * \ingroup sptr
 **/
bool sptr_set_dtorw(t_weak_ptr *ptr, t_dtor dtor) {
	if (!ptr || !ptr->weak_count || !ptr->rcb.rc) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	(void)dtor;
	LOG(C_MINI__SPTR_LOG_ILL);
	return (false);
}
/**
 * \brief get the dtor of a uniq pointer
 * \param ptr uniq pointer to get the uniq count from
 * \param dtor the new dtor value to set
 * \return true on success, flase otherwise
 * \ingroup sptr
 **/
bool sptr_set_dtoru(t_uniq_ptr ptr, t_dtor dtor) {
	if (!ptr.wcbox || !ptr.owner || !ptr.wcbox->weak_count ||
	    ptr.wcbox->rcb.rc != 1) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	ptr.wcbox->rcb.dtor = dtor;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (true);
}
