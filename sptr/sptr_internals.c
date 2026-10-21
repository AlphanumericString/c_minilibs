#define C_MINI__SPTR_INTERNAL
#include "smart_pointer.h"

#ifdef C_MINI__SPTR_ATOMIC
// `memory_order_relaxed` for increments
// `memory_order_acq_rel` for decrements
void sptri_rc_inc(t_fbox *b) {
	atomic_fetch_add_explicit(&b->rc, 1, memory_order_acq_rel);
}
void sptri_rc_dec(t_fbox *b) {
	atomic_fetch_sub_explicit(&b->rc, 1, memory_order_relaxed);
}

void sptri_wc_inc(t_fbox *b) {
	atomic_fetch_add_explicit(&b->wc, 1, memory_order_acq_rel);
}
void sptri_wc_dec(t_fbox *b) {
	atomic_fetch_sub_explicit(&b->wc, 1, memory_order_relaxed);
}

size_t sptri_rc_get(t_fbox *b) {
	return (atomic_load_explicit(&b->rc, memory_order_seq_cst));
}
size_t sptri_wc_get(t_fbox *b) {
	return (atomic_load_explicit(&b->wc, memory_order_seq_cst));
}

#else

void sptri_rc_inc(t_fbox *b) {
	b->rc++;
}
void sptri_rc_dec(t_fbox *b) {
	b->rc--;
}

void sptri_wc_inc(t_fbox *b) {
	b->wc++;
}
void sptri_wc_dec(t_fbox *b) {
	b->wc--;
}

size_t sptri_rc_get(t_fbox *b) {
	return b->rc;
}
size_t sptri_wc_get(t_fbox *b) {
	return b->wc;
}
#endif

/*********************/
/* is_sole_box_owner */
/*********************/

/**
 * \brief checks whether a shared (strong) pointer is the sole owner of an
 * internal box
 * \param ptr the shared (strong) pointer to check
 * \return true if he is sole owner, otherwise false
 * \ingroup sptr
**/
bool sptri_is_sole_box_owns(const t_strong_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr && sptr_get_strongcount_s(ptr) == 1 &&
	        !sptr_get_weakcount_s(ptr));
}
/**
 * \brief checks whether a shared (weak) pointer is the sole owner of an
 * internal box
 * \param ptr the shared (weak) pointer to check
 * \return true if he is sole owner, otherwise false
 * \ingroup sptr
**/
bool sptri_is_sole_box_ownw(const t_weak_ptr *ptr) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ptr && !sptr_get_strongcount_w(ptr) &&
	        sptr_get_weakcount_w(ptr) == 1);
}
/**
 * \brief check whether the owned pointer is the sole owner of an internal box
 * \param ptr the owned pointer to check
 * \return true if he is sole owner, otherwise false
 * \ingroup sptr
**/
bool sptri_is_sole_box_owno(const t_owned_ptr *ptr) {
	size_t rc;
	size_t wc;

	if (!ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	rc = sptr_get_strongcount_o(ptr);
	wc = sptr_get_weakcount_o(ptr);
	return ((rc == 1 && !wc) || (!rc && wc == 1));
}
/**
 * \brief check whether the uniq pointer is the sole owner of an internal box
 * \param u the uniq pointer to check
 * \return false. Unique pointer interface does not have a box.
 * \ingroup sptr
**/
bool sptri_is_sole_box_ownu(const t_uniq_ptr *u) {
	if (!u || !sptr_getuc(u)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (false);
}
