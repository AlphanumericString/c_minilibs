#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_at(void) {
	t_cm_rb rb     = {0};
	int     arr[3] = {1, 2, 3};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_at(rb, 1) != NULL);
	assert(*(int *)rb_at(rb, 1) == 2);
	assert(rb_at(rb, 0) != NULL);
	assert(*(int *)rb_at(rb, 0) == 1);
	assert(rb_at(rb, 2) != NULL);
	assert(*(int *)rb_at(rb, 2) == 3);

	assert(rb_at(rb, 999) == NULL);
	assert(rb_at((t_cm_rb){0}, 2) == NULL);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_nat(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *arr;
	int    *out;

	rb_create(&rb, sizeof(int), 10);
	cap = rb_capacity(rb);
	arr = malloc(sizeof(int) * cap);
	out = malloc(sizeof(int) * (cap + 2));
	assert(arr != NULL && out != NULL);
	for (size_t i = 0; i < cap; i++)
		arr[i] = (int)i + 1;
	rb_npush(&rb, arr, cap);

	assert(rb.o_pos == 0);
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, 0, 2, out) != NULL); // contiguous start
	assert(out[0] == 1 && out[1] == 2);
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, cap - 1, 2, out) != NULL); // only one element left
	assert(out[0] == (int)cap && out[1] == 0);
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, 2, (size_t)-1, out) != NULL); // n > left is clamped
	assert(out[0] == 3 && out[cap - 3] == (int)cap && out[cap - 2] == 0);

	// move rb's o_pos across the window
	rb.o_pos = cap - 2;
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, 0, 2, out) != NULL); // contiguous start
	assert(out[0] == (int)cap - 1 && out[1] == (int)cap);
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, 1, 3, out) != NULL); // crosses the window
	assert(out[0] == (int)cap && out[1] == 1 && out[2] == 2);
	bzero(out, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, cap - 1, 2, out) != NULL); // ends on the window edge
	assert(out[0] == (int)cap - 2 && out[1] == 0);

	out[0] = -1;
	out[1] = -1;
	assert(rb_nat(rb, cap + 1, 2, out) == NULL);
	assert(out[0] == -1 && out[1] == -1);
	assert(rb_nat(rb, 0, 2, NULL) == NULL);
	assert(rb_nat((t_cm_rb){0}, 2, 2, out) == NULL);
	assert(rb_nat(rb, 2, 0, out) == NULL);

	free(arr);
	free(out);
	rb_destroy(&rb);
}
