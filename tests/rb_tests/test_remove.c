#include "rb_tests.h"

void test_rb_drop(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val);
	rb_push(&rb, &val);

	assert(rb_drop(&rb, 1) == true);
	assert(rb_size(rb) == 1);

	rb_destroy(&rb);
}

void test_rb_pop(void) {
	t_rb rb   = {0};
	int  val1 = 42, val2 = 84, out = 0;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);
	rb_push(&rb, &val2);

	rb_pop(&rb, &out);
	assert(out == 42);
	assert(rb_size(rb) == 1);

	rb_destroy(&rb);
}

void test_rb_npop(void) {
	t_rb rb   = {0};
	int  val1 = 1, val2 = 2, val3 = 3;
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);
	rb_push(&rb, &val2);
	rb_push(&rb, &val3);

	rb_npop(&rb, 2, out);
	assert(out[0] == 1);
	assert(out[1] == 2);
	assert(rb_size(rb) == 1);

	rb_destroy(&rb);
}

void test_rb_peek(void) {
	t_rb rb   = {0};
	int  val1 = 42, out = 0;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);

	rb_peek(rb, &out);
	assert(out == 42);
	assert(rb_size(rb) == 1);

	rb_destroy(&rb);
}

void test_rb_npeek(void) {
	t_rb rb   = {0};
	int  val1 = 1, val2 = 2, val3 = 3;
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);
	rb_push(&rb, &val2);
	rb_push(&rb, &val3);

	rb_npeek(rb, 2, out);
	assert(out[0] == 1);
	assert(out[1] == 2);
	assert(rb_size(rb) == 3);

	rb_destroy(&rb);
}
