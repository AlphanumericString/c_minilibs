#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_reverse(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head  = NULL;
	t_cm_idl       *empty = NULL;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	idl_reverse(&head);
	assert(head == &n3.dnode);
	assert(n3.dnode.next == &n2.dnode);
	assert(n3.dnode.prev == NULL);
	assert(n2.dnode.next == &n1.dnode);
	assert(n2.dnode.prev == &n3.dnode);
	assert(n1.dnode.next == NULL);
	assert(n1.dnode.prev == &n2.dnode);

	idl_reverse(&head);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == NULL);
	assert(n2.dnode.next == &n3.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n3.dnode.next == NULL);
	assert(n3.dnode.prev == &n2.dnode);

	idl_reverse(NULL);
	idl_reverse(&empty);
	assert(empty == NULL);
}
