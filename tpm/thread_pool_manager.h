#ifndef THREAD_POOL_MANAGER_H
#define THREAD_POOL_MANAGER_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef C_MINI__TPM_PAGE_SIZE
#	define C_MINI__TPM_PAGE_SIZE 4096
#endif

// -- threads
#ifndef TPM_THREADS_MAX_NB
#	define TPM_THREADS_MAX_NB 255
#endif
#ifndef TPM_THREADS_DFLT_NB
#	define TPM_THREADS_DFLT_NB 16
#endif

// -- batches

#ifndef TPM_BATCH_MAX_SIZE
#	define TPM_BATCH_MAX_SIZE 255 // max batch size 255
#endif
#ifndef TPM_BATCH_DEFAULT_SIZE
#	define TPM_BATCH_DEFAULT_SIZE 16 // default
#endif

// -- checks
#if (TPM_THREADS_DFLT_NB > TPM_THREADS_MAX_NB) ||                              \
    (TPM_BATCH_DEFAULT_SIZE > TPM_BATCH_MAX_SIZE)
#	error "default size is bigger than max size"
#endif

typedef struct s_tpm_base_job {
	void *(*func)(void *);
	void *arg;
	union {
		struct {
			char reccuring : 1;
			// 7 fields left
		} detail;
		uint8_t condensed;
	} infos;
} t_tpm_base_job;

typedef struct s_tpm_job {
	t_tpm_base_job job;

	struct s_tpm_job *next;
	struct s_tpm_job *prev;
} t_tpm_job;

// -- jobs
#ifndef C_MINI__TPM_MAX_JOBS
#	define C_MINI__TPM_MAX_JOBS C_MINI__TPM_PAGE_SIZE / sizeof(t_tpm_job)
#endif // !C_MINI__TPM_MAX_JOBS

#ifndef C_MINI__TPM_NB_JOBS
#	define C_MINI__TPM_NB_JOBS C_MINI__TPM_MAX_JOBS
#endif // !C_MINI__TPM_NB_JOBS

typedef struct s_tpm_thread_internal {
	t_tpm_job  job[C_MINI__TPM_NB_JOBS];
	atomic_int nb_jobs;

	pthread_t trd;
} t_tpm_thread_internal;

typedef struct {
	size_t total_jobs_completed;
	size_t total_jobs_failed;
	// ... to be determined
} t_tpm_stats;

typedef union {
	uint8_t num_threads;
	bool    shutting_down;
} t_tpm_internal;

typedef struct {
	t_tpm_thread_internal trd_ids[C_MINI__TPM_NB_JOBS]; // threads
	atomic_int            nb_threads;

	t_tpm_job      *q_start;
	t_tpm_job      *q_end;
	pthread_mutex_t q_lock;
	pthread_cond_t  q_cond; // sleep / wakeup

	t_tpm_internal internal_val;
	t_tpm_stats    stats;
} t_tpm;

#ifdef C_MINI__TESTS
#	include <stdio.h>
extern pthread_mutex_t console_lock;

#	ifndef LOG
#		define LOG(fmt, ...)                                                  \
			do {                                                               \
				pthread_mutex_lock(&console_lock);                             \
				printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                      \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
				pthread_mutex_unlock(&console_lock);                           \
			} while (0);
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...)                                                  \
			do {                                                               \
				pthread_mutex_lock(&console_lock);                             \
				printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                    \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
				pthread_mutex_unlock(&console_lock);                           \
			} while (0);
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS                                                        \
			do {                                                               \
				pthread_mutex_lock(&console_lock);                             \
				printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__, __LINE__,  \
				       __FUNCTION__);                                          \
				fflush(stdout);                                                \
				pthread_mutex_unlock(&console_lock);                           \
			} while (0);
#	endif // !DBG_POS

#	define C_MINI__TPM_LOG_ETHREADS "Thread error."
#	define C_MINI__TPM_LOG_EARG     "Bad arguments."
#	define C_MINI__TPM_LOG_ECLOCK   "Clock function faillure."
#	define C_MINI__TPM_LOG_ESPACE   "Not enough space."
#	define C_MINI__TPM_LOG_EALLOC   "Allocation Fail."
#	define C_MINI__TPM_LOG_ECB      "Callback failed."
#	define C_MINI__TPM_LOG_FAIL     "Failure."
#	define C_MINI__TPM_LOG_SUCCESS  "Success."
#	define C_MINI__TPM_LOG_ETODO    "Not yet implemented."

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TESTS

// create
t_tpm *tpm_new(size_t nb_thread);
t_tpm *tpm_init(t_tpm *tpm, size_t nb_thread);
// delete
void   tpm_delete(t_tpm *tpm);
void   tpm_free(t_tpm *tpm);
// add
void   tpm_add_job(t_tpm *tpm, void *(fun)(void *arg), void *arg);
void   tpm_add_reccuring_job(t_tpm *tpm, void *(fun)(void *), void *arg);
void tpm_add_jobs(t_tpm *tpm, size_t nb, void *(*funs)(void *arg), void **args);
// info
t_tpm_stats tpm_get_stats(t_tpm *tpm);
int         tpm_get_nbthread(const t_tpm *tpm);
// directing
bool        tpm_stop(t_tpm *tpm);

#ifdef C_MINI__TPM_INTERNAL

void *tpmi_worker(void *tpm);

#endif // C_MINI__TPM_INTERNAL

#endif // THREAD_POOL_MANAGER_H
