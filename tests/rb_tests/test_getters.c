#include <assert.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_full(void) {
	t_cm_rb rb  = {0};
	int     val = 42;

	rb_create(&rb, sizeof(int), 2);

	assert(rb_full(rb) == false);
	rb_push(&rb, &val);
	assert(rb_full(rb) == false);
	rb_push(&rb, &val);
	assert(rb_full(rb) == true);

	rb_destroy(&rb);
	assert(rb_full((t_cm_rb){0}) == false);
}

NO_COV void test_cm_rb_empty(void) {
	t_cm_rb rb  = {0};
	int     val = 42;

	rb_init(&rb, sizeof(int));

	assert(rb_empty(rb) == true);
	rb_push(&rb, &val);
	assert(rb_empty(rb) == false);

	rb_destroy(&rb);
	assert(rb_empty((t_cm_rb){0}) == true);
}

NO_COV void test_cm_rb_size(void) {
	t_cm_rb rb  = {0};
	int     val = 42;

	rb_init(&rb, sizeof(int));

	assert(rb_size(rb) == 0);
	rb_push(&rb, &val);
	assert(rb_size(rb) == 1);
	rb_push(&rb, &val);
	assert(rb_size(rb) == 2);

	rb_destroy(&rb);
	assert(rb_size((t_cm_rb){0}) == 0);
}

NO_COV void test_cm_rb_capacity(void) {
	t_cm_rb rb = {0};

	rb_create(&rb, sizeof(int), 10);

	assert(rb_capacity(rb) == 10);

	rb_destroy(&rb);
	assert(rb_capacity((t_cm_rb){0}) == 0);
}

NO_COV void test_cm_rb_remaining_space(void) {
	t_cm_rb rb  = {0};
	int     val = 42;
	rb_create(&rb, sizeof(int), 10);

	assert(rb_remaining_space(rb) == 10);
	rb_push(&rb, &val);
	assert(rb_remaining_space(rb) == 9);

	rb_destroy(&rb);
	assert(rb_remaining_space((t_cm_rb){0}) == 0);
}
