#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_push_back(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	assert(idl_push_back(&head, &n4.dnode) != NULL);
	// State assert: [n1, n2, n4]
	assert(n2.dnode.next == &n4.dnode);
	assert(n4.dnode.prev == &n2.dnode);
	assert(n4.dnode.next == NULL);

	assert(idl_push_back(NULL, &n2.dnode) == NULL);
	assert(idl_push_back(&head, NULL) == NULL);
	(void)n3;
}
