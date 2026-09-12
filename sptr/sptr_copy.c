#include "sptr/smart_pointer.h"

t_strong_ptr *sptr_copys(t_strong_ptr *strong_ptr) {
	if (!strong_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (!strong_ptr->rc) {
		LOG(C_MINI__SPTR_LOG_ILLDATA);
		return (NULL);
	}
	strong_ptr->rc++;
	return (strong_ptr);
}

t_weak_ptr *sptr_copyw(t_weak_ptr *weak_ptr) {
	if (!weak_ptr) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return (NULL);
	}
	if (!weak_ptr->weak_count) {
		LOG(C_MINI__SPTR_LOG_ILLDATA);
		return (NULL);
	}
	weak_ptr->weak_count++;
	return (weak_ptr);
}

t_uniq_ptr sptr_copyu(t_uniq_ptr uniq_ptr) {
	t_uniq_ptr ret;

	if (!uniq_ptr.wcbox) {
		LOG(C_MINI__SPTR_LOG_EARG);
		return ((t_uniq_ptr){0});
	}
	if (!uniq_ptr.owner) {
		LOG(C_MINI__SPTR_LOG_ILLDATA);
		return ((t_uniq_ptr){0});
	}
	uniq_ptr.wcbox->weak_count++;
	ret = (t_uniq_ptr){.wcbox = uniq_ptr.wcbox, .owner = false};
	return (ret);
}
