#ifndef THREAD_POOL_MANAGER_H
#define THREAD_POOL_MANAGER_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define C_MINI__LOG_THREADSAFE
#include "common_base/log.h"

#ifdef C_MINI__TESTS
#	define C_MINI__TPM_LOG_ETODO    C_MINI__LOG_ETODO
#	define C_MINI__TPM_LOG_FAIL     C_MINI__LOG_FAIL
#	define C_MINI__TPM_LOG_EARG     C_MINI__LOG_EARG
#	define C_MINI__TPM_LOG_EALLOC   C_MINI__LOG_EALLOC
#	define C_MINI__TPM_LOG_ETHREADS C_MINI__LOG_F_PREFIX "Thread error."
#	define C_MINI__TPM_LOG_ECLOCK   C_MINI__LOG_F_PREFIX "Clock."
#	define C_MINI__TPM_LOG_ESPACE   C_MINI__LOG_F_PREFIX "Not enough space."
#	define C_MINI__TPM_LOG_ECB      C_MINI__LOG_F_PREFIX "Callback."
#	define C_MINI__TPM_LOG_EDOWN    C_MINI__LOG_F_PREFIX "Shutdown."

#	define C_MINI__TPM_LOG_SUCCESS C_MINI__LOG_SUCCESS

#	define C_MINI__TPM_LOG_TRD_INFO "Thread#%lu : %s"

#endif // !C_MINI__TESTS

// BASE DEFINES

// -- constants
#ifndef C_MINI__TPM_PAGE_SIZE
#	define C_MINI__TPM_PAGE_SIZE 4096
#endif

// -- threads numbers
#ifndef TPM_THREADS_MAX_NB
#	define TPM_THREADS_MAX_NB 255
#endif
#ifndef TPM_THREADS_DFLT_NB
#	define TPM_THREADS_DFLT_NB 16
#endif

// // --batches
// #ifndef TPM_BATCH_MAX_SIZE
// #	define TPM_BATCH_MAX_SIZE 255 // max batch size 255
// #endif
// #ifndef TPM_BATCH_DEFAULT_SIZE
// #	define TPM_BATCH_DEFAULT_SIZE 16 // default
// #endif

// -- heartbeat
// HEARTBEAT_RYTHM -> wake every X seconds even without jobs to send info to
// main thread about state HBM_Inactive -> maximum number of heartbeats without
// jobs
//		before thread gets recalled
#ifndef C_MINI__TPM_HEARTBEAT_RYTHM
#	define C_MINI__TPM_HEARTBEAT_RYTHM 2
#endif
#ifndef C_MINI__TPM_HBM_INACTIVE
#	define C_MINI__TPM_HBM_INACTIVE 16
#endif

// -- checks
// #if (TPM_THREADS_DFLT_NB > TPM_THREADS_MAX_NB) || \
//     (TPM_BATCH_DEFAULT_SIZE > TPM_BATCH_MAX_SIZE)
// #	error "default size is bigger than max size"
// #endif // !checks

typedef struct s_tpm_base_job {
	void *(*func)(void *);
	void *arg;
	struct s_job_infos {
		bool reccuring;
	} infos;
} t_tpm_job;

// job container
// -- > split for batching later ...
//	-> batching size + other infos about it stored here
//	-> keeps job struct clean
typedef struct s_tpm_job {
	t_tpm_job         job;
	struct s_tpm_job *next;
	struct s_tpm_job *prev;

	struct s_tpm_job *hash_colision;
} t_tpm_jc;

typedef struct s_thread_pool_manager t_tpm;

// -- Thread local job caching
#ifndef C_MINI__TPM_THRD_JOBS_CACHE
#	define C_MINI__TPM_THRD_JOBS_CACHE 8
#endif // !C_MINI__TPM_THRD_JOBS_CACHE

typedef struct s_tpm_thread_internal {
	t_tpm_jc       *jc[C_MINI__TPM_THRD_JOBS_CACHE];
	uint8_t         start;
	uint8_t         end;
	pthread_mutex_t job_cache_lock;

	t_tpm    *pool;        // parent struct for work-stealing
	pthread_t id_absolute; // linux thread id (attribute setting, debug, infos)
	size_t    id_sequential; // pool thread id (debug, work stealing)
} t_tpmi_thread;

// -- jobs main queue pool
#ifndef C_MINI__TPM_MAX_JOBS
#	define C_MINI__TPM_MAX_JOBS C_MINI__TPM_PAGE_SIZE / sizeof(t_tpm_jc)
#endif // !C_MINI__TPM_MAX_JOBS

#ifndef C_MINI__TPM_NB_JOBS
#	define C_MINI__TPM_NB_JOBS C_MINI__TPM_MAX_JOBS
#endif // !C_MINI__TPM_NB_JOBS

#ifndef C_MINI__TPM_BUCKETS
#	define C_MINI__TPM_BUCKETS C_MINI__TPM_NB_JOBS / 2
#endif

struct s_tpm_stats {
	size_t total_jobs_completed;
	size_t total_jobs_failed;
	// ... to be determined
};

typedef struct s_thread_pool_manager {
	t_tpmi_thread threads[C_MINI__TPM_NB_JOBS]; // threads
	atomic_int    nb_threads;

	t_tpm_jc       *q_buckets[C_MINI__TPM_BUCKETS];
	t_tpm_jc       *q_start;
	t_tpm_jc       *q_end;
	pthread_mutex_t q_lock;
	pthread_cond_t  q_cond; // sleep / wakeup

	struct s_tpm_internal {
		uint8_t num_threads;
		bool    shutting_down;
		bool    queue_closed;
	} internal_val;
	struct s_tpm_stats stats;
} t_tpm;

// TODO: (non-ordered)
// 1. fix error. losing jobs.
//
// 3. heartbeat system -> clock heartbeat
//				monitor thread-> if threads[nb%max].hb - now > hb_steps*hb !
//				monitor thread-> sleep for (hb_steps - 1) hb between cycles
//				threads-> register hb during cond_wait in the while
//				threads-> cond_wait changed to cond_timedwait
//
// 4. job create return handle for canceling -> hashmap
//				container-> hash map for delete
//				container-> colison_next in container
//				tpm-> buckets + nb_j
//				add_job-> if (nb_j / buckets) > HASH_MAX_LOAD then re-distribute
//				add_job-> redistribute aim HASH_IDEAL_LOAD
//				pre-processor->check MAX > IDEAL
//
// 6. job arg delete function attached ? load even more the struct
//				add job-> takes free func
//				add jobs-> takes free funcs
//
// attack plan : 4 & 5
//				-> 3 rest of the week
//				-> 6 if time
// 4 - later
// 5. future for then chaining and await
//				future_create-> alloc new struct, take a job_handle
//				job struct-> field for future
//				worker->job exec end -> if future 'then' pass return to func
//				worker->job exec end -> if future 'await' ptread_cond
//				void *(*then_function)(void *);
//				?future cancelling if task is cancelled
//				?independant future cancelling
//
//				-> problem cant use jc as return to user since operations on
//					them might invalidate them
//				idea -> future struct, pass it when job creation

// func typdefs
#ifndef C_MINI__CALL
#	define C_MINI__CALL

typedef void (*t_call)(void *arg);

#endif // !C_MINI__CALL
#ifndef C_MINI__WORK
#	define C_MINI__WORK

typedef void *(*t_work)(void *arg);

#endif // !C_MINI__WORK

// create
t_tpm *tpm_new(size_t nb_thread);
t_tpm *tpm_init(t_tpm *tpm, size_t nb_thread);

// delete
void tpm_delete(t_tpm *tpm, t_call arg_cleanup);
void tpm_free(t_tpm *tpm, t_call arg_cleanup);
void tpm_clear_job_list(t_tpm_jc *head, t_call arg_cleanup);

// add
t_tpm_jc *tpm_add_job(t_tpm *tpm, t_work work, void *arg);
t_tpm_jc *tpm_add_reccuring_job(t_tpm *tpm, t_work work, void *arg);
t_tpm_jc *tpm_add_batch(t_tpm *tpm, size_t nb, t_work work, void **args);

// info
struct s_tpm_stats tpm_get_stats(t_tpm *tpm);
int                tpm_get_nbthread(const t_tpm *tpm);

// directing
bool tpm_stop(t_tpm *tpm);
bool tpm_wait_end(t_tpm *tpm);

#ifdef C_MINI__TPM_INTERNAL

// worker specific
void     *tpmi_worker(void *tpm);
t_tpm_jc *tpmi_create_job(t_work w_func, void *a);

// job handling - consider lock as caller responsability
bool     tpmi_job_push(t_tpm_jc *to_push, t_tpm_jc **head, t_tpm_jc **tail,
                       bool queu_down);
t_tpm_jc tpmi_job_pop(t_tpm *tpm);
int      tpmi_job_thrd_cache_pop(t_tpm    *tpm,
                                 t_tpm_jc *buffer[C_MINI__TPM_THRD_JOBS_CACHE],
                                 size_t    start);

#endif // C_MINI__TPM_INTERNAL

#endif // THREAD_POOL_MANAGER_H
