#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "tls_tests.h"

static void *ftls_dummy_decrease(void *arg) {
	size_t  i = (size_t)arg;
	t_ftls *a;

	while (i--) {
		a = ftls_get();
		assert(a != NULL);
		a->value_a -= 32;
		a->value_b += 32;
		usleep(100);
	}
	return (NULL);
}
static void *ftls_dummy_increase(void *arg) {
	size_t  i = (size_t)arg;
	t_ftls *a;

	while (i--) {
		a = ftls_get();
		assert(a != NULL);
		a->value_a++;
		a->value_b--;
		assert(a->value_a > 0);
		assert(a->value_b < 0);
		usleep(100);
	}
	return (NULL);
}

void test_ftls_separate(void) {
	pthread_t t1 = 0, t2 = 0;
	void     *ret      = NULL;
	size_t    nb_steps = 10;

	pthread_create(&t1, NULL, ftls_dummy_decrease, (void *)nb_steps);
	pthread_create(&t2, NULL, ftls_dummy_increase, (void *)nb_steps);
	pthread_join(t1, &ret);
	pthread_join(t2, &ret);
}
