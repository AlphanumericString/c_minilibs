#include <assert.h>

#include "ill_tests.h"

void test_ill_rm(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));
	ill_init(&n4.node, offsetof(t_test_node, node));

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	t_ill *popped = ill_pop(&head);
	assert(popped == &n1.node);
	assert(head == &n2.node);

	t_ill *popped_back = ill_pop_back(&head);
	assert(popped_back == &n4.node);
	assert(n3.node.next == NULL);

	t_ill *rm_after = ill_remove_after(&n2.node);
	assert(rm_after == &n3.node);
	assert(n2.node.next == NULL);
	assert(head == &n2.node);

	ill_push_back(&head, &n3.node);
	bool rm_node = ill_remove_node(&head, &n3.node);
	assert(rm_node == true);
	assert(head == &n2.node);
	assert(n2.node.next == NULL);

	rm_node = ill_remove_node(&head, &n2.node);
	assert(rm_node == true);
	assert(head == NULL);
}
