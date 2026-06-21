#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "tls_tests.h"

static void *ftls_dummy_decrease(void *__arg) {
	size_t  i = 5 * 2;
	t_ftls *a;

	(void)__arg;
	while (i--) {
		a = ftls_get();
		assert(a != NULL);
		a->value_a -= 32;
		a->value_b += 32;
		sleep(1);
	}
	return (NULL);
}
static void *ftls_dummy_increase(void *__arg) {
	size_t  i = 5;
	t_ftls *a;

	(void)__arg;
	while (i--) {
		a = ftls_get();
		assert(a != NULL);
		a->value_a++;
		a->value_b--;
		assert(a->value_a > 0);
		assert(a->value_b < 0);
		sleep(2);
	}
	return (NULL);
}

void test_ftls_separate(void) {
	pthread_t t1 = 0, t2 = 0;
	void     *ret = NULL;

	pthread_create(&t1, NULL, ftls_dummy_decrease, NULL);
	pthread_create(&t2, NULL, ftls_dummy_increase, NULL);
	pthread_join(t1, &ret);
	pthread_join(t2, &ret);
}
