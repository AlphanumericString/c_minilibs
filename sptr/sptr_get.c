#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

/**************/
/* const data */
/**************/

/**
 * \brief get the constant pointer from a strong pointer
 * \param sptr strong pointer to get the raw pointer from
 * \return the raw pointer, or NULL if argument is NULL or strong count is 0
 * \note returns NULL if the strong reference count is 0
 * \ingroup sptr
**/
const void *sptr_getsc(const t_strong_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	if (!sptr_get_rc(sptr)) return (NULL);
	return (sptr->data);
}
/**
 * \brief get the constant pointer from a weak pointer
 * \param sptr weak pointer to get the raw pointer from
 * \return the raw pointer, or NULL if argument is NULL or strong count is 0
 * \note returns NULL if the underlying object strong count is 0 (data
 * destroyed)
 * \ingroup sptr
**/
const void *sptr_getwc(const t_weak_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	if (!sptr_get_rc(sptr)) return (NULL);
	return (sptr->meta->data);
}
/**
 * \brief get the const data from a owned pointer
 * \param o owned pointer to get the raw pointer from
 * \return the const pointer, or NULL if an error occurred
 * \ingroup sptr
**/
const void *sptr_getoc(const t_owned_ptr *o) {
	if (!o || !o->meta || sptr_get_rc(o) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (o->data);
}
/**
 * \brief get the const data from a uniq pointer
 * \param u uniq pointer to get the raw pointer from
 * \return the const pointer, or NULL if an error occurred
 * \ingroup sptr
**/
const void *sptr_getuc(const t_uniq_ptr *u) {
	if (!u) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (u->data);
}
/******************/
/* non-const data */
/******************/

/**
 * \brief get the raw pointer from a strong pointer
 * \param sptr strong pointer to get the raw pointer from
 * \return the raw pointer (non-const), or NULL if sptr is NULL or strong count
 * is 0
 * \ingroup sptr
**/
void *sptr_gets(t_strong_ptr *sptr) {
	return ((void *)sptr_getsc(sptr));
}
/**
 * \brief get the raw pointer from a weak pointer
 * \param sptr weak pointer to get the raw pointer from
 * \return the raw pointer (non-const), or NULL if sptr is NULL or strong count
 * is 0
 * \note returns NULL if the underlying object strong count is 0 (data
 * destroyed)
 * \ingroup sptr
**/
void *sptr_getw(t_weak_ptr *sptr) {
	return ((void *)sptr_getwc(sptr));
}
/**
 * \brief get the raw pointer from a owned pointer
 * \param o owned pointer to get the raw pointer from
 * \return the raw pointer (non-const), or NULL if an error occurred
 * \ingroup sptr
**/
void *sptr_geto(t_owned_ptr *o) {
	return ((void *)sptr_getoc(o));
}
/**
 * \brief get the raw pointer from a uniq pointer
 * \param u uniq pointer to get the raw pointer from
 * \return the raw pointer (non-const), or NULL if an error occurred
 * \ingroup sptr
**/
void *sptr_getu(t_uniq_ptr *u) {
	return ((void *)sptr_getuc(u));
}

/*****************/
/* strong counts */
/*****************/

/**
 * \brief get the strong count from a strong pointer
 * \param sptr strong pointer to get the strong count from
 * \return the strong count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_strongcount_s(const t_strong_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_rc_get(sptr->meta));
}
/**
 * \brief get the strong count from a weak pointer
 * \param sptr weak pointer to get the strong count from
 * \return the strong count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_strongcount_w(const t_weak_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_rc_get(sptr->meta));
}
/**
 * \brief get the strong count from an owned pointer
 * \param sptr owned pointer to get the strong count from
 * \return the strong count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_strongcount_o(const t_owned_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_rc_get(sptr->meta));
}
/**
 * \brief get the strong count from a uniq pointer
 * \param ptr uniq pointer to get the strong count from
 * \return 1 if ptr is valid, 0 otherwise.
 * \ingroup sptr
**/
size_t sptr_get_strongcount_u(const t_uniq_ptr *ptr) {
	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr->data ? 1 : 0);
}

/***************/
/* weak counts */
/***************/

/**
 * \brief get the weak count from a strong pointer
 * \param sptr strong pointer to get the weak count from
 * \return the weak count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_weakcount_s(const t_strong_ptr *sptr) {
	if (!sptr || !sptr->meta || !sptri_rc_get(sptr->meta)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_wc_get(sptr->meta));
}
/**
 * \brief get the weak count from a weak pointer
 * \param sptr weak pointer to get the weak count from
 * \return the weak count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_weakcount_w(const t_weak_ptr *sptr) {
	if (!sptr || !sptr->meta || !sptri_wc_get(sptr->meta)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_wc_get(sptr->meta));
}
/**
 * \brief get the weak count from an owned pointer
 * \param sptr owned pointer to get the weak count from
 * \return the weak count, or 0 on error
 * \ingroup sptr
**/
size_t sptr_get_weakcount_o(const t_owned_ptr *sptr) {
	if (!sptr || !sptr->meta) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (sptri_wc_get(sptr->meta));
}
/**
 * \brief get the weak count from a uniq pointer
 * \param u uniq pointer to get the weak count from
 * \return always return 0, uniq has no weak count.
 * \ingroup sptr
**/
size_t sptr_get_weakcount_u(const t_uniq_ptr *u) {
	if (!u) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (0);
}

/******************/
/* dtor (get+set) */
/******************/

/**
 * \brief get the dtor from a strong pointer
 * \param ptr strong pointer to get the dtor from
 * \return the dtor function pointer, or NULL on error
 * \ingroup sptr
**/
t_dtor sptr_get_dtors(const t_strong_ptr *ptr) {
	if (!ptr || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr->meta->dtor);
}
/**
 * \brief get the dtor from a weak pointer
 * \param ptr weak pointer to get the dtor from
 * \return the dtor function pointer, or NULL on error
 * \note returns NULL if weak count is 0 or strong count is 0
 * \ingroup sptr
**/
t_dtor sptr_get_dtorw(const t_weak_ptr *ptr) {
	if (!ptr || !ptr->meta || !sptr_get_wc(ptr) || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr->meta->dtor);
}
/**
 * \brief get the dtor from an owned pointer
 * \param ptr owned pointer to get the dtor from
 * \return the dtor function pointer, or NULL on error
 * \note returns NULL if strong count is 0
 * \ingroup sptr
**/
t_dtor sptr_get_dtoro(const t_owned_ptr *ptr) {
	if (!ptr || !ptr->meta || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr->meta->dtor);
}
/**
 * \brief get the dtor from a uniq pointer
 * \param ptr uniq pointer to get the dtor from
 * \return the dtor function pointer, or NULL on error
 * \ingroup sptr
**/
t_dtor sptr_get_dtoru(const t_uniq_ptr *ptr) {
	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr->dtor);
}

// ---
// set dtors
// ---

/**
 * \brief set the dtor of a strong pointer
 * \param ptr strong pointer to set the dtor on
 * \param dtor the new dtor value to set
 * \return true on success, false otherwise
 * \note fails if ptr is NULL or strong count is 0
 * \ingroup sptr
**/
bool sptr_set_dtors(t_strong_ptr *ptr, t_dtor dtor) {
	if (!ptr || !ptr->meta || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	ptr->meta->dtor = dtor;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (true);
}
/**
 * \brief attempt to set the dtor of a weak pointer
 * \param ptr weak pointer
 * \param dtor the new dtor value to set (ignored)
 * \return false always
 * \note weak pointers cannot change the dtor because they do not own the data
 * \ingroup sptr
**/
bool sptr_set_dtorw(t_weak_ptr *ptr, t_dtor dtor) {

	if (!ptr || !ptr->meta || !sptr_get_wc(ptr) || !sptr_get_rc(ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	(void)dtor;
	CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
	return (false);
}
/**
 * \brief set the dtor of an owned pointer
 * \param ptr the owned pointer to set the dtor on
 * \param dtor the new dtor value to set
 * \return true on success, false otherwise
 * \note only allowed if ptr is the owner and strong count is 1
 * \ingroup sptr
**/
bool sptr_set_dtoro(t_owned_ptr *ptr, t_dtor dtor) {
	if (!ptr || !ptr->owner || !ptr->meta || sptr_get_rc(ptr) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	ptr->meta->dtor = dtor;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (true);
}
/**
 * \brief set the dtor of a uniq pointer
 * \param ptr the uniq pointer to update the value of
 * \param dtor new dtor value to set
 * \return true on success, false otherwise
 * \ingroup sptr
**/
bool sptr_set_dtoru(t_uniq_ptr *ptr, t_dtor dtor) {
	if (!ptr || !ptr->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	ptr->dtor = dtor;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (true);
}
