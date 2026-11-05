#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_reverse(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	INIT_NODE(n1, 42, 1);
	INIT_NODE(n2, 43, 2);
	INIT_NODE(n3, 44, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	t_cm_ill *reversed = ill_reverse(head);
	assert(reversed == &n3.node);
	ASSERT_ORDER_ILL(&n3.node, &n2.node);
	ASSERT_ORDER_ILL(&n2.node, &n1.node);
	assert(n1.node.next == NULL);

	t_cm_ill *reversed_back = ill_reverse(reversed);
	assert(reversed_back == &n1.node);
	ASSERT_ORDER_ILL(&n1.node, &n2.node);
	ASSERT_ORDER_ILL(&n2.node, &n3.node);
	assert(n3.node.next == NULL);

	assert(ill_reverse(NULL) == NULL);
}
