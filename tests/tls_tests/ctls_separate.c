#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <unistd.h>

#include "tls_tests.h"

void *ctls_dummy_increment(void *a) {
	size_t              i = (size_t)a;
	t_cm_ctls_typewrap *v;

	ctls_set("a", (t_cm_ctls_typewrap){.value.l = 0, .type = CTLS_LONG});
	v = ctls_get("a");
	while (i--) {
		v->value.l += 32;
		assert(ctls_get("a") != NULL);
		assert(ctls_get("a")->type == CTLS_LONG);
		assert(ctls_get("a")->value.l > 0);
		usleep(100);
	}
	ctls_rm("a");
	return (NULL);
}

void *ctls_dummy_decrement(void *a) {
	size_t              i = (size_t)a;
	t_cm_ctls_typewrap *v;

	ctls_set("a", (t_cm_ctls_typewrap){.value.l = 0, .type = CTLS_LONG});
	v = ctls_get("a");
	while (i--) {
		v->value.l -= 32;
		assert(ctls_get("a") != NULL);
		assert(ctls_get("a")->type == CTLS_LONG);
		assert(ctls_get("a")->value.l < 0);
		usleep(100);
	}
	ctls_rm("a");
	return (NULL);
}

NO_COV void test_ctls_separate(void) {
	pthread_t t1, t2;
	void     *ret;
	size_t    nb_steps = 10;

	pthread_create(&t1, NULL, ctls_dummy_increment, (void *)nb_steps);
	pthread_create(&t2, NULL, ctls_dummy_decrement, (void *)nb_steps);
	pthread_join(t1, &ret);
	pthread_join(t2, &ret);
	assert(ctls_len() == 0);
}
