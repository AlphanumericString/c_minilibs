#include <assert.h>

#include "ill_tests.h"

static bool add_val(void *elem, void *arg) {
	t_test_dnode *n   = (t_test_dnode *)elem;
	int          *val = (int *)arg;
	n->value         += *val;
	return true;
}
static bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
static bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}
static bool stop_at_arg(void *elem, void *arg) {
	t_test_dnode *n = (t_test_dnode *)elem;
	int           val;

	if (!arg) return (false);
	val = *(int *)arg;
	return n->value <= val;
}

void test_idl_foreach(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;
	bool         ret;
	int          to_add = 5;
	//	int          val    = 30;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	ret = idl_foreach(head, add_val, &to_add);
	assert(n1.value == (10 + to_add));
	assert(n2.value == (20 + to_add));
	assert(n3.value == (30 + to_add));
	assert(ret == true);
	ret = idl_foreach(head, cb_true, NULL);
	assert(ret == true);

	ret = idl_foreach(head, cb_false, NULL);
	assert(ret == false);
	ret = idl_foreach(head, stop_at_arg, NULL);
	assert(ret == false);
	ret = idl_foreach(head, NULL, NULL);
	assert(ret == false);
	ret = idl_foreach(NULL, stop_at_arg, NULL);
	assert(ret == false);

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	ret = idl_for(head, 2, add_val, &to_add);
	assert(n1.value == (10 + to_add));
	assert(n2.value == (20 + to_add));
	assert(n3.value == 30);
	assert(ret == true);
	ret = idl_for(head, 3, add_val, &to_add);
	assert(n1.value == (10 + to_add * 2));
	assert(n2.value == (20 + to_add * 2));
	assert(n3.value == (30 + to_add));
	assert(ret == true);
	ret = idl_for(head, 1, add_val, &to_add);
	assert(n1.value == (10 + to_add * 3));
	assert(n2.value == (20 + to_add * 2));
	assert(n3.value == (30 + to_add));
	assert(ret == true);
	ret = idl_for(head, 999, cb_true, NULL);
	assert(ret == true);
	ret = idl_for(head, -1, cb_true, NULL);
	assert(ret == true);
	ret = idl_for(head, 2, cb_true, NULL);
	assert(ret == true);

	ret = idl_for(head, 2, cb_false, NULL);
	assert(ret == false);
	ret = idl_for(head, 2, NULL, NULL);
	assert(ret == false);
	ret = idl_for(NULL, 2, stop_at_arg, NULL);
	assert(ret == false);
}
