#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_kth_node(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;
	t_cm_idl       *node_not_found, *node_error;
	t_cm_idl       *node_ok;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	node_ok = idl_get_kth_node(head, 0);
	assert(node_ok == &n1.dnode);
	node_ok = idl_get_kth_node(head, 1);
	assert(node_ok == &n2.dnode);
	node_not_found = idl_get_kth_node(head, 99);
	assert(node_not_found == NULL);
	node_error = idl_get_kth_node(NULL, 3);
	assert(node_error == NULL);
}
