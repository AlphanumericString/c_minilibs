#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_push(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_NODE(n1, 1, 1);
	INIT_NODE(n2, 2, 2);
	INIT_NODE(n3, 3, 3);
	INIT_NODE(n4, 4, 4);
	(void)n4;

	assert(ill_push(&head, &n2.node) != NULL);
	assert(head == &n2.node);

	assert(ill_push(&head, &n1.node) != NULL);
	assert(head == &n1.node);
	ASSERT_ORDER_ILL(&n1.node, &n2.node);
	assert(ill_push(NULL, &n3.node) == NULL);
	assert(ill_push(&head, NULL) == NULL);
}
