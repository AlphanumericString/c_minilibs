#include <rb/round_buffer.h>
#include <stdio.h>

#include "rb_tests.h"

void test_rb_at(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	int *p = rb_at(&rb, 1);
	assert(p != NULL);
	assert(*p == 2);

	rb_destroy(&rb);
}

void test_rb_nat(void) { printf("MARKER_START\n");
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	printf("%s\nstate:", __PRETTY_FUNCTION__);
	rb_apply(&rb, cb_dump_int, NULL); printf("\n"); fflush(stdout);
	assert(rb_nat(&rb, 1, 2, out) != NULL);
	for (int i = 0; i < 2; i++)
		printf("out[%d]:%d\n", i, out[i]);
	assert(out[0] == 2);
	assert(out[1] == 3);

	rb_destroy(&rb);
	printf("MARKER_END\n");
}
