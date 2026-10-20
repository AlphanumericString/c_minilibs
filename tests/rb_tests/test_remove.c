#include <assert.h>

#include "rb_tests.h"

NO_COV void test_rb_drop(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val);
	rb_push(&rb, &val);

	assert(rb_drop(&rb, 1) == true);
	assert(rb_size(rb) == 1);
	assert(rb_drop(&rb, 99) == true);
	assert(rb_size(rb) == 0);

	assert(rb_drop(NULL, 1) == false);
	assert(rb_drop(&rb, 0) == false);

	rb_destroy(&rb);
}

NO_COV void test_rb_pop(void) {
	t_rb  rb   = {0};
	int   val1 = 42, val2 = 84, out = 0;
	void *p, *p2;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);
	rb_push(&rb, &val2);

	p = rb_at(rb, 0);
	assert(rb_pop(&rb, &out) == p && p != NULL);
	assert(out == 42);
	assert(rb_size(rb) == 1);

	p = NULL;
	assert(rb_pop(NULL, &out) == p && p == NULL);
	assert(out == 42);
	assert(rb_size(rb) == 1);

	p  = rb_at(rb, 0);
	p2 = rb_pop(&rb, NULL);
	assert(p2 != NULL && *(int *)p2 == 84 && p == p2);
	assert(rb_size(rb) == 0);

	assert(rb_pop(&rb, &out) == NULL);
	assert(out == 0);
	assert(rb_size(rb) == 0);

	rb_destroy(&rb);
}

NO_COV void test_rb_npop(void) {
	t_rb  rb   = {0};
	int   val1 = 1, val2 = 2, val3 = 3;
	int   out[2] = {0};
	void *p;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);
	rb_push(&rb, &val2);
	rb_push(&rb, &val3);

	p = rb_at(rb, 0);
	assert(rb_npop(&rb, 2, out) == p && p != NULL);
	assert(out[0] == 1);
	assert(out[1] == 2);
	assert(rb_size(rb) == 1);

	p = rb_at(rb, 0);
	assert(rb_npop(&rb, 2, out) == p && p != NULL);
	assert(out[0] == 3);
	// assert(out[1] == 0);
	assert(rb_size(rb) == 0);

	assert(rb_npop(NULL, 2, out) == NULL);
	assert(rb_npop(&rb, 0, out) == NULL);
	assert(rb_npop(&rb, 99, out) == NULL);

	rb_destroy(&rb);
}

NO_COV void test_rb_peek(void) {
	t_rb  rb   = {0};
	int   val1 = 42, out = 0;
	void *p;

	rb_init(&rb, sizeof(int));
	rb_push(&rb, &val1);

	out = 0;
	p   = rb_at(rb, 0);
	assert(rb_peek(rb, &out) == p && p != NULL);
	assert(out == 42);
	assert(rb_size(rb) == 1);

	out = 99;
	p   = rb_at(rb, 0);
	assert(rb_peek(rb, NULL) == p && p != NULL);
	assert(out == 99);
	assert(rb_size(rb) == 1);

	assert(rb_peek((t_rb){0}, &out) == NULL);
	rb_destroy(&rb);
}

NO_COV void test_rb_npeek(void) {
	t_rb  rb = {0};
	t_rb  malicious;
	int   vals[]                                  = {1, 2, 3};
	int   out[2]                                  = {0};
	int   out_big[sizeof(vals) / sizeof(vals[0])] = {0};
	void *p;

	rb_from_array(&rb, vals, 3, sizeof(int));
	rb.count = 3;

	p = rb_at(rb, 0);
	assert(rb_npeek(rb, 2, out) == p && p != NULL);
	assert(out[0] == 1);
	assert(out[1] == 2);
	assert(rb_size(rb) == 3);

	rb.o_pos = 2;
	p        = rb_at(rb, 0);
	assert(rb_npeek(rb, 2, out) == p && p != NULL);
	assert(out[0] == 3);
	assert(out[1] == 1);
	assert(rb_size(rb) == 3);

	p = rb_at(rb, 0);
	assert(rb_npeek(rb, -1, out_big) == p && p != NULL);
	assert(out_big[0] == 3);
	assert(out_big[1] == 1);
	assert(out_big[2] == 2);
	assert(rb_size(rb) == 3);
	assert(rb_npeek(rb, 1, NULL) != NULL);
	assert(rb_npeek(rb, 1, NULL) == rb_at(rb, 0));
	assert(rb_npeek(rb, 99, NULL) == rb_at(rb, 0));
	assert(rb_npeek(rb, -1, NULL) == rb_at(rb, 0));
	malicious       = rb;
	malicious.array = NULL;
	assert(rb_npeek(malicious, 2, out) == NULL);
	malicious       = rb;
	malicious.count = 0;
	assert(rb_npeek(malicious, 2, out) == NULL);
	assert(rb_npeek(rb, 0, out) == NULL);
	assert(rb_npeek((t_rb){0}, 2, out) == NULL);
}
