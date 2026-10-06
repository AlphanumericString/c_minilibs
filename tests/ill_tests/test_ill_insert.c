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

	assert(ill_push(&head, &n2.node) != NULL);
	assert(head == &n2.node);

	assert(ill_push(&head, &n1.node) != NULL);
	assert(head == &n1.node);
	assert(n1.node.next == &n2.node);
	assert(ill_push(NULL, &n3.node) == NULL);
	assert(ill_push(&head, NULL) == NULL);

	assert(ill_push_back(&head, &n4.node) != NULL);
	assert(n2.node.next == &n4.node);
	assert(n4.node.next == NULL);
	assert(ill_push_back(NULL, &n3.node) == NULL);
	assert(ill_push_back(&head, NULL) == NULL);

	ill_append(&n2.node, &n3.node);
	assert(n2.node.next == &n3.node);
	assert(n3.node.next == &n4.node);
	ill_append(NULL, &n3.node);
	ill_append(&n2.node, NULL);

	t_test_node n5, n6;
	ill_init(&n5.node, offsetof(t_test_node, node));
	ill_init(&n6.node, offsetof(t_test_node, node));
	n5.node.next = &n6.node;

	ill_nappend(&n4.node, &n5.node);
	assert(n4.node.next == &n5.node);
	assert(n5.node.next == &n6.node);
	assert(n6.node.next == NULL);
	ill_nappend(&n4.node, NULL);
	ill_nappend(NULL, &n5.node);

	t_test_node n7, n8;
	ill_init(&n7.node, offsetof(t_test_node, node));
	ill_init(&n8.node, offsetof(t_test_node, node));
	n7.node.next = &n8.node;

	assert(ill_npush(&head, &n7.node) != NULL);
	assert(head == &n7.node);
	assert(n8.node.next == &n1.node);
	assert(ill_npush(&head, NULL) == NULL);
	assert(ill_npush(NULL, &n7.node) == NULL);
}
