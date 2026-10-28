#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_npush(void) {
	t_test_node n1, n2, n3, n4;
	t_test_node n7, n8;
	t_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	INIT_NODE(n7, 46, 7);
	INIT_NODE(n8, 47, 8);
	LINK_UP_NODES(&n7.node, &n8.node);

	assert(ill_npush(&head, &n7.node) != NULL);
	assert(head == &n7.node);
	assert(n8.node.next == &n1.node);
	assert(ill_npush(&head, NULL) == NULL);
	assert(ill_npush(NULL, &n7.node) == NULL);
}
