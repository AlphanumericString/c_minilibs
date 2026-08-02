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
// #ifndef TPM_BATCH_MAX_SIZE
// #	define TPM_BATCH_MAX_SIZE 255 // max batch size 255
// #endif
// #ifndef TPM_BATCH_DEFAULT_SIZE
// #	define TPM_BATCH_DEFAULT_SIZE 16 // default
// #endif

// // -- Thread locacl job caching
// #ifndef C_MINI__TPM_THRD_JOBS_CACHE
// #	define C_MINI__TPM_THRD_JOBS_CACHE 8
// #endif // !C_MINI__TPM_THRD_JOBS_CACHE

// -- heartbeat
// HEARTBEAT_RYTHM -> wake every X secend even without jobs to send info to main
// thread abt state HBM_Inactive -> maximal number of hearbeats without jobs
//		before thread gets recalled
#ifndef C_MINI__TPM_HEARTBEAT_RYTHM
#	define C_MINI__TPM_HEARTBEAT_RYTHM 2
#endif
#ifndef C_MINI__TPM_HBM_INACTIVE
#	define C_MINI__TPM_HBM_INACTIVE 16
#endif

// -- checks
#if (TPM_THREADS_DFLT_NB > TPM_THREADS_MAX_NB) ||                              \
    (TPM_BATCH_DEFAULT_SIZE > TPM_BATCH_MAX_SIZE)
#	error "default size is bigger than max size"
#endif

typedef struct s_tpm_base_job {
	void *(*func)(void *);
	void *arg;
	struct s_job_infos {
		bool  reccuring;
		void *(*then_function)(void *);
	} infos;
} t_tpm_base_job;

// -- > split for batching later ...
//	-> batching size + other infos abt it stored here
//	-> keeps job struct clean
typedef struct s_tpm_job {
	t_tpm_base_job job;

	struct s_tpm_job *next;
	struct s_tpm_job *prev;
} t_tpm_job_container;

// -- jobs
#ifndef C_MINI__TPM_MAX_JOBS
#	define C_MINI__TPM_MAX_JOBS                                               \
		C_MINI__TPM_PAGE_SIZE / sizeof(t_tpm_job_container)
#endif // !C_MINI__TPM_MAX_JOBS

#ifndef C_MINI__TPM_NB_JOBS
#	define C_MINI__TPM_NB_JOBS C_MINI__TPM_MAX_JOBS
#endif // !C_MINI__TPM_NB_JOBS

typedef struct s_thread_pool_manager t_tpm;

typedef struct s_tpm_thread_internal {
	t_tpm_job_container job[C_MINI__TPM_NB_JOBS];
	atomic_int          nb_jobs;

	t_tpm    *pool;
	pthread_t id_absolute;
	size_t    id_sequential;
} t_tpm_thread_internal;

typedef struct s_thread_pool_manager {
	t_tpm_thread_internal threads[C_MINI__TPM_NB_JOBS]; // threads
	atomic_int            nb_threads;

	t_tpm_job_container *q_start;
	t_tpm_job_container *q_end;
	pthread_mutex_t      q_lock;
	pthread_cond_t       q_cond; // sleep / wakeup

	struct s_tpm_internal {
		uint8_t num_threads;
		bool    shutting_down;
		bool    queue_closed;
	} internal_val;
	struct s_tpm_stats {
		size_t total_jobs_completed;
		size_t total_jobs_failed;
		// ... to be determined
	} stats;
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
#	define C_MINI__TPM_LOG_ECLOCK   "Clock faillure."
#	define C_MINI__TPM_LOG_ESPACE   "Not enough space."
#	define C_MINI__TPM_LOG_EALLOC   "Allocation Fail."
#	define C_MINI__TPM_LOG_ECB      "Callback failed."
#	define C_MINI__TPM_LOG_FAIL     "Failure."
#	define C_MINI__TPM_LOG_SUCCESS  "Success."
#	define C_MINI__TPM_LOG_ETODO    "Not yet implemented."

#	define C_MINI__TPM_LOG_TRD_INFO "Thread#%lu : %s"
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
void tpm_delete(t_tpm *tpm, void (*delete_func)(void *));
void tpm_free(t_tpm *tpm, void (*delete_func)(void *));
void tpm_clear_job_list(t_tpm_job_container *head, void (*arg_freeer)(void *));

// add
void tpm_add_job(t_tpm *tpm, void *(fun)(void *arg), void *arg);
void tpm_add_reccuring_job(t_tpm *tpm, void *(fun)(void *), void *arg);
void tpm_add_batch(t_tpm *tpm, size_t nb, void *(fun)(void *arg), void **args);

// info
struct s_tpm_stats tpm_get_stats(t_tpm *tpm);
int                tpm_get_nbthread(const t_tpm *tpm);

// directing
bool tpm_stop(t_tpm *tpm);
bool tpm_wait_end(t_tpm *tpm);

#ifdef C_MINI__TPM_INTERNAL

void *tpmi_worker(void *tpm);
void  tpmi_job_push(t_tpm_job_container *to_push, t_tpm_job_container **head,
                    t_tpm_job_container **tail, bool queu_down);

t_tpm_job_container tpmi_job_pop(t_tpm *tpm);

#endif // C_MINI__TPM_INTERNAL

#endif // THREAD_POOL_MANAGER_H
