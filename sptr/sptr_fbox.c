#include <stdlib.h>
#define C_MINI__SPTR_INTERNAL
#include "smart_pointer.h"

size_t sptr_fb_get_count(const t_cm_fbox *f) {
	if (!f) return (0);
	return (sptri_rc_get(f));
}
bool sptr_fb_increment(t_cm_fbox *f) {
	if (!f) return (false);
	if (!sptri_rc_get(f)) return (false);
	return (sptri_rc_inc(f), true);
}
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
bool sptr_fb_get_cm_data(const t_cm_fbox *f) {
	return (f->data);
}
t_cm_fbox *sptr_fb_init(t_cm_fbox *f, void *d, t_cm_dtor dt) {
	if (!f) return (NULL);
	f->data = d;
	f->dtor = dt;
	f->rc   = 0;
	f->wc   = 0;
	return (f);
}
t_cm_fbox *sptr_fb_new(void *d, t_cm_dtor dt) {
	t_cm_fbox *f;

	f = calloc(sizeof(*f), 1);
	return (sptr_fb_init(f, d, dt));
}
