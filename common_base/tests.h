#pragma once

#ifdef C_MINI__TESTS
#	include <bits/types/clockid_t.h>
#	include <bits/types/struct_timespec.h>
#	include <errno.h>
#	include <pthread.h>
#	include <sys/mman.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <time.h>
#	include <unistd.h>

#	include <stddef.h>
#	include <stdint.h>
#	define NO_COV            __attribute__((no_profile_instrument_function))
#	define ALLOC_DFT_LIMIT   SIZE_MAX
#	define CLOCK_DFT_LIMIT   SIZE_MAX
#	define PTHREAD_DFT_LIMIT SIZE_MAX

extern size_t nb_alloc;
extern size_t nb_clock;
extern size_t clock_limit;
extern size_t alloc_limit;

// per-primitive call budgets: the (limit+1)th call returns EAGAIN. The setters
// reset the matching counter so a test only has to arm/re-arm the budget.
extern size_t nb_pthread_create;
extern size_t pthread_create_limit;
extern size_t nb_pthread_cond_init;
extern size_t pthread_cond_init_limit;

// mapping primitive budgets for the magic round buffer tests
extern size_t nb_shm_open;
extern size_t shm_open_limit;
extern size_t nb_ftruncate;
extern size_t ftruncate_limit;
extern size_t nb_mmap;
extern size_t mmap_limit;
extern size_t nb_sysconf;
extern size_t sysconf_limit;

// when enabled __wrap_clock_gettime reports this fixed time instead of the
// real one, letting tests force deadlines to be in the past (or to overflow
// the nanosecond field) without touching wall/monotonic time.
extern int             fake_clock_enabled;
extern struct timespec fake_clock;

NO_COV void *__wrap_calloc(size_t sz_e, size_t nb_e);
NO_COV void *__wrap_malloc(size_t size);
NO_COV void *__wrap_realloc(void *ptr, size_t new_size);
NO_COV int   __wrap_clock_gettime(clockid_t __clock_id, struct timespec *__tp);
NO_COV int __wrap_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                                 void *(*start_routine)(void *), void *arg);
NO_COV int __wrap_pthread_cond_init(pthread_cond_t           *cond,
                                    const pthread_condattr_t *attr);
NO_COV int __wrap_shm_open(const char *name, int oflag, mode_t mode);
NO_COV int __wrap_ftruncate(int fd, off_t length);
NO_COV void *__wrap_mmap(void *addr, size_t length, int prot, int flags, int fd,
                         off_t offset);
NO_COV long  __wrap_sysconf(int name);
NO_COV void  alloc_set_call_limit(size_t lim);
NO_COV void  alloc_reset_counter(void);
NO_COV void  clock_set_call_limit(size_t lim);
NO_COV void  clock_reset_counter(void);
NO_COV void  clock_set_fake_time(time_t sec, long nsec);
NO_COV void  clock_clear_fake_time(void);
NO_COV void  pthread_create_set_call_limit(size_t lim);
NO_COV void  pthread_cond_init_set_call_limit(size_t lim);
NO_COV void  shm_open_set_call_limit(size_t lim);
NO_COV void  ftruncate_set_call_limit(size_t lim);
NO_COV void  mmap_set_call_limit(size_t lim);
NO_COV void  sysconf_set_call_limit(size_t lim);

#	ifdef C_MINI__TWRAP_IMPL

size_t nb_clock    = 0;
size_t nb_alloc    = 0;
size_t clock_limit = CLOCK_DFT_LIMIT;
size_t alloc_limit = ALLOC_DFT_LIMIT;

int             fake_clock_enabled = 0;
struct timespec fake_clock         = {0, 0};

size_t nb_pthread_create       = 0;
size_t pthread_create_limit    = PTHREAD_DFT_LIMIT;
size_t nb_pthread_cond_init    = 0;
size_t pthread_cond_init_limit = PTHREAD_DFT_LIMIT;

size_t nb_shm_open     = 0;
size_t shm_open_limit  = SIZE_MAX;
size_t nb_ftruncate    = 0;
size_t ftruncate_limit = SIZE_MAX;
size_t nb_mmap         = 0;
size_t mmap_limit      = SIZE_MAX;
size_t nb_sysconf      = 0;
size_t sysconf_limit   = SIZE_MAX;

extern void *__real_calloc(size_t, size_t);
extern void *__real_malloc(size_t);
extern void *__real_realloc(void *, size_t);
extern int   __real_clock_gettime(clockid_t __clock_id, struct timespec *__tp);
extern int __real_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                                 void *(*start_routine)(void *), void *arg);
extern int __real_pthread_cond_init(pthread_cond_t           *cond,
                                    const pthread_condattr_t *attr);
extern int __real_shm_open(const char *name, int oflag, mode_t mode);
extern int __real_ftruncate(int fd, off_t length);
extern void *__real_mmap(void *addr, size_t length, int prot, int flags, int fd,
                         off_t offset);
extern long  __real_sysconf(int name);

NO_COV int __wrap_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                                 void *(*start_routine)(void *), void *arg) {
	if (nb_pthread_create >= pthread_create_limit) return (EAGAIN);
	nb_pthread_create++;
	return __real_pthread_create(thread, attr, start_routine, arg);
}
NO_COV int __wrap_pthread_cond_init(pthread_cond_t           *cond,
                                    const pthread_condattr_t *attr) {
	if (nb_pthread_cond_init >= pthread_cond_init_limit) return (EAGAIN);
	nb_pthread_cond_init++;
	return __real_pthread_cond_init(cond, attr);
}
NO_COV void pthread_create_set_call_limit(size_t lim) {
	pthread_create_limit = lim;
	nb_pthread_create    = 0;
}
NO_COV void pthread_cond_init_set_call_limit(size_t lim) {
	pthread_cond_init_limit = lim;
	nb_pthread_cond_init    = 0;
}

NO_COV int __wrap_shm_open(const char *name, int oflag, mode_t mode) {
	if (nb_shm_open >= shm_open_limit) return (-1);
	nb_shm_open++;
	return __real_shm_open(name, oflag, mode);
}
NO_COV int __wrap_ftruncate(int fd, off_t length) {
	if (nb_ftruncate >= ftruncate_limit) return (-1);
	nb_ftruncate++;
	return __real_ftruncate(fd, length);
}
NO_COV void *__wrap_mmap(void *addr, size_t length, int prot, int flags, int fd,
                         off_t offset) {
	if (nb_mmap >= mmap_limit) return (MAP_FAILED);
	nb_mmap++;
	return __real_mmap(addr, length, prot, flags, fd, offset);
}
NO_COV void shm_open_set_call_limit(size_t lim) {
	shm_open_limit = lim;
	nb_shm_open    = 0;
}
NO_COV void ftruncate_set_call_limit(size_t lim) {
	ftruncate_limit = lim;
	nb_ftruncate    = 0;
}
NO_COV void mmap_set_call_limit(size_t lim) {
	mmap_limit = lim;
	nb_mmap    = 0;
}
NO_COV long __wrap_sysconf(int name) {
	if (nb_sysconf >= sysconf_limit) return (-1);
	nb_sysconf++;
	return __real_sysconf(name);
}
NO_COV void sysconf_set_call_limit(size_t lim) {
	sysconf_limit = lim;
	nb_sysconf    = 0;
}

NO_COV int __wrap_clock_gettime(clockid_t id, struct timespec *tp) {
	if (fake_clock_enabled) {
		*tp = fake_clock;
		return (0);
	}
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
NO_COV void clock_set_fake_time(time_t sec, long nsec) {
	fake_clock.tv_sec  = sec;
	fake_clock.tv_nsec = nsec;
	fake_clock_enabled = 1;
}
NO_COV void clock_clear_fake_time(void) {
	fake_clock_enabled = 0;
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
