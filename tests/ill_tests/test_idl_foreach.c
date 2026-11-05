#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_foreach(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;
	bool            ret;
	int             to_add = 5;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	ret = idl_foreach(head, cb_idl_add_val_as_int_ptr, &to_add);
	assert(n1.value == (10 + to_add));
	assert(n2.value == (20 + to_add));
	assert(n3.value == (30 + to_add));
	assert(ret == true);
	ret = idl_foreach(head, cb_true, NULL);
	assert(ret == true);

	ret = idl_foreach(head, cb_false, NULL);
	assert(ret == false);
	ret = idl_foreach(head, cb_idl_false_on_arg, NULL);
	assert(ret == false);
	ret = idl_foreach(head, NULL, NULL);
	assert(ret == false);
	ret = idl_foreach(NULL, cb_idl_false_on_arg, NULL);
	assert(ret == false);
}
