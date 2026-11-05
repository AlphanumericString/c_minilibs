#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "tls/voided_thread_local_storage.h"

typedef struct {
	int    a;
	double b;
} t_cm_pair;

static void *worker(void *arg) {
	long id = (long)(intptr_t)arg;
	long n  = id * 10;

	vtls_set("n", &n, sizeof(n));
	printf("thread %ld sees n=%ld len=%zu\n", id, *(long *)vtls_get("n"),
	       vtls_len());
	return (NULL);
}

int main(void) {
	pthread_t th[2];

	int       i   = 42;
	char      c   = 'z';
	long      l   = 123456789L;
	t_cm_pair p   = {.a = 7, .b = 3.5};
	char      s[] = "hello";

	vtls_set("int", &i, sizeof(i));
	vtls_set("char", &c, sizeof(c));
	vtls_set("long", &l, sizeof(l));
	vtls_set("pair", &p, sizeof(p));
	vtls_set("str", s, strlen(s) + 1);

	printf("stored %zu blobs\n", vtls_len());
	fflush(stdout);
	vtls_dump_all(1);

	int       oi;
	char      oc;
	long      ol;
	t_cm_pair op;
	char      os[16];
	vtls_getout("int", &oi);
	vtls_getout("char", &oc);
	vtls_getout("long", &ol);
	vtls_getout("pair", &op);
	vtls_getout("str", os);

	printf("int=%d char=%c long=%ld pair={%d,%.1f} str=%s\n", oi, oc, ol, op.a,
	       op.b, os);
	printf("typeof(pair)=%zu bytes has(str)=%s\n", vtls_sizeof("pair"),
	       vtls_has("str") ? "yes" : "no");

	vtls_rm("char");
	printf("after rm(char): has(char)=%s len=%zu\n",
	       vtls_has("char") ? "yes" : "no", vtls_len());

	for (long t = 0; t < 2; t++)
		pthread_create(&th[t], NULL, worker, (void *)(intptr_t)(t + 1));
	for (int t = 0; t < 2; t++)
		pthread_join(th[t], NULL);

	printf("main still sees int=%d\n", *(int *)vtls_get("int"));
	return (0);
}
