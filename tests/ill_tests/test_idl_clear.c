#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_clear(void) {
	t_cm_test_dnode nodes[3];
	t_cm_idl       *head = NULL;
	size_t          i;

	for (i = 0; i < 3; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back_node(&head, &nodes[i].dnode);
	}

	idl_clear(NULL, cb_count_dtor);
	g_dtor_hits = 0;
	idl_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(head == NULL);
	for (i = 0; i < 3; i++) {
		assert(nodes[i].dnode.next == NULL);
		assert(nodes[i].dnode.prev == NULL);
	}

	for (i = 0; i < 3; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back_node(&head, &nodes[i].dnode);
	}
	idl_clear(&head, NULL);
	assert(head == NULL);
	for (i = 0; i < 3; i++) {
		assert(nodes[i].dnode.next == NULL);
		assert(nodes[i].dnode.prev == NULL);
	}

	// clearing an empty list is a valid no-op
	idl_clear(&head, cb_count_dtor);
	assert(head == NULL);
}
