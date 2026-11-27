#include <assert.h>

#include "ill_tests.h"

NO_COV static bool add_until_30(void *elem, void *arg) {
	t_cm_test_dnode *n   = (t_cm_test_dnode *)elem;
	int             *add = (int *)arg;

	if (n->value >= 30) return (false);
	n->value += *add;
	return (true);
}

NO_COV void test_cm_idl_apply_range(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;
	size_t          off  = offsetof(t_cm_test_dnode, dnode);
	int             add  = 5;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	INIT_DNODE(n4, 40, 4);
	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);
	idl_push_back_node(&head, &n4.dnode);

	assert(idl_apply_range(NULL, NULL, cb_idl_add_val_as_int_ptr, &add, off) ==
	       &add);
	assert(idl_apply_range(&n1, NULL, NULL, &add, off) == &add);

	// empty range
	assert(idl_apply_range(&n1, &n1, cb_idl_add_val_as_int_ptr, &add, off) ==
	       &add);
	assert(n1.value == 10 && n4.value == 40);

	// bounded range [n1, n3)
	assert(idl_apply_range(&n1, &n3, cb_idl_add_val_as_int_ptr, &add, off) ==
	       &add);
	assert(n1.value == 15);
	assert(n2.value == 25);
	assert(n3.value == 30);
	assert(n4.value == 40);

	// open ended range
	assert(idl_apply_range(&n1, NULL, cb_idl_add_val_as_int_ptr, &add, off) ==
	       &add);
	assert(n1.value == 20);
	assert(n2.value == 30);
	assert(n3.value == 35);
	assert(n4.value == 45);

	// stops early when the callback returns false
	assert(idl_apply_range(&n1, NULL, add_until_30, &add, off) == &add);
	assert(n1.value == 25);
	assert(n2.value == 30);
	assert(n3.value == 35);
	assert(n4.value == 45);
}
