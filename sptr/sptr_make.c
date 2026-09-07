#include <stdlib.h>

#include "sptr/smart_pointer.h"

/**
 * \brief makes a shared pointer out of a raw pointer value
 * \param ptr value to encapsulate
 * \param closure function to call when values is destroyed
 * \return the t_sptr initialized or a 0 filled sptr struct
 * \ingroup sptr
 **/
t_sptr sptr_make_shared_v(void *ptr, void (*closure)(void *)) {
	t_sptr ptr_ret;

	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	ptr_ret.smart_strength = SPTR_S;
	ptr_ret.box            = malloc(sizeof(*ptr_ret.box));
	if (!ptr_ret.box) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return ((t_sptr){0});
	}
	ptr_ret.box->raw  = ptr;
	ptr_ret.box->type = SPTR_SHRD;
	ptr_ret.box->dtor = closure;
	ptr_ret.box->sc   = 1;
	ptr_ret.box->wc   = 0;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr_ret);
}
/**
 * \brief makes a shared pointer out of an other smart pointer
 * \param ptr smart pointer value to encapsulate
 * \param closure function to call when values is destroyed
 * \return the t_sptr initialized or a 0 filled sptr struct
 * \ingroup sptr
 **/
t_sptr sptr_make_shared_fromsptr(t_sptr *ptr, void (*closure)(void *)) {
	t_sptr r;
	if (!ptr || !ptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	if (ptr->box->dtor && closure && ptr->box->dtor != closure) {
		LOG("Sudden change of free-er function at contruction, illegal if not "
		    "explicit.");
		return ((t_sptr){0});
	}
	switch (ptr->box->type) {
	case SPTR_SHRD:
		ptr->box->wc++;
		break;
	case SPTR_UNIQ:
		ptr->box->wc++;
		ptr->box->type = SPTR_SHRD;
		break;
	}
	r                = *ptr;
	r.smart_strength = SPTR_W;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (r);
}

/**
 * \brief makes a uniq pointer out of a raw pointer value
 * \param ptr value to encapsulate
 * \param closure the closure to call when destroying the value
 * \return the sptr initialized or a 0 filled sptr struct
 * \ingroup sptr
 **/
t_sptr sptr_make_uniq_v(void *ptr, void (*closure)(void *)) {
	t_sptr ptr_ret;

	if (!ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	ptr_ret.smart_strength = SPTR_S;
	ptr_ret.box            = malloc(sizeof(*ptr_ret.box));
	if (!ptr_ret.box) {
		LOG(C_MINI__SPTR_LOG_EALLOC);
		return ((t_sptr){0});
	}
	ptr_ret.box->raw  = ptr;
	ptr_ret.box->type = SPTR_UNIQ;
	ptr_ret.box->dtor = closure;
	ptr_ret.box->sc   = 1;
	ptr_ret.box->wc   = 0;
	LOG(C_MINI__SPTR_LOG_SUCCESS);
	return (ptr_ret);
}

/**
 * \brief makes a uniq pointer out of an other smart pointer
 * \param sptr value to encapsulate/turn into uniq
 * \param closure function to call when values is destroyed
 * \return the sptr initialized or a 0 filled sptr struct
 * \ingroup sptr
 **/
t_sptr sptr_make_uniq_fromsptr(t_sptr *sptr, void (*closure)(void *)) {
	if (!sptr || !sptr->box) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_sptr){0});
	}
	if (sptr->box->dtor && closure && sptr->box->dtor != closure) {
		LOG("Sudden change of free-er function at contruction, illegal if not "
		    "explicit.");
		return ((t_sptr){0});
	}
	switch (sptr->box->type) {
	case SPTR_SHRD:
		if (sptr->box->sc != 1) {
			LOG(C_MINI__SPTR_LOG_EARG
			    " Cannot make uniq from mutiple instances of strong ptr");
			return ((t_sptr){0});
		}
		if (sptr->smart_strength == SPTR_W) {
			LOG(C_MINI__SPTR_LOG_EARG " Cannot make uniq from weak ptr");
			return ((t_sptr){0});
		}
		sptr->box->type = SPTR_UNIQ;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return (*sptr);
	case SPTR_UNIQ:
		sptr->box->wc++;
		LOG(C_MINI__SPTR_LOG_SUCCESS);
		return ((t_sptr){.smart_strength = SPTR_W, .box = sptr->box});
	}
}
