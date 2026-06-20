#include "rb_tests.h"

void test_rb_full(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	assert(rb_full(rb) == false);
	rb_push(&rb, &val);
	assert(rb_full(rb) == false);
	rb_push(&rb, &val);
	assert(rb_full(rb) == true);

	rb_destroy(&rb);
}

void test_rb_empty(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_init(&rb, sizeof(int));

	assert(rb_empty(rb) == true);
	rb_push(&rb, &val);
	assert(rb_empty(rb) == false);

	rb_destroy(&rb);
}

void test_rb_size(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_init(&rb, sizeof(int));

	assert(rb_size(rb) == 0);
	rb_push(&rb, &val);
	assert(rb_size(rb) == 1);
	rb_push(&rb, &val);
	assert(rb_size(rb) == 2);

	rb_destroy(&rb);
}

void test_rb_cappacity(void) {
	t_rb rb = {0};

	rb_create(&rb, sizeof(int), 10);

	assert(rb_cappacity(rb) == 10);

	rb_destroy(&rb);
}

void test_rb_free(void) {
	t_rb rb  = {0};
	int  val = 42;
	rb_create(&rb, sizeof(int), 10);

	assert(rb_free(rb) == 10);
	rb_push(&rb, &val);
	assert(rb_free(rb) == 9);

	rb_destroy(&rb);
}
