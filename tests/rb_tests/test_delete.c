#include <assert.h>

#include "rb_tests.h"

void test_rb_destroy(void) {
	t_rb rb = {0};

	rb_init(&rb, sizeof(int));

	rb_destroy(&rb);
	assert(rb.o_pos == 0);
	assert(rb.i_pos == 0);
	assert(rb.array == NULL);
	assert(rb.nb_max == 0);
	assert(rb.sz_e == 0);
}

void test_rb_clr(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val);

	rb_clr(&rb);
	assert(rb_empty(rb) == true);
	assert(rb.i_pos == rb.o_pos);

	rb_destroy(&rb);
}
