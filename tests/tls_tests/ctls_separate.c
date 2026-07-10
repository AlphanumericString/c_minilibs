#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <unistd.h>

#include "tls_tests.h"

void *ctls_dummy_increment(void *a) {
	(void)a;
	size_t           i = 5;
	t_ctls_typewrap *v;

	ctls_set("a", (t_ctls_typewrap){.value.l = 0, .type = CTLS_LONG});
	v = ctls_get("a");
	while (i--) {
		v->value.l += 32;
		assert(ctls_get("a") != NULL);
		assert(ctls_get("a")->type == CTLS_LONG);
		assert(ctls_get("a")->value.l > 0);
		sleep(2);
	}
	ctls_rm("a");
	return (NULL);
}

void *ctls_dummy_decrement(void *a) {
	(void)a;
	size_t           i = 10;
	t_ctls_typewrap *v;

	ctls_set("a", (t_ctls_typewrap){.value.l = 0, .type = CTLS_LONG});
	v = ctls_get("a");
	while (i--) {
		v->value.l -= 1;
		assert(ctls_get("a") != NULL);
		assert(ctls_get("a")->type == CTLS_LONG);
		assert(ctls_get("a")->value.l < 0);
		sleep(1);
	}
	ctls_rm("a");
	return (NULL);
}

void test_ctls_separate() {
	pthread_t t1, t2;
	void     *ret;

	pthread_create(&t1, NULL, ctls_dummy_increment, NULL);
	pthread_create(&t2, NULL, ctls_dummy_decrement, NULL);
	pthread_join(t1, &ret);
	pthread_join(t2, &ret);
	assert(ctls_len() == 0);
}
