#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_push_back(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);

	assert(ill_push_back(&head, &n4.node) != NULL);
	ASSERT_ORDER_ILL(&n2.node, &n4.node);
	assert(n4.node.next == NULL);
	assert(ill_push_back(NULL, &n3.node) == NULL);
	assert(ill_push_back(&head, NULL) == NULL);
}
