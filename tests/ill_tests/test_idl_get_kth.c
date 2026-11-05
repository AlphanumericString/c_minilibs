#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_kth(void) {
	t_cm_test_dnode  n1, n2, n3;
	t_cm_idl        *head = NULL;
	t_cm_test_dnode *node_not_found, *node_error;
	t_cm_test_dnode *node_ok;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	node_ok = (t_cm_test_dnode *)idl_get_kth(head, 0);
	assert(node_ok == &n1);
	node_ok = (t_cm_test_dnode *)idl_get_kth(head, 1);
	assert(node_ok == &n2);
	node_not_found = (t_cm_test_dnode *)idl_get_kth(head, 99);
	assert(node_not_found == NULL);
	node_error = (t_cm_test_dnode *)idl_get_kth(NULL, 3);
	assert(node_error == NULL);
}
