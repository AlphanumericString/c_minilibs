#include <assert.h>

#include "ill_tests.h"

void test_ill_insert(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));
	ill_init(&n4.node, offsetof(t_test_node, node));

	n1.value = 1;
	n2.value = 2;
	n3.value = 3;
	n4.value = 4;

	ill_push(&head, &n2.node);
	assert(head == &n2.node);

	ill_push(&head, &n1.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n2.node);

	ill_push_back(&head, &n4.node);
	assert(n2.node.next == &n4.node);
	assert(n4.node.next == NULL);

	ill_append(&n2.node, &n3.node);
	assert(n2.node.next == &n3.node);
	assert(n3.node.next == &n4.node);

	t_test_node n5, n6;
	ill_init(&n5.node, offsetof(t_test_node, node));
	ill_init(&n6.node, offsetof(t_test_node, node));
	n5.node.next = &n6.node;

	ill_nappend(&n4.node, &n5.node);
	assert(n4.node.next == &n5.node);
	assert(n5.node.next == &n6.node);
	assert(n6.node.next == NULL);

	t_test_node n7, n8;
	ill_init(&n7.node, offsetof(t_test_node, node));
	ill_init(&n8.node, offsetof(t_test_node, node));
	n7.node.next = &n8.node;

	ill_npush(&head, &n7.node);
	assert(head == &n7.node);
	assert(n8.node.next == &n1.node);
}
