#include <stdlib.h>

#include "sptr/smart_pointer.h"

/**
 * \brief delete a smart pointer
 * \param sptr the smart pointer to delete
 * \return true if the smart pointer was deleted successfully false otherwise
 * \note if sc reaches 0, calls dtor on data, if wc and sc reach 0 box is free'd
 * \note in case of uniq pointer, sc is always 1
 * \ingroup sptr
 **/
bool sptr_delete(t_sptr *sptr) {
	typeof(sptr->box) b;

	if (!sptr || (sptr->smart_strength == SPTR_S && !sptr->box)) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_S) {
		sptr->box->sc--;
	} else if (sptr->smart_strength == SPTR_W && sptr->box) {
		sptr->box->wc--;
	}
	b = sptr->box;
	if (b) {
		if (b->raw && !b->sc) {
			if (b->dtor) b->dtor(b->raw);
			b->raw = NULL;
		}
		if (!b->sc && !b->wc) free(b);
	}
	sptr->box            = NULL;
	sptr->smart_strength = 0;
	return (true);
}
