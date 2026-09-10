#include "sptr/smart_pointer.h"

/**************/
/* const data */
/**************/

/**
 * \brief get the constant raw pointer from a strong pointer
 * \param ptr strong pointer to get the raw pointer from
 * \return the raw pointer
 * \ingroup sptr
 **/
const void *sptr_getsc(const t_strong_ptr sptr) {
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
 * \param ptr weak pointer to get the raw pointer from
 * \return the raw pointer
 * \ingroup sptr
 **/
const void *sptr_getwc(const t_weak_ptr sptr) {
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
 * \param ptr strong pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_gets(t_strong_ptr sptr) {
	return ((void *)sptr_getsc(sptr));
}
/**
 * \brief get the raw pointer from a weak pointer
 * \param ptr weak pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_getw(t_weak_ptr sptr) {
	return ((void *)sptr_getwc(sptr));
}
/**
 * \brief get the raw pointer from a uniq pointer
 * \param ptr uniq pointer to get the raw pointer from
 * \return the raw pointer (non-const)
 * \ingroup sptr
 **/
void *sptr_getu(t_uniq_ptr sptr) {
	if (!sptr.rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (sptr.rcbox->data);
}

/*****************/
/* strong counts */
/*****************/

/**
 * \brief get the strong count from a strong pointer
 * \param ptr strong pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_rcs(const t_strong_ptr ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rc);
}
/**
 * \brief get the strong count from a weak pointer
 * \param ptr weak pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_rcw(const t_weak_ptr ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->rcb.rc);
}
/**
 * \brief get the strong count from a uniq pointer
 * \param ptr uniq pointer to get the strong count from
 * \return the strong count
 * \ingroup sptr
 **/
size_t sptr_get_rcu(const t_uniq_ptr ptr) {
	if (!ptr.rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.rcbox->rc);
}

/***************/
/* weak counts */
/***************/

/**
 * \brief get the weak count from a strong pointer
 * \param ptr strong pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_wcs(const t_strong_ptr ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (((t_wcbox *)ptr)->weak_count);
}
/**
 * \brief get the weak count from a weak pointer
 * \param ptr weak pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_wcw(const t_weak_ptr ptr) {
	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr->weak_count);
}
/**
 * \brief get the weak count from a uniq pointer
 * \param ptr uniq pointer to get the weak count from
 * \return the weak count
 * \ingroup sptr
 **/
size_t sptr_get_wcu(const t_uniq_ptr ptr) {
	if (!ptr.rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr.rcbox->rc - 1);
}
