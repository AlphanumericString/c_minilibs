#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_get_tail(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	assert(idl_get_tail(head) == NULL);
	assert(idl_get_tail(NULL) == NULL);

	idl_push_back(&head, &n1.dnode);
	assert(idl_get_tail(head) == &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	assert(idl_get_tail(head) == &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	assert(idl_get_tail(head) == &n3.dnode);
	assert(idl_get_tail(&n3.dnode) == &n3.dnode);
	assert(idl_get_tail(&n2.dnode) == &n3.dnode);
	assert(idl_get_tail(&n1.dnode) == &n3.dnode);
}
