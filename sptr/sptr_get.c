#include "sptr/smart_pointer.h"

void *sptr_get(const t_sptr *s) {
	if (!s || !s->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (s->box->raw);
}
size_t sptr_get_wc(const t_sptr *s) {
	if (!s || !s->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (s->box->wc);
}
size_t sptr_get_sc(const t_sptr *s) {
	if (!s || !s->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (s->box->sc);
}
void (*sptr_get_closure(const t_sptr *s))(void *) {
	if (!s || !s->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (0);
	}
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (s->box->closure);
}
