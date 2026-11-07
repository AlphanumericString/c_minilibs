#include <stdlib.h>
#define C_MINI__SPTR_INTERNAL
#include "smart_pointer.h"

/**
 * \brief get the strong reference count (rc) of an fbox
 * \param f the fbox to read
 * \return the strong count, or 0 if f is NULL
 * \ingroup sptr
**/
size_t sptr_fb_get_count(const t_cm_fbox *f) {
	if (!f) return (0);
	return (sptri_rc_get(f));
}
/**
 * \brief increment the strong reference count (rc) of an fbox
 * \param f the fbox to increment
 * \return true on success, false if f is NULL or has a zero strong count
 * \ingroup sptr
**/
bool sptr_fb_increment(t_cm_fbox *f) {
	if (!f) return (false);
	if (!sptri_rc_get(f)) return (false);
	return (sptri_rc_inc(f), true);
}
/**
 * \brief decrement the strong reference count (rc) of an fbox and run its
 * destructor once the count reaches 0
 * \param f the fbox to decrement
 * \return true on success, false if f is NULL or has a zero strong count
 * \note when the count reaches 0 the destructor is called on the data and both
 * data and destructor are reset to NULL
 * \ingroup sptr
**/
bool sptr_fb_decrement(t_cm_fbox *f) {
	if (!f) return (false);
	if (!sptri_rc_get(f)) return (false);
	sptri_rc_dec(f);
	if (!sptri_rc_get(f)) {
		if (f->dtor) f->dtor(f->data);
		f->data = NULL;
		f->dtor = NULL;
	}
	return (true);
}
/**
 * \brief check whether an fbox wraps a non-NULL data pointer
 * \param f the fbox to inspect
 * \return true if data is non-NULL, false otherwise
 * \ingroup sptr
**/
bool sptr_fb_get_cm_data(const t_cm_fbox *f) {
	return (f->data);
}
/**
 * \brief initialize a caller-owned fbox with data, destructor and zeroed
 * counts
 * \param f the fbox to initialize
 * \param d pointer to the wrapped data
 * \param dt destructor to call once the strong count reaches 0
 * \return f on success, NULL if f is NULL
 * \ingroup sptr
**/
t_cm_fbox *sptr_fb_init(t_cm_fbox *f, void *d, t_cm_dtor dt) {
	if (!f) return (NULL);
	f->data = d;
	f->dtor = dt;
	f->rc   = 0;
	f->wc   = 0;
	return (f);
}
/**
 * \brief allocate and initialize a new fbox
 * \param d pointer to the wrapped data
 * \param dt destructor to call once the strong count reaches 0
 * \return pointer to the new fbox, or NULL on allocation or initialization
 * failure
 * \ingroup sptr
**/
t_cm_fbox *sptr_fb_new(void *d, t_cm_dtor dt) {
	t_cm_fbox *f;

	f = calloc(sizeof(*f), 1);
	return (sptr_fb_init(f, d, dt));
}
