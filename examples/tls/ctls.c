#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#include "tls/common_thread_local_storage.h"

static void *worker(void *arg) {
	long id = (long)(intptr_t)arg;

	ctls_set("who", (t_ctls_typewrap){.type = CTLS_LONG, .value.l = id});
	ctls_set("visited", (t_ctls_typewrap){.type = CTLS_INT, .value.i = 1});

	printf("thread %ld sees who=%ld len=%zu\n", id, ctls_get("who")->value.l,
	       ctls_len());
	return (NULL);
}

int main(void) {
	pthread_t th[3];

	ctls_set("counter", (t_ctls_typewrap){.type = CTLS_LONG, .value.l = 0});
	ctls_set("who", (t_ctls_typewrap){.type = CTLS_LONG, .value.l = 42});
	ctls_set("tag", (t_ctls_typewrap){.type = CTLS_CHAR, .value.c = 'x'});
	ctls_set("handle", (t_ctls_typewrap){.type    = CTLS_PTR,
	                                     .value.p = (void *)(intptr_t)0xBEEF});

	printf("main dump (len=%zu):\n", ctls_len());
	fflush(stdout);
	ctls_dump_all(1);

	ctls_get("counter")->value.l += 5;
	printf("counter=%ld has(counter)=%s\n", ctls_get("counter")->value.l,
	       ctls_has("counter") ? "yes" : "no");

	ctls_rm("tag");
	printf("after rm(tag): has(tag)=%s len=%zu\n",
	       ctls_has("tag") ? "yes" : "no", ctls_len());

	for (long i = 0; i < 3; i++)
		pthread_create(&th[i], NULL, worker, (void *)(intptr_t)(i + 1));
	for (int i = 0; i < 3; i++)
		pthread_join(th[i], NULL);

	printf("main still sees who=%ld (threads did not clobber it)\n",
	       ctls_get("who")->value.l);
	return (0);
}
