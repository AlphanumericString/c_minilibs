#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_nappend(void) {
	t_test_dnode n1, n2, n3, n4;
	t_test_dnode n5, n6, n_tmp1;
	t_idl       *head = NULL;

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	INIT_DNODE(n5, 46, 5);
	INIT_DNODE(n6, 47, 6);
	idl_append(&n5.dnode, &n6.dnode);
	// State assert: [n1, n2, n3, n4] [n5, n6]
	idl_nappend(&n4.dnode, &n5.dnode);
	// State assert: [n1, n2, n3, n4, n5, n6]
	assert(n4.dnode.next == &n5.dnode && n5.dnode.prev == &n4.dnode);
	assert(n5.dnode.next == &n6.dnode && n6.dnode.prev == &n5.dnode);
	assert(n6.dnode.next == NULL);
	idl_nappend(&n4.dnode, NULL);
	idl_nappend(NULL, &n5.dnode);

	INIT_DNODE(n_tmp1, 48, 7);
	idl_nappend(&n1.dnode, &n_tmp1.dnode);
	assert(n1.dnode.next == &n_tmp1.dnode && n1.dnode.prev == NULL);
	assert(n_tmp1.dnode.next == &n2.dnode && n_tmp1.dnode.prev == &n1.dnode);
	n1.dnode.next = &n2.dnode;
	n2.dnode.prev = &n1.dnode;
}
