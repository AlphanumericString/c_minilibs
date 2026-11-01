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

NO_COV void test_ill_filter(void) {
	t_test_node nodes[6];
	t_ill      *head = NULL;
	t_ill      *filtered;
	int         out[6];
	int         exp_keep[3]     = {2, 4, 6};
	int         exp_filtered[3] = {1, 3, 5};
	size_t      i, n;

	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodes[i].node);
	}

	n = ill_filter(&head, cb_ill_is_even, NULL, &filtered);
	assert(n == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);
	assert(collect(filtered, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_filtered[i]);

	// all elements moved out => head becomes NULL
	{
		t_test_node e1, e2, e3;
		t_ill      *h = NULL;

		INIT_NODE(e1, 1, 1);
		INIT_NODE(e2, 3, 2);
		INIT_NODE(e3, 5, 3);
		ill_push_back(&h, &e1.node);
		ill_push_back(&h, &e2.node);
		ill_push_back(&h, &e3.node);
		assert(ill_filter(&h, cb_ill_is_even, NULL, &filtered) == 3);
		assert(h == NULL);
		assert(collect(filtered, out, 6) == 3);

		assert(ill_filter(&h, cb_ill_is_even, NULL, &filtered) == 0);
		assert(h == NULL);
	}

	assert(ill_filter(NULL, cb_ill_is_even, NULL, &filtered) == 0);
	assert(ill_filter(&head, NULL, NULL, &filtered) == 0);
	assert(ill_filter(&head, cb_ill_is_even, NULL, NULL) == 0);
}

NO_COV void test_ill_filter_out(void) {
	t_test_node nodes[6];
	t_ill      *head = NULL;
	t_ill      *filtered;
	int         out[6];
	int         exp_keep[3]     = {1, 3, 5};
	int         exp_filtered[3] = {2, 4, 6};
	size_t      i, n;

	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	n = ill_filter_out(&head, cb_ill_is_even, NULL, &filtered);
	assert(n == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);
	assert(collect(filtered, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_filtered[i]);

	// head that would be "out" is now moved out
	{
		t_test_node e1, e2, e3;
		t_ill      *h = NULL;

		INIT_NODE(e1, 2, 1);
		INIT_NODE(e2, 4, 2);
		INIT_NODE(e3, 5, 3);
		ill_push_back(&h, &e1.node);
		ill_push_back(&h, &e2.node);
		ill_push_back(&h, &e3.node);
		assert(ill_filter_out(&h, cb_ill_is_even, NULL, &filtered) == 2);
		assert(collect(h, out, 6) == 1);
		assert(out[0] == 5);
		assert(collect(filtered, out, 6) == 2);
		assert(out[0] == 2 && out[1] == 4);
	}

	assert(ill_filter_out(NULL, cb_ill_is_even, NULL, &filtered) == 0);
	assert(ill_filter_out(&head, NULL, NULL, &filtered) == 0);
	assert(ill_filter_out(&head, cb_ill_is_even, NULL, NULL) == 0);
}
