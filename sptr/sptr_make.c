#include <stdlib.h>

#include "sptr/smart_pointer.h"

t_sptr sptr_make_shared_v(void *ptr, void (*free_er)(void *)) {
	t_sptr ptr_ret;

	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	ptr_ret.smart_strength = SPTR_STRONG;
	ptr_ret.box            = malloc(sizeof(*ptr_ret.box));
	if (!ptr_ret.box) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return ((t_sptr){0});
	}
	ptr_ret.box->raw     = ptr;
	ptr_ret.box->type    = SPTR_SHARED;
	ptr_ret.box->closure = free_er;
	ptr_ret.box->sc      = 1;
	ptr_ret.box->wc      = 0;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr_ret);
}
t_sptr sptr_make_shared_fromsptr(t_sptr *ptr, void (*free_er)(void *)) {
	t_sptr r;
	if (!ptr || !ptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	if (ptr->box->closure && free_er && ptr->box->closure != free_er) {
		LOG("Sudden change of free-er function at contruction, illegal if not "
		    "explicit.");
		return ((t_sptr){0});
	}
	switch (ptr->box->type) {
	case SPTR_SHARED:
		ptr->box->wc++;
		break;
	case SPTR_UNIQ:
		ptr->box->wc++;
		ptr->box->type = SPTR_SHARED;
		break;
	}
	r                = *ptr;
	r.smart_strength = SPTR_WEAK;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (r);
}

t_sptr sptr_make_uniq_v(void *ptr, void (*free_er)(void *)) {
	t_sptr ptr_ret;

	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	ptr_ret.smart_strength = SPTR_STRONG;
	ptr_ret.box            = malloc(sizeof(*ptr_ret.box));
	if (!ptr_ret.box) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return ((t_sptr){0});
	}
	ptr_ret.box->raw     = ptr;
	ptr_ret.box->type    = SPTR_UNIQ;
	ptr_ret.box->closure = free_er;
	ptr_ret.box->sc      = 1;
	ptr_ret.box->wc      = 0;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr_ret);
}
t_sptr sptr_make_uniq_fromsptr(t_sptr *ptr, void (*free_er)(void *)) {
	if (!ptr || !ptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	if (ptr->box->closure && free_er && ptr->box->closure != free_er) {
		LOG("Sudden change of free-er function at contruction, illegal if not "
		    "explicit.");
		return ((t_sptr){0});
	}
	switch (ptr->box->type) {
	case SPTR_SHARED:
		if (ptr->box->sc != 1) {
			LOG(C_MINI__SPTR_LOG_EARG
			    " Cannot make uniq from mutiple instances of strong ptr");
			return ((t_sptr){0});
		}
		if (ptr->smart_strength == SPTR_WEAK) {
			LOG(C_MINI__SPTR_LOG_EARG " Cannot make uniq from weak ptr");
			return ((t_sptr){0});
		}
		ptr->box->type = SPTR_UNIQ;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (*ptr);
	case SPTR_UNIQ:
		ptr->box->wc++;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return ((t_sptr){.smart_strength = SPTR_WEAK, .box = ptr->box});
	}
}
