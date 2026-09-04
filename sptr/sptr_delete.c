#include <stdlib.h>

#include "sptr/smart_pointer.h"

bool sptr_delete(t_sptr *sptr) {
	typeof(sptr->box) b;

	if (!sptr || (sptr->smart_strength == SPTR_STRONG && !sptr->box)) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (false);
	}
	if (sptr->smart_strength == SPTR_STRONG) {
		sptr->box->sc--;
	} else if (sptr->smart_strength == SPTR_WEAK && sptr->box) {
		sptr->box->wc--;
	}
	b = sptr->box;
	if (b) {
		if (b->raw && !b->sc) {
			if (b->closure) b->closure(b->raw);
			b->raw = NULL;
		}
		if (!b->sc && !b->wc) free(b);
	}
	sptr->box            = NULL;
	sptr->smart_strength = 0;
	return (true);
}
