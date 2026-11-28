#include <assert.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_full(void) {
	t_cm_rb rb  = {0};
	int     val = 42;
	size_t  cap;
	size_t  i;

	assert(rb_create(&rb, sizeof(int), 2) != NULL);
	cap = rb_capacity(rb);

	assert(rb_full(rb) == false);
	for (i = 0; i < cap - 1; i++)
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

	assert(rb_is_magic(rb) == true);
	assert(rb_capacity(rb) == RB_TEST_PAGE_CAP(sizeof(int), 10));
	assert(rb_capacity(rb) == rb.nb_max);
	assert(rb.map_size == RB_TEST_ROUND_BYTES(sizeof(int) * 10));

	rb_destroy(&rb);

	// non magic fallback keeps the exact requested capacity
	shm_open_set_call_limit(0);
	rb_create(&rb, sizeof(int), 10);
	assert(rb_is_magic(rb) == false);
	assert(rb_capacity(rb) == 10);
	rb_destroy(&rb);
	shm_open_set_call_limit(SIZE_MAX);

	assert(rb_capacity((t_cm_rb){0}) == 0);
}

NO_COV void test_cm_rb_remaining_space(void) {
	t_cm_rb rb  = {0};
	int     val = 42;
	size_t  cap;

	rb_create(&rb, sizeof(int), 10);
	cap = rb_capacity(rb);

	assert(rb_remaining_space(rb) == cap);
	rb_push(&rb, &val);
	assert(rb_remaining_space(rb) == cap - 1);

	rb_destroy(&rb);
	assert(rb_remaining_space((t_cm_rb){0}) == 0);
}
