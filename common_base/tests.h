#pragma once

#ifdef C_MINI__TESTS
#	include <stddef.h>
#	include <stdint.h>
#	define NO_COV          __attribute__((no_profile_instrument_function))
#	define ALLOC_DFT_LIMIT SIZE_MAX

extern void  *std_handle;
extern size_t nb_alloc;
extern size_t alloc_limit;

NO_COV void *__wrap_calloc(size_t sz_e, size_t nb_e);
NO_COV void *__wrap_malloc(size_t size);
NO_COV void *__wrap_realloc(void *ptr, size_t new_size);
NO_COV void  alloc_set_call_limit(size_t lim);
NO_COV void  alloc_reset_counter(void);

#	ifdef C_MINI__TWRAP_IMPL

void  *std_handle  = NULL;
size_t nb_alloc    = 0;
size_t alloc_limit = ALLOC_DFT_LIMIT;

extern void *__real_calloc(size_t, size_t);
extern void *__real_malloc(size_t);
extern void *__real_realloc(void *, size_t);

NO_COV void *__wrap_calloc(size_t y, size_t z) {
	if (nb_alloc >= alloc_limit) return (NULL);
	nb_alloc++;
	return (__real_calloc(y, z));
}
NO_COV void *__wrap_malloc(size_t z) {
	if (nb_alloc >= alloc_limit) return (NULL);
	nb_alloc++;
	return (__real_malloc(z));
}
NO_COV void *__wrap_realloc(void *d, size_t nz) {
	if (nb_alloc >= alloc_limit) return (NULL);
	nb_alloc++;
	return (__real_realloc(d, nz));
}
NO_COV void alloc_set_call_limit(size_t lim) {
	alloc_limit = lim;
}
NO_COV void alloc_reset_counter(void) {
	nb_alloc = 0;
};
#	endif // !C_MINI__TWRAP_IMPL

#endif // ! C_MINI__TESTS
