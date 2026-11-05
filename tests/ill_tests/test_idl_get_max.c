#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_max(void) {
	t_cm_test_dnode  n1, n2, n3, n4;
	t_cm_idl        *head = NULL;
	t_cm_test_dnode *node_ok, *node_error;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	INIT_DNODE(n4, 5, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	node_ok = (t_cm_test_dnode *)idl_get_max(head, cb_idl_cmp_min_max);
	assert(node_ok == &n3);
	node_error = (t_cm_test_dnode *)idl_get_max(head, NULL);
	assert(node_error == NULL);
	node_error = (t_cm_test_dnode *)idl_get_max(NULL, cb_idl_cmp_min_max);
	assert(node_error == NULL);
}
