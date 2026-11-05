#include <assert.h>

#include "ill_tests.h"

NO_COV static size_t collect(t_cm_ill *head, int *out, size_t cap) {
	size_t n = 0;

	while (head && n < cap) {
		out[n++] = ((t_cm_test_node *)ill_ntoe(head))->value;
		head     = head->next;
	}
	return n;
}

NO_COV void test_cm_ill_unique(void) {
	t_cm_test_node nodes[6];
	t_cm_test_node nodup[4];
	t_cm_ill      *head        = NULL;
	int            values[][6] = {
        {1, 1, 2, 2, 2, 3},
        {1, 1, 2, 2, 3, 3},
    };
	int    exp[3] = {1, 2, 3};
	int    out[6];
	size_t i;

	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], values[0][i], (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	g_dtor_hits = 0;
	ill_unique(head, cb_ill_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(ill_length(head) == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp[i]);

	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], values[1][i], (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	g_dtor_hits = 0;
	ill_unique(head, cb_ill_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(ill_length(head) == 3);
	assert(collect(head, out, 6) == 3);
	for (i = 0; i < 3; i++)
		assert(out[i] == exp[i]);

	// without dtor
	head = NULL;
	for (i = 0; i < 6; i++) {
		INIT_NODE(nodes[i], values[0][i], (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	ill_unique(head, cb_ill_cmp_min_max, NULL);
	assert(ill_length(head) == 3);

	// no consecutive duplicates
	head = NULL;
	for (i = 0; i < 4; i++) {
		INIT_NODE(nodup[i], (int)(i + 1), (int)i);
		ill_push_back(&head, &nodup[i].node);
	}
	g_dtor_hits = 0;
	ill_unique(head, cb_ill_cmp_min_max, cb_count_dtor);
	assert(g_dtor_hits == 0);
	assert(ill_length(head) == 4);

	// bad args
	ill_unique(NULL, cb_ill_cmp_min_max, cb_count_dtor);
	ill_unique(head, NULL, cb_count_dtor);
}
