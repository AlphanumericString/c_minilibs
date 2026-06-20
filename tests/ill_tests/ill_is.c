#include "ill_tests.h"

static int cmp_val(void *elem1, void *elem2) {
	t_test_node *n1 = (t_test_node *)elem1;
	t_test_node *n2 = (t_test_node *)elem2;
	return n1->value - n2->value;
}

static bool has_val_42(void *elem, void *arg) {
	t_test_node *n = (t_test_node *)elem;
	(void)arg;
	return n->value == 42;
}

void test_ill_is(void) {
	t_test_node n1, n2, n3;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	assert(ill_is_isolated(&n1.node) == true);

	n1.node.next = &n2.node;
	n2.node.next = &n3.node;

	assert(ill_is_isolated(&n1.node) == false);
	assert(ill_is_isolated(&n3.node) == true);

	assert(ill_is_circular(&n1.node) == false);
	n3.node.next = &n1.node;
	assert(ill_is_circular(&n1.node) == true);

	n3.node.next = NULL;
	assert(ill_is_sorted(&n1.node, cmp_val) == true);

	n2.value = 40;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cmp_val) == false);

	n3.value = 42;
	assert(ill_has(&n1.node, has_val_42, NULL) == true);

	n3.value = 43;
	assert(ill_has(&n1.node, has_val_42, NULL) == false);
}
