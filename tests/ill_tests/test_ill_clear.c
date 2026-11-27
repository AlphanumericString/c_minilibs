#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_clear(void) {
	t_cm_test_node nodes[3];
	t_cm_ill      *head = NULL;
	size_t         i;

	for (i = 0; i < 3; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back_node(&head, &nodes[i].node);
	}

	ill_clear(NULL, cb_count_dtor);
	g_dtor_hits = 0;
	ill_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(head == NULL);
	for (i = 0; i < 3; i++)
		assert(ill_is_isolated_node(&nodes[i].node) == true);

	for (i = 0; i < 3; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back_node(&head, &nodes[i].node);
	}
	ill_clear(&head, NULL);
	assert(head == NULL);
	for (i = 0; i < 3; i++)
		assert(ill_is_isolated_node(&nodes[i].node) == true);

	// clearing an empty list is a valid no-op
	ill_clear(&head, cb_count_dtor);
	assert(head == NULL);
}
