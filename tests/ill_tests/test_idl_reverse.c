#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_reverse(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	t_cm_idl *reversed = idl_reverse(head);
	assert(reversed == &n3.dnode);
	assert(n3.dnode.next == &n2.dnode);
	assert(n3.dnode.prev == NULL);
	assert(n2.dnode.next == &n1.dnode);
	assert(n2.dnode.prev == &n3.dnode);
	assert(n1.dnode.next == NULL);
	assert(n1.dnode.prev == &n2.dnode);

	t_cm_idl *reversed_back = idl_reverse(reversed);
	assert(reversed_back == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == NULL);
	assert(n2.dnode.next == &n3.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n3.dnode.next == NULL);
	assert(n3.dnode.prev == &n2.dnode);

	assert(idl_reverse(NULL) == NULL);
}
