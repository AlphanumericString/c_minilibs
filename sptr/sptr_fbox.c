#include <stdlib.h>
#define C_MINI__SPTR_INTERNAL
#include "smart_pointer.h"

size_t sptr_fb_get_count(const t_fbox *f) {
	if (!f) return (0);
	return (sptri_rc_get(f));
}
bool sptr_fb_increment(t_fbox *f) {
	if (!f) return (false);
	if (!sptri_rc_get(f)) return (false);
	return (sptri_rc_inc(f), true);
}
bool sptr_fb_decrement(t_fbox *f) {
	if (!f) return (false);
	if (!sptri_rc_get(f)) return (false);
	sptri_rc_dec(f);
	if (!sptri_rc_get(f)) {
		f->dtor(f->data);
		f->data = NULL;
		f->dtor = NULL;
	}
	return (true);
}
bool sptr_fb_get_data(t_fbox *f) {
	return (f->data);
}
t_fbox *sptr_fb_init(t_fbox *f, void *d, t_cm_dtor dt) {
	f->data = d;
	f->dtor = dt;
	f->rc   = 0;
	f->wc   = 0;
	return (f);
}
t_fbox *sptr_fb_new(void *d, t_cm_dtor dt) {
	t_fbox *f;

	f = calloc(sizeof(*f), 1);
	return (sptr_fb_init(f, d, dt));
}
