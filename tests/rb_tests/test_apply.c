#include <assert.h>

#include "rb_tests.h"

void test_rb_apply(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_apply(&rb, cb_add_one, NULL) == true);
	assert(*(int *)rb_at(rb, 0) == 2);
	assert(*(int *)rb_at(rb, 1) == 3);
	assert(*(int *)rb_at(rb, 2) == 4);

	rb_destroy(&rb);
}

void test_rb_naplly(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};
	int  out    = 0;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_naplly(&rb, 2, cb_add_one, NULL) == true);
	rb_pop(&rb, &out);
	assert(out == 2);
	rb_pop(&rb, &out);
	assert(out == 3);
	rb_pop(&rb, &out);
	assert(out == 3);

	rb_destroy(&rb);
}
