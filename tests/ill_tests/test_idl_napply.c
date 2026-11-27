#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_napply(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;
	void           *ret;
	int             to_add = 5;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	ret = idl_napply(head, 2, cb_idl_add_val_as_int_ptr, &to_add);
	assert(n1.value == (10 + to_add));
	assert(n2.value == (20 + to_add));
	assert(n3.value == 30);
	assert(ret == &to_add);
	ret = idl_napply(head, 3, cb_idl_add_val_as_int_ptr, &to_add);
	assert(n1.value == (10 + to_add * 2));
	assert(n2.value == (20 + to_add * 2));
	assert(n3.value == (30 + to_add));
	assert(ret == &to_add);
	ret = idl_napply(head, 1, cb_idl_add_val_as_int_ptr, &to_add);
	assert(n1.value == (10 + to_add * 3));
	assert(n2.value == (20 + to_add * 2));
	assert(n3.value == (30 + to_add));
	assert(ret == &to_add);
	ret = idl_napply(head, 999, cb_true, &to_add);
	assert(ret == &to_add);
	ret = idl_napply(head, SIZE_MAX, cb_true, &to_add);
	assert(ret == &to_add);
	ret = idl_napply(head, 2, cb_true, &to_add);
	assert(ret == &to_add);
	ret = idl_napply(head, 0, cb_false, &to_add);
	assert(ret == &to_add);

	ret = idl_napply(head, 2, cb_false, &to_add);
	assert(ret == &to_add);
	ret = idl_napply(head, 2, NULL, &to_add);
	assert(ret == &to_add);
	ret = idl_napply(NULL, 2, cb_idl_false_on_arg, &to_add);
	assert(ret == &to_add);
}
