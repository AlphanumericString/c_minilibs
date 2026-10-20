#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_reverse(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	t_ill *reversed = ill_reverse(head);
	assert(reversed == &n3.node);
	assert(n3.node.next == &n2.node);
	assert(n2.node.next == &n1.node);
	assert(n1.node.next == NULL);

	t_ill *reversed_back = ill_reverse(reversed);
	assert(reversed_back == &n1.node);
	assert(n1.node.next == &n2.node);
	assert(n2.node.next == &n3.node);
	assert(n3.node.next == NULL);

	assert(ill_reverse(NULL) == NULL);
}
