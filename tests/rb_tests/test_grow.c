#include <assert.h>
#include <stdlib.h>

#include "rb_tests.h"

void test_rb_grow(void) {
	t_rb  rb = {0};
	t_rb *p;

	rb_create(&rb, sizeof(int), 2);

	assert(rb.nb_max == 2);
	p = rb_grow(&rb, 5);
	assert(p != NULL);
	assert(p->nb_max == 7);
	assert(rb.nb_max == 2);

	rb_destroy(&rb);
	rb_destroy(p);
	free(p);
}
