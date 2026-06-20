#include "ill_tests.h"

static int cmp_val(void *elem1, void *elem2) {
	t_test_dnode *n1 = (t_test_dnode *)elem1;
	t_test_dnode *n2 = (t_test_dnode *)elem2;
	return n1->value - n2->value;
}

static bool has_val_42(void *elem, void *arg) {
	t_test_dnode *n = (t_test_dnode *)elem;
	(void)arg;
	return n->value == 42;
}

void test_idl_is(void) {
	t_test_dnode n1, n2, n3;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	assert(idl_is_isolated(&n1.dnode) == true);

	n1.dnode.next = &n2.dnode;
	n2.dnode.prev = &n1.dnode;
	n2.dnode.next = &n3.dnode;
	n3.dnode.prev = &n2.dnode;

	assert(idl_is_isolated(&n1.dnode) == false);
	assert(idl_is_isolated(&n3.dnode) == false);

	assert(idl_is_circular(&n1.dnode) == false);
	n3.dnode.next = &n1.dnode;
	n1.dnode.prev = &n3.dnode;
	assert(idl_is_circular(&n1.dnode) == true);

	n3.dnode.next = NULL;
	n1.dnode.prev = NULL;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);

	n2.value = 40;
	n3.value = 30;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == false);

	n3.value = 42;
	assert(idl_has(&n1.dnode, has_val_42, NULL) == true);

	n3.value = 43;
	assert(idl_has(&n1.dnode, has_val_42, NULL) == false);
}
