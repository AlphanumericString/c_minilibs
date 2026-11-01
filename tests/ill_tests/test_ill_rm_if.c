#include <assert.h>

#include "ill_tests.h"

NO_COV static size_t collect(t_ill *head, int *out, size_t cap) {
	size_t n = 0;

	while (head && n < cap) {
		out[n++] = ((t_test_node *)ill_ntoe(head))->value;
		head     = head->next;
	}
	return n;
}

NO_COV void test_ill_rm_if(void) {
	t_test_node nodes[6];
	t_ill      *head = NULL;
	int         out[6];
	int         exp_keep[3] = {2, 4, 6};
	size_t      i, n;

	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodes[i].node);
	}

	g_dtor_hits = 0;
	n           = ill_rm_if(&head, cb_ill_is_even, NULL, cb_count_dtor);
	assert(n == 3);
	assert(g_dtor_hits == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);

	// without dtor
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	assert(ill_rm_if(&head, cb_ill_is_even, NULL, NULL) == 3);
	assert(ill_length(head) == 3);

	// all removed => head becomes NULL
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	assert(ill_rm_if(&head, cb_false, NULL, NULL) == 6);
	assert(head == NULL);

	// bad args
	assert(ill_rm_if(NULL, cb_ill_is_even, NULL, NULL) == 0);
	assert(ill_rm_if(&head, NULL, NULL, NULL) == 0);
}
