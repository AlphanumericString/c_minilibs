#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_prepend(void) {
	t_test_dnode n0, n1, n2, n3, n4;
	t_test_dnode n7, n8;
	t_idl       *head = NULL;

	INIT_DNODE(n0, 0, 0);
	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	INIT_DNODE(n7, 46, 7);
	INIT_DNODE(n8, 47, 8);
	LINK_UP_DNODES(&n7.dnode, &n8.dnode);
	idl_npush(&head, &n7.dnode);

	idl_prepend(&n1.dnode, &n0.dnode);
	assert(n8.dnode.next == &n0.dnode);
	assert(n0.dnode.prev == &n8.dnode);
	assert(n0.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n0.dnode);
	idl_prepend(&n1.dnode, NULL);
	idl_prepend(NULL, &n0.dnode);
}
