#include "rb_tests.h"

void test_rb_create(void) {
	t_rb rb = {0};

	rb_create(&rb, sizeof(int), 10);
	ASSERT_RB_STATE(rb, sizeof(int), 10, 0, 0);

	rb_destroy(&rb);
}

void test_rb_init(void) {
	t_rb rb = {0};

	rb_init(&rb, sizeof(int));
	ASSERT_RB_STATE(rb, sizeof(int), C_MINI_RB_NB_ELEM_DFLT, 0, 0);

	rb_destroy(&rb);
}

void test_rb_from_array(void) {
	t_rb rb     = {0};
	int  arr[5] = {1, 2, 3, 4, 5};

	rb_from_array(&rb, arr, 5, sizeof(int));
	ASSERT_RB_STATE(rb, sizeof(int), 5, 0, 0);
	assert(((int *)rb.array)[0] == 1);
	assert(((int *)rb.array)[4] == 5);
}

void test_rb_new(void) {
	t_rb *rb = NULL;

	rb = rb_new(sizeof(int));
	ASSERT_RB_STATE(*rb, sizeof(int), C_MINI_RB_NB_ELEM_DFLT, 0, 0);

	rb_destroy(rb);
	free(rb);
}
