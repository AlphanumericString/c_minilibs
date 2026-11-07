#ifndef THREAD_POOL_MANAGER_H
#define THREAD_POOL_MANAGER_H

// tldr: thread pool manager
// collect NB work in a main queue (can grow w/ bounds)
// dispatch TNB thread to do said jobs (max THREAD_MAX_NB threads)
// worker threads can take up to TRD_JOB_CACHE_LEN jobs inside them
//  but they take only 2/3 of their possible cache from main queue of jobs
// if any thread in the next STEAL_WINDOW threads has more than 2 jobs and
//  more jobs than them, steal. if nothing was stolen take 1/3 remaining from
//  main queue.
// if a thread is idle try to steal jobs every STEAL_RETRY_NS.

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define C_MINI__LOG_THREADSAFE
#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS
#	define C_MINI__TPM_LOG_ETODO    C_MINI__LOG_ETODO
#	define C_MINI__TPM_LOG_FAIL     C_MINI__LOG_FAIL
#	define C_MINI__TPM_LOG_EARG     C_MINI__LOG_EARG
#	define C_MINI__TPM_LOG_EALLOC   C_MINI__LOG_EALLOC
#	define C_MINI__TPM_LOG_ETHREADS C_MINI__LOG_F_PREFIX "Thread error."
#	define C_MINI__TPM_LOG_CATTR                                              \
		C_MINI__LOG_F_PREFIX "Condition attribute error."
#	define C_MINI__TPM_LOG_ECLOCK C_MINI__LOG_F_PREFIX "Clock."
#	define C_MINI__TPM_LOG_ESPACE C_MINI__LOG_F_PREFIX "Not enough space."
#	define C_MINI__TPM_LOG_ECB    C_MINI__LOG_F_PREFIX "Callback."
#	define C_MINI__TPM_LOG_EDOWN  C_MINI__LOG_F_PREFIX "Shutdown."

#	define C_MINI__TPM_LOG_SUCCESS C_MINI__LOG_SUCCESS

#	define C_MINI__TPM_LOG_TRD_INFO "Thread#%lu : %s"

#endif // !C_MINI__TESTS

/**
 * \defgroup tpm Thread Pool Manager
**/

// -- constants
#ifndef C_MINI__TPM_PAGE_SIZE
#	define C_MINI__TPM_PAGE_SIZE 4096 // standard unix system pagesize
#endif
#ifndef C_MINI__2MS_AS_NS
#	define C_MINI__2MS_AS_NS 2000000L
#endif

// -- thread pool manager variables: threads numbers
// arbitrary cap on the number of thread, feel free to change
#ifndef C_MINI__TPM_THREADS_MAX_NB
#	define C_MINI__TPM_THREADS_MAX_NB 255
#endif
// default
#ifndef C_MINI__TPM_THREADS_DFLT_NB
#	define C_MINI__TPM_THREADS_DFLT_NB 16
#endif

// -- thread internal variables: cache and steal
// internal thread local job cache len
#ifndef C_MINI__TPM_TRD_JOB_CACHE_LEN
#	define C_MINI__TPM_TRD_JOB_CACHE_LEN 8
#endif
// number of jobs taken from main queue by worker threads
#ifndef C_MINI__TPM_TRD_MGET_NB
#	define C_MINI__TPM_TRD_MGET_NB (C_MINI__TPM_TRD_JOB_CACHE_LEN * 2 / 3)
#endif
// number of thread to try to steal from for each threads (round robin style)
#ifndef C_MINI__TPM_STEAL_WINDOW
#	define C_MINI__TPM_STEAL_WINDOW 4
#endif
// number of NS to wait before trying to steal if thread is "sleeping"
#ifndef C_MINI__TPM_STEAL_RETRY_NS
#	define C_MINI__TPM_STEAL_RETRY_NS C_MINI__2MS_AS_NS
#endif

// -- checks
#if (C_MINI__TPM_TRD_JOB_CACHE_LEN < C_MINI__TPM_TRD_MGET_NB)
#	error "thread will try to take more jobs that they can hold." \
	" if you want to pursue, modify the other variable too."
#endif
#if (C_MINI__TPM_THREADS_DFLT_NB > C_MINI__TPM_THREADS_MAX_NB)
#	error "default size is bigger than max size"
#endif
// !checks

/**
 * \typedef t_cm_tpm_job
 * \brief the handle for jobs inside the thread pool manager system
 * \param func the function to execute for the job
 * \param arg the argument to call the work function with
 * \param infos job-specific metadata fields.
 * \note TODO: infos is incomplete; it currently only carries the recurring
 * flag.
 * \ingroup tpm
**/
typedef struct s_cm_tpm_base_job {
	void *(*func)(void *);
	void *arg;
	struct s_cm_job_infos {
		bool reccuring;
	} infos;
} t_cm_tpm_job;

/* \var hash_colision not used yet, belongs to the bucket system for fast
 * retrieval of jobs without scouring the whole list */
/**
 * \typedef t_cm_tpm_jc
 * \brief this is the thread pool manager job container
 * \param job the job contained
 * \param next next job in the queue, used by main queue
 * \param prev prev job in the queue, used by main queue
 * \note TODO: this is at this stage we batch jobs, add the h_map system for
 * random access speed
 * \ingroup tpm
**/
typedef struct s_cm_tpm_job {
	t_cm_tpm_job         job;
	struct s_cm_tpm_job *next;
	struct s_cm_tpm_job *prev;
	//	struct s_cm_tpm_job *hash_colision;
} t_cm_tpm_jc;

typedef struct s_cm_thread_pool_manager t_cm_tpm;

/**
 * \typedef t_cm_tpmi_thread
 * \brief internal thread structure for the thread pool manager
 * \param jc local job cache for the thread
 * \param start beginning of jobs in the local cache
 * \param end stop of jobs in the local cache (modified by other thread when
 * stealing jobs)
 * \param job_cache_lock mutex lock for local cache lock contention
 * \param pool pointer to the main thread pool manager
 * \param id_absolute OS thread handle (pthread_t) of the worker
 * \param id_sequential sequential index of the thread inside the pool
 * \note work stealing policy: collect TRD_MGET_NB (2/3 of cache len) from the
 * main queue, then try to steal from STEAL_WINDOW for the remaining 1/3.
 * Only steal if a neighboring thread has strictly more than one job and more
 * jobs than the stealer. If a thread is idle, wake up and try to steal jobs
 * every STEAL_RETRY_NS.
 * If no job was stolen, fill the remaining 1/3 by taking tpm main queue jobs
 * \ingroup tpm
**/
typedef struct s_cm_tpm_thread_internal {
	t_cm_tpm_jc    *jc[C_MINI__TPM_TRD_JOB_CACHE_LEN];
	uint8_t         start;
	uint8_t         end;
	pthread_mutex_t job_cache_lock;
	t_cm_tpm       *pool;
	pthread_t       id_absolute;
	size_t          id_sequential;
} t_cm_tpmi_thread;

/**
 * \struct s_cm_tpm_stats
 * \brief thread pool manager stats structure
 * \param total_jobs_completed number of jobs completed
 * \param total_jobs_failed number of jobs failed
 * \note TODO: add other stats
 * \ingroup tpm
**/
struct s_cm_tpm_stats {
	size_t total_jobs_completed;
	size_t total_jobs_failed;
};

/**
 * \typedef t_cm_tpm
 * \brief thread pool manager main structure
 * \param threads internal threads to dispatch for the works
 * \param nb_threads number of threads
 * \param q_start start of queue of jobs
 * \param q_end end of queue of jobs
 * \param q_lock mutex for queue contention
 * \param q_cond condition variable used to wake or sleep the worker threads
 * \param internal_val internal values for the thread pool manager
 * \param stats statistics for the thread pool manager
 * \note TODO: add hash map for jobs for retrieval/deletion
 * \ingroup tpm
**/
typedef struct s_cm_thread_pool_manager {
	t_cm_tpmi_thread threads[C_MINI__TPM_THREADS_MAX_NB]; // threads
	atomic_size_t    nb_threads;

	//	t_cm_tpm_jc      **hash_jobs; // job removal/cancelation - unused for
	// now 	atomic_int		size_hash; // nb of buckets

	t_cm_tpm_jc    *q_start;
	t_cm_tpm_jc    *q_end;
	pthread_mutex_t q_lock;
	pthread_cond_t  q_cond; // sleep / wakeup

	struct s_cm_tpm_internal {
		uint8_t num_threads;
		bool    shutting_down;
		bool    queue_closed;
	} internal_val;
	struct s_cm_tpm_stats stats;
} t_cm_tpm;

// TODO: (non-ordered)
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
// 5. future for then chaining and await
//				future_create-> alloc new struct, take a job_handle
//				job struct-> field for future
//				worker->job exec end -> if future 'then' pass return to given
//				 func
//				worker->job exec end -> if future 'await' ptread_cond for a
//				 wakup
//				void *(*then_function)(void *);
//				?future cancelling if task is cancelled
//				?independant future cancelling
//
//				-> problem cant use jc as return to user since operations on
//					them might invalidate them
//				idea -> future struct, pass it when job creation
// -- heartbeat impl idea
// HEARTBEAT_RYTHM -> wake every X seconds even without jobs to send info to
// main thread about state HBM_Inactive -> maximum number of heartbeats without
// jobs before thread gets recalled
// #ifndef C_MINI__TPM_HEARTBEAT_RYTHM
// #	define C_MINI__TPM_HEARTBEAT_RYTHM 2
// #endif
// #ifndef C_MINI__TPM_HBM_INACTIVE
// #	define C_MINI__TPM_HBM_INACTIVE 16
// #endif

// create
t_cm_tpm *tpm_new(size_t nb_thread);
t_cm_tpm *tpm_init(t_cm_tpm *tpm, size_t nb_thread);

// delete
void tpm_delete(t_cm_tpm *tpm, t_cm_call arg_cleanup);
void tpm_free(t_cm_tpm *tpm, t_cm_call arg_cleanup);
void tpm_clear_job_list(t_cm_tpm_jc *head, t_cm_call arg_cleanup);

// add
t_cm_tpm_jc *tpm_add_job(t_cm_tpm *tpm, t_cm_work work, void *arg);
// t_cm_tpm_jc *tpm_add_reccuring_job(t_cm_tpm *tpm, t_cm_work work, void *arg);
t_cm_tpm_jc *tpm_add_batch(t_cm_tpm *tpm, size_t nb, t_cm_work work,
                           void **args);

// info
struct s_cm_tpm_stats tpm_get_stats(t_cm_tpm *tpm);
size_t                tpm_get_nbthread(const t_cm_tpm *tpm);

// directing
bool tpm_stop(t_cm_tpm *tpm);
bool tpm_wait_end(t_cm_tpm *tpm);

#ifdef C_MINI__TPM_INTERNAL

// worker specific
void        *tpmi_worker(void *tpm);
t_cm_tpm_jc *tpmi_create_job(t_cm_work w_func, void *a);

// job handling - consider lock as caller responsability
bool tpmi_job_push(t_cm_tpm_jc *to_push, t_cm_tpm_jc **head, t_cm_tpm_jc **tail,
                   bool queu_down);
t_cm_tpm_jc tpmi_job_pop(t_cm_tpm *tpm);
int tpmi_job_thrd_cache_pop(t_cm_tpm *tpm, t_cm_tpm_jc **buffer, size_t start,
                            size_t max);

#endif // C_MINI__TPM_INTERNAL

#endif // THREAD_POOL_MANAGER_H
