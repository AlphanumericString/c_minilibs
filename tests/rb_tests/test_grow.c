#include <assert.h>
#include <stdlib.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_grow(void) {
	int      arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
	t_cm_rb  rb    = {0};
	t_cm_rb  malicious;
	t_cm_rb *p;

	rb_create(&rb, sizeof(int), 2);
	rb_npush(&rb, arr, 2);
	assert(rb.nb_max == 2 && rb.count == 2); // state pre-tests

	assert(rb.o_pos == 0);
	p = rb_grow(&rb, 5);
	assert(p != NULL);
	assert(p->nb_max == 7 && rb.nb_max == 2);
	assert(*(int *)rb_at(*p, 0) == *(int *)rb_at(rb, 0));
	assert(*(int *)rb_at(*p, 1) == *(int *)rb_at(rb, 1));
	assert(p->i_pos == 2 && p->o_pos == 0);
	assert(rb.o_pos == 0);
	rb_destroy(p);
	free(p);

	rb.o_pos = 1; // change state for tests

	assert(rb.nb_max == 2 && rb.count == 2); // state pre-tests
	assert(rb.o_pos == 1);

	p = rb_grow(&rb, 5);
	assert(p != NULL);
	assert(p->nb_max == 7 && rb.nb_max == 2);
	assert(*(int *)rb_at(*p, 0) == *(int *)rb_at(rb, 0));
	assert(*(int *)rb_at(*p, 1) == *(int *)rb_at(rb, 1));
	assert(p->i_pos == 2 && p->o_pos == 0);
	assert(rb.o_pos == 1);
	rb_destroy(p);
	free(p);

	malicious      = rb;
	malicious.sz_e = 0;
	p              = rb_grow(&malicious, 5);
	assert(p == NULL);
	p = rb_grow(NULL, 5);
	assert(p == NULL);

	alloc_set_call_limit(0);
	p = rb_grow(&rb, 5);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(p == NULL);

	rb_destroy(&rb);
}
