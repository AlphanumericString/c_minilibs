#include <assert.h>
#include <strings.h>

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

	assert(rb_at(rb, 999) == NULL);
	assert(rb_at((t_rb){0}, 2) == NULL);

	rb_destroy(&rb);
}

void test_rb_nat(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb.o_pos == 0);
	assert(rb_nat(rb, 0, 2, out) != NULL); // contiguous start
	assert(out[0] == 1);
	assert(out[1] == 2);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, 1, 2, out) != NULL); // contiguous {pos}
	assert(out[0] == 2);
	assert(out[1] == 3);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, 2, 99, out) != NULL); // contiguous {pos} + n > max
	assert(out[0] == 3);
	assert(out[1] == 4);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, rb_size(rb) - 1, 2, out) != NULL); // loop past %
	assert(out[0] == 4);
	assert(out[1] == 1);

	// move rb's o_pos
	rb.o_pos = 2;
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, 0, 2, out) != NULL); // contiguous start
	assert(out[0] == 3);
	assert(out[1] == 4);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, 1, 2, out) != NULL); // contiguous {pos}
	assert(out[0] == 4);
	assert(out[1] == 1);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, 1, 99, out) != NULL); // contiguous {pos} + n > max
	assert(out[0] == 4);
	assert(out[1] == 1);
	bzero(&out, sizeof(out));
	assert(rb_nat(rb, rb_size(rb) - 1, 2, out) != NULL); // loop past %
	assert(out[0] == 2);
	assert(out[1] == 3);

	out[0] = -1;
	out[1] = -1;
	assert(rb_nat(rb, 999, 2, out) == NULL);
	assert(out[0] == -1);
	assert(out[1] == -1);
	assert(rb_nat(rb, 0, 2, NULL) == NULL);
	assert(out[0] == -1);
	assert(out[1] == -1);
	assert(rb_nat((t_rb){0}, 2, 2, out) == NULL);
	assert(out[0] == -1);
	assert(out[1] == -1);
	assert(rb_nat(rb, 2, 0, out) == NULL);
	assert(out[0] == -1);
	assert(out[1] == -1);

	rb_destroy(&rb);
}
