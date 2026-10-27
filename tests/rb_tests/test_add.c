#include <assert.h>

#include "rb_tests.h"

NO_COV void test_rb_push(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	rb_push(&rb, &val);
	assert(rb_size(rb) == 1);
	rb_push(&rb, NULL);
	rb_push(NULL, &val);

	rb_destroy(&rb);
}

NO_COV void test_rb_npush(void) {
	t_rb rb          = {0};
	int  arr[3]      = {1, 2, 3};
	int  arr_large[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

	rb_create(&rb, sizeof(int), 4);

	rb_npush(&rb, arr, 3);
	assert(rb_size(rb) == 3);
	// i_pos at 3 - cycle "moved" by 3
	rb_npush(&rb, arr_large, sizeof(arr_large) / sizeof(arr_large[0]));
	assert(*(int *)rb_at(rb, 0) == 10);
	assert(*(int *)rb_at(rb, 1) == 11);
	assert(*(int *)rb_at(rb, 2) == 12);
	assert(*(int *)rb_at(rb, 3) == 9);

	rb_clr(&rb);
	// i_pos at 0 - cycle "fixed"
	rb_npush(&rb, arr_large, sizeof(arr_large) / sizeof(arr_large[0]));
	assert(rb_size(rb) == 4);
	assert(*(int *)rb_at(rb, 0) == 9);
	assert(*(int *)rb_at(rb, 1) == 10);
	assert(*(int *)rb_at(rb, 2) == 11);
	assert(*(int *)rb_at(rb, 3) == 12);

	rb_npush(&rb, NULL, 12);
	rb_npush(NULL, arr, 12);
	rb_destroy(&rb);
}

NO_COV void test_rb_push_safe(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == false);
	assert(rb_push_safe(NULL, &val) == false);
	assert(rb_push_safe(&rb, false) == false);

	rb_destroy(&rb);
}

NO_COV void test_rb_npush_safe(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_create(&rb, sizeof(int), 2);

	assert(rb_npush_safe(&rb, arr, 3) == false);
	assert(rb_size(rb) == 0);
	rb_destroy(&rb);
	rb_create(&rb, sizeof(int), 4);
	assert(rb_npush_safe(&rb, arr, 3) == true);
	assert(rb_size(rb) == 3 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, arr, 2) == false);
	assert(rb_size(rb) == 3 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, arr, 1) == true);
	assert(rb_size(rb) == 4 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, NULL, 12) == false);
	assert(rb_npush_safe(NULL, arr, 12) == false);

	rb_destroy(&rb);
}
