#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

struct s_cm_mine {
	long counter;
	long id;
};

#define C_MINI__FTLS_STRUCT struct s_cm_mine
#define C_MINI__FTLS_GET
#include "tls/fast_thread_local_storage.h"

static void *worker(void *arg) {
	t_cm_ftls *tls = ftls_get();

	tls->id = (long)(intptr_t)arg;
	for (int i = 0; i < 5; i++)
		tls->counter += tls->id;

	printf("thread %ld counter=%ld\n", tls->id, tls->counter);
	return (NULL);
}

int main(void) {
	pthread_t  th[3];
	t_cm_ftls *tls = ftls_get();

	tls->counter = 100;
	tls->id      = 0;
	printf("main counter=%ld\n", ftls_get()->counter);

	for (long i = 0; i < 3; i++)
		pthread_create(&th[i], NULL, worker, (void *)(intptr_t)(i + 1));
	for (int i = 0; i < 3; i++)
		pthread_join(th[i], NULL);

	printf("main counter after threads=%ld (thread-isolated)\n",
	       ftls_get()->counter);
	return (0);
}
