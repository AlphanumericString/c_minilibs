#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_append(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n4.node);

	ill_append(&n2.node, &n3.node);
	ASSERT_ORDER_ILL(&n2.node, &n3.node);
	ASSERT_ORDER_ILL(&n3.node, &n4.node);
	ill_append(NULL, &n3.node);
	ill_append(&n2.node, NULL);
}
