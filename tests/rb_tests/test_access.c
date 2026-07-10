#include <assert.h>

#include "rb_tests.h"

void test_rb_at(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_at(rb, 1) != NULL);
	assert(*(int *)rb_at(rb, 1) == 2);
	assert(rb_at(rb, 0) != NULL);
	assert(*(int *)rb_at(rb, 0) == 1);
	assert(rb_at(rb, 2) != NULL);
	assert(*(int *)rb_at(rb, 2) == 3);

	rb_destroy(&rb);
}

void test_rb_nat(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_nat(rb, 1, 2, out) != NULL);
	assert(out[0] == 3);
	assert(out[1] == 4);
	assert(rb_nat(rb, 0, 2, out) != NULL);
	assert(out[0] == 2);
	assert(out[1] == 3);

	rb_destroy(&rb);
}
