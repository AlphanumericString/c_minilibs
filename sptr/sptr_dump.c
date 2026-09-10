#include <stdio.h>

#include "sptr/smart_pointer.h"

void sptr_dumps(int fd, const t_strong_ptr ptr) {
	if (fd < 0 || !ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return;
	}
	dprintf(fd, "SPTR STRONG { data %p, count %lu (weak:%lu), dtor %p}",
	        sptr_get(ptr), sptr_get_rc(ptr), sptr_get_wc(ptr), ptr->dtor);
}

void sptr_dumpw(int fd, const t_weak_ptr weak_ptr) {
	if (fd < 0 || !weak_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return;
	}
	dprintf(fd, "SPTR WEAK { data %p, count %lu (weak:%lu), dtor %p}",
	        sptr_get(weak_ptr), sptr_get_rc(weak_ptr), sptr_get_wc(weak_ptr),
	        weak_ptr->rcb.dtor);
}

void sptr_dumpu(int fd, const t_uniq_ptr uniq_ptr) {
	if (fd < 0 || !uniq_ptr.rcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return;
	}
	dprintf(fd, "SPTR UNIQ %s { data %p, count %lu (weak:%lu), dtor %p}",
	        sptr_is_ownu(uniq_ptr) ? "owned" : "borrowed", sptr_get(uniq_ptr),
	        sptr_get_rc(uniq_ptr), sptr_get_wc(uniq_ptr), uniq_ptr.rcbox->dtor);
}
