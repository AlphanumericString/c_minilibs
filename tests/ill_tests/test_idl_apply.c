#include "ill_tests.h"

static bool add_val(void *elem, void *arg) {
	t_test_dnode *n   = (t_test_dnode *)elem;
	int          *val = (int *)arg;
	n->value         += *val;
	return true;
}

static bool stop_at_30(void *elem, void *arg) {
	t_test_dnode *n = (t_test_dnode *)elem;
	(void)arg;
	return n->value != 30;
}

void test_idl_apply(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	int to_add = 5;
	idl_foreach(head, add_val, &to_add);
	assert(n1.value == 15);
	assert(n2.value == 25);
	assert(n3.value == 35);

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	bool ret = idl_foreach(head, stop_at_30, NULL);
	assert(ret == false);

	idl_for(head, 2, add_val, &to_add);
	assert(n1.value == 15);
	assert(n2.value == 25);
	assert(n3.value == 30);
}
