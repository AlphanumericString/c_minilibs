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

NO_COV void test_idl_rm_if(void) {
	t_test_dnode nodes[6];
	t_idl       *head = NULL;
	int          out[6];
	int          exp_keep[3] = {2, 4, 6};
	size_t       i, n;

	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}

	g_dtor_hits = 0;
	n           = idl_rm_if(&head, cb_idl_is_even, NULL, cb_count_dtor);
	assert(n == 3);
	assert(g_dtor_hits == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp_keep[i]);

	// without dtor
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	assert(idl_rm_if(&head, cb_idl_is_even, NULL, NULL) == 3);
	assert(idl_length(head) == 3);

	// all removed => head becomes NULL
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	assert(idl_rm_if(&head, cb_false, NULL, NULL) == 6);
	assert(head == NULL);

	// bad args
	assert(idl_rm_if(NULL, cb_idl_is_even, NULL, NULL) == 0);
	assert(idl_rm_if(&head, NULL, NULL, NULL) == 0);
}
