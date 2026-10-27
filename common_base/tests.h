#pragma once

#ifdef C_MINI__TESTS
#	include <bits/types/clockid_t.h>
#	include <bits/types/struct_timespec.h>
#	include <time.h>

#	include <stddef.h>
#	include <stdint.h>
#	define NO_COV          __attribute__((no_profile_instrument_function))
#	define ALLOC_DFT_LIMIT SIZE_MAX
#	define CLOCK_DFT_LIMIT SIZE_MAX

extern size_t nb_alloc;
extern size_t nb_clock;
extern size_t clock_limit;
extern size_t alloc_limit;

NO_COV void *__wrap_calloc(size_t sz_e, size_t nb_e);
NO_COV void *__wrap_malloc(size_t size);
NO_COV void *__wrap_realloc(void *ptr, size_t new_size);
NO_COV int   __wrap_clock_gettime(clockid_t __clock_id, struct timespec *__tp);
NO_COV void  alloc_set_call_limit(size_t lim);
NO_COV void  alloc_reset_counter(void);
NO_COV void  clock_set_call_limit(size_t lim);
NO_COV void  clock_reset_counter(void);

#	ifdef C_MINI__TWRAP_IMPL

size_t nb_clock    = 0;
size_t nb_alloc    = 0;
size_t clock_limit = CLOCK_DFT_LIMIT;
size_t alloc_limit = ALLOC_DFT_LIMIT;

extern void *__real_calloc(size_t, size_t);
extern void *__real_malloc(size_t);
extern void *__real_realloc(void *, size_t);
extern int   __real_clock_gettime(clockid_t __clock_id, struct timespec *__tp);

NO_COV int __wrap_clock_gettime(clockid_t id, struct timespec *tp) {
	if (nb_clock >= clock_limit) return (-1);
	nb_clock++;
	return __real_clock_gettime(id, tp);
}
NO_COV void clock_set_call_limit(size_t lim) {
	clock_limit = lim;
}
NO_COV void clock_reset_counter(void) {
	nb_clock = 0;
}

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
}
#	endif // !C_MINI__TWRAP_IMPL

#endif // ! C_MINI__TESTS
