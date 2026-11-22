#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_node(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	t_cm_idl *idl_error, *idl_not_found;
	t_cm_idl *idl_ok;
	int       ok_search_val  = 20;
	int       bad_search_val = 40;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	idl_ok = idl_get_node(head, cb_idl_cmp_val, &ok_search_val);
	assert(idl_ok == &n2.dnode);
	idl_not_found = idl_get_node(head, cb_idl_cmp_val, &bad_search_val);
	assert(idl_not_found == NULL);
	idl_error = idl_get_node(NULL, cb_idl_cmp_val, &bad_search_val);
	assert(idl_error == NULL);
	idl_error = idl_get_node(head, NULL, &bad_search_val);
	assert(idl_error == NULL);
}
