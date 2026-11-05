#include <assert.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_free(void) {
	t_cm_rb *rb = rb_new(sizeof(int));

	assert(rb != NULL);
	rb_free(&rb);
	assert(rb == NULL);
	rb_free(&rb);
	assert(rb == NULL);
	rb_free(NULL);
}
NO_COV void test_cm_rb_destroy(void) {
	t_cm_rb rb = {0};

	rb_init(&rb, sizeof(int));

	rb_destroy(&rb);
	assert(rb.o_pos == 0);
	assert(rb.i_pos == 0);
	assert(rb.array == NULL);
	assert(rb.nb_max == 0);
	assert(rb.sz_e == 0);
	rb_destroy(&rb);
	assert(rb.o_pos == 0);
	assert(rb.i_pos == 0);
	assert(rb.array == NULL);
	assert(rb.nb_max == 0);
	assert(rb.sz_e == 0);
	rb_destroy(&rb);
	assert(rb.o_pos == 0);
	assert(rb.i_pos == 0);
	assert(rb.array == NULL);
	assert(rb.nb_max == 0);
	assert(rb.sz_e == 0);
	rb_destroy(NULL);
}

NO_COV void test_cm_rb_clr(void) {
	t_cm_rb rb  = {0};
	int     val = 42;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val);

	rb_clr(&rb);
	assert(rb_empty(rb) == true);
	assert(rb.i_pos == rb.o_pos);
	rb_clr(NULL);

	rb_destroy(&rb);
}
