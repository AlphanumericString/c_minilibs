#include <assert.h>

#include "ill_tests.h"

static bool add_val(void *elem, void *arg) {
	t_test_node *n   = (t_test_node *)elem;
	int         *val = (int *)arg;
	n->value        += *val;
	return true;
}

static bool stop_at_30(void *elem, void *arg) {
	t_test_node *n = (t_test_node *)elem;
	(void)arg;
	return n->value != 30;
}

void test_ill_foreach(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	int to_add = 5;
	ill_foreach(head, add_val, &to_add);
	assert(n1.value == 15);
	assert(n2.value == 25);
	assert(n3.value == 35);

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	bool ret = ill_foreach(head, stop_at_30, NULL);
	assert(ret == false);

	ill_for(head, 2, add_val, &to_add);
	assert(n1.value == 15);
	assert(n2.value == 25);
	assert(n3.value == 30);
}
