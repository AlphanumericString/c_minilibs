#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_tail_node(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	assert(idl_get_tail_node(head) == NULL);
	assert(idl_get_tail_node(NULL) == NULL);

	idl_push_back_node(&head, &n1.dnode);
	assert(idl_get_tail_node(head) == &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	assert(idl_get_tail_node(head) == &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);
	assert(idl_get_tail_node(head) == &n3.dnode);
	assert(idl_get_tail_node(&n3.dnode) == &n3.dnode);
	assert(idl_get_tail_node(&n2.dnode) == &n3.dnode);
	assert(idl_get_tail_node(&n1.dnode) == &n3.dnode);
}
