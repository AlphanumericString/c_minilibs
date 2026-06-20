#include "rb_tests.h"

void test_rb_push(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	rb_push(&rb, &val);
	assert(rb_size(rb) == 1);

	rb_destroy(&rb);
}

void test_rb_npush(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_create(&rb, sizeof(int), 4);

	rb_npush(&rb, arr, 3);
	assert(rb_size(rb) == 3);

	rb_destroy(&rb);
}

void test_rb_push_safe(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == false);

	rb_destroy(&rb);
}

void test_rb_npush_safe(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_create(&rb, sizeof(int), 2);

	assert(rb_npush_safe(&rb, arr, 3) == false);
	assert(rb_size(rb) == 0);
	rb_destroy(&rb);
	rb_create(&rb, sizeof(int), 4);
	assert(rb_npush_safe(&rb, arr, 3) == true);
	assert(rb_size(rb) == 3);

	rb_destroy(&rb);
}
