#include <assert.h>

#include "ill_tests.h"

NO_COV static size_t collect(t_idl *head, int *out, size_t cap) {
	size_t n    = 0;
	t_idl *prev = NULL;

	while (head && n < cap) {
		assert(head->prev == prev);
		out[n++] = ((t_test_dnode *)idl_ntoe(head))->value;
		prev     = head;
		head     = head->next;
	}
	return n;
}

NO_COV void test_idl_filter(void) {
	t_test_dnode nodes[6];
	t_idl       *head = NULL;
	t_idl       *filtered;
	int          out[6];
	int          exp_keep[3]     = {2, 4, 6};
	int          exp_filtered[3] = {1, 3, 5};
	size_t       i, n;

	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}

	n = idl_filter(&head, cb_idl_is_even, NULL, &filtered);
	assert(n == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);
	assert(collect(filtered, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_filtered[i]);

	// all elements moved out => head becomes NULL
	{
		t_test_dnode e1, e2, e3;
		t_idl       *h = NULL;

		INIT_DNODE(e1, 1, 1);
		INIT_DNODE(e2, 3, 2);
		INIT_DNODE(e3, 5, 3);
		idl_push_back(&h, &e1.dnode);
		idl_push_back(&h, &e2.dnode);
		idl_push_back(&h, &e3.dnode);
		assert(idl_filter(&h, cb_idl_is_even, NULL, &filtered) == 3);
		assert(h == NULL);
		assert(collect(filtered, out, 6) == 3);

		assert(idl_filter(&h, cb_idl_is_even, NULL, &filtered) == 0);
		assert(h == NULL);
	}

	assert(idl_filter(NULL, cb_idl_is_even, NULL, &filtered) == 0);
	assert(idl_filter(&head, NULL, NULL, &filtered) == 0);
	assert(idl_filter(&head, cb_idl_is_even, NULL, NULL) == 0);
}

NO_COV void test_idl_filter_out(void) {
	t_test_dnode nodes[6];
	t_idl       *head = NULL;
	t_idl       *filtered;
	int          out[6];
	int          exp_keep[3]     = {1, 3, 5};
	int          exp_filtered[3] = {2, 4, 6};
	size_t       i, n;

	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	n = idl_filter_out(&head, cb_idl_is_even, NULL, &filtered);
	assert(n == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);
	assert(collect(filtered, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_filtered[i]);

	// head that would be "out" is now moved out
	{
		t_test_dnode e1, e2, e3;
		t_idl       *h = NULL;

		INIT_DNODE(e1, 2, 1);
		INIT_DNODE(e2, 4, 2);
		INIT_DNODE(e3, 5, 3);
		idl_push_back(&h, &e1.dnode);
		idl_push_back(&h, &e2.dnode);
		idl_push_back(&h, &e3.dnode);
		assert(idl_filter_out(&h, cb_idl_is_even, NULL, &filtered) == 2);
		assert(collect(h, out, 6) == 1);
		assert(out[0] == 5);
		assert(collect(filtered, out, 6) == 2);
		assert(out[0] == 2 && out[1] == 4);
	}

	assert(idl_filter_out(NULL, cb_idl_is_even, NULL, &filtered) == 0);
	assert(idl_filter_out(&head, NULL, NULL, &filtered) == 0);
	assert(idl_filter_out(&head, cb_idl_is_even, NULL, NULL) == 0);
}
