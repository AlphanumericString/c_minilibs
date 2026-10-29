#include <stdbool.h>
#include <stdlib.h>

#define C_MINI__SPTR_INTERNAL
#define C_MINI__SPTR_CONSOLE_LOCK // make_shared
#include "sptr/smart_pointer.h"

// *------*
// |strong|
// *------*

/**
 * \brief create a strong pointer from a raw pointer
 * \param raw_ptr pointer to the object to wrap
 * \param d destructor to use when the object will be deallocated
 * \return the strong pointer on success, NULL on failure
 * \note allocates a complete box with strong count = 1 and weak count = 0
 * \ingroup sptr
**/
t_strong_ptr sptr_make_shared_from_raw(void *raw_ptr, t_cm_dtor d) {
	t_fbox *complete_box;

	if (!raw_ptr) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_strong_ptr){0});
	}
	complete_box = malloc(sizeof(*complete_box));
	if (!complete_box) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return ((t_strong_ptr){0});
	}
	*complete_box = (t_fbox){.data = raw_ptr, .dtor = d, .rc = 1, .wc = 0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_strong_ptr){.data = raw_ptr, .meta = complete_box});
}
/**
 * \brief create a strong pointer from a weak pointer
 * \param weak_ptr weak pointer to copy
 * \param d destructor to update or NULL to keep existing
 * \return the strong pointer on success, NULL on failure
 * \note increments strong count; fails if d is provided and conflicts with an
 * existing non-NULL destructor
 * \ingroup sptr
**/
t_strong_ptr sptr_make_shared_from_weak(t_weak_ptr *weak_ptr, t_cm_dtor d) {
	t_strong_ptr ret;

	if (!weak_ptr || !weak_ptr->meta || !sptr_get_rc(weak_ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_strong_ptr){0});
	}
	if (d && sptr_get_dtor(weak_ptr) && sptr_get_dtor(weak_ptr) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_strong_ptr){0});
	}
	ret = (t_strong_ptr){.meta = weak_ptr->meta, .data = weak_ptr->meta->data};
	sptri_rc_inc(ret.meta);
	if (d) sptr_set_dtor(&ret, d);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief create a strong pointer from a strong pointer
 * \param strong_ptr strong pointer to copy
 * \param d destructor to update or NULL to keep existing
 * \return the strong pointer on success, NULL on failure
 * \note increments strong count on the underlying box; fails if d is provided
 * and conflicts with an existing destructor
 * \ingroup sptr
**/
t_strong_ptr sptr_make_shared_from_strong(t_strong_ptr *strong_ptr,
                                          t_cm_dtor     d) {
	t_strong_ptr ret;

	if (!strong_ptr || !sptr_get_rc(strong_ptr)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_strong_ptr){0});
	}
	ret = *strong_ptr;
	if (d && sptr_get_dtor(strong_ptr) && sptr_get_dtor(strong_ptr) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_strong_ptr){0});
	}
	sptri_rc_inc(ret.meta);
	if (d) sptr_set_dtor(&ret, d);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief convert an owned unique pointer into a new strong shared pointer
 * \param own owned pointer to convert
 * \param d destructor to update or NULL to keep existing
 * \return the strong pointer on success, NULL on failure
 * \note transfers ownership; owned loses ownership (owner becomes false, strong
 * count decremented); fails if owned is not owner or strong count != 1
 * \ingroup sptr
**/
t_strong_ptr sptr_make_shared_from_owned(t_owned_ptr *own, t_cm_dtor d) {
	t_strong_ptr ret;

	if (!own || !own->meta || own->data != own->meta->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_strong_ptr){0});
	}
	if (own->owner != true || sptr_get_rc(own) != 1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_strong_ptr){0});
	}
	if (d && sptr_get_dtor(own) && sptr_get_dtor(own) != d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_strong_ptr){0});
	}
	if (d) sptr_set_dtor(own, d);
	ret = (t_strong_ptr){.meta = own->meta, .data = own->meta->data};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*own = (t_owned_ptr){0};
	return (ret);
}
/**
 * \brief makes a shared (strong) pointer from a uniq pointer
 * \param u the uniq pointer used a source
 * \param d the dtor used for innit (works as innit only if !dtor in uniq)
 * \return the shared (strong) pointer
 * \ingroup sptr
**/
t_strong_ptr sptr_make_shared_from_uniq(t_uniq_ptr *u, t_cm_dtor d) {
	t_strong_ptr ret;

	if (!u) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_strong_ptr){0});
	}
	if (d && u->dtor && d != u->dtor) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_ILL)
		return ((t_strong_ptr){0});
	}
	ret.meta = malloc(sizeof(*ret.meta));
	if (!ret.meta) return ((t_strong_ptr){0});
	*(ret.meta) = (t_fbox){.data = u->data, .dtor = u->dtor, .rc = 1, .wc = 0};
	ret.data    = ret.meta->data;
	if (d) sptr_set_dtor(&ret, d);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	*u = (t_uniq_ptr){0};
	return (ret);
}

// *----*
// |weak|
// *----*

/**
 * \brief makes a weak copy of a shared (strong) pointer
 * \param s the strong pointer to copy
 * \return the pointer to the weak pointer or NULL on error
 * \ingroup sptr
**/
t_weak_ptr sptr_make_weak_from_strong(t_strong_ptr *s) {
	if (!s || !sptr_get_rc(s) || s->data != s->meta->data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_weak_ptr){0});
	}
	sptri_wc_inc(s->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_weak_ptr){s->meta});
}
/**
 * \brief makes a weak copy of a shared (weak) pointer
 * \param w the weak pointer to copy
 * \return the pointer to the weak pointer or NULL on error
 * \ingroup sptr
**/
t_weak_ptr sptr_make_weak_from_weak(t_weak_ptr *w) {

	if (!w || !sptr_get_wc(w)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return ((t_weak_ptr){0});
	}
	sptri_wc_inc(w->meta);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__SPTR_LOG_SUCCESS)
	return ((t_weak_ptr){w->meta});
}
