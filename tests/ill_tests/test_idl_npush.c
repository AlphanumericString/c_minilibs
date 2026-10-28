#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_npush(void) {
	t_test_dnode n1, n2, n3, n4;
	t_test_dnode n7, n8;
	t_idl       *head = NULL;

	INIT_STD_DNODES(n1, n2, n3, n4);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);
	INIT_DNODE(n7, 46, 7);
	INIT_DNODE(n8, 47, 8);
	LINK_UP_DNODES(&n7.dnode, &n8.dnode);

	assert(idl_npush(&head, &n7.dnode) != NULL);
	assert(head == &n7.dnode);
	assert(n8.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n8.dnode);
	assert(idl_npush(&head, NULL) == NULL);
	assert(idl_npush(NULL, &n7.dnode) == NULL);

	head = NULL;
	INIT_DNODE(n7, 46, 7);
	INIT_DNODE(n8, 47, 8);
	LINK_UP_DNODES(&n7.dnode, &n8.dnode);
	assert(idl_npush(&head, &n7.dnode) != NULL);
	assert(head == &n7.dnode);
}
