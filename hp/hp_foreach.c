#include "heap.h"

/**
 * \brief For each element in the heap, apply a callback
 * \param h heap pointer
 * \param cb callback function
 * \param u_arg user argument
 * \return true if all callbacks returned true, false otherwise
 * \ingroup hp
**/
bool hp_foreach(t_heap *h, t_callback cb, void *data) {
	if (!h || !cb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (false);
	}
	for (size_t i = 0; i < h->nb; i++)
		if (cb(h->elems[i], data) == false) {
			LOG(C_MINI__HP_LOG_CB_FAIL, i);
			return (false);
		}
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (true);
}

static bool hp_internal_prefix(t_heap *h, size_t i, t_callback cb, void *u) {
	if (i >= h->nb) return (true);
	if (!hp_internal_prefix(h, i * 2 + 1, cb, u)) return (false);
	if (!hp_internal_prefix(h, i * 2 + 2, cb, u)) return (false);
	if (cb(h->elems[i], u) == false) {
		LOG(C_MINI__HP_LOG_CB_FAIL, i);
		return (false);
	}
	return (true);
}
static bool hp_internal_postfix(t_heap *h, size_t i, t_callback cb, void *u) {
	if (i >= h->nb) return (true);
	if (cb(h->elems[i], u) == false) {
		LOG(C_MINI__HP_LOG_CB_FAIL, i);
		return (false);
	}
	if (!hp_internal_postfix(h, i * 2 + 1, cb, u)) return (false);
	if (!hp_internal_postfix(h, i * 2 + 2, cb, u)) return (false);
	return (true);
}
static bool hp_internal_infix(t_heap *h, size_t i, t_callback cb, void *u) {
	if (i >= h->nb) return (true);
	if (!hp_internal_infix(h, i * 2 + 1, cb, u)) return (false);
	if (cb(h->elems[i], u) == false) {
		LOG(C_MINI__HP_LOG_CB_FAIL, i);
		return (false);
	}
	if (!hp_internal_infix(h, i * 2 + 2, cb, u)) return (false);
	return (true);
}

/**
 * \brief Apply a callback to each element in the heap (infix)
 * \param h heap pointer
 * \param cb callback function
 * \param u_arg user argument
 * \return true if all callbacks returned true, false otherwise
 * \ingroup hp
**/
bool hp_foreach_in(t_heap *h, t_callback cb, void *data) {
	bool r;

	if (!h || !cb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (false);
	}
	r = hp_internal_infix(h, 0, cb, data);
	if (!r) return (false);
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (true);
}
/**
 * \brief Apply a callback to each element in the heap (prefix)
 * \param h heap pointer
 * \param cb callback function
 * \param u_arg user argument
 * \return true if all callbacks returned true, false otherwise
 * \ingroup hp
**/
bool hp_foreach_pre(t_heap *h, t_callback cb, void *data) {
	bool r;

	if (!h || !cb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (false);
	}
	r = hp_internal_prefix(h, 0, cb, data);
	if (!r) return (false);
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (true);
}
/**
 * \brief Apply a callback to each element in the heap (postfix)
 * \param h heap pointer
 * \param cb callback function
 * \param u_arg user argument
 * \return true if all callbacks returned true, false otherwise
 * \ingroup hp
**/
bool hp_foreach_post(t_heap *h, t_callback cb, void *u_arg) {
	bool r;

	if (!h || !cb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (false);
	}
	r = hp_internal_postfix(h, 0, cb, u_arg);
	if (!r) return (false);
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (true);
}
