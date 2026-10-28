#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_push(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);
	(void)n3;
	(void)n4;

	assert(idl_push(&head, &n2.dnode) != NULL);
	assert(head == &n2.dnode);
	assert(idl_push(NULL, &n2.dnode) == NULL);
	assert(idl_push(&head, NULL) == NULL);

	idl_push(&head, &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n2.dnode.prev == &n1.dnode);
}
