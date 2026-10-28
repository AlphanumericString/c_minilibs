#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_get_prev(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	t_idl *idl_error, *idl_not_found;
	t_idl *idl_ok;
	int    ok_search_val  = 20;
	int    bad_search_val = 40;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	idl_ok = idl_get_prev(head, cb_idl_cmp_val, &ok_search_val);
	assert(idl_ok == &n1.dnode);
	idl_not_found = idl_get_prev(head, cb_idl_cmp_val, &bad_search_val);
	assert(idl_not_found == NULL);
	idl_error = idl_get_prev(NULL, cb_idl_cmp_val, &ok_search_val);
	assert(idl_error == NULL);
	idl_error = idl_get_prev(head, NULL, &ok_search_val);
	assert(idl_error == NULL);
}
