#include <assert.h>

#include "rb_tests.h"

void test_rb_create(void) {
	t_rb rb = {0};

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	ASSERT_RB_STATE(rb, sizeof(int), 10, 0, 0);

	rb_destroy(&rb);
	assert(rb_create(&rb, 0, 10) == NULL);
	assert(rb_create(&rb, sizeof(int), 0) == NULL);
	assert(rb_create(NULL, sizeof(int), 10) == NULL);
}

void test_rb_init(void) {
	t_rb rb = {0};

	assert(rb_init(&rb, sizeof(int)) != NULL);
	ASSERT_RB_STATE(rb, sizeof(int), C_MINI_RB_NB_ELEM_DFLT, 0, 0);

	rb_destroy(&rb);
	assert(rb_init(NULL, sizeof(int)) == NULL);
	assert(rb_init(&rb, 0) == NULL);
}

void test_rb_from_array(void) {
	t_rb rb     = {0};
	int  arr[5] = {1, 2, 3, 4, 5};

	assert(rb_from_array(&rb, arr, 5, sizeof(int)) != NULL);
	ASSERT_RB_STATE(rb, sizeof(int), 5, 0, 0);
	assert(((int *)rb.array)[0] == 1);
	assert(((int *)rb.array)[4] == 5);

	assert(rb_from_array(NULL, arr, 5, sizeof(int)) == NULL);
	// TODO:
	// assert(rb_from_array(&rb, NULL, 5, sizeof(int)) == NULL);
	// assert(rb_from_array(&rb, arr, 0, sizeof(int)) == NULL);
	// assert(rb_from_array(&rb, arr, 5, 0)) == NULL);
}

void test_rb_new(void) {
	t_rb *rb = NULL;

	rb = rb_new(sizeof(int));
	ASSERT_RB_STATE(*rb, sizeof(int), C_MINI_RB_NB_ELEM_DFLT, 0, 0);
	rb_free(&rb);

	assert(rb_new(0) == NULL);
}
