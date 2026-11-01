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

NO_COV void test_idl_unique(void) {
	t_test_dnode nodes[6];
	t_test_dnode nodup[4];
	t_idl       *head        = NULL;
	int          values[][6] = {
        {1, 1, 2, 2, 2, 3},
        {1, 1, 2, 2, 3, 3},
    };
	int    exp[3] = {1, 2, 3};
	int    out[6];
	size_t i;

	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], values[0][i], (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	g_dtor_hits = 0;
	idl_unique(head, cb_idl_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(idl_length(head) == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp[i]);

	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], values[1][i], (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	g_dtor_hits = 0;
	idl_unique(head, cb_idl_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(idl_length(head) == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp[i]);

	// without dtor
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_DNODE(nodes[i], values[0][i], (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	idl_unique(head, cb_idl_cmp_min_max, NULL);
	assert(idl_length(head) == 3);

	// no consecutive duplicates
	head = NULL;
	for (i = 0; i < 4; i++) {
		INIT_DNODE(nodup[i], (int)(i + 1), (int)i);
		idl_push_back(&head, &nodup[i].dnode);
	}
	g_dtor_hits = 0;
	idl_unique(head, cb_idl_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 0);
	assert(idl_length(head) == 4);

	// bad args
	idl_unique(NULL, cb_idl_cmp_min_max, cb_count_dtor);
	idl_unique(head, NULL, cb_count_dtor);
}
