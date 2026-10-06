#include <assert.h>

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
	t_test_node isolated_node;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));
	ill_init(&isolated_node.node, offsetof(t_test_node, node));

	n1.value            = 10;
	n2.value            = 20;
	n3.value            = 30;
	isolated_node.value = 99;

	assert(ill_is_isolated(&n1.node) == true);

	n1.node.next = &n2.node;
	n2.node.next = &n3.node;

	assert(ill_is_isolated(&n1.node) == false);
	assert(ill_is_isolated(&n3.node) == true);
	assert(ill_is_isolated(&isolated_node.node) == true);
	assert(ill_is_isolated(NULL) == false);

	assert(ill_is_circular(&n1.node) == false);
	n3.node.next = &n1.node;
	assert(ill_is_circular(&n1.node) == true);
	assert(ill_is_circular(&n2.node) == true);
	assert(ill_is_circular(&n3.node) == true);
	isolated_node.node.next = &isolated_node.node;
	assert(ill_is_circular(&isolated_node.node) == true);
	isolated_node.node.next = NULL;
	assert(ill_is_circular(&isolated_node.node) == false);
	assert(ill_is_circular(NULL) == false);
	n3.node.next = NULL;

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cmp_val) == true);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(ill_is_sorted(&n1.node, cmp_val) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 20;
	assert(ill_is_sorted(&n1.node, cmp_val) == true);
	assert(ill_is_sorted(&isolated_node.node, cmp_val) == true);
	n1.value = 10;
	n2.value = 40;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cmp_val) == false);
	n1.value = 10;
	n2.value = 0;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cmp_val) == false);

	assert(ill_is_sorted(&n1.node, NULL) == false);
	assert(ill_is_sorted(NULL, cmp_val) == false);

	n3.value = 42;
	assert(ill_has(&n1.node, has_val_42, NULL) == true);
	n3.value = 43;
	assert(ill_has(&n1.node, has_val_42, NULL) == false);
	assert(ill_has(&n1.node, NULL, NULL) == false);
	assert(ill_has(NULL, has_val_42, NULL) == false);
}
